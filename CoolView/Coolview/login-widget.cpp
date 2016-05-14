#include "login-widget.h"
#include "profile/RunningProfile.h"
#include "config/RunningConfig.h"
#include "util/ini/CVIniConfig.h"
#include "main-window.h"
//#include "WeblibConfigDialog.h"

#include <QtNetwork/QNetworkInterface>
#include <QMessageBox>

#include <dbus/sipwrapper/client/CvSipWrapperIf.h>
#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include <DeviceManager/DeviceManager.h>

#include <util/NetworkUtil.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <util/ProcessManager.h>

#include "IpConfigDialog.h"

#include <log/Log.h>

#include "util/GetFileVersion.h"

QString LoginWidget::getVersionByName(const TCHAR * cName)
{
	char strVersion[20];

	memset(strVersion, 0, sizeof(strVersion));
	//使用CGetFileVersion类读取rtpware.dll的版本信息
	CGetFileVersion verReader;
	verReader.Open(cName);	//打开文件，读取版本信息
	if (!verReader.IsValid())
	{
		//读取错误
		printf("error to read the version of %s \n", cName);
	}
	else
	{
		//输出版本号信息
		sprintf(strVersion, "%d.%d.%d.%d", 
			verReader.GetFileVersionMajor(),
			verReader.GetFileVersionMinor(), 
			verReader.GetFileVersionBuild(), 
			verReader.GetFileVersionQFE());
	}
	return QString::fromStdString(std::string(strVersion));
}

//比较两个IP地址，相同返回true，否则返回false
bool compareIPAddress( const char* address1, const char* address2 )
{
	if( address1==NULL || NULL==address2 )
	{
		return false;
	}

	if( strstr(address1 ,"." )!=NULL && strstr(address2,".")!=NULL )
	{
		//IPv4
		IN_ADDR addr1;
		IN_ADDR addr2;
		inet_pton( AF_INET , address1 , &addr1 );
		inet_pton( AF_INET , address2 , &addr2 );
		if( addr1.s_addr==addr2.s_addr )
			return true;
		else
			return false;
	}

	if( strstr(address1 ,":" )!=NULL && strstr(address2,":")!=NULL )
	{
		//IPv6
		IN6_ADDR addr1;
		IN6_ADDR addr2;
		inet_pton( AF_INET6 , address1 , &addr1 );
		inet_pton( AF_INET6 , address2 , &addr2 );
		bool unEqual = false;
		for(int i=0 ; i<8 ; i++ )
		{
			if( addr1.u.Word[i]!=addr2.u.Word[i] )
			{
				unEqual = true;
				break;
			}
		}
		if( unEqual )
			return false;
		else
			return true;
	}

	return false;
}

LoginWidget::LoginWidget( MainWindow* mainWindow) : _mainWindow( mainWindow)
{
    setupUi(this);

	setWindowFlags( Qt::WindowStaysOnTopHint );

	//本地界面事件处理
    connect(this->loginButton,SIGNAL(clicked()),SLOT(loginSlot()));
    connect(this->cancelButton,SIGNAL(clicked()),SLOT(close()));
	//connect( this , SIGNAL(enterMainWindowSignal()) , this , SLOT(enterMainWindowSlot()) );
	connect( this->webcamComboBox , SIGNAL(currentIndexChanged( const QString & )),
		this , SLOT( webcamChangedSlot( const QString& ) ) );
	connect(this->toolButton,SIGNAL(clicked()),this,SLOT(onToolButtonClicked()));
	connect( this->autoLoginCheckbox , SIGNAL(stateChanged(int)) , SLOT(autoLoginChecked(int)));
	connect( this->ipConfigButton , SIGNAL(clicked()), this , SLOT(showIpConfigDialog() ) );
	connect( this->weblibConfigButton , SIGNAL(clicked()), this , SLOT(showWeblibConfigDialog() ) );
	connect( this->refreshIPListButton, SIGNAL(clicked()) ,this,SLOT(loadIpListSlot()) );
	connect( this->realmComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(realmChangedSlot(const QString&) ) );

	this->netGroupBox->setVisible(false);
	this->avGroupBox->setVisible(false);
	this->ipConfigButton->setVisible(false);
	this->weblibConfigButton->setVisible(false);

	this->layout()->setSizeConstraint(QLayout::SetFixedSize);

	this->_sipWrapperIf = new CvSipWrapperIf( SIP_WRAPPER_SERVICE_NAME, SIP_WRAPPER_OBJECT_PATH,
		QDBusConnection::sessionBus() );

	//销毁sipwrapper上的所有信息
	this->_sipWrapperIf->terminate();

	//连接用户状态发生变更的信号
	connect( _sipWrapperIf , SIGNAL(phoneLineStateChangedSignal(const QString &, int)), 
		this , SLOT(SipAccountStateChangedSlot(const QString &, int)));


	//调整位置
	ResizeWidgetInMiddle( this );

	_isLogin = false;


	_checkNetworkTimer = new QTimer(this);
	connect( _checkNetworkTimer , SIGNAL(timeout()),this , SLOT(checkNetworkSlot()) );

	_registTimer = new QTimer(this );
	connect( _registTimer , SIGNAL(timeout()) , this , SLOT(registUserSlot() ) );
	
	_onlineTimer = new QTimer(this);
	connect( _onlineTimer , SIGNAL(timeout()) , this , SLOT(onlineInfoSlot() ) );

	_ipConfig = new IPConfigDialog( this );

	//可能会有自动登录，因此需要在所有东西都初始化后才进行配置读取
	loadConfig();

	//如果用户选择了自动登录
	autoLogin();

	//_weblibConfig = NULL;

}

