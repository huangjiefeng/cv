#include "Global_Define.h"

#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>

#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

#include "util/MediaManager.h"
#include "config/RunningConfig.h"
#include "main-window.h"
#include "login-widget.h"
#include "SuspendedWindow.h"
#include "monitorConfig/MonitorConfig.h"

MainWindow::MainWindow( QWidget *parent )
{
    setupUi(this);
  //  setWindowTitle(QLatin1String("Roster"));

    setupGui();
    createActions();

	_quitProcess = false;
	_panelViesibleState=true;
    start=0;
	islogout=false;
    nam = new QNetworkAccessManager();
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));

	_teleControllerProxy = new TelecontrollerIf( _GetTelecontrollerServiceName("sock"),
		_GetTelecontrollerObjectPath("sock") , QDBusConnection::sessionBus() );


    qRegisterMetaType <MapList>("MapList");
 //   setAttribute(Qt::WA_DeleteOnClose);

	 loadMonitorConfig();
}

MainWindow::~MainWindow()
{
    qDebug()<<"release";

    //disconnect(qs,SIGNAL(gotRoomListSignal(const MapList&)),conflistwidget,SLOT(onRoomList(const MapList&)));
    //disconnect(qs,SIGNAL(gotControllerListSignal(const MapList&)),conflistwidget,SLOT(onGotMemberList(const MapList&)));
    //disconnect(qs,SIGNAL(gotTerminalListSignal(const MapList&)),conflistwidget,SLOT(onGotTerminalList(const MapList&)));
    //disconnect(qs,SIGNAL(EnterMeetingError(QString)),conflistwidget,SLOT(enterConferencErrorSlot(QString)));
    //disconnect(qs,SIGNAL(controllerJointSignal(const QVariantMap &)),conflistwidget,SLOT(onMemberJoint(const QVariantMap &)));
    //disconnect(qs,SIGNAL(terminalJointSignal(const QVariantMap &)),conflistwidget,SLOT(onTerminalJoint(const QVariantMap &)));
    //disconnect(qs,SIGNAL(memberLeftSignal(const QVariantMap &)),conflistwidget,SLOT(onMemberLeft(const QVariantMap &)));
    //disconnect(qs,SIGNAL(memberHandUpSignal(QString)),conflistwidget,SLOT(onMemberHandUp(QString)));
    //disconnect(qs,SIGNAL(memberHandDownSignal(QString)),conflistwidget,SLOT(onMemberHandDown(QString)));

    //disconnect(conflistwidget,SIGNAL(enterBtnClickedSignal(QString,QString)),qs,SLOT(enterConference(QString,QString)));
    //disconnect(conflistwidget,SIGNAL(handupSIGNAL(bool)),qs,SLOT(onHandUpOrDown(bool)));
    //disconnect(conflistwidget,SIGNAL(quitConfSIGNAL()),qs,SLOT(quitConference()));

    delete conflistwidget;
    delete dListWidget;
    delete aListWidget;
    delete helpWidget;
    delete meetingToolButton;
    delete menu;

    qDebug()<<"release success!";

    //MainWindowPointer=NULL;
}

void MainWindow::finished(QNetworkReply *reply)
{
    QVariant statusCodeV =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"status"<<statusCodeV;
    if (statusCodeV.toInt()==200)
    {
        QString all = QString::fromUtf8( reply->readAll().constData() );
		if( all.indexOf("\"code\":\"401\"")>0 )
		{
			//没有访问地址簿的权限，直接返回
			return;
		}else
		{
			if(aListWidget->TextParsing(all))
			{
				QUrl url2("http://202.38.192.215:8080/ConferenceControl-war/userManager/getAllUserList?start="+QString::number(start)+"&limit=10" );
				nam->get(QNetworkRequest(url2));
				start+=10;
			}
		}
    }
    else
    {
        qDebug()<<"error";
    }
}

