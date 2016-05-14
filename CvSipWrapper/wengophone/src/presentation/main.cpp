#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#if defined( WIN32 ) 
#include <crtdbg.h>
#endif

#include <system/Processes.h>

//for msdx client procedure...zhenHua.sun 2010-07-20
#include "msdxclient.h"
#include "ProcessManager.h"
#include <tchar.h>
#include <model/WengoPhone.h>
#include <model/config/CommandLineParser.h>
#include <model/config/Config.h>
#include <model/config/ConfigImporter.h>
#include <model/config/ConfigManagerFileStorage.h>
#include <model/config/ConfigManager.h>
#include <model/config/StartupSettingListener.h>
#include <model/classic/ClassicExterminator.h>
#include <model/commandserver/CommandClient.h>


#include <control/CWengoPhone.h>

#ifdef GTKINTERFACE
	#include <presentation/gtk/GtkFactory.h>
#else
	#include <presentation/qt/QtFactory.h>
	#include <presentation/qt/QtLanguage.h>
#endif

#include <imwrapper/IMWrapperFactory.h>
#include <sipwrapper/SipWrapperFactory.h>
#include <sipwrapper/SipWrapper.h>

#include <WengoPhoneBuildId.h>

#ifdef PHAPIWRAPPER
	#include <PhApiFactory.h>
#elif defined(SIPXWRAPPER)
	#include <SipXFactory.h>
	#include <NullIMFactory.h>
#elif defined (MULTIIMWRAPPER)
	#include <PhApiFactory.h>
	#include <multiim/MultiIMFactory.h>
	#include <PurpleIMFactory.h>
#else
	#include <NullSipFactory.h>
	#include <NullIMFactory.h>
#endif

#include <util/File.h>
#include <util/Logger.h>
#include <util/Path.h>
#include <util/SafeDelete.h>
#include <util/WebBrowser.h>

#include <system/RegisterProtocol.h>

#include <qtutil/FSResourceFileEngineHandler.h>

#include <cutil/global.h>

#if defined(CC_MSVC)
	#include <memorydump/MSVCMemoryDump.h>
#elif defined(CC_MINGW)
	#include <winsock2.h>
#elif defined(OS_LINUX)
	#include <memorydump/UNIXMemoryDump.h>
#elif defined(OS_MACOSX)
	#include <memorydump/MACMemoryDump.h>
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
	#include <signal.h>
	#include <sys/wait.h>
#endif

// to hide the console. by lizhongkan 2008-12-19
#define HIDE_CONSOLE
#if defined WIN32 && defined HIDE_CONSOLE
	#pragma comment (linker, "/ENTRY:mainCRTStartup")
	#pragma comment (linker, "/subsystem:windows")
#endif

#include <QtGui/QtGui>

#include <sstream>

#include <buildconfig.h>

static const char* CONFIG_SUBDIR = "/config/";

#include <dbus/channel/type/streamedMedia/CvStreamedMediaClient.h>
#define COOLVIEW_PROC_NAME					"CoolView.exe"
#define COOLVIEW_UPGRADER_PROC_NAME			"CoolViewUpgrader.exe"
#define COOLVIEW_TELECONTROLLER_PROC_NAME	"CvTelecontroller.exe"
#define COOLVIEW_CONFERENCE_ROOM_PROC_NAME	"ConferenceRoom.exe"
#define COOLVIEW_MSDX_PROC_NAME				"CvChannelDispatcher.exe"
#define COOLVIEW_LOCAL_MONITOR_PROC_NAME    "CvLocalMonitorWinForm.exe"

/**
 * Helper class to instantiate and delete factories based on the compilation
 * flags.
 */