LoginWidget::~LoginWidget()
{
	disconnect( _checkNetworkTimer , SIGNAL(timeout()),this , SLOT(checkNetworkSlot()) );
	SAFE_DELETE(_checkNetworkTimer);
	disconnect( _registTimer , SIGNAL(timeout()) , this , SLOT(registUserSlot()) );
	SAFE_DELETE( _registTimer );
	disconnect( _onlineTimer , SIGNAL(timeout()) , this , SLOT(onlineInfoSlot() ) );
	SAFE_DELETE(_onlineTimer);
}

void LoginWidget::loginSlot()
{

	//_islogining = true;
    if(this->userNameLineEdit->text().isEmpty()||this->passwordLineEdit->text().isEmpty())
    {
        warningLabel->setText(QString::fromLocal8Bit("请输入帐号和密码！ "));
    }
    else
    {
		//先保存配置
		saveConfig();

		if( RunningConfig::Instance()->isEnableKinect() )
		{
			//如果默认使用kinect，则开启kinect进程
			startKinectDaemon();
		}

		//清空警示标题
		warningLabel->setText(QString::fromLocal8Bit(""));

		QString login = this->userNameLineEdit->text().trimmed().toLower();
		QString password = this->passwordLineEdit->text().trimmed();
		QString realm = this->realmComboBox->currentText();
		QString globalService = this->globalServiceLineEdit->text().trimmed();

		int registerPort = SIP_REGISTER_PORT;
		int proxyPort = SIP_PROXY_PORT;

		QString registerServer = realm;
		QString proxyServer = registerServer;
		QString accountname = login + "@" + registerServer;
		QString displayname = accountname;

		//fill ampty lines
		if (password.size()==0) {
			password = login;
		}
		if (registerServer.size()==0.) {
			registerServer = realm;
		}
		if (proxyServer.isEmpty()) {
			proxyServer = registerServer;
		}
		if (proxyPort==0) {
			proxyPort = 5060;
		}
		if (accountname.isEmpty()) {
			accountname = login + "@" + registerServer;
		}
		if (displayname.isEmpty()) {
			displayname = accountname;
		}

		_username = login;
		_password = password;

		_sipWrapperIf->terminate();	//清空sipwarpper

		//判断用户的IP地址
		if( RunningProfile::getInstance()->ip_address().find(":")!= std::string::npos )
		{
			//采用IPv6
			_sipWrapperIf->init( true );
		this->_sipWrapperIf->addVirtualLine( displayname , login , login , password , realm , proxyServer,
				registerServer, AF_INET6 );
		}else
		{
			//采用IPv4
			_sipWrapperIf->init( false );
			this->_sipWrapperIf->addVirtualLine( displayname , login , login , password , realm , proxyServer,
				registerServer, AF_INET );
		}

		//获取mac地址
		QString mac = "";
		QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
		for( QList<QNetworkInterface>::iterator it=interfaces.begin() ; it!=interfaces.end() ; it++ )
		{
			QList<QNetworkAddressEntry> addressList = it->addressEntries();
			bool findMac = false;
			for( QList<QNetworkAddressEntry>::iterator addressIt=addressList.begin(); addressIt!=addressList.end() ; addressIt++ )
			{
				QString ipAddress = RunningProfile::getInstance()->ip_address().c_str();
				QString ipAddressFromIF = addressIt->ip().toString(); 
				if( compareIPAddress( ipAddress.toStdString().c_str() , ipAddressFromIF.toStdString().c_str()) )
				{
					findMac = true;
					mac = it->hardwareAddress();
					RunningProfile::getInstance()->set_mac_address(mac.toStdString() );
				}
			}
			if( findMac )
				break;
		}
		//生成注册content消息
		_registContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"Register\"><mac>" + mac + "</mac></coolview>";

		//Sip帐号登录
		this->_sipWrapperIf->registerVirtualLine(login, _registContent);

		_checkNetworkTimer->start( 3000 );
	}
}