/*void MainWindow::createIMWidgetSlot(IMChannel *channel)
{
    QString cid = channel->cid;
    for(int i = 0 ;i<imTabWidget->count() ; i++)
    {
        if(imTabWidget->tabText(i) == cid)
        {
         //   widget = qobject_cast<IMWidget *>parent->imTabWidget->widget(i);
            return;
        }
    }
    IMWidget *widget = new IMWidget(this,cid);
    QString name = cid.left( cid.indexOf("@") );             //12.31
  //  widget = new IMWidget(textchannel,tConns,this);
    imTabWidget->addTab(widget,name);                     //12.31
    qDebug()<<"addTab!!!!!!!";
    actionIM->setEnabled(true);
    showIMSlot();
    imTabWidget->setCurrentWidget(widget);
    imcm->createIMWidgetSlot(channel);
   // imchannels.append(this);

    connect(channel,SIGNAL(showIMWidgetSignal(QString)),widget,SLOT(showIMWidgetSlot(QString)));
    connect(channel,SIGNAL(clearIMChannelSignal(QString)),this,SLOT(clearIMChannelSlot(QString)));
    //connect(channel->textInterface(),SIGNAL(SendError(uint,uint,uint,QString)),widget,SLOT(SendErrorSlot(uint,uint,uint,QString)));
    connect(widget,SIGNAL(sendTextSignal(QString)),channel,SLOT(sendTextSlot(QString)));
    connect(this,SIGNAL(closeConfSignal()),channel,SLOT(closeTextChannel()));
    connect(channel,SIGNAL(clearIMChannelSignal(QString)),this,SLOT(clearIMChannelSlot(QString)));

}
*/
void MainWindow::setupGui()
{
	setWindowFlags( Qt::WindowStaysOnTopHint );

    //this->setMaximumSize(640,360);
    //this->setMinimumSize(640,360);
    /*******************************1.16 dhx***********************************/
 /*   QFont font;
    font.setPointSize(10);
    this->setFont(font);
*/
    this->statuslabel = new QLabel();
    statuslabel->setAlignment(Qt::AlignCenter);
    statuslabel->setMinimumWidth(QApplication::desktop()->width());
 //   this->statusBar()->addWidget(statuslabel,0);
	this->statuslabel->setText(RunningProfile::getInstance()->username().c_str()+
		QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！"));
 //   int desktopwidth = 620;//QApplication::desktop()->availableGeometry().width();
//    int desktopheight =390;//QApplication::desktop()->availableGeometry().height();
//    int width = QApplication::desktop()->availableGeometry().width();
    //int height =QApplication::desktop()->availableGeometry().height();

   // this->setMaximumSize(desktopwidth,desktopheight);
  //  this->setMinimumSize(desktopwidth,desktopheight);
  //  this->setGeometry((width-desktopwidth)/2,80,desktopwidth,desktopheight);
 //   this->showMaximized();
//    this->setWindowState(Qt::WindowFullScreen);
    /*******************************1.16 dhx end***********************************/

    meetingToolButton = new QToolButton();
    QIcon icon;
    icon.addFile(":/image/users_chat.png");
    meetingToolButton->setIcon(icon);
    QSize iconsize;
    iconsize.setHeight(24);
    iconsize.setWidth(24);
    meetingToolButton->setIconSize(iconsize);
    meetingToolButton->setToolTip(QString::fromLocal8Bit("视频会议"));
    menu = new QMenu("Meeting");
    menu->addAction(this->actionConference);
    menu->addAction(this->actionMember);
    meetingToolButton->setMenu(menu);
    this->toolBar->insertWidget(this->actionSipCalling,meetingToolButton);//addWidget(meetingToolButton);
    connect(meetingToolButton,SIGNAL(clicked()),meetingToolButton,SLOT(showMenu()));


	//先初始化conferenceManager-----------add by lzb
	_confManager = new ConferenceManager( );
	MediaManager::getInstance()->setConferenceManager( _confManager );

    conflistwidget = new ConferenceListWidget(this);
	connect( conflistwidget , SIGNAL(quitConfSIGNAL()),this,SLOT(quitConference()));
    QGridLayout *cglayout = new QGridLayout(this);
    cglayout->addWidget(conflistwidget);
    cglayout->setMargin(0);                                     //12.31
    this->conferencePage->setLayout(cglayout);
   // MemberListWidget = new MemberListWidget(this);

    dListWidget = new DeviceListWidget();
    QGridLayout *dglayout = new QGridLayout(this);
    dglayout->addWidget(dListWidget);
    dglayout->setMargin(0);
    this->deviceListPage->setLayout(dglayout);
	connect(this->actionDevice,SIGNAL(triggered()),dListWidget,SLOT(ReadConfig()));//连接设备界面

    aListWidget = new AddressListWidget();
    QGridLayout *aglayout = new QGridLayout(this);
    aglayout->addWidget(aListWidget);
    aglayout->setMargin(0);
    aglayout->setSpacing(0);
    this->addressListPage->setLayout(aglayout);
    connect(aListWidget,SIGNAL(callingFromAddressSignal(QString)),this,SLOT(callingFromAddressSlot(QString)));

    helpWidget = new HelpWidget(this);
    QGridLayout *hglayout = new QGridLayout(this);
    hglayout->addWidget(helpWidget);
    hglayout->setMargin(0);
    hglayout->setSpacing(0);
    this->helpPage->setLayout(hglayout);

    sipCallWidget = new SipCallWidget(this);
    QGridLayout *sglayout = new QGridLayout(this);
    sglayout->addWidget(sipCallWidget);
    sglayout->setMargin(0);
    sglayout->setSpacing(0);
    this->sipCallingPage->setLayout(sglayout);

    //mouseSimWidget = new MouseSimulateWidget();
    //QGridLayout *mouglayout = new QGridLayout(this);
    //mouglayout->addWidget(mouseSimWidget);
    //mouglayout->setMargin(0);
    //mouglayout->setSpacing(0);
    //this->mousePage->setLayout(mouglayout);

    /*iWidget = new IMChannel(this,tConns);
    QGridLayout *iglayout = new QGridLayout(this);
    iglayout->addWidget(iWidget);
    iglayout->setMargin(0);
    this->imPage->setLayout(iglayout);*/

    this->actionIM->setEnabled(false);
	this->actionMouseSim->setEnabled(false);

    this->mainStackedWidget->setCurrentWidget(this->conferencePage);



	//会议面板
	connect( _confManager , SIGNAL(cvConfListReceivedSignal(const ConferenceVector&) ) , 
		conflistwidget, SLOT(cvConfListReceivedSlot( const ConferenceVector&)) );
	connect( _confManager , SIGNAL(cvMemberListReceivedSignal( const MemberVector& , const std::string& )),
		conflistwidget , SLOT( cvMemberListReceivedSlot( const MemberVector&, const std::string& )));
	connect( _confManager , SIGNAL(cvJoinRejectionSignal( const std::string& , const std::string&)) , 
		conflistwidget , SLOT(cvJoinRejectionSlot( const std::string& , const std::string&)) );


	connect( _confManager , SIGNAL( cvMemberLoginSignal( const MemberInfo& )) ,
		conflistwidget, SLOT( cvMemberLoginSlot( const MemberInfo&)));
	/*connect( _confManager , SIGNAL( cvMemberLogoutSignal( const std::string& )) ,
		conflistwidget, SLOT( cvMemberLogoutSlot( const std::string&)));*/

	connect( _confManager , SIGNAL( cvRemoteTerminalAskForMediaSignal( const std::string&, const std::string&)),
		conflistwidget, SLOT(cvRemoteTerminalAskForMediaSlot( const std::string&, const std::string&) ) );

	
	connect(_confManager,SIGNAL(RecvSaviSignal(bool)),this ,SLOT(HandleRecvSAVANotifySlot(bool)));

	//发言权控制  zhongBao.liang 2011-09-05

	//举手
	connect(_confManager,SIGNAL(cvMemberHandUpSignal(QString)),conflistwidget,SLOT(onMemberHandUp(QString)));
	connect(_confManager,SIGNAL(cvMemberHandDownSignal(QString)),conflistwidget,SLOT(onMemberHandDown(QString)));

	
	connect(_confManager,SIGNAL(cvSetMemberWindowToFirstPositionSignal(QString)),conflistwidget,SLOT(onSetMemberWindowToFirstPosition(QString)));

	//Terminal
	connect( _confManager , SIGNAL( cvTerminalListReceivedSignal( const TerminalVector& , const std::string&)),
		conflistwidget , SLOT( cvTerminalListReceivedSlot( const TerminalVector& , const std::string&)) );
	connect( _confManager , SIGNAL( cvTerminalLoginSignal(const TerminalVector&)) , 
		conflistwidget , SLOT( cvTerminalLoginSlot(const TerminalVector&)) );
	connect( _confManager , SIGNAL( cvTerminalLogoutSignal(const QString&)) , 
		conflistwidget , SLOT( cvTerminalLogoutSlot(const QString&)) );
	connect( _confManager , SIGNAL(cvUpgradeTerminalInfoSignal(const TerminalInfo&)) , 
		conflistwidget , SLOT( cvUpgradeTerminalInfo(const TerminalInfo&)) );
	connect( _confManager , SIGNAL(cvTermianlCountChangeNotify(int)) , 
		conflistwidget ,SLOT(TermianlCountChangeNotify(int)));

	// Tx 录制相关
	qRegisterMetaType<RtpStatItem>("RtpStatItem"); 
	connect( _confManager , SIGNAL(receiveRtpStatSignal(const RtpStatItem&)) , 
		conflistwidget ,SLOT(receiveRtpStatSlot(const RtpStatItem&)));
	qRegisterMetaType<RecStatItem>("RecStatItem"); 
	connect( _confManager , SIGNAL(receiveRecStatSignal(const RecStatItem&)) , 
		conflistwidget ,SLOT(receiveRecStatSlot(const RecStatItem&)));
	connect( _confManager , SIGNAL(streamMediaReceiveStartedSignal(QString)) , 
		conflistwidget ,SLOT(streamMediaReceiveStartedSlot(QString)));
	connect( _confManager , SIGNAL(streamMediaReceiveStoppedSignal(QString)) , 
		conflistwidget ,SLOT(streamMediaReceiveStoppedSlot(QString)));

	
	connect(_confManager,SIGNAL(cvModifyStopMyAudioState(bool)),conflistwidget,SIGNAL(ModifyStopMyAudioState(bool)));

	connect(this,SIGNAL(LogOutSignal()),_confManager,SLOT(TerminalLogOutSlot()));


	_qtLoginWidget = new LoginWidget( this );
	connect(_qtLoginWidget, SIGNAL(TerminalLoginSignal(QString,QString,QString)),this,SLOT(TerminalLoginSlot(QString,QString,QString)));
	connect( _qtLoginWidget , SIGNAL(enterMainWindowSignal()) , this , SLOT( enterMainWindowSlot()));
	_qtLoginWidget->show();
	
	//终端到服务器时间测试相关连接
	connect(this->dListWidget,SIGNAL(serverTestSignal()),_confManager,SLOT(StartServerTsetSlot()));
	connect(this->_confManager,SIGNAL(cvServerTestResultSignal(bool,int )),this->dListWidget,SLOT(recvServerTestResultSlot(bool ,int )));

	//_qtSuspendedWidget = NULL;

     qDebug()<<"ADFAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	 //隐藏地址簿和sip呼叫的按钮
	 this->actionSipCalling->setVisible(false);
	 this->actionAddressList->setVisible(false);

}

void MainWindow::createActions()
{
    connect(this->actionSipCalling,SIGNAL(triggered()),this,SLOT(showSipCallingSlot()));
    connect(this->actionConference,SIGNAL(triggered()),this,SLOT(showConferenceListSlot()));
    connect(this->actionMember,SIGNAL(triggered()),this,SLOT(showMemberListSlot()));
    connect(this->actionAddressList,SIGNAL(triggered()),this,SLOT(showAddressListSlot()));
    connect(this->actionDevice,SIGNAL(triggered()),this,SLOT(showDeviceListSlot()));
    connect(this->actionIM,SIGNAL(triggered()),this,SLOT(showIMSlot()));
#ifdef _DEBUG
    connect(this->actionQuit,SIGNAL(triggered()),this,SLOT(quitSystemSlot()));
#else
	connect(this->actionQuit,SIGNAL(triggered()),this,SLOT(quitSystemAndShutdownSlot()));
#endif
    connect(this->actionLogOut,SIGNAL(triggered()),this,SLOT(logOutSlot()));
    connect(this->actionHelp,SIGNAL(triggered()),this,SLOT(showHelpSlot()));        //12.25
    connect(this->actionMouseSim,SIGNAL(triggered()),this,SLOT(showMouseSlot()));
	connect(this->actionRestart, SIGNAL(triggered()),this,SLOT(restartSystemSlot()) );
}

void MainWindow::callingFromAddressSlot(QString sip)
{
    this->showSipCallingSlot();
    this->sipCallWidget->CallingOut(sip);
}

void MainWindow::showSipCallingSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->sipCallingPage);
}

