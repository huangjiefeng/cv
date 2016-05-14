#include <QtWidgets/QtWidgets>

#include <cassert>

#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h>
#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h>

//core proxy
#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/core/client/CvCoreIf.h>

#include "QtScreenShare.h"
#include "qtconferenceroom.h"
#include <cwebpage/cwebpage.h>
#include "uicommon.h"

#include "MediaWindowManager.h"

#include "QtScreenWidget.h"

DWORD pptProcessID = 0;

//查找PPT播放窗口
BOOL CALLBACK FindPPTWindowsProc( HWND hwnd , LPARAM lparam )
{
	char windowClass[_MAX_PATH]={0};
	char windowTitle[_MAX_PATH]={0};
	GetClassName( hwnd , windowClass , _MAX_PATH );
	GetWindowText(hwnd , windowTitle , _MAX_PATH );
	DWORD processID;
	GetWindowThreadProcessId(hwnd , &processID);
	if( processID==pptProcessID )
	{
		if( strcmp( windowClass,"PPTFrameClass")==0 )
		{
			HWND* phwnd = (HWND*)lparam ;
			*phwnd = hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

//查找幻灯片的播放窗口
BOOL CALLBACK FindSlideWindowProc( HWND hwnd, LPARAM lparam )
{
	char windowClass[_MAX_PATH]={0};
	char windowTitle[_MAX_PATH]={0};
	GetClassName( hwnd , windowClass , _MAX_PATH );
	GetWindowText(hwnd , windowTitle , _MAX_PATH );
	DWORD processID;
	if( strcmp( windowClass,"paneClassDC")==0 )
	{
		HWND* phwnd = (HWND*)lparam ;
		*phwnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

QtScreenShare::QtScreenShare(QtConferenceRoom* confRoom ):_confRoom(confRoom)
{
	_ui = new Ui::ScreenShare;
	_ui->setupUi(this);
	installEventFilter(this);
	connect(_ui->shareButton, SIGNAL(clicked()), this, SLOT(openFile()));
	widget = new QtScreenWidget();
	//_ui->gridLayout_2->addWidget(widget);
	_ui->scrollArea->setWidget(widget);
	firstOpenFile = false;
	isSendScreen = false;
	isRecvScreen = false;
	isScreenShare = false;

	_channelProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME , CHANNEL_DISPATCHER_OBJECT_PATH,
		QDBusConnection::sessionBus());

	_userID = "";
	_pptProcess = NULL;
	_pptWnd = NULL;
	_slideWnd = NULL;
    _airServerWnd = NULL;
    _sharedMode = kDocumentMode;
	ControlScreenShare(false);
}

QtScreenShare::~QtScreenShare(void)
{
	SAFE_DELETE(_pptProcess );
	stopScreenGraph();
	closeFile();
    notifyAllScreenSharedState(false);
	SAFE_DELETE(widget);
	SAFE_DELETE(_ui);
	SAFE_DELETE(_channelProxy);
}

void QtScreenShare::openFile()
{
    if (_sharedMode == kAirPlayMode) {
        if (isSendScreen) {
          stopScreenGraph();
          notifyAllScreenSharedState(false);
        } else {
          activateAirServer();
        }
        return;
    }

	if (firstOpenFile)
	{
      stopScreenGraph();
      closeFile();
      notifyAllScreenSharedState(false);

      bool enable =false;
      QByteArray output_bytes;
      QDataStream out( &output_bytes, QIODevice::WriteOnly );
      out.setVersion(QDataStream::Qt_4_4 );
      out << enable;
      CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
      coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

      isScreenShare = false;
    } else {
		if( MediaWindowManager::getInstance()->isSendingScreen( ) )
		{
			//QMessageBox::information(this ,QString::fromLocal8Bit("提示"),
			//	QString::fromLocal8Bit("正在共享屏幕，请先退出原来的共享文档") ,
			//	QMessageBox::Ok);
			return;
		}

		QString fileName = QFileDialog::getOpenFileName(this,
			QString::fromLocal8Bit("打开文件"),
			"C:\\",
			tr("Files (*.txt *.doc *.ppt *.pdf *.jpg)"));
		if (fileName.isEmpty())
		{
			return;
		}
		fileName.replace("/", "\\");
		if( fileName.indexOf(".pdf")!=-1 )
		{
			//如果是pdf的话就借助浏览器控件打开
			EmbedBrowserObject((HWND)(widget->winId()));
			DisplayHTMLPage((HWND)widget->winId(), fileName.toLocal8Bit().data() );
			firstOpenFile = true;

			RECT wndRect;
			GetWindowRect( (HWND)(widget->winId()) , &wndRect );
			sendSpecificScreen(  wndRect.left, wndRect.top,wndRect.right, wndRect.bottom );
		}else
		{
			//如果是ppt的话需要解决pptviewer打开
			if( fileName.indexOf(".ppt")!=-1 
				|| fileName.indexOf(".pptx")!=-1 )
			{
				QString pptKey = "SOFTWARE\\Classes\\PowerPointViewer.Show.12\\shell\\Show\\command";

				//在注册表中找到pptviewer的程序路径
				HKEY hKey;
				if( RegOpenKeyEx( HKEY_LOCAL_MACHINE , pptKey.toStdString().c_str() , 0, KEY_READ , &hKey )!=ERROR_SUCCESS )
				{
					QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请安装最新版的PowerPointViewer来打开ppt文件"), QMessageBox::Ok); 
					return;
				}

				DWORD dwBufSize = 256;
				DWORD dwDataType=REG_SZ;
				unsigned char szData[256] = {0};
				char keyValue[256]={0};
				//读取默认项，即命令行参数
				if( RegQueryValueEx( hKey , "" , NULL , &dwDataType , szData , &dwBufSize )!=ERROR_SUCCESS )
				{	
					return;
				}

				memcpy( keyValue , szData,sizeof(szData) );
				QString cmd = keyValue;
				QString processPath = cmd.left( cmd.lastIndexOf(" ") );	//截取前面的命令路径，C:\Program Files\Microsoft Office\Office14\pptview.exe "%1"
				processPath = "\"" + processPath + "\"";		//为了解决路径存在空格的问题

				//使用QProcess打开
				if( _pptProcess==NULL )
				{
					_pptProcess = new QProcess(0);
					connect(_pptProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this , SLOT(pptProcessFinishSlot(int , QProcess::ExitStatus)));
					QStringList arguments;
					arguments << fileName;
					_pptProcess->start( processPath ,arguments  );
					_PROCESS_INFORMATION* info = _pptProcess->pid();

					pptProcessID = info->dwProcessId;

					//枚举所有窗口并找到ppt进程所属的窗口句柄
					_pptWnd = NULL;
					int tryTimes = 0;
					while( tryTimes<10 && _pptWnd==NULL )
					{
						EnumWindows( FindPPTWindowsProc, (LPARAM)&_pptWnd );
						Sleep(500);
					}

					if( _pptWnd==NULL )
					{
						QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("异常情况：没有找到PPT的窗口"), QMessageBox::Ok); 
						return;
					}

					//枚举所有窗口并找到ppt进程中幻灯片的窗口
					HWND _slideWnd = NULL;
					tryTimes = 0;
					while( tryTimes<10 && _slideWnd==NULL )
					{
						EnumChildWindows( _pptWnd, FindSlideWindowProc, (LPARAM)&_slideWnd );
						Sleep(500);
					}
					if( _slideWnd==NULL )
					{
						QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("异常情况：没有找到幻灯片的窗口"), QMessageBox::Ok); 
						return;
					}
					
					//获取当前组件所在屏幕的坐标信息gmlan20150828
					auto screenShareRect = QApplication::desktop()->screenGeometry(this);
					qDebug()<<"----------2-----screenShareRect:"<<screenShareRect;
					//首先将PPT窗口移动到当前显示器
					SetWindowPos( _pptWnd , HWND_TOP, screenShareRect.left() , screenShareRect.top() , screenShareRect.width() , screenShareRect.bottom() ,SWP_FRAMECHANGED);

					//然后最大化
					ShowWindow( _pptWnd , SW_MAXIMIZE );
					
					//获取窗口的配置属性
					long lStyle = GetWindowLong( _pptWnd , GWL_STYLE );
					lStyle = lStyle & ~WS_SYSMENU;
					lStyle = lStyle & ~WS_MAXIMIZEBOX;
					lStyle = lStyle & ~WS_MINIMIZEBOX;
					lStyle = lStyle & ~WS_CAPTION;
					SetWindowLong( _pptWnd , GWL_STYLE , lStyle );

					RECT slideRect;
					GetWindowRect( _slideWnd , &slideRect );
					sendSpecificScreen(  slideRect.left, slideRect.top,slideRect.right, slideRect.bottom );
				}
			}
		}

		bool enable =true;
		QByteArray output_bytes;
		QDataStream out( &output_bytes, QIODevice::WriteOnly );
		out.setVersion(QDataStream::Qt_4_4 );
		out << enable;
		CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
		coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

		isScreenShare = true;

		MediaWindowManager::getInstance()->ScreenShareStateUpdateNotify();

	}
}

void QtScreenShare::openFile(QString filename,int screenIndex)
{
	if (firstOpenFile)
	{
      stopScreenGraph();
      closeFile();
      notifyAllScreenSharedState(false);

      bool enable = false;
      QByteArray output_bytes;
      QDataStream out( &output_bytes, QIODevice::WriteOnly );
      out.setVersion(QDataStream::Qt_4_4 );
      out << enable;
      CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
      coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

    } else {
		if( MediaWindowManager::getInstance()->isSendingScreen( ) )
		{
			//QMessageBox::information(this ,QString::fromLocal8Bit("提示"),
			//	QString::fromLocal8Bit("正在共享屏幕，请先退出原来的共享文档") ,
			//	QMessageBox::Ok);
			return;
		}

		QString fileName = filename;
		if (fileName.isEmpty())
		{
			return;
		}
		fileName.replace("/", "\\");
		if( fileName.indexOf(".pdf")!=-1 )
		{
			//如果是pdf的话就借助浏览器控件打开
			EmbedBrowserObject((HWND)widget->winId());
			DisplayHTMLPage((HWND)widget->winId(), fileName.toLocal8Bit().data() );
			firstOpenFile = true;

			RECT wndRect;
			GetWindowRect( (HWND)(widget->winId()) , &wndRect );
			sendSpecificScreen(  wndRect.left, wndRect.top,wndRect.right, wndRect.bottom );
		}else
		{
			//如果是ppt的话需要解决pptviewer打开
			if( fileName.indexOf(".ppt")!=-1 
				|| fileName.indexOf(".pptx")!=-1 )
			{
				QString pptKey = "SOFTWARE\\Classes\\PowerPointViewer.Show.12\\shell\\Show\\command";

				//在注册表中找到pptviewer的程序路径
				HKEY hKey;
				if( RegOpenKeyEx( HKEY_LOCAL_MACHINE , pptKey.toStdString().c_str() , 0, KEY_READ , &hKey )!=ERROR_SUCCESS )
				{
					QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请安装最新版的PowerPointViewer来打开ppt文件"), QMessageBox::Ok); 
					return;
				}

				DWORD dwBufSize = 256;
				DWORD dwDataType=REG_SZ;
				unsigned char szData[256] = {0};
				char keyValue[256]={0};
				//读取默认项，即命令行参数
				if( RegQueryValueEx( hKey , "" , NULL , &dwDataType , szData , &dwBufSize )!=ERROR_SUCCESS )
				{	
					return;
				}

				memcpy( keyValue , szData,sizeof(szData) );
				QString cmd = keyValue;
				QString processPath = cmd.left( cmd.lastIndexOf(" ") );	//截取前面的命令路径，C:\Program Files\Microsoft Office\Office14\pptview.exe "%1"
				processPath = "\"" + processPath + "\"";		//为了解决路径存在空格的问题

				//使用QProcess打开
				if( _pptProcess==NULL )
				{
					_pptProcess = new QProcess(0);
					connect(_pptProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this , SLOT(pptProcessFinishSlot(int , QProcess::ExitStatus)));
					QStringList arguments;
					arguments << fileName;
					_pptProcess->start( processPath ,arguments  );
					_PROCESS_INFORMATION* info = _pptProcess->pid();

					pptProcessID = info->dwProcessId;

					//枚举所有窗口并找到ppt进程所属的窗口句柄
					_pptWnd = NULL;
					int tryTimes = 0;
					while( tryTimes<10 && _pptWnd==NULL )
					{
						EnumWindows( FindPPTWindowsProc, (LPARAM)&_pptWnd );
						Sleep(500);
					}

					if( _pptWnd==NULL )
					{
						QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("异常情况：没有找到PPT的窗口"), QMessageBox::Ok); 
						return;
					}

					//枚举所有窗口并找到ppt进程中幻灯片的窗口
					HWND _slideWnd = NULL;
					tryTimes = 0;
					while( tryTimes<10 && _slideWnd==NULL )
					{
						EnumChildWindows( _pptWnd, FindSlideWindowProc, (LPARAM)&_slideWnd );
						Sleep(500);
					}
					if( _slideWnd==NULL )
					{
						QMessageBox::information(this , QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("异常情况：没有找到幻灯片的窗口"), QMessageBox::Ok); 
						return;
					}
					
					//获取当前组件所在屏幕的坐标信息gmlan20150828
					auto screenShareRect = QApplication::desktop()->screenGeometry(this);
					qDebug()<<"----------3-----screenShareRect:"<<screenShareRect;
					//首先将PPT窗口移动到当前显示器
					SetWindowPos( _pptWnd , HWND_TOP, screenShareRect.left() , screenShareRect.top() , screenShareRect.width() , screenShareRect.bottom() ,SWP_FRAMECHANGED);

					//然后最大化
					ShowWindow( _pptWnd , SW_MAXIMIZE );

					//获取窗口的配置属性
					long lStyle = GetWindowLong( _pptWnd , GWL_STYLE );
					lStyle = lStyle & ~WS_SYSMENU;
					lStyle = lStyle & ~WS_MAXIMIZEBOX;
					lStyle = lStyle & ~WS_MINIMIZEBOX;
					lStyle = lStyle & ~WS_CAPTION;
					SetWindowLong( _pptWnd , GWL_STYLE , lStyle );

					RECT slideRect;
					GetWindowRect( _slideWnd , &slideRect );
					sendSpecificScreen(  slideRect.left, slideRect.top,slideRect.right, slideRect.bottom );


				}
			}
		}

		bool enable =true;
		QByteArray output_bytes;
		QDataStream out( &output_bytes, QIODevice::WriteOnly );
		out.setVersion(QDataStream::Qt_4_4 );
		out << enable;
		CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
		coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

		isScreenShare = true;

		MediaWindowManager::getInstance()->ScreenShareStateUpdateNotify();

	}
	
}



void QtScreenShare::closeFile()
{
	if(firstOpenFile)
		UnEmbedBrowserObject((HWND)widget->winId());
	
	//如果打开了PPT那么需要同时关闭PPT......zhenHua.sun 2011-11-21
	killPPT();

    firstOpenFile = false;
}

void QtScreenShare :: killPPT()
{
	if(_pptProcess!=NULL)
	{
		_pptProcess->kill();
	}
}

void QtScreenShare::stopScreenGraph()
{
  if( isSendScreen )
  {
    _channelProxy->ReleaseChannel(SEND_SCREEN_MEDIA_ID, SCREEN_MEDIA_SERVICE_NAME);
    
    isSendScreen = false;

    MediaWindowManager::getInstance()->isSendingScreen(false);
    _ui->shareButton->setText(QString::fromLocal8Bit("共享屏幕"));
  }
}

void QtScreenShare::notifyAllScreenSharedState(bool isShared) {
  isScreenShare = isShared;
  MediaWindowManager::getInstance()->ScreenShareStateUpdateNotify();
}

void QtScreenShare::recvScreen(const bool enable)
{
	if( enable )
	{
		widget->startDisplayScreen();
	}else
	{
		widget->stopDisplayScreen();
	}
}


bool QtScreenShare::eventFilter(QObject * object, QEvent * event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

		if (keyEvent->key() == Qt::Key_PageDown/*|Qt::Key_PageUp|Qt::Key_Left|Qt::Key_Right*/
			||keyEvent->key()==Qt::Key_PageUp ){
				QMessageBox::information( this , "Info",
					"PageDownOrPageUp" );
			return true;
		} 

	}else
	{
		if( event->type()==QEvent::MouseButtonRelease )
		{
			int i=0;
		}
	}

	return false;
}