void LoginWidget::SipAccountStateChangedSlot( const QString& username , int state )
{
	if( !_isLogin )
	{
		if( username==_username )
		{
			switch( state )
			{
			case SipAccountStateOk:
				{
					_isLogin = true;
					//发送终端登录消息
					getMyIpAddr();
					QString myVersion=getVersionByName(L"coolview.exe");
					TerminalLoginSignal(_myIpV4Addr,_myIpV6Addr,myVersion);   //向服务器发送真实源地址认证消息
					_registTimer->start(600000);	//十分钟发送一份regist报告，用于与SIP服务器的保活
					_onlineTimer->start(5000);    //每1分钟发送一份Online报告到GlobalService，用于登录时sip服务器的保活
					enterMainWindowSignal();
					hide();
					break;
				}
			case SipAccountStateTimeout:
				{
					warningLabel->setText(QString::fromLocal8Bit("登录超时"));
					break;
				}
			case SipAccountStateAuthenticationError:
				{
					warningLabel->setText(QString::fromLocal8Bit("认证失败"));
					break;
				}
			case SipAccountStateServerError:
				{
					warningLabel->setText(QString::fromLocal8Bit("无法连接到服务器"));
					break;
				}
			case SipAccountStateProgress:
				{
					warningLabel->setText(QString::fromLocal8Bit("登录中..."));
					break;
				}
			case SipAccountStateClosed:
				{
					warningLabel->setText(QString::fromLocal8Bit("帐号退出"));
					break;		
				}
			case SipAccountStateNotPermitted:
				{
					warningLabel->setText(QString::fromLocal8Bit("终端没有权限使用本账号登陆") );
					break;
				}
				/** Unknown state. */
			case SipAccountStateUnknown:
				{
					warningLabel->setText(QString::fromLocal8Bit("未知错误"));
					break;
				}
			}
		}
	}
}

void LoginWidget::saveConfig()
{
	RunningProfile* profile = RunningProfile::getInstance();

	if( this->autoLoginCheckbox->isChecked() )
	{
		profile->set_auto_login(true );
	}else
	{
		profile->set_auto_login(false );
	}

	
	if( this->rememberCheckBox->isChecked() )
	{
		profile->set_remember_profile(true);
	}else
	{
		profile->set_remember_profile(false);
	}

	QString login = this->userNameLineEdit->text().trimmed().toLower();
	profile->set_username(login.toStdString());
	
	QString password = this->passwordLineEdit->text().trimmed();
	profile->set_password(password.toStdString());
	
	QString realm = this->realmComboBox->currentText();
	profile->set_realm(realm.toStdString());

	QString ipAddr = this->IPListComboBox->currentText();
	profile->set_ip_address(ipAddr.toStdString());

	profile->SaveFile();

	RunningConfig* config = RunningConfig::Instance();

	//视频宽高 - 必须先设置此项，详见RunningConfig::VideoCaptureDevice =_=!
	QStringList sizes = this->videoSizeComboBox->currentText().split("X");
	if( sizes.size()>=2 )
	{
		int width = sizes.at(0).toUInt();
		int height = sizes.at(1).toUInt();
		int fps = 30;
		if( sizes.size()==3 )
			fps = sizes.at(2).toUInt();
		/*config->VideoWidth(width);
		config->VideoHeight(height);
		config->VideoFps(fps);*/
	}else
	{
		/*config->VideoWidth(0);
		config->VideoHeight(0);
		config->VideoFps(0);*/
	}
	//config->VideoCaptureDevice( this->webcamComboBox->currentText() );
	config->AudioCaptureDevice(this->audioInputComboBox->currentText() );
	config->AudioOutputDevice( this->audioOutputComboBox->currentText() );
	config->GlobalService( this->globalServiceLineEdit->text().trimmed() );
	config->saveConfig();
}