void MainWindow::showConferenceListSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->conferencePage);

}

void MainWindow::showMemberListSlot()
{
     this->mainStackedWidget->setCurrentWidget(this->memberPage);
}

void MainWindow::showAddressListSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->addressListPage);
}

void MainWindow::showDeviceListSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->deviceListPage);
}

void MainWindow::showIMSlot()
{
    //this->imTabWidget->addTab(iWidget,"im");
    this->mainStackedWidget->setCurrentWidget(this->imPage);
}

void MainWindow::showHelpSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->helpPage);
}

void MainWindow::showMouseSlot()
{
    this->mainStackedWidget->setCurrentWidget(this->mousePage);
}

void MainWindow::quitConference()
{
    qDebug()<<"quit conference";
    emit closeConfSignal();

    this->mainStackedWidget->setCurrentWidget(this->conferencePage);
    //int i = confTabWidget->currentIndex();
    this->memberPage->setEnabled(false);       //2.17
    //confTabWidget->setCurrentIndex(0);          //2.17

//	QString username = RunningProfile::getInstance()->getUserName().c_str();
//    this->statuslabel->setText(username+QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！"));//setWindowTitle(mUsername+QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！"));

	QString windowTitleName = QString::fromLocal8Bit( RunningProfile::getInstance()->username().c_str() )
		+QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！");
	this->setWindowTitle( windowTitleName );
    //confTabWidget->currentWidget()->setEnabled(false);

   // confTabWidget->removeTab(i);//removeWidget(mWidget);

   // qDebug()<<"quit conference";

    /*disconnect(tCM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),this,
            SLOT(onTCMReady(Tp::PendingOperation *)));*/

    //tConns->requestDisconnect();

    for(int i = 0 ; i<this->imTabWidget->count();i++)
    {
        this->imTabWidget->removeTab(i);
    }

	//如果不是注销，那需要重新获取会议列表
	if(!islogout)
	{
		_confManager->startConference( RunningProfile::getInstance()->username() , 
			RunningConfig::Instance()->GlobalService().toStdString());
	}


}