void QtScreenShare::resizeEvent ( QResizeEvent * event )
{
	//_cWengoPhone.getCUserProfileHandler().getCUserProfile()->resizeVideoWindow("screen_share",widget->winId(),0);
}

void QtScreenShare::cvHostUnShareScreenEventHandler()
{
	//cvHostUnShareScreenSinal();
}

void QtScreenShare::cvHostUnShareScreenSlot()
{

	if (isRecvScreen)
	{
		//CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
		//QMessageBox::warning(this, tr("@product@"),
		//	tr("\344\270\273\346\214\201\344\272\272\346\262\241\346\234\211\345\205\261\344\272\253!"), QMessageBox::Ok);
		//cUserProfile->screen_share_recv_stop();
		//_ui->recvScreenShareButton->setText(QApplication::translate("ScreenShare", "\346\216\245\346\224\266\345\261\217\345\271\225\345\205\261\344\272\253", 0, QApplication::UnicodeUTF8));
		//isRecvScreen = false;
	}
}

int QtScreenShare::getScreenWindow()
{
	if( widget )
	{
		return (int)widget->winId();
	}else
	{
		return 0;
	}
}

void QtScreenShare::setMediaState( const UiMediaState state )
{
	if( widget )
	{
		widget->setMediaState( state );
	}
}