void LoginWidget::loadConfig()
{
	RunningProfile* profile = RunningProfile::getInstance();
	profile->LoadFile(); // 强制还原为保存的设置

	int index = -1;

	loadIpListSlot();

	//读取用户登录参数
	if( profile->ukey_certify() || profile->remember_profile() )
	{
		this->rememberCheckBox->setChecked(true);
		this->userNameLineEdit->setText( QString::fromStdString(profile->username()) );
		this->passwordLineEdit->setText( QString::fromStdString(profile->password()) );
		
		//realm & global Service
		//index = this->realmComboBox->findText( profile->realm().c_str() );
		//this->realmComboBox->setCurrentIndex( index>0? index:0 );
		realmComboBox->setCurrentText(profile->realm().c_str());
        RunningConfig::Instance()->GlobalService(
          QString("sip:globalService@") + QString::fromStdString(profile->realm()));

		this->globalServiceLineEdit->setText( RunningConfig::Instance()->GlobalService( ) );
	}else
	{
		this->userNameLineEdit->setText("");
		this->passwordLineEdit->setText("");	//清空用户名和密码...2011-11-12
		this->rememberCheckBox->setChecked(false);
	}

	//读取用户设备参数
	QStringList devices;
	vector<string> stringDevices;
	DeviceManager::GetInstatnce()->GetVideoCaptureDevices( &stringDevices );
	TransformStringVectorToQStringList( &stringDevices , &devices );
	this->webcamComboBox->clear();
	this->webcamComboBox->addItems( devices );
	//devices.push_front("PCIe HD");

	//start...以下代码主要是为了让用户首选USB设备
	//先过滤掉终端的采集卡，剩下的就是USB设备
	for( int i=0 ; i<devices.size() ; i++ )
	{
		if( devices.at(i).indexOf("PCIe")>0 || devices.at(i).indexOf("Device")>0 )
			continue;
		int index = this->webcamComboBox->findText( devices.at(i) );
		this->webcamComboBox->setCurrentIndex( index>0? index: 0 );
	}
	//end


	stringDevices.clear();
	DeviceManager::GetInstatnce()->GetAudioCaptureDevices( &stringDevices );
	TransformStringVectorToQStringList( &stringDevices , &devices );
	this->audioInputComboBox->clear();
	this->audioInputComboBox->addItems( devices );

	stringDevices.clear();
	DeviceManager::GetInstatnce()->GetAudioOutputDevices( &stringDevices );
	TransformStringVectorToQStringList( &stringDevices , &devices );
	this->audioOutputComboBox->clear();
	this->audioOutputComboBox->addItems( devices );

	index = 0;//this->webcamComboBox->findText( RunningConfig::Instance()->VideoCaptureDevice() );
	if( index>=0 )
		this->webcamComboBox->setCurrentIndex( index );
	index = this->audioInputComboBox->findText( RunningConfig::Instance()->AudioCaptureDevice() );
	this->audioInputComboBox->setCurrentIndex( index>0? index:0 );
	index = this->audioOutputComboBox->findText( RunningConfig::Instance()->AudioOutputDevice() );
	this->audioOutputComboBox->setCurrentIndex( index>0 ? index: 0 );

	vector<string> stringTypeList;
	QString webcam = this->webcamComboBox->currentText();
	DeviceManager::GetInstatnce()->GetVideoDeviceMediaType( webcam.toLocal8Bit().constData() , &stringTypeList );
	QStringList typeList;
	TransformStringVectorToQStringList( &stringTypeList ,&typeList );
	this->videoSizeComboBox->clear();
	this->videoSizeComboBox->addItems( typeList );

	/*QString videoSize = QString::number(RunningConfig::Instance()->VideoWidth()) + "X" + QString::number(RunningConfig::Instance()->VideoHeight( )) 
		+ "X"+ QString::number(RunningConfig::Instance()->VideoFps());; */
	index = 0;//this->videoSizeComboBox->findText( videoSize );
	this->videoSizeComboBox->setCurrentIndex( index>0? index:0 );
}