struct FactoryHelper {
	FactoryHelper() {
	#ifdef SIPXWRAPPER
		_sipFactory = new SipXFactory();
		_imFactory = new NullIMFactory();

	#elif defined(PHAPIWRAPPER)
		_imFactory = _sipFactory = new PhApiFactory();

	#elif defined(MULTIIMWRAPPER)
		PhApiFactory* phApiFactory = new PhApiFactory();
		_sipFactory = phApiFactory;
		_purpleIMFactory = new PurpleIMFactory();
		_imFactory = new MultiIMFactory(*phApiFactory, *_purpleIMFactory);

	#else
		_sipFactory = new NullSipFactory();
		_imFactory = new NullIMFactory();
	#endif

		SipWrapperFactory::setFactory(_sipFactory);
		IMWrapperFactory::setFactory(_imFactory);
	}

	~FactoryHelper() {
	#ifdef PHAPIWRAPPER
		// In this case _imFactory == _sipFactory, so don't delete it twice
		OWSAFE_DELETE(_imFactory);
	#else
		OWSAFE_DELETE(_sipFactory);
		OWSAFE_DELETE(_imFactory);
	#endif

	#ifdef MULTIIMWRAPPER
		OWSAFE_DELETE(_purpleIMFactory);
	#endif
	}

	IMWrapperFactory * _imFactory;
	SipWrapperFactory * _sipFactory;

#if defined(MULTIIMWRAPPER)
	PurpleIMFactory* _purpleIMFactory;
#endif

};


/**
 * Stub function to make GCC silent.
 *
 * @see http://www-eleves-isia.cma.fr/documentation/BoostDoc/boost_1_29_0/libs/test/doc/minimal.htm
 */
/*int test_main(int argc, char *argv[]) {
	return 1;
}*/

static void initLogger(const std::string& userConfigDir) {
	std::string logFileName = userConfigDir + BINARY_NAME + ".log";
	// create directory if it doesn't exist
	if (!File::exists(userConfigDir)) {
		File::createPath(userConfigDir);
	}
	if (!File::isDirectory(userConfigDir)) {
		LOG_FATAL("User configuration dir '" + userConfigDir + "' does not exist or is not a directory");
	}

	Logger::getInstance()->setLogFileName(logFileName);
}

static void initConfig(const CommandLineParser& cmdLineParser) {
	ConfigManagerFileStorage configManagerStorage(ConfigManager::getInstance());

	std::string resourcesDir = cmdLineParser.getResourcesDir();
	if (!File::isDirectory(resourcesDir)) {
		LOG_FATAL("Resources dir '" + resourcesDir + "' does not exist or is not a directory");
	}

	std::string userConfigDir = cmdLineParser.getUserConfigDir();
	configManagerStorage.loadSystemConfig(resourcesDir + CONFIG_SUBDIR);
	Config::setConfigDir(userConfigDir);
	ConfigImporter importer;
	importer.importConfig();
	configManagerStorage.loadUserConfig(userConfigDir);

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	config.setResourcesDir(resourcesDir);
	WebBrowser::setBrowser(config.getLinuxPreferedBrowser());
}

static void registerHyperlinkProtocol(const Config& config, const std::string& executableName) {
	RegisterProtocol registerProtocol(config.getHyperlinkProtocol());
	std::string executableFullName = Path::getApplicationDirPath() + executableName;
	registerProtocol.bind(executableFullName + " -c %1", executableFullName + ",0", config.getCompanyWebSiteUrl());
}

/**
 * Callback additional info for MemoryDump.
 */
std::string getAdditionalInfo() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	std::stringstream stream;
	stream
		<< "User: " << config.getProfileLastUsedName() << std::endl
		<< "buildid: " << WengoPhoneBuildId::getBuildId() << std::endl
		<< "revision: " << WengoPhoneBuildId::getSvnRevision() << std::endl;
	return stream.str();
}