/**
 * @brief 释放共享屏幕打开的相关文件，并清空屏幕发送进程
 */
void QtScreenShare::release()
{
	setMediaState(UiMediaState_Destroy);
	widget->stopDisplayScreen();
    stopScreenGraph();
	closeFile();
    notifyAllScreenSharedState(false);
	UserID("");
}

void QtScreenShare::sendSpecificScreen( const int left/*=0*/,const int top/*=0*/, const int right/*=0 */, const int bottom/*=0 */ )
{
	if( isSendScreen )
	{
		stopScreenGraph();
	}else
	{
		if( MediaWindowManager::getInstance()->isSendingScreen() )
		{
			QMessageBox::information( this ,QString::fromLocal8Bit("提示") , 
				QString::fromLocal8Bit("屏幕共享已经开启，请先关闭原来的屏幕共享。" ) , QMessageBox::Ok );
			return;
		}

		MediaWindowManager::getInstance()->isSendingScreen(true);

		QRect wndRect;
		wndRect.setLeft(left);
		wndRect.setTop(top);
		wndRect.setRight(right);
		wndRect.setBottom(bottom);
		QByteArray output_bytes;
		QDataStream out( &output_bytes, QIODevice::WriteOnly );
		out.setVersion(QDataStream::Qt_4_4 );
		out << wndRect;
		CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
		coreProxy.TeleCommand( CoreAction_StartSendScreenMedia , output_bytes );

		_ui->shareButton->setText(QString::fromLocal8Bit("停止共享"));
		isSendScreen = true;
	}
}