void LoginWidget::webcamChangedSlot( const QString& webcam )
{
	this->videoSizeComboBox->clear();

	//设备属性
	vector<string> videoSizeList;
	DeviceManager::GetInstatnce()->GetVideoDeviceMediaType( webcam.toLocal8Bit().constData() , &videoSizeList );
	QStringList itemList;
	TransformStringVectorToQStringList( &videoSizeList , &itemList );
	this->videoSizeComboBox->addItems( itemList );

	/*
	foreach(QString item, typeList)
	{
		int index = item.lastIndexOf("X");
		QString resolution = item.left(index);
		QString fps = item.mid(index + 1);
		this->videoSizeComboBox->addItem(resolution, QVariant(fps));
	}*/

}

void LoginWidget::logoutSlot()
{
	_isLogin = false;

	this->loadConfig();
	
	warningLabel->setText("");
	_sipWrapperIf->removeVirtualLine( _username , true );
	_sipWrapperIf->terminate();

	this->_checkNetworkTimer->stop();
	this->_registTimer->stop();
	this->_onlineTimer->stop();
}

void LoginWidget::onToolButtonClicked()
{
	if(this->toolButton->arrowType() == Qt::RightArrow)
	{
		this->toolButton->setArrowType(Qt::DownArrow);
		this->netGroupBox->show();
		if (CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_HD)
		{
			this->avGroupBox->show();
		}
		else
		{
			this->weblibConfigButton->show();
		}
		this->ipConfigButton->show();
	}
	else
	{
		this->toolButton->setArrowType(Qt::RightArrow);
		this->netGroupBox->hide();
		if (CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_HD)
		{
			this->avGroupBox->hide();
		}
		else
		{
			this->weblibConfigButton->hide();
		}
		this->ipConfigButton->hide();
	}
}

void LoginWidget::autoLoginChecked( int state)
{
	if( state==Qt::Checked )
	{
		this->rememberCheckBox->setChecked( true );
	}
}

void LoginWidget::closeEvent( QCloseEvent * event )
{
	if( this->_mainWindow->isHidden() )
	{
		QString infoMsg;
#ifdef _DEBUG
		infoMsg = QString::fromLocal8Bit("确定退出系统？");
#else
		infoMsg = QString::fromLocal8Bit("确定退出系统并关机？");
#endif
		//如果只有一个登录框，那么询问是否退出进程并关机
		int reply = QMessageBox::warning(this,tr("CoolView"),infoMsg , QMessageBox::Yes|QMessageBox::No);
		if(reply != QMessageBox::Yes)
		{
			event->ignore();
			return;
		}

		event->accept();

#ifndef _DEBUG
		system("shutdown -s -t 0");
#endif

	}else
	{
		event->ignore();
	}
}

void LoginWidget::showIpConfigDialog()
{
	_ipConfig->show();
}

void LoginWidget::showWeblibConfigDialog()
{
	/*if (_weblibConfig == NULL)
	{
		_weblibConfig = new WeblibConfigDialog(this);
	}
	_weblibConfig->show();*/
}

void LoginWidget::loadIpListSlot()
{
	QStringList ipList = GetHostIPList();
	this->IPListComboBox->clear();
	this->IPListComboBox->addItems( ipList);
	for( int i=0 ; i<ipList.size() ; i++ )
	{
		QString ip = ipList.at(i);
		if( ip.indexOf(".")!=-1 )
		{
			this->IPListComboBox->setCurrentIndex( this->IPListComboBox->findText(ip) );
			break;
		}
	}

	RunningProfile* profile = RunningProfile::getInstance();
	if( profile->remember_profile() )
	{
		int index = this->IPListComboBox->findText( QString::fromStdString(profile->ip_address()) );
		this->IPListComboBox->setCurrentIndex( index>0 ? index : 0 );
	}

}