void MainWindow::quitSystemSlot()
{
	QString infoMsg;
	if(this->memberPage->isEnabled())//confTabWidget->count()>1)     //2.17
	{
		infoMsg = QString::fromLocal8Bit("您正在参加会议，确定退出系统？");

		if((QMessageBox::warning(this,tr("CoolView"),infoMsg,QMessageBox::Yes|QMessageBox::No))
			!=QMessageBox::Yes)
		{
			return;
		}
	}else
	{
		infoMsg = QString::fromLocal8Bit("确定退出系统？");
		int reply = QMessageBox::warning(this,tr("CoolView"),infoMsg , QMessageBox::Yes|QMessageBox::No);
		if(reply != QMessageBox::Yes)
		{
			return ;
		}
	}
	int result = quitSystem();
	if( result==-1 )
		return;

	close();
}

void MainWindow::quitSystemAndShutdownSlot()
{
	QString infoMsg;
	if(this->memberPage->isEnabled())//confTabWidget->count()>1)     //2.17
	{
		infoMsg = QString::fromLocal8Bit("您正在参加会议，确定退出系统并关机？");
		if((QMessageBox::warning(this,tr("CoolView"),infoMsg,QMessageBox::Yes|QMessageBox::No))
			!=QMessageBox::Yes)
		{
			return ;
		}
		conflistwidget->quitConference();
		memberPage->setEnabled(false);
	}else
	{
		infoMsg = QString::fromLocal8Bit("确定退出系统并关机？");
		int reply = QMessageBox::warning(this,tr("CoolView"),infoMsg , QMessageBox::Yes|QMessageBox::No);
		if(reply != QMessageBox::Yes)
		{
			return ;
		}
	}

	int result = quitSystem();
	if( result==-1)
		return;
	system("shutdown -s -f -t 0");
	//close();
}