void QtScreenShare::pptProcessFinishSlot( int exitCode ,QProcess::ExitStatus exitStatus )
{
	stopScreenGraph();
	if( _pptProcess )
	{
		disconnect(_pptProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this , SLOT(pptProcessFinishSlot(int , QProcess::ExitStatus)));
		SAFE_DELETE(_pptProcess );
	}
	_pptWnd = NULL;
	_slideWnd = NULL;
	
	if(isScreenShare)
	{
		bool enable =false;
		QByteArray output_bytes;
		QDataStream out( &output_bytes, QIODevice::WriteOnly );
		out.setVersion(QDataStream::Qt_4_4 );
		out << enable;
		CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
		coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

	}


	isScreenShare = false;
	MediaWindowManager::getInstance()->ScreenShareStateUpdateNotify();

}

void QtScreenShare::controlPPT( const PPTControlType type )
{
	if( _pptWnd!=NULL && type!=PPTControl_Unknow )
	{
		//利用Spy++监听到PowerpointViewer翻页的命令消息的格式
		if( type==QtScreenShare::PPTControl_Next )
		{
			::PostMessage(_pptWnd , WM_COMMAND, MAKELONG(393,1),0 );
		}else if( type==QtScreenShare::PPTControl_Pre )
		{
			::PostMessage(_pptWnd , WM_COMMAND, MAKELONG(394,1),0 );
		}else if( type==QtScreenShare::PPTControl_Close )
		{
			::PostMessage( _pptWnd , WM_CLOSE, 0 , 0 );
		}
	}
}