#if defined(OS_MACOSX) || defined(OS_LINUX)
static void sigpipe_catcher(int sig) {
#ifndef NDEBUG
	// Do not use LOG_DEBUG. There is only a limited set of functions you are
	// allowed to call from withing a signal catcher. See signal man page.
	static char msg[] = "Caught signal: SIGPIPE\n";
	write(2, msg, sizeof(msg));
#endif
}
#endif
/**
* Add by: hexianfu
* for the telecontroller
* 2009-12-12
*/
//#define CONSOLE_SHOW_WINDOW		SW_SHOW 		//显示控制台程序的窗口
#define CONSOLE_SHOW_WINDOW		SW_HIDE			//不显示控制台程序的窗口

int startExe( const char* name )
{
	const int iMaxpath = 256; 
	char szCurrentPath[iMaxpath];
	memset(szCurrentPath, 0, iMaxpath);

	GetModuleFileName(NULL,szCurrentPath, sizeof(szCurrentPath)); // 模锟斤拷锟斤拷锟斤拷锟铰凤拷锟? 目录+模锟斤拷锟斤拷
	std::string strTmp = szCurrentPath;
	unsigned int iPos = strTmp.rfind ('\\');    // 锟斤拷位锟斤拷目录末尾锟斤拷锟斤拷锟斤拷锟揭伙拷锟?\'锟街凤拷
	if(iPos!=strTmp.npos) { 
		strTmp.erase (iPos ); // 删锟斤拷模锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷目录
	}

	int result=(int)ShellExecute(NULL,"open",name,NULL,strTmp.c_str(),SW_HIDE);//SW_HIDE
	if(result>32)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

bool killExe( std::string dstProcessName )
{	
	return Processes::killProcess(dstProcessName);
}

int startTeleEXE()
{
	//telecontroller
	if( ProcessManager::isRunning(_T(COOLVIEW_TELECONTROLLER_PROC_NAME) ) )
	{
		//如果telecontroller已经在运行，则删除它
		killExe( COOLVIEW_TELECONTROLLER_PROC_NAME );
	}
	//再启动
	return startExe(COOLVIEW_TELECONTROLLER_PROC_NAME);
	
}


/*************************************************/

//zhenHua.sun 2010-07-28

int startLocalMonitor()
{
	if( !ProcessManager::isRunning( _T(COOLVIEW_LOCAL_MONITOR_PROC_NAME) ) )
		return startExe( COOLVIEW_LOCAL_MONITOR_PROC_NAME );
}

bool closeLocalMonitor()
{
	return killExe( COOLVIEW_LOCAL_MONITOR_PROC_NAME);
}


//开启更新功能.....................zhenHua.sun 2010-09-01
int startUpgrader()
{
	if( ProcessManager::isRunning( COOLVIEW_UPGRADER_PROC_NAME ) )
		killExe( COOLVIEW_UPGRADER_PROC_NAME );
	
	return startExe( COOLVIEW_UPGRADER_PROC_NAME );
}

int closeUpgrader()
{
	return killExe( COOLVIEW_UPGRADER_PROC_NAME );
}

//dbus session....zhenHua.sun 2010-10-07
int startDBusService()
{

	const int iMaxpath = 256; 
	char szCurrentPath[iMaxpath];
	memset(szCurrentPath, 0, iMaxpath);

	GetModuleFileName(NULL,szCurrentPath, sizeof(szCurrentPath)); //获取应用程序全路径
	std::string strTmp = szCurrentPath;
	unsigned int iPos = strTmp.rfind ('\\');    
	if(iPos!=strTmp.npos) { 
		strTmp.erase (iPos ); //获取当前目录全路径
	}
	std::string appDir = strTmp;
	
	int result = -1;
	if( !ProcessManager::isRunning( _T("dbus-daemon.exe" ) ) )
	{
		strTmp.append( "\\bin" );
		result=(int)ShellExecute(NULL,"open","dbus-monitor.exe",NULL,strTmp.c_str(),CONSOLE_SHOW_WINDOW);
		
		//等待dbus-daemon启动
		int retryTimes = 4;
		while( !ProcessManager::isRunning(_T("dbus-daemon.exe")) && retryTimes>0 )
		{
			Sleep(200);
			retryTimes--;
		}
		if ( !ProcessManager::isRunning(_T("dbus-daemon.exe")) &&  retryTimes==0 )
		{
			result = -1;
		}
	}else
	{
		result = 1;
	}

	return result;
}

int stopDBusService()
{
	//bool result = killExe( "CvMsdxSvr.exe" );
	bool result;
	result = killExe( "dbus-daemon.exe");
	result = killExe( "dbus-monitor.exe");
	return 0;
}

int startMsdxSvr()
{
	int result = 0;
	if( !ProcessManager::isRunning(COOLVIEW_MSDX_PROC_NAME))
	{
		result = startExe( COOLVIEW_MSDX_PROC_NAME );
	}else
	{
		//如果CvChannelDispatcher在运行，说明Coolview异常退出了。
		CvStreamedMediaClient::getInstance()->exitConference();
	}

	////启动client服务，用于函数回调
	//msdx_client_startCallbackService();

	return result;
}

/**
 * @brief 开启Coolview所需的相关进程
 */
int startReleativeServices()
{
	//DBusService
	int result = startDBusService();
	if( result<0 )
	{
		return result;
	}

	//msdxsvr
	result = startMsdxSvr();
	if( result<0 )
	{
		return result;
	}

	//telecontroller
	result = startTeleEXE();
	if( result<0 )
	{
		return result;
	}

	//远程更新
	result = startUpgrader();		//zhenHua.sun 2010-09-01
	//if( result<0 )
	//{
	//	return result;
	//}

	//LocalMonitor
	result = startLocalMonitor();


	return 1;
}

/**
 * @brief 关闭Coolview相关的所有进程
 */
void stopReleativeServices()
{
	//关闭远程更新
	killExe( COOLVIEW_UPGRADER_PROC_NAME );

	//关闭Telecontroller
	killExe( COOLVIEW_TELECONTROLLER_PROC_NAME );

	//关闭会议室
	killExe( COOLVIEW_CONFERENCE_ROOM_PROC_NAME );

	//关闭channelDispatcher
	killExe( COOLVIEW_MSDX_PROC_NAME );

	//关闭LocalMonitor
	killExe(  COOLVIEW_LOCAL_MONITOR_PROC_NAME );

	//关闭dbus，必须保证最后关闭
	stopDBusService();

}
int main(int argc, char * argv[]) {

	if( !Processes::isRunning( COOLVIEW_PROC_NAME) )
	{
		//如果已经有CoolView实例，则直接跳过该步骤......zhenHua.sun 2010-12-07
		int result = startReleativeServices();
	}

	//Init presentation factories before parsing the command line so that Qt or
	//Gtk get a chance to parse their command line options ('-style' for Qt for
	//example)
	PFactory * pFactory = NULL;
#ifdef GTKINTERFACE
	pFactory = new GtkFactory(argc, argv);
#else
	pFactory = new QtFactory(argc, argv);
#endif
        
#if defined(OS_MACOSX)
        QDir dir(QApplication::applicationDirPath());
        dir.cdUp();
        dir.cd("plugins");
        QApplication::setLibraryPaths(QStringList(dir.absolutePath()));
#endif
        
	PFactory::setFactory(pFactory);

	CommandLineParser cmdLineParser(BINARY_NAME, argc, argv);
	initLogger(cmdLineParser.getUserConfigDir());
	LOG_DEBUG("Started");

#if defined(OS_MACOSX) || defined(OS_LINUX)
	signal(SIGPIPE, sigpipe_catcher);
#endif

	// Setup factories. These must be initialized before loading config because
	// it's used when importing configuration from the Classic version.
	FactoryHelper factoryHelper;

	initConfig(cmdLineParser);

	Config & config = ConfigManager::getInstance().getCurrentConfig();

	SipWrapper* w = factoryHelper._sipFactory->createSipWrapper();
	
	w->setSipOptions("sip.register_timeout", QString::number(config.getSipRegisterTimeOut()).toStdString());
	w->setSipOptions("sip.publish_timeout", QString::number(config.getSipPublishTimeOut()).toStdString());
	if(config.getSipUseOptions())
		w->setSipOptions("sip.use_options_request", "true");
	else
		w->setSipOptions("sip.use_options_request", "false");

	if(config.getSipP2pPresence())
		w->setSipOptions("sip.p2p_presence", "true");
	else
		w->setSipOptions("sip.p2p_presence", "false");

	if(config.getSipChatWithoutPresence())
		w->setSipOptions("sip.chat.without.presence", "true");
	else
		w->setSipOptions("sip.chat.without.presence", "false");

	if(config.getSipUseTypingState())
		w->setSipOptions("sip.use_typing_state", "true");
	else
		w->setSipOptions("sip.use_typing_state", "false");


	//Remove WengoPhone Classic from startup registry
	ClassicExterminator::killClassicExecutable();

	//Remove WengoPhone Classic from startup registry
	ClassicExterminator::removeClassicFromStartup();

#if defined(ENABLE_CRASHREPORT)
    #if defined(CC_MSVC)
	MSVCMemoryDump * memoryDump = new MSVCMemoryDump("QuteCom", WengoPhoneBuildId::getSvnRevision());
	// This is needed for CRT to not show dialog for invalid param
	// failures and instead let the code handle it.
	_CrtSetReportMode(_CRT_ASSERT, 0);
    #endif

    #if defined(OS_MACOSX)
	MACMemoryDump * memoryDump = new MACMemoryDump("QuteCom", WengoPhoneBuildId::getSvnRevision());
    #endif

    #if defined(OS_LINUX)
	UNIXMemoryDump * memoryDump = new UNIXMemoryDump("QuteCom", WengoPhoneBuildId::getSvnRevision());
    #endif

	memoryDump->setGetAdditionalInfo(getAdditionalInfo);

#endif // ENABLE_CRASHREPORT

	// Uncomment to test crash report
	
	/*int* pointer;
	pointer = 0;
	*pointer = 12;*/
	

	//No 2 qtwengophone at the same time
	if (Processes::isRunning(BINARY_NAME) && !cmdLineParser.isSeveralWengoPhoneAllowed()) {
		const std::string callFromCommandLine = cmdLineParser.getCommand();
		CommandClient client;
		client.connect();
		if (!callFromCommandLine.empty()) {
			client.call(callFromCommandLine);
		} else {
#ifndef DBUS_ENABLE
			//该命令会把隐藏的QtWengoPhone显示出来，因此需要注释掉...zhenHua.sun 2010-10-29
			client.bringToFront();
#endif
		}
		return EXIT_SUCCESS;
	}

	// Register protocol used to associate the application in HTML links
	registerHyperlinkProtocol(config, cmdLineParser.getExecutableName());

	// Make the application starts when user logs on computer
	StartupSettingListener settingListener(cmdLineParser.getExecutableName());

	// Init file system resource engine
	FSResourceFileEngineHandler handler(QString::fromStdString(config.getResourcesDir()));

	// CWengoPhone creates QtWengoPhone (QtWengoPhone, GtkWengoPhone...)
	// and then starts the model thread. This way the gui is shown as soon as
	// possible
	WengoPhone & wengoPhone = WengoPhone::getInstance();

	wengoPhone.setStartupCall(cmdLineParser.getCommand());
	CWengoPhone cWengoPhone(wengoPhone, cmdLineParser.getRunInBackground());


	pFactory->exec();

	cWengoPhone._qtWengoPhone->SendInfo("Exit\r\n");

	//zhenHua.sun 2010-12-07
	stopReleativeServices();

	WengoPhone::deleteInstance();

	LOG_DEBUG("Ended");

	return EXIT_SUCCESS;
}