void MainWindow::logOutSlot()
{
    if(this->memberPage->isEnabled())//this->confTabWidget->count()>1)      //2.17
    {
        if(QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("您正在参加会议，确定要注销吗？"),QMessageBox::Yes|QMessageBox::No)
			!=QMessageBox::Yes)
        {
			return;
        }
    }else
    {
        if(QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("确定要注销吗？"),QMessageBox::Yes|QMessageBox::No)
			!=QMessageBox::Yes)
			return;
    }

	this->logout();
}

/******************************************coferencechannel******************************************/

/******************************************device******************************************/

/******************************************device******************************************/

int MainWindow::quitSystem()
{
	//如果正在进行会议则先退出会议
	if(this->memberPage->isEnabled())//confTabWidget->count()>1)     //2.17
	{
		conflistwidget->quitConference();
		memberPage->setEnabled(false);
	}

	//注销用户
	//_qtLoginWidget->logoutSlot();
	this->logout();
	_quitProcess = true;
	this->_qtLoginWidget->hide();

	//SAFE_DELETE(_qtSuspendedWidget);	//删除悬浮窗口
	return 0;
}

void MainWindow::enterMainWindowSlot()
{
	islogout=false;
	QString windowTitleName = QString::fromLocal8Bit( RunningProfile::getInstance()->username().c_str() )
		+QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！");
	this->setWindowTitle( windowTitleName );
	this->mainStackedWidget->setCurrentWidget(this->conferencePage);

	//if( _qtSuspendedWidget==NULL )
	//	_qtSuspendedWidget = new SuspendedWindow( this );

	//_qtSuspendedWidget->raise();
	//_qtSuspendedWidget->show();

	qDebug( "@Enter Main Window\n" );


	////IM面板
	//connect( _confManager , SIGNAL(cvTextMsgReceivedSignal(const std::string& ,const std::string&) ) , 
	//	_qtIMWidget , SLOT(cvTextMsgReceivedSlot( const std::string& , const std::string& )));
	//connect( _confManager , SIGNAL(cvMemberListReceivedSignal( const MemberVector& , const std::string& )),
	//	_qtIMWidget , SLOT( cvMemberListReceivedSlot( const MemberVector&, const std::string& )));

	//_confManager->startConference( RunningProfile::getInstance()->getUserName() , 
	//	RunningConfig::getInstance()->GlobalService().toStdString());

	////登录IM...zhenHua.sun
	//string userAccount = RunningProfile::getInstance()->getUserName() + "@" + JABBER_SERVER;
	//string password = RunningProfile::getInstance()->getPassword();
	//QString qUserAccount = QString::fromStdString(userAccount).toLower();
	//IMManager::getInstance()->Login( qUserAccount , password.c_str() );
	//connect( IMManager::getInstance() , SIGNAL(jabberMessageReceivedSignal(const QString &, const QString &, const QString &, const QString &, const QString &)),
	//	this , SLOT(jabberMessageReceivedSlot( const QString &, const QString &, const QString &, const QString &, const QString & )) );

	//请求获取地址列表
	//QString username = RunningProfile::getInstance()->getUserName().c_str();
	//QString password = RunningProfile::getInstance()->getPassword().c_str();
	//QUrl url("http://202.38.192.215:8080/ConferenceControl-war/user/login.action?username="+username+"&password="+password);
	//nam->get(QNetworkRequest(url));

	if (CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_TX)
	{
		startWeblibUploader();
	}
	
	//this->hide();
	showMinimized();
	_panelViesibleState=true;
	//showMaximized();
	//showNormal();
}