/**
 * @brief 控制共享屏幕的界面的按钮，如果当前终端为主讲人则把“屏幕共享”按钮显示出来，否则隐藏它
 * @param control 如果为true，则显示按钮，否则隐藏按钮
 */
void QtScreenShare::ControlScreenShare( const bool control )
{
	if( control )
	{
		//显示共享按钮
		this->_ui->shareButton->show();
	}else
	{
		//关闭PPT进程
		this->killPPT();
		this->release();

		//隐藏共享按钮
		this->_ui->shareButton->hide();
		
	}
}

void QtScreenShare::SetSharedMode( SharedMode mode ) {
  if (_sharedMode == mode) {
    return;
  }
  switch (mode) {
    case QtScreenShare::kDocumentMode:
      changeToDocumentMode();
      break;
    case QtScreenShare::kAirPlayMode:
      changeToAirPlayMode();
      break;
    case QtScreenShare::kVideoAudioMode:
      changeToVideoAudioMode();
      break;
    default:
      assert(false);
      break;
  }
}

void QtScreenShare::changeToDocumentMode() {
  if (isSendScreen) {
    stopScreenGraph();

    // 告知终端关闭共享，使得core proxy能够告知相关的终端接收该数据
    bool enable = false;
    QByteArray output_bytes;
    QDataStream out( &output_bytes, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_4_4 );
    out << enable;
    CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
    coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );
  }
  if (_airServerWnd) {
    ShowWindow(_airServerWnd, SW_HIDE);
    _airServerWnd = NULL;
  }
  _sharedMode = kDocumentMode;
}