void LoginWidget::autoLogin()
{
	RunningProfile* profile = RunningProfile::getInstance();
	if( profile->ukey_certify() || profile->auto_login() )
	{
		//如果通过了ukey认证，或者用户选择了自动登录，那么直接登录服务器
		this->autoLoginCheckbox->setChecked(true);

		if(this->checkIPState() == false)
		{
			//不存在可用的IP地址
			QMessageBox::critical( this , 
				QString::fromLocal8Bit("IP错误"),
				QString::fromLocal8Bit("没有可用的IP地址！"),
				QMessageBox::Ok
				);
			return;
		}

		this->loginSlot();
	}
}

void LoginWidget::realmChangedSlot( const QString& realm)
{
	QString globalService = "sip:globalService@"+realm;
	this->globalServiceLineEdit->setText(globalService );
}

void LoginWidget::checkNetworkSlot()
{
	static bool alarm = false;
	string mac = RunningProfile::getInstance()->mac_address();
	string ip = RunningProfile::getInstance()->ip_address();

	//首先检查本机的自动分配的IPv4地址是否发生更改
	if( ip.find(".")!=string::npos )
	{
		std::vector<std::string> ipList;
		WSADATA wsaData; 
		int rc = WSAStartup(MAKEWORD(2,2),&wsaData);

		GetIPList( AF_INET , &ipList );

		WSACleanup();
		bool currentIPExist = false;
		for(int i=0 ; i<ipList.size(); i++ )
		{
			if( ipList.at(i)==ip )
			{
				currentIPExist = true;
				break;
			}
		}
		if( !currentIPExist )
		{
			this->loadIpListSlot();
			_mainWindow->logout();
			QMessageBox::critical( this , QString::fromLocal8Bit("错误"),
				QString::fromLocal8Bit("网络出现异常，请检查网络并重新登录！"),
				QMessageBox::Ok );
			return;
		}
	}
	

	//检查网卡的网络状态
	if( _isLogin && mac!="" )
	{
		InterfaceStatus status;
		if( DeviceManager::GetInstatnce()->GetNetworkAdaptorState(mac , status )==0 )
		{
			static QTime lastInteruptTime;
			if( status!=IF_STATUS_OPERATIONAL )
			{
				if( !alarm )
				{
					//警告用户
					alarm = true;
					
					lastInteruptTime.restart();	//开始计时

					QMessageBox::critical( this , QString::fromLocal8Bit("错误"),
						QString::fromLocal8Bit("网络连接出现问题，请检查网络设备！"),
						QMessageBox::Ok );
				}else
				{

					//如果网络中断30s之后仍然没有得到解决，那么清空所有资源
					int interval = lastInteruptTime.elapsed();
					if( interval > 30000 )
					{
						_mainWindow->logout();
					}
				}
			}
			if( status==IF_STATUS_OPERATIONAL )
			{
				alarm = false;
				return;
			}

		}else
		{
			CV_LOG_ERROR( "获取网络状态失败" );
		}
	}
}

void LoginWidget::registUserSlot()
{
	
	_sipWrapperIf->registerVirtualLine(_username, _registContent  );
	

}

void LoginWidget:: onlineInfoSlot()
{
	string username=RunningProfile::getInstance()->username();
	QString cvMessage = "<?xml version=\"1.0\"?><coolview command=\"Online\"><type>heartbeat</type></coolview>";
	_sipWrapperIf->startConference( QString::fromStdString(username) , RunningConfig::Instance()->GlobalService(), cvMessage );

}

void LoginWidget::getMyIpAddr()
{
	QStringList ipList = GetHostIPList();
	_myIpV4Addr="";
	_myIpV6Addr="";
	for( int i=0 ; i<ipList.size() ; i++ )
	{
		QString ip = ipList.at(i);
		if( ip.indexOf(".")!=-1 )
		{
			_myIpV4Addr=ip;
		}
		if( ip.indexOf(":")!=-1 )
		{
			_myIpV6Addr=ip;
		}
	}
}

bool LoginWidget::checkIPState()
{
	if( this->IPListComboBox->count()==0 )
	{
		for(int i=0; i<3; i++)
		{
			Sleep(3000);
			this->loadIpListSlot();
			if(this->IPListComboBox->count() != 0)
				return true;
		}
		return false;
	}
	else
		return true;
}