void MainWindow::closeEvent( QCloseEvent * event )
{
	if( _quitProcess )
	{
		//关闭相关的服务进程...zhenHua.sun 2011-11-25
		stopReleativeServices();
		event->accept();
		
	}else
	{
#ifdef _DEBUG
		quitSystemSlot();
#else
		quitSystemAndShutdownSlot();
#endif
		event->ignore();
	}
}

void MainWindow::changeEvent( QEvent * event )
{
	//if( event->type()== QEvent::WindowStateChange )
	//{
	//	//对最小化进行处理
	//	if( QWidget::windowState() == Qt::WindowMinimized )
	//	{
	//		_qtSuspendedWidget->setVisible(true);
	//		_qtSuspendedWidget->raise();
	//	}else
	//	{
	//	}
	//}
}

int MainWindow::logout()
{
	
	islogout=true;
	if(this->memberPage->isEnabled())//this->confTabWidget->count()>1)      //2.17
	{
		conflistwidget->quitConference();
	}
	else
	{
		conflistwidget->clearConfList();
	}

	LogOutSignal();

	//RunningProfile::getInstance()->SaveFile();

	//注销用户
	this->_qtLoginWidget->logoutSlot();
	this->hide();
	//if( _qtSuspendedWidget )
	//{
	//	_qtSuspendedWidget->hide();
	//}

	if (CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_TX)
	{
		stopWeblibUploader();
	}

	//显示登录框
	this->_qtLoginWidget->show();
	this->_qtLoginWidget->raise();

	//清理媒体流进程
	//MediaManager::getInstance()->ClearStreamedMedia();

	return 0;
}