void QtScreenShare::changeToAirPlayMode() {
  if (isSendScreen) {
    // TODO: close file?
    stopScreenGraph();
    closeFile();
    notifyAllScreenSharedState(false);

    // 告知终端关闭共享，使得core proxy能够告知相关的终端接收该数据
    bool enable = false;
    QByteArray output_bytes;
    QDataStream out( &output_bytes, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_4_4 );
    out << enable;
    CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
    coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );
  }

  activateAirServer();
}

void QtScreenShare::changeToVideoAudioMode() {
  if (isSendScreen) {
    stopScreenGraph();

    // 告知终端共享模式结束，使得core proxy能够告知相关的终端停止接收该数据
    bool enable = false;
    QByteArray output_bytes;
    QDataStream out( &output_bytes, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_4_4 );
    out << enable;
    CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
    coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

  }
  if (_airServerWnd) {
    ShowWindow(_airServerWnd, SW_HIDE);
    _airServerWnd = NULL;
  }
  _sharedMode = kVideoAudioMode;
}

bool QtScreenShare::activateAirServer() {
  _sharedMode = kAirPlayMode;
  if (NULL == _airServerWnd) {
    _airServerWnd = FindWindow("AirServer::MainFrame", NULL);
  }
  if (NULL == _airServerWnd) {
    QMessageBox::information( this ,QString::fromLocal8Bit("提示") , 
      QString::fromLocal8Bit("AirServer未启动，请启动AirServer程序，再打开屏幕共享（AirPlay）") , QMessageBox::Ok);
    return false;

  } else {
    if (MediaWindowManager::getInstance()->isSendingScreen()) {
      QMessageBox::information( this ,QString::fromLocal8Bit("提示") , 
        QString::fromLocal8Bit("屏幕共享已经开启，请先关闭原来的屏幕共享。" ) , QMessageBox::Ok );
      return false;
    }

    QRect desktopRect = QApplication::desktop()->availableGeometry(_confRoom);
    
    ShowWindow(_airServerWnd, SW_SHOW);

    RECT airRect;
    GetWindowRect(_airServerWnd, &airRect);
    LONG airWidth = airRect.right - airRect.left;
    LONG airHeight = airRect.bottom - airRect.top;
    MoveWindow(
      _airServerWnd, 
      desktopRect.x() + (desktopRect.width() - airWidth) / 2,
      desktopRect.y() + (desktopRect.height() - airHeight)/2,
      airWidth, airHeight, TRUE);
    
    // 判断AirServer窗口是否是全屏
    if (airWidth != desktopRect.width() || airHeight != desktopRect.height()) {
      // 使用spy++监听消息，然后得出full screen的消息。
      PostMessage(_airServerWnd, WM_COMMAND, 0x00018004, 0);
    }

    RECT wndRect;
    GetWindowRect( (HWND)(widget->winId()) , &wndRect );
    sendSpecificScreen(  wndRect.left, wndRect.top,wndRect.right, wndRect.bottom );

    _sharedMode = kAirPlayMode;
    notifyAllScreenSharedState(true);

    // 告知终端AirServer共享模式开始，使得core proxy能够告知相关的终端接收该数据
    bool enable = true;
    QByteArray output_bytes;
    QDataStream out( &output_bytes, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_4_4 );
    out << enable;
    CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
    coreProxy.TeleCommand( CoreAction_ShareScreenControl , output_bytes );

    
  }
  return true;
}