void MainWindow::restartSystemSlot()
{
	QString infoMsg;
	if(this->memberPage->isEnabled())//confTabWidget->count()>1)     //2.17
	{
		infoMsg = QString::fromLocal8Bit("您正在参加会议，确定重启电脑？");

		if((QMessageBox::warning(this,tr("CoolView"),infoMsg,QMessageBox::Yes|QMessageBox::No))
			!=QMessageBox::Yes)
		{
			return;
		}
	}else
	{
		infoMsg = QString::fromLocal8Bit("确定重启电脑？");
		int reply = QMessageBox::warning(this,tr("CoolView"),infoMsg , QMessageBox::Yes|QMessageBox::No);
		if(reply != QMessageBox::Yes)
		{
			return ;
		}
	}

	restartSystem();
}

void MainWindow::restartSystem()
{
	QString infoMsg;
	if(this->memberPage->isEnabled())//confTabWidget->count()>1)     //2.17
	{
		conflistwidget->quitConference();
		memberPage->setEnabled(false);
	}

	int result = quitSystem();
	if( result==-1)
		return;
	system("shutdown -r -f -t 0");
	close();
}


void MainWindow:: TerminalLoginSlot(QString ipv4,QString ipv6,QString myVersion)
{
	int screenNum = QApplication::desktop()->numScreens();
	_confManager->TerminalLogin( RunningProfile::getInstance()->username() , 
		RunningConfig::Instance()->GlobalService().toStdString(),ipv4,ipv6,myVersion,screenNum);

}

void MainWindow:: HandleRecvSAVANotifySlot(bool res)
{
   if (res)
   {
	   SaviAction->setText(QString::fromLocal8Bit("通过真实源地址认证"));
   } 
   else
   {
	   SaviAction->setText(QString::fromLocal8Bit("没有通过真实源地址认证"));
   }
}

void MainWindow::ShowMinimizedMainWindow()
{
	this->showMinimized();
	_panelViesibleState=true;
	PanelVisibleStateUpdateNotify();
}

void MainWindow::HideMainWindow()
{
	this->hide();
	_panelViesibleState=false;
	PanelVisibleStateUpdateNotify();
}

void MainWindow:: PanelVisibleStateUpdateNotify( const int controllerIndex/*=-1*/)
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out<<TELE_PanelState;
	out<<_panelViesibleState;

	if( _teleControllerProxy )
	{
		_teleControllerProxy->TeleInfo( TELE_PanelState , controllerIndex, output_array );
	}


}

void MainWindow:: UpgradeMonitorCofInfo(MonitorConfigInfo monitorConfigInfo)
{
  //TODO:添加系统监控参数
	/*dListWidget->CPUUsageShowText->setText(QString::number(monitorConfigInfo.cpuUsage));
	dListWidget->MemoryUsageShowText->setText(QString::number(monitorConfigInfo.totalMemory));
	dListWidget->AvailableMemoryUsageShowText->setText(QString::number(monitorConfigInfo.availableMemory));

	if(monitorConfigInfo.netWorkOutBandWidth >1024)
	{
		monitorConfigInfo.netWorkOutBandWidth/=1024;
		dListWidget->OutBandWidthShowText->setText(QString("%1MB/s").arg(monitorConfigInfo.netWorkOutBandWidth, 0 , 'f', 2));
	}
	else
	{
		dListWidget->OutBandWidthShowText->setText(QString("%1KB/s").arg(monitorConfigInfo.netWorkOutBandWidth, 0 , 'f', 2));
	}
	if(monitorConfigInfo.netWorkInBandWidth >1024)
	{
		monitorConfigInfo.netWorkInBandWidth/=1024;
		dListWidget->InBandWidthShowText->setText(QString("%1MB/s").arg(monitorConfigInfo.netWorkInBandWidth, 0 , 'f', 2));
	}
	else
	{
		dListWidget->InBandWidthShowText->setText(QString("%1KB/s").arg(monitorConfigInfo.netWorkInBandWidth, 0 , 'f', 2));
	}*/

}


void MainWindow::loadMonitorConfig()
{
	/*MonitorConfig::getInstance()->loadConfig();
	
	dListWidget->CPUUsageWarningEdit->setText(QString::number((MonitorConfig::getInstance()->CpuUsage())));
	dListWidget->MemoryUsageWarningEdit->setText(QString::number((MonitorConfig::getInstance()->MemoryUsage())));*/


}