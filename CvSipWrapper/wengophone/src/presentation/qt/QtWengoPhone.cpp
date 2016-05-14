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

#include "QtWengoPhone.h"

#include "ui_WengoPhoneWindow.h"

#include <presentation/PFactory.h>
#include <presentation/qt/chat/QtChatWindow.h>

#include <cutil/global.h>

#include "QtSystray.h"
#include "QtHttpProxyLogin.h"
#include "QtDialpad.h"
#include "QtIdle.h"
#include "QtLanguage.h"
#include "QtToolBar.h"
#include "QtBrowserWidget.h"
#include "QtIMAccountMonitor.h"
#include "callbar/QtCallBar.h"
#include "callbar/QtPhoneComboBox.h"
#include "contactlist/QtContactList.h"
#include "filetransfer/QtFileTransfer.h"
#include "history/QtHistoryWidget.h"
//#include "imaccount/QtIMAccountManager.h"			精简zhenHua.sun 2010-09-03
#include "phonecall/QtContactCallListWidget.h"
#include "phonecall/QtPhoneCall.h"
#include "profile/QtProfileDetails.h"
#include "profilebar/QtProfileBar.h"
//#include "profilebar/QtIMProfileWidget.h"			精简zhenHua.sun 2010-09-03
#include "statusbar/QtStatusBar.h"
#include "webservices/sms/QtSms.h"
#include "conference/QtConferenceRoomWidget.h"
#include "conference/QtInstantConferenceWidget.h" //Add by hexianfu
#include "conference/QtToolWidget.h"	//zhenHua.sun 2010-08-19
#include "profile/QtUserProfileHandler.h"
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	#include "webdirectory/QtWebDirectory.h"
#endif

#include <control/CWengoPhone.h>
#include <control/history/CHistory.h>
#include <control/phoneline/CPhoneLine.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/WengoPhone.h>
#include <model/commandserver/ContactInfo.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/connect/ConnectHandler.h>
#include <model/contactlist/ContactList.h>
#include <model/contactlist/Contact.h>
#include <model/history/History.h>
#include <model/phonecall/PhoneCall.h>
#include <model/phonecall/SipAddress.h>
#include <model/phonecall/ConferenceCall.h>
#include <model/phoneline/IPhoneLine.h>
#include <model/profile/AvatarList.h>
#include <model/profile/UserProfile.h>
#include <model/account/EnumSipLoginState.h>

#include <imwrapper/EnumPresenceState.h>

#include <qtutil/CloseEventFilter.h>
#include <qtutil/Widget.h>
#include <qtutil/SafeConnect.h>
#include <qtutil/LanguageChangeEventFilter.h>

#include <cutil/global.h>
#include <thread/Thread.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <webcam/WebcamDriver.h>
#include "ExportUtil.h"

#include <QtGui/QtGui>

#include <RunningProfile.h>
#include <SessionManager.h>
/**
* Add by: hexianfu
* for the telecontroller message analysis
* 2009-10-21
*/
#include "control/MessageFrame.h"
#include "login/ILogin.h"
#include "login/QtAddSIPAccount.h"
#include "login/QtLoginDialog.h"
#include "login/QtLogAccount.h"
#include "login/QtAddAccount.h"
#include <QtCore/QTimer>
/***********************************/

#include <sound/VolumeControl.h>
#include <sound/AudioDevice.h>

#include "conference/QtToolWidget.h"


//zhenHua.sun  2010-10-26
#ifdef DBUS_ENABLE
#include <dbus/core/service/CvCoreService.h>
#include <dbus/telecontroller/CvTelecontrollerClient.h>		//zhenHua.sun 2010-11-02
#endif


#if defined(OS_MACOSX)
	#include <Carbon/Carbon.h>
#endif

static const char * CSS_DIR = "css";

using namespace std;

QtWengoPhone::QtWengoPhone(CWengoPhone & cWengoPhone)
	: QObjectThreadSafe(NULL),
	_cWengoPhone(cWengoPhone) {

		QtWengoPhone::UI_State_Code=UI_S_INIT_BEGIN;//Add By LZY 2010-09-19

		//-------Add By LZY 2010-09-27-------
		//   -   邀请信息储存队列相关变量初始化
		QtWengoPhone::InviteList=(InviteListNode *)malloc(sizeof(InviteListNode)*5);
		for (int LoopVar=0;LoopVar<5;++LoopVar)
		{
			QtWengoPhone::InviteList[LoopVar].IdentifyNum=0;
			QtWengoPhone::InviteList[LoopVar].MsgPointer=NULL;
		}
		QtWengoPhone::InviteListCount=5;
		QtWengoPhone::InviteListNextIdentify=1;
		QtWengoPhone::InviteListLockNum=0;
		//-------End of Add By LZY-----------

	_wengoPhoneWindow = NULL;
	_qtSystray = NULL;
	_qtSms = NULL;
	_qtContactList = NULL;
	_qtHistoryWidget = NULL;
	_qtContactCallListWidget = NULL;
	_qtIdle = NULL;
	_qtLanguage = NULL;
	_qtStatusBar = NULL;
	_qtFileTransfer = NULL;
	_qtBrowserWidget = NULL;
	_chatWindow = NULL;
	_activeTabBeforeCall = NULL;
	m_qtConferenceRoomWidget = NULL;

	_dialTabWidget =new QTabWidget();    //huixin.du 2010-09-15
	_toolButton = new QPushButton();
	_qtToolWidget = NULL;

	_qtLoginDialog = NULL;

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	_qtWebDirectory = NULL;
#endif

	NetworkProxyDiscovery::getInstance().proxyNeedsAuthenticationEvent +=
		boost::bind(&QtWengoPhone::proxyNeedsAuthenticationEventHandler, this, _1, _2);
	NetworkProxyDiscovery::getInstance().wrongProxyAuthenticationEvent +=
		boost::bind(&QtWengoPhone::wrongProxyAuthenticationEventHandler, this, _1, _2);

	//Check if the event has not already been sent
	if (NetworkProxyDiscovery::getInstance().getState() ==
		NetworkProxyDiscovery::NetworkProxyDiscoveryStateNeedsAuthentication) {
		proxyNeedsAuthenticationEventHandler(NetworkProxyDiscovery::getInstance(),
			NetworkProxyDiscovery::getInstance().getNetworkProxy());
	}

	qRegisterMetaType<QVariant>("QVariant");

	//Initialize libwebcam for the Qt thread.
	WebcamDriver::apiInitialize();
	////

	initUi();

	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtWengoPhone::initThreadSafe, this));
	postEvent(event);

	SAFE_CONNECT_RECEIVER(this,SIGNAL(ActionAcceptInviteSignal(int,int)),this,SLOT(RemoveInviteMsg(int,int)));//Add By LZY 2010-09-28 绑定响应AcceptInvite动作Signal的函数
	SAFE_CONNECT_RECEIVER(this,SIGNAL(ActionCreateMeetingSignal(QString,QString,int,int,QString)),this,SLOT(CreateMeeting(QString,QString,int,int,QString)));//Add By LZY 2010-09-28 绑定响应CreateMeeting动作Signal的函数

#ifdef DBUS_ENABLE
	CvCoreService::getInstance()->setQtWengoPhone( this );
#endif
}

QtWengoPhone::~QtWengoPhone() {

	disconnect(this,SIGNAL(ActionAcceptInviteSignal(int,int)),this,SLOT(RemoveInviteMsg(int,int)));//Add By LZY 2010-09-28 解绑响应AcceptInvite动作Signal的函数
	disconnect(this,SIGNAL(ActionCreateMeetingSignal(QString,QString,int,int,QString)),this,SLOT(CreateMeeting(QString,QString,int,int,QString)));//Add By LZY 2010-09-28 解绑响应CreateMeeting动作Signal的函数

	free(QtWengoPhone::InviteList);//Add By LZY 2010-09-27
}

void QtWengoPhone::initUi() {
	// Init parts of the UI which do not rely on Control layer
	QApplication::setQuitOnLastWindowClosed(false);

	loadStyleSheets();

	//Translation
	_qtLanguage = new QtLanguage(this);

	_wengoPhoneWindow = new QMainWindow(NULL);
	_wengoPhoneWindow->setWindowFlags(Qt::WindowStaysOnTopHint);  //add by dhx 2010-10-27
	LANGUAGE_CHANGE(_wengoPhoneWindow);

	_ui = new Ui::WengoPhoneWindow();
	_ui->setupUi(_wengoPhoneWindow);

#ifndef CUSTOM_ACCOUNT
	_ui->actionShowWengoAccount->setVisible(false);
#endif

#ifdef OS_MACOSX
	fixMacOSXMenus();
#endif

#ifdef OS_LINUX
	std::string data = AvatarList::getInstance().getDefaultAvatarPicture().getData();
	QPixmap defaultAvatar;
	defaultAvatar.loadFromData((uchar*) data.c_str(), data.size());
	_wengoPhoneWindow->setWindowIcon(QIcon(defaultAvatar));
#endif

	//zhenHua.sun 2010-10-20
	_wengoPhoneWindow->setWindowIcon(QIcon(":/coolview3/application.ico"));

	//Install the close event filter
	CloseEventFilter * closeEventFilter = new CloseEventFilter(this, SLOT(closeWindow()));
	_wengoPhoneWindow->installEventFilter(closeEventFilter);

	//QtCallBar
	_ui->callBar->getQtPhoneComboBox()->setQtWengoPhone(this);

	//QtToolBar
	_qtToolBar = new QtToolBar(*this, _ui, _wengoPhoneWindow);

	//phoneComboBox
	SAFE_CONNECT(_ui->callBar, SIGNAL(phoneComboBoxClicked()), SLOT(phoneComboBoxClicked()));

	//Buttons initialization
	initCallButtons();

	//QtStatusBar
	_qtStatusBar = new QtStatusBar(this, _ui->statusBar);

	/****************SipCall  huixin.du 2010-09-15**********************/
	//_ui->dialWidget->layout()->addWidget(_dialTabWidget);

    _toolButton->setMaximumSize(30,30);  //控制按钮大小
    _toolButton->setMinimumSize(30,30);

	QIcon *icon = new QIcon(":/coolview3/setting.bmp"); //按钮添加icon
    QSize *isize = new QSize(25,25);
    _toolButton->setIcon(*icon);
    _toolButton->setIconSize(*isize);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(_toolButton);
	vLayout->addSpacing(_dialTabWidget->height()+300);      //add by huixin.du 2010-09-06
	vLayout->setMargin(0);                     //add by huixin.du 2010-09-06
    mainLayout->addLayout(vLayout);
	mainLayout->addWidget(_dialTabWidget);
	mainLayout->setMargin(0);                 //add by huixin.du 2010-09-06
	_ui->dialDialpad->setLayout(mainLayout);
    _ui->dialWidget->hide();


    /******************************************************************/

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	_qtWebDirectory = new QtWebDirectory(0);
#endif

	installQtBrowserWidget();

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	mainWindowGeometry(config);


	//隐藏menubar  zhenHua.sun 2010-08-19
	_ui->menuBar->hide();

	//隐藏statusbar  huixin.du 2010-09-06
	_ui->statusBar->hide();
}

void QtWengoPhone::initThreadSafe() {
	// The code in this method relies on the Control layer
	_qtSystray = new QtSystray(this);

	updatePresentation();
}

void QtWengoPhone::loadStyleSheets() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	QDir dir(QString::fromStdString(config.getResourcesDir()) + CSS_DIR);

	QStringList filters;
	filters << "*.css";
	QStringList cssList;
	Q_FOREACH(QFileInfo fileInfo, dir.entryInfoList(filters)) {
		QString path = fileInfo.absoluteFilePath();
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			LOG_WARN("Can't open " + path.toStdString());
			continue;
		}
		QByteArray content = file.readAll();
		cssList << QString::fromUtf8(content);
	}
	QString styleSheet = cssList.join("\n");
	qApp->setStyleSheet(styleSheet);
}

void QtWengoPhone::mainWindowGeometry(Config & config) {
	//default position and size given by Qt
	QPoint defaultPos = _wengoPhoneWindow->pos();
	QSize defaultSize = _wengoPhoneWindow->size();

	//int profileWidth = config.getProfileWidth();
	//if (profileWidth == 0) {
	//	// Make sure the toolbar is fully visible
	//	profileWidth = qMax(
	//		_ui->toolBar->sizeHint().width(),
	//		_wengoPhoneWindow->sizeHint().width()
	//		);
	//}

	//int profileHeight = config.getProfileHeight();

	//设置全屏 zhenHua.sun 2010-08-18
	int x = QApplication::desktop()->width();
	int y = QApplication::desktop()->height();
	int profileWidth = x*0.9;
	int profileHeight = y*0.9;

	//Main window size and position saved
	_wengoPhoneWindow->resize(profileWidth, profileHeight);
	_wengoPhoneWindow->showMaximized();
	//_wengoPhoneWindow->move(QPoint(config.getProfilePosX(), config.getProfilePoxY()));
	_wengoPhoneWindow->move( 0 , 0) ;	//左上角

	//tests if the Wengophone is visible, if not sends it back to its default position and size
	QDesktopWidget* desktop = QApplication::desktop();
	if (desktop->screenNumber(_wengoPhoneWindow) == -1) {
		LOG_DEBUG("Main window is NOT visible !!");
		_wengoPhoneWindow->resize(defaultSize);
	 	_wengoPhoneWindow->move(defaultPos);
	}
}

QWidget * QtWengoPhone::getWidget() const {
	return _wengoPhoneWindow;
}

QtProfileBar * QtWengoPhone::getQtProfileBar() const {
	return _ui->profileBar;
}

QtBrowserWidget * QtWengoPhone::getQtBrowserWidget() const {
	return _qtBrowserWidget;
}

CWengoPhone & QtWengoPhone::getCWengoPhone() const {
	return _cWengoPhone;
}

void QtWengoPhone::setQtSms(QtSms * qtSms) {
	_qtSms = qtSms;
}

QtSms * QtWengoPhone::getQtSms() const {
	return _qtSms;
}

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
QtWebDirectory * QtWengoPhone::getQtWebDirectory() const {
	return _qtWebDirectory;
}
#endif

QtToolBar & QtWengoPhone::getQtToolBar() const {
	return *_qtToolBar;
}

QtStatusBar & QtWengoPhone::getQtStatusBar() const {
	return *_qtStatusBar;
}

QtSystray & QtWengoPhone::getQtSystray() const {
	return *_qtSystray;
}

QtCallBar & QtWengoPhone::getQtCallBar() const {
	return *_ui->callBar;
}

void QtWengoPhone::setChatWindow(QWidget * chatWindow) {
	if (!chatWindow) {
		_chatWindow = NULL;
		_ui->actionOpenChatWindow->setEnabled(false);
	}
	else {
		_chatWindow = chatWindow;
		_ui->actionOpenChatWindow->setEnabled(true);
	}
}

QWidget * QtWengoPhone::getChatWindow() const {
	return _chatWindow;
}

void QtWengoPhone::closeChatWindow() {
        if (_chatWindow)
        {
            ((QtChatWindow *)_chatWindow)->closeAllTabs ();
        }
}

void QtWengoPhone::setQtContactList(QtContactList * qtContactList) {
	_qtContactList = qtContactList;
/*
	if (!_ui->tabContactList->layout()) {
		Widget::createLayout(_ui->tabContactList);
	}

	_ui->tabContactList->layout()->addWidget(_qtContactList->getWidget());
	_ui->tabContactList->setHidden(true);
	LOG_DEBUG("QtContactList added");
	*/
}

QtContactList * QtWengoPhone::getQtContactList() const {
	return _qtContactList;
}

void QtWengoPhone::setQtHistoryWidget(QtHistoryWidget * qtHistoryWidget) {
	_qtHistoryWidget = qtHistoryWidget;
	_qtHistoryWidget->setQtToolBar(_qtToolBar);
	_qtHistoryWidget->setQtCallBar(_ui->callBar);

	//if (!_ui->tabHistory->layout()) {
	//	Widget::createLayout(_ui->tabHistory);
	//}

	//_qtHistoryWidget->getWidget()->setParent(_ui->tabHistory);
	//_ui->tabHistory->layout()->addWidget(_qtHistoryWidget->getWidget());

	LOG_DEBUG("QtHistoryWidget added");
}

void QtWengoPhone::setQtDialpad(QtDialpad * qtDialpad) {
//	Widget::createLayout(_ui->tabDialpad)->addWidget(qtDialpad);
	LOG_DEBUG("QtDialpad added");
}

void QtWengoPhone::initCallButtons() {
	//callButton
	SAFE_CONNECT(_ui->callBar, SIGNAL(callButtonClicked()), SLOT(callButtonClicked()));
	enableCallButton();

	//hangUpButton
	SAFE_CONNECT(_ui->callBar, SIGNAL(hangUpButtonClicked()), SLOT(hangUpButtonClicked()));
	_ui->callBar->setEnabledHangUpButton(false);

	//phoneComboBox
	SAFE_CONNECT(_ui->callBar, SIGNAL(phoneComboBoxReturnPressed()), SLOT(callButtonClicked()));
	SAFE_CONNECT(_ui->callBar, SIGNAL(phoneComboBoxEditTextChanged(const QString &)), SLOT(enableCallButton()));
}

void QtWengoPhone::enableCallButton() {
	std::string phoneNumber = _ui->callBar->getPhoneComboBoxCurrentText();
	_ui->callBar->setEnabledCallButton(!phoneNumber.empty());
}

void QtWengoPhone::hangUpButtonClicked() {
    //modified by huixin.du 2010-09-13
	QtContactCallListWidget * widget =
		dynamic_cast<QtContactCallListWidget *>(_ui->dialWidget);//dynamic_cast<QtContactCallListWidget *>(_ui->tabWidget->currentWidget());
	if (widget) {
		widget->hangup();
	}
}

void QtWengoPhone::callButtonClicked() {
	if (_cWengoPhone.getCUserProfileHandler().getCUserProfile()) {
		std::string phoneNumber = _ui->callBar->getPhoneComboBoxCurrentText();
		if (!phoneNumber.empty()) {
			CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
			cUserProfile->makeCallErrorEvent += boost::bind(&QtWengoPhone::makeCallErrorEventHandler, this, _2);
			cUserProfile->makeCall(phoneNumber);
		}
		_ui->callBar->clearPhoneComboBoxEditText();
	}
}

void QtWengoPhone::setActiveTabBeforeCallCurrent() {
	//_ui->tabWidget->setCurrentWidget(_activeTabBeforeCall);   modified by huixin.du 2010-09-13
	if(_activeTabBeforeCall == _ui->tabWidget)
	{
		_ui->tabWidget->show();
		_ui->dialWidget->hide();
	}
	else
	{
		_ui->dialWidget->show();
		_ui->tabWidget->hide();
	}

}

void QtWengoPhone::makeCallErrorEventHandler(EnumMakeCallError::MakeCallError error) {
	typedef PostEvent1<void (EnumMakeCallError::MakeCallError), EnumMakeCallError::MakeCallError> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtWengoPhone::makeCallErrorEventHandlerThreadSafe, this, _1), error);
	postEvent(event);
}

void QtWengoPhone::makeCallErrorEventHandlerThreadSafe(EnumMakeCallError::MakeCallError error) {
	QString message;
	switch (error) {
	case EnumMakeCallError::CallNotHeld:
		message = tr("Please hold all the phone calls before placing a new one");
		break;
	case EnumMakeCallError::NotConnected:
		message = tr("Can't place a call, you are not connected");
		break;
	case EnumMakeCallError::EmptyPhoneNumber:
		message = tr("You must enter a phone number");
		break;
	case EnumMakeCallError::SipError:
		message = tr("An error has occured trying to place the call");
		break;
	case EnumMakeCallError::NoError:
		LOG_FATAL("Should not happen");
		break;
	case EnumMakeCallError::RepeatCall:
		message = tr("you are talking with the person");
		break;
	}
	QMessageBox::warning(_wengoPhoneWindow, tr("@product@ - Call Error"),
		message, QMessageBox::Ok);
}

void QtWengoPhone::addPhoneCall(QtPhoneCall * qtPhoneCall) {

	_activeTabBeforeCall = _ui->dialWidget;//_activeTabBeforeCall = _ui->tabWidget->currentWidget();
	_ui->tabWidget->hide();_ui->dialWidget->show();       //added by huixin.du 2010-09-06
	QtContactCallListWidget * qtContactCallListWidget = new QtContactCallListWidget(_cWengoPhone, NULL);
	//SAFE_CONNECT(qtContactCallListWidget, SIGNAL(aboutToClose()), SLOT(setActiveTabBeforeCallCurrent()) );

    connect(qtContactCallListWidget,SIGNAL(aboutToClose()),this,SLOT(hideDialWidget()));       //added by huixin.du 2010-09-13

	if(m_qtConferenceRoomWidget)                                   //added by huixin.du 2010-09-16
	{
		connect(_toolButton,SIGNAL(clicked()),m_qtConferenceRoomWidget->_tools,SLOT(hideToolWidget()));
	}

	/************************************************************************///Add by zhuang 09-01-08
	/*                                                                      */
	QString userName = qtPhoneCall->getNickNameLabelText();
	userName.append("-call");
	//_ui->dialWidget->layout()->addWidget(qtContactCallListWidget);  //added by huixin.du 2010-09-13
	_dialTabWidget->addTab(qtContactCallListWidget, userName);   //added by huixin.du 2010-09-15
	//_ui->tabWidget->addTab(qtContactCallListWidget, userName);
	/************************************************************************/
	//_ui->tabWidget->addTab(qtContactCallListWidget, tr("Call"));
	_dialTabWidget->setCurrentWidget(qtContactCallListWidget);
	qtContactCallListWidget->addPhoneCall(qtPhoneCall);

	_ui->callBar->setEnabledHangUpButton(true);

	if (qtPhoneCall->isIncoming()) {
		_ui->callBar->setEnabledCallButton(true);
	}
}

void QtWengoPhone::addToConference(QString phoneNumber, PhoneCall * targetCall) {
	//FIXME conference has to be 100% rewritten...
	/*bool conferenceAlreadyStarted = false;

	if (_cWengoPhone.getCUserProfileHandler().getCUserProfile()) {
		int nbtab = _ui->tabWidget->count();

		for (int i = 0; i < nbtab; i++) {
			if (_ui->tabWidget->tabText(i) == QString(tr("Conference"))) {
				return;
			}

			for (int j = 0; j < _ui->tabWidget->count(); j++) {
				QtContactCallListWidget * qtContactCallListWidget = dynamic_cast<QtContactCallListWidget *>(_ui->tabWidget->widget(j));
				if (qtContactCallListWidget) {
					if (qtContactCallListWidget->hasPhoneCall(targetCall)) {
						_ui->tabWidget->setTabText(j, tr("Conference"));
						IPhoneLine * phoneLine = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getActivePhoneLine();

						if (phoneLine != NULL) {
							if (!conferenceAlreadyStarted) {
								conferenceAlreadyStarted = true;
								ConferenceCall * confCall = new ConferenceCall(*phoneLine);
								confCall->addPhoneCall(*targetCall);
								confCall->addPhoneNumber(phoneNumber.toStdString());
							}
						} else {
							LOG_DEBUG("phoneLine is NULL");
						}
					}
				}
			}
		}
	}*/

	if (!_cWengoPhone.getCUserProfileHandler().getCUserProfile())
		return;

	IPhoneLine * phoneLine = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getActivePhoneLine();
	if(!phoneLine)
		return;

	//for (int i = 0; i < _ui->tabWidget->count(); i++)                   //deleted by huixin.du 2010-09-14
	//{
	//      if (_ui->tabWidget->tabText(i) == QString(tr("Conference")))
	//		return;
	//}

	ConferenceCall * confCall = new ConferenceCall(*phoneLine);
	targetCall->setConferenceCall(confCall);

	PhoneCall * phoneCall2 = confCall->getPhoneCall(phoneNumber.toStdString());

	QtContactCallListWidget * qtContactCallListWidgetTarget = 0;
	QtPhoneCall * qtPhoneTarget = 0;
	QtPhoneCall * qtPhoneCall2 = 0;
	int index2 = -1;

//	for (int j = 0; j < _ui->tabWidget->count(); j++)                  //deleted by huixin.du 2010-09-14
//	{
		QtContactCallListWidget * qtContactCallListWidget = dynamic_cast<QtContactCallListWidget *>(_ui->tabWidget);//->widget(j));
		if (qtContactCallListWidget)
		{
			if (!qtContactCallListWidgetTarget && qtContactCallListWidget->hasPhoneCall(targetCall))
			{
//				_ui->tabWidget->setTabText(j, tr("Conference"));       //deleted by huixin.du 2010-09-14

				qtContactCallListWidgetTarget = qtContactCallListWidget;
				qtPhoneTarget = qtContactCallListWidgetTarget->takeQtPhoneCall(targetCall);
			}
			else if(!qtPhoneCall2 && qtContactCallListWidget->hasPhoneCall(phoneCall2))
			{
				qtPhoneCall2 = qtContactCallListWidget->takeQtPhoneCall(phoneCall2);
//				index2 = j;

			}
		}
//	}

	if( qtContactCallListWidgetTarget)
	{
		if(qtPhoneCall2)
		{
			qtPhoneCall2->hideConfButton();
			qtContactCallListWidgetTarget->addPhoneCall(qtPhoneCall2);     //modified by huixin.du 2010-09-13
//			_ui->tabWidget->removeTab(index2);
		}
		if(qtPhoneTarget)
		{
			qtPhoneTarget->hideConfButton();
			qtContactCallListWidgetTarget->addPhoneCall(qtPhoneTarget);
		}
	}

	QString address = QString::fromStdString(targetCall->getPeerSipAddress().getSipAddress());

	if(address.contains("@"))
		address = address.section("@",0,0);

	if (address.startsWith("sip:")) {
		address = address.mid(4);
	}

	confCall->addPhoneNumber(address.toStdString());
	confCall->addPhoneNumber(phoneNumber.toStdString());

	if(phoneCall2)
		phoneCall2->setConferenceCall(confCall);

	if(qtContactCallListWidgetTarget)
		qtContactCallListWidgetTarget->slotStartedTalking(qtPhoneTarget);
}

void QtWengoPhone::addToConference(QtPhoneCall * qtPhoneCall) {

	QtContactCallListWidget * qtContactCallListWidget;
	_ui->tabWidget->show();_ui->dialWidget->hide();
//	int nbtab = _ui->tabWidget->count();                         //modified by huixin.du 2010-09-13

//	for (int i = 0; i < nbtab; i++) {
//		if (_ui->tabWidget->tabText(i) == QString(tr("Conference"))) {
			//i is the index of the conference tab
			qtContactCallListWidget = (QtContactCallListWidget *) _ui->tabWidget;//->widget(i);
			qtContactCallListWidget->addPhoneCall(qtPhoneCall);
//			_ui->tabWidget->setCurrentWidget(qtContactCallListWidget);
			return;
//		}
//	}

	_activeTabBeforeCall = _ui->tabWidget;//->currentWidget();            //modified by huixin.du 2010-09-13

	//conference tab not found, create a new one                          //modified by huixin.du 2010-09-13
	qtContactCallListWidget = new QtContactCallListWidget(_cWengoPhone,_wengoPhoneWindow);
	SAFE_CONNECT(qtContactCallListWidget, SIGNAL(aboutToClose()), SLOT(setActiveTabBeforeCallCurrent()) );
	qtContactCallListWidget->setParent(_ui->tabWidget);//_ui->tabWidget->addTab(qtContactCallListWidget, tr("Conference"));
//	_ui->tabWidget->setCurrentWidget(qtContactCallListWidget);
	qtContactCallListWidget->addPhoneCall(qtPhoneCall);
	_ui->callBar->setEnabledHangUpButton(true);
}

/************************************************************************/
/*加入会议*/
void QtWengoPhone::addToConference()
{   //modified by huixin.du 2010-09-13
//	int nbtab = _ui->tabWidget->count();
//	_ui->tabWidget->show();_ui->dialWidget->hide();
//	for (int i = 0; i < nbtab; i++) {
//		if (_ui->tabWidget->tabText(i) == QString(tr("Conference"))) {
			//i is the index of the conference tab
//			_ui->tabWidget->setCurrentWidget(_ui->tabWidget->widget(i));
//			return;
//		}
//	}
	m_qtConferenceRoomWidget = new QtConferenceRoomWidget(*this);
	m_qtConferenceRoomWidget->setParent(_ui->tabWidget);//_ui->tabWidget->addTab((QWidget*)m_qtConferenceRoomWidget, tr("Conference"));
	//_ui->tabWidget->setCurrentWidget(m_qtConferenceRoomWidget);

	//getQtProfileBar()->getQtIMProfileWidget()->getWidget()->setVisible(false);
	getQtProfileBar()->nicknameClicked();
	//_wengoPhoneWindow->showMaximized();
	getQtProfileBar()->hide();
	getQtCallBar().hide();
}
/*删除会议*/
void QtWengoPhone::deleteConference()
{
//	int nbtab = _ui->tabWidget->count();
//	for (int i = 0; i < nbtab; i++) {
//		if (_ui->tabWidget->tabText(i) == QString(tr("Conference"))) {
			//i is the index of the conference tab
//	_ui->tabWidget->hide();_ui->dialWidget->show();//_ui->tabWidget->removeTab(i);
//			break;
//		}
//	}
	OWSAFE_DELETE(m_qtConferenceRoomWidget);
	//getQtProfileBar()->getQtIMProfileWidget()->getWidget()->setVisible(true);
	getQtProfileBar()->nicknameClicked();
	getQtProfileBar()->show();
	getQtCallBar().show();
}
/************************************************************************/

QtFileTransfer * QtWengoPhone::getFileTransfer() const {
	return _qtFileTransfer;
}

void QtWengoPhone::updatePresentation() {
	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtWengoPhone::updatePresentationThreadSafe, this));
	postEvent(event);
}

void QtWengoPhone::updatePresentationThreadSafe() {
	if (_cWengoPhone.getCUserProfileHandler().getCUserProfile()) {
		//disabled some actions if no SIP Account is used
		bool hasSipAccount = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().hasSipAccount();

		_ui->actionShowWengoAccount->setEnabled(_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().hasWengoAccount());
		_ui->actionSendSms->setEnabled(hasSipAccount);
		_ui->actionCreateConferenceCall->setEnabled(hasSipAccount);
		_ui->actionSearchWengoContact->setEnabled(hasSipAccount);
	}
}

void QtWengoPhone::dialpad(const std::string & tone) {
	if (_cWengoPhone.getCUserProfileHandler().getCUserProfile()) {
		_ui->callBar->setPhoneComboBoxEditText(_ui->callBar->getPhoneComboBoxCurrentText() + tone);
	}
}

void QtWengoPhone::prepareToExitApplication() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	// check for pending calls
	CUserProfile *cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (cUserProfile) {
		CPhoneLine *cPhoneLine = cUserProfile->getCPhoneLine();
		if (cPhoneLine) {
			if (cPhoneLine->hasPendingCalls()) {

				if (QMessageBox::question(
					getWidget(),
					tr("@product@ - Warning"),
					tr("You have unfinished call(s).") + "\n" +
						tr("Are you sure you want to exit the application?"),
					tr("&Exit"),
					tr("&Cancel")
				) == 1)  {
					return;
				}
			}
		}
	}
	////


	_qtSystray->hide();

	//Save the window size
	QSize winsize = _wengoPhoneWindow->size();
	config.set(Config::PROFILE_WIDTH_KEY, winsize.width());
	config.set(Config::PROFILE_HEIGHT_KEY, winsize.height());

	//Save the window position
	QPoint winpos = _wengoPhoneWindow->pos();
	config.set(Config::PROFILE_POSX_KEY, winpos.x());
	config.set(Config::PROFILE_POSY_KEY, winpos.y());

	QApplication::closeAllWindows();
	QCoreApplication::processEvents();

	//destroyed chatWindow so that chats are saved
	OWSAFE_DELETE(_chatWindow);
	////

	_cWengoPhone.terminate();
}

void QtWengoPhone::phoneComboBoxClicked() {

	_ui->callBar->clearPhoneComboBox();
	_ui->callBar->clearPhoneComboBoxEditText();
}

void QtWengoPhone::exitEvent() {
	QCoreApplication::exit(EXIT_SUCCESS);
}

void QtWengoPhone::showHistory() {
//	_ui->tabWidget->setCurrentWidget(_ui->tabHistory);
}

void QtWengoPhone::currentUserProfileWillDieEventHandlerSlot() {
	//Signal for re-initializing graphical components
	//when there is no user profile anymore
	userProfileDeleted();

	// Reset _qtIMAccountMonitor so that it does not emit signals anymore
	_qtIMAccountMonitor.reset(0);

	OWSAFE_DELETE(_qtFileTransfer);
	OWSAFE_DELETE(_qtIdle);
	_ui->profileBar->reset();

	//if (_qtContactList) {
	//	_ui->tabContactList->layout()->removeWidget(_qtContactList->getWidget());
	//	OWSAFE_DELETE(_qtContactList);
	//}

	//if (_qtHistoryWidget) {
	//	_ui->tabHistory->layout()->removeWidget(_qtHistoryWidget->getWidget());
	//	OWSAFE_DELETE(_qtHistoryWidget);
	//}
	if(m_qtConferenceRoomWidget)
	{
		_ui->tabDialpad->layout()->removeWidget(m_qtConferenceRoomWidget);
		OWSAFE_DELETE(m_qtConferenceRoomWidget);
	}

	uninstallQtBrowserWidget();
}

void QtWengoPhone::userProfileInitializedEventHandlerSlot() {
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();

	//if( !(cUserProfile->getUserProfile().isConnected()))
	//{
	//
	//	SipAccount* sipAccount = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getSipAccount()->clone();
	//	PUserProfileHandler* pUserprofileHandler = _cWengoPhone.getCUserProfileHandler().getPresentation();
	//	pUserprofileHandler->sipAccountConnectionFailedEventHandler( *sipAccount, EnumSipLoginState::SipLoginStatePasswordError );

	//	return;
	//}
	//Idle detection
	//FIXME: QtIdle must not use UserProfile but CUserProfile
	_qtIdle = new QtIdle(cUserProfile->getUserProfile(), _wengoPhoneWindow);

	// Create account monitor
	_qtIMAccountMonitor.reset( new QtIMAccountMonitor(0, cUserProfile) );

	//Create the profilebar
	_ui->profileBar->init(&_cWengoPhone, cUserProfile, _qtIMAccountMonitor.get());


	// Systray
	SAFE_CONNECT_RECEIVER_TYPE(_qtIMAccountMonitor.get(), SIGNAL(imAccountAdded(QString)),
		_qtSystray, SLOT(updateSystrayIcon()), Qt::QueuedConnection);
	SAFE_CONNECT_RECEIVER_TYPE(_qtIMAccountMonitor.get(), SIGNAL(imAccountUpdated(QString)),
		_qtSystray, SLOT(updateSystrayIcon()), Qt::QueuedConnection);
	SAFE_CONNECT_RECEIVER_TYPE(_qtIMAccountMonitor.get(), SIGNAL(imAccountRemoved(QString)),
		_qtSystray, SLOT(updateSystrayIcon()), Qt::QueuedConnection);

	_qtSystray->setTrayMenu();
	_qtSystray->updateSystrayIcon();


	if( _qtFileTransfer==NULL )		//防止内存泄露......zhenHua.sun 2010-12-06
		_qtFileTransfer = new QtFileTransfer(this, _cWengoPhone.getWengoPhone().getCoIpManager());

	//menu
	_qtToolBar->userProfileIsInitialized();

	//启动会议界面
	if( m_qtConferenceRoomWidget==NULL )	//防止内存泄露......zhenHua.sun 2010-12-06
	{
		m_qtConferenceRoomWidget = new QtConferenceRoomWidget(*this);
		//_ui->tabWidget->addTab((QWidget*)m_qtConferenceRoomWidget, tr("Conference"));
		//_ui->tabWidget->setCurrentWidget(m_qtConferenceRoomWidget);
		if (!_ui->tabDialpad->layout()) {
			Widget::createLayout(_ui->tabDialpad);
		}
		m_qtConferenceRoomWidget->setParent(_ui->tabDialpad);
		_ui->tabDialpad->layout()->addWidget(m_qtConferenceRoomWidget);
		/********huixin.du  2010-09-16**********************/
		connect(m_qtConferenceRoomWidget->_tools->_ui->callingButton,SIGNAL(clicked()),this,SLOT(hideDialWidget()));
		//connect(m_qtConferenceRoomWidget->_tools->_ui->callingButton,SIGNAL(clicked()),_ui->dialWidget,SLOT(hideDialWidget()));
		connect(m_qtConferenceRoomWidget->_tools->_ui->conferenceButton,SIGNAL(clicked()),_ui->dialWidget,SLOT(hide()));
		connect(m_qtConferenceRoomWidget->_tools->_ui->conferenceButton,SIGNAL(clicked()),_ui->tabWidget,SLOT(show()));

	#ifdef DBUS_ENABLE
		connect(m_qtConferenceRoomWidget->_tools->_ui->conferenceButton,SIGNAL(clicked()),_wengoPhoneWindow,SLOT(hide()));
	#endif
	}

	//如果是因为异常而重新登陆，那么应该重新进入会议...zhenHua.sun 2010-12-06
	ConferenceSession * confSession = RunningProfile::getInstance().getSessionManager().getConfSession();
	if( confSession )
	{
		//首先退出出现异常之前加入的会议
		QtWengoPhone::ActionQuitMeetingSignal();

		Sleep(1000);

		//然后重新加入会议
		QString msg = QString::fromStdString(confSession->_focusUI) + "\r\n" + QString::fromStdString(confSession->_password);
		QtWengoPhone::ActionEnterMeetingSignal( msg );
	}
		


	/***************************************************/
	//Widget::createLayout(_ui->tabDialpad)->addWidget(m_qtConferenceRoomWidget);
	//m_qtConferenceRoomWidget->show();

	QtWengoPhone::SetUIState(UI_S_LOGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
}

void QtWengoPhone::proxyNeedsAuthenticationEventHandler(NetworkProxyDiscovery & sender, NetworkProxy networkProxy) {
	typedef PostEvent1<void (NetworkProxy networkProxy), NetworkProxy> MyPostEvent;
	MyPostEvent * event =
		new MyPostEvent(boost::bind(&QtWengoPhone::proxyNeedsAuthenticationEventHandlerThreadSafe, this, _1), networkProxy);
	postEvent(event);
}

void QtWengoPhone::wrongProxyAuthenticationEventHandler(NetworkProxyDiscovery & sender, NetworkProxy networkProxy) {
	typedef PostEvent1<void (NetworkProxy networkProxy), NetworkProxy> MyPostEvent;
	MyPostEvent * event =
		new MyPostEvent(boost::bind(&QtWengoPhone::proxyNeedsAuthenticationEventHandlerThreadSafe, this, _1), networkProxy);
	postEvent(event);
}

void QtWengoPhone::proxyNeedsAuthenticationEventHandlerThreadSafe(NetworkProxy networkProxy) {
	static QtHttpProxyLogin * httpProxy =
		new QtHttpProxyLogin(getWidget(),
			networkProxy.getServer(), networkProxy.getServerPort());

	int ret = httpProxy->show();

	if (ret == QDialog::Accepted) {
		NetworkProxy myNetworkProxy;
		myNetworkProxy.setServer(httpProxy->getProxyAddress());
		myNetworkProxy.setServerPort(httpProxy->getProxyPort());
		myNetworkProxy.setLogin(httpProxy->getLogin());
		myNetworkProxy.setPassword(httpProxy->getPassword());

		NetworkProxyDiscovery::getInstance().setProxySettings(myNetworkProxy);
	}
}

void QtWengoPhone::closeWindow() {
	_wengoPhoneWindow->hide();
}

#if defined(OS_MACOSX)
void QtWengoPhone::fixMacOSXMenus() {
	// Avoids translation of these menus on Mac OS X. Thus Qt
	// will put these under the Application menu
	_ui->actionShowConfig->setText("Preferences");
	_ui->actionShowAbout->setText("About");
}
#endif

void QtWengoPhone::languageChanged() {
	LOG_DEBUG("retranslate main window ui");
	_ui->retranslateUi(_wengoPhoneWindow);
	_qtToolBar->retranslateUi();
#if defined(OS_MACOSX)
	fixMacOSXMenus();
#endif

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getIEActiveXEnable() && _qtBrowserWidget) {
		_ui->tabWidget->setTabText(_qtBrowserWidgetTabIndex, tr("Home"));
	}
#endif

	//if (_qtHistoryWidget) {
	//	_qtHistoryWidget->retranslateUi();
	//}	zhenHua.sun 2010-10-27
}

void QtWengoPhone::showAddContact(ContactInfo contactInfo) {

	ensureVisible();

	if (_cWengoPhone.getCUserProfileHandler().getCUserProfile()) {

		//FIXME this method should not be called if no UserProfile has been set
		ContactProfile contactProfile;
		QtProfileDetails qtProfileDetails(*_cWengoPhone.getCUserProfileHandler().getCUserProfile(),
			contactProfile, _wengoPhoneWindow, tr("Add a Contact"));

		//FIXME to remove when wdeal will be able to handle SIP presence
		if (contactInfo.group == "WDeal") {
			qtProfileDetails.setHomePhone(QUrl::fromPercentEncoding(QByteArray(contactInfo.sip.c_str())));
		} else {
			qtProfileDetails.setWengoName(QUrl::fromPercentEncoding(QByteArray(contactInfo.wengoName.c_str())));
		}
		///

		if (contactInfo.group == "WDeal") {
			qtProfileDetails.setFirstName(QUrl::fromPercentEncoding(QByteArray(contactInfo.wdealServiceTitle.c_str())));
		} else {
			qtProfileDetails.setFirstName(QUrl::fromPercentEncoding(QByteArray(contactInfo.firstname.c_str())));
		}

		qtProfileDetails.setLastName(QUrl::fromPercentEncoding(QByteArray(contactInfo.lastname.c_str())));
		qtProfileDetails.setCountry(QUrl::fromPercentEncoding(QByteArray(contactInfo.country.c_str())));
		qtProfileDetails.setCity(QUrl::fromPercentEncoding(QByteArray(contactInfo.city.c_str())));
		qtProfileDetails.setState(QUrl::fromPercentEncoding(QByteArray(contactInfo.state.c_str())));
		qtProfileDetails.setGroup(QUrl::fromPercentEncoding(QByteArray(contactInfo.group.c_str())));
		qtProfileDetails.setWebsite(QUrl::fromPercentEncoding(QByteArray(contactInfo.website.c_str())));

		if (qtProfileDetails.show()) {
			_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList().addContact(contactProfile);
		}
	}
}

void QtWengoPhone::bringMainWindowToFront() {
	ensureVisible();
}

void QtWengoPhone::ensureVisible() {
	_wengoPhoneWindow->activateWindow();
	_wengoPhoneWindow->showNormal();
	_wengoPhoneWindow->raise();
}

void QtWengoPhone::installQtBrowserWidget() {
	_qtBrowserWidget = NULL;
	/** 精简  zhenHua.sun 2010-10-29
	_qtBrowserWidget = new QtBrowserWidget(*this);
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getIEActiveXEnable()) {
		_qtBrowserWidgetTabIndex = _ui->tabWidget->addTab(_qtBrowserWidget->getWidget(), tr("Home"));
	}
#else
	//_ui->tabWidget->show();_ui->dialWidget->hide();//_ui->tabWidget->setCurrentIndex(0);
#endif
	*/
	
}

void QtWengoPhone::uninstallQtBrowserWidget() {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getIEActiveXEnable() && _qtBrowserWidget) {
		_ui->tabWidget->widget(_qtBrowserWidgetTabIndex)->layout()->removeWidget(_qtBrowserWidget->getWidget());
		_ui->tabWidget->removeTab(_qtBrowserWidgetTabIndex);
		OWSAFE_DELETE(_qtBrowserWidget);
	}
#endif
}

void QtWengoPhone::setCurrentUserProfileEventHandlerSlot()
{
	_qtToolBar->tryingToConnect();
}

QtConferenceRoomWidget * QtWengoPhone::getQtConferenceRoomWidget()
{
	return m_qtConferenceRoomWidget;
}


/**
* Add by: hexianfu
* for the telecontroller
* 2009-10-21
*/
char *QtWengoPhone::getPrivateMeetPwd()
{
	return privateMeetingPwd;
}

QtLoginDialog & QtWengoPhone::getQtLoginDialog() const
{
	return *_qtLoginDialog;
}

void QtWengoPhone::setQtLoginDialog(QtLoginDialog * qtLoginDialog)
{
	_qtLoginDialog = qtLoginDialog;
}


void QtWengoPhone::setQtToolWidget( QtToolWidget* qtToolWidget )
{
	_qtToolWidget = qtToolWidget;
}
QtToolWidget* QtWengoPhone::getQtToolWidget() const
{
	return _qtToolWidget;
}

/*
 * Add by huixin.du for delete conference tab  2010-09-14
 * 关闭呼叫界面，返回会议界面
 */
void QtWengoPhone::hideDialWidget()
{

	if(_dialTabWidget->count() == 0)
	{
		_ui->dialWidget->hide();
		_ui->tabWidget->show();		
#ifdef DBUS_ENABLE
		_wengoPhoneWindow->hide();	//zhenHua.sun
#endif
	}
	else
	{
		_ui->tabWidget->hide();
		_ui->dialWidget->show();
#ifdef DBUS_ENABLE
		_wengoPhoneWindow->show();	//zhenHua.sun
#endif
	}

}

//-------Add By LZY 2010-09-27-------
/*
 * 添加新的邀请信息框指针到消息框指针队列中
 * @param QMessageBox *MsgBoxPointer,要加入列表中的消息框指针
 * @return int,返回加入的消息框在队列中的唯一识别号，移除时需要用到此识别号
 */
int QtWengoPhone::AddInviteMsg(QMessageBox *MsgBoxPointer)
{
	long LockNum=(long)GetCurrentThreadId();
	QtWengoPhone::LockInviteList(LockNum);
	int LoopVar;
	for (LoopVar=0;LoopVar<QtWengoPhone::InviteListCount;++LoopVar)
	{
		if (QtWengoPhone::InviteList[LoopVar].MsgPointer==NULL)
		{
			QtWengoPhone::InviteList[LoopVar].MsgPointer=MsgBoxPointer;
			QtWengoPhone::InviteList[LoopVar].IdentifyNum=QtWengoPhone::InviteListNextIdentify;
			++QtWengoPhone::InviteListNextIdentify;
			if (QtWengoPhone::InviteListNextIdentify==0)
				QtWengoPhone::InviteListNextIdentify=1;
			break;
		}
	}
	if (LoopVar>=QtWengoPhone::InviteListCount)
	{
		LoopVar=QtWengoPhone::InviteListCount;
		QtWengoPhone::InviteListCount+=5;
		QtWengoPhone::InviteList=(QtWengoPhone::InviteListNode *)realloc(QtWengoPhone::InviteList,sizeof(QtWengoPhone::InviteListCount));
		QtWengoPhone::InviteList[LoopVar].MsgPointer=MsgBoxPointer;
		QtWengoPhone::InviteList[LoopVar].IdentifyNum=QtWengoPhone::InviteListNextIdentify;
		++QtWengoPhone::InviteListNextIdentify;
		if (QtWengoPhone::InviteListNextIdentify==0)
			QtWengoPhone::InviteListNextIdentify=1;
		for (int LoopVar2=LoopVar+1;LoopVar2<QtWengoPhone::InviteListCount;++LoopVar2)
		{
			QtWengoPhone::InviteList[LoopVar2].IdentifyNum=0;
			QtWengoPhone::InviteList[LoopVar2].MsgPointer=NULL;
		}
	}
	QtWengoPhone::UnLockInviteList(LockNum);
	return QtWengoPhone::InviteList[LoopVar].IdentifyNum;
}

/*
 * 移除指定索引位置的信息框指针，如果WithAccept为true，将在移除前执行该消息框的accept操作
 * @param int IdentifyNum,唯一识别号，用其定位列表中的对话框
 * @param int AdditionMethod=0,定位对话框成功后的附加操作，0-无操作 >0-accept() <0-reject()
 * @return void
 */
void QtWengoPhone::RemoveInviteMsg(int IdentifyNum,int AdditionMethod)
{
	long LockNum=(long)GetCurrentThreadId();
	QtWengoPhone::LockInviteList(LockNum);
	for (int LoopVar=0;LoopVar<QtWengoPhone::InviteListCount;++LoopVar)
	{
		if (QtWengoPhone::InviteList[LoopVar].IdentifyNum==IdentifyNum)
		{
			if (QtWengoPhone::InviteList[LoopVar].MsgPointer!=NULL)
			{
				QtWengoPhone::sendTeleStateChanged(CHANGE_INVITE,"\r\n");
				if (AdditionMethod>0)
					QtWengoPhone::InviteList[LoopVar].MsgPointer->accept();
				else if (AdditionMethod<0)
					QtWengoPhone::InviteList[LoopVar].MsgPointer->reject();
				QtWengoPhone::InviteList[LoopVar].MsgPointer=NULL;
				break;
			}
		}
	}
	QtWengoPhone::UnLockInviteList(LockNum);
}

/*
 * 移除所有当前正在显示的信息框（邀请确认，注销确认等），所有信息框将执行reject操作
 * @return void
 */
void QtWengoPhone::RemoveAllMsg()
{
	//移除邀请框
	long LockNum=(long)GetCurrentThreadId();
	QtWengoPhone::LockInviteList(LockNum);
	for (int LoopVar=0;LoopVar<QtWengoPhone::InviteListCount;++LoopVar)
	{
		if (QtWengoPhone::InviteList[LoopVar].MsgPointer!=NULL)
			QtWengoPhone::InviteList[LoopVar].MsgPointer->reject();
	}
	QtWengoPhone::UnLockInviteList(LockNum);
	//移除其它提示框
	;
}

/*
 * InviteList互斥访问锁加锁函数
 * @param long LockNum,加锁值，解琐时需提供相同的值才能解琐成功
 * @return void
 */
void QtWengoPhone::LockInviteList(long LockNum)
{
	while (QtWengoPhone::InviteListLockNum!=LockNum)
	{
		if (QtWengoPhone::InviteListLockNum==0)
			QtWengoPhone::InviteListLockNum=LockNum;
		Sleep(15);
	}
}

/*
 * InviteList互斥访问锁解锁函数
 * @param long UnLockNum,解锁值，提供的值必须与加琐时用的值一致才能解琐成功
 * @return bool,返回解琐结果
 */
bool QtWengoPhone::UnLockInviteList(long UnLockNum)
{
	if (QtWengoPhone::InviteListLockNum==UnLockNum)
	{
		QtWengoPhone::InviteListLockNum=0;
		return true;
	}
	else
		return false;
}
//-------End of Add By LZY-----------

//-------Add By LZY 自2010-09-19------
/*
 * 2010-11-02 发送信息给teleController，目前使用DBus机制管理发送，不需要锁
 * @param char *Message,指向要发送数据的指针
 * @return void
 */
void QtWengoPhone::SendInfo(char *Message)
{
	CvTelecontrollerClient::getInstance()->TeleInfo( QString::fromUtf8(Message) );
}

void QtWengoPhone::commandProcess(const char *recvedMess)
{
	CMessageFrame recvedMessFram(recvedMess);
	char CommandName[50];//动作名缓存区
	int TLIndex;//存放动作在teleController接收列表中的索引号
	int TLAvailable;//存放动作在teleController接收列表中的有效标记值
	//----BG 立即返回动作信息的相关控制变量----
	bool ResponseTele=false;//是否在函数末给teleController发送返回信息，对于立即返回的动作都应该把此值设置为true
	CMessageFrame *ResponseBuffer=NULL;//额外信息返回用指针，当动作需要返回teleController附加信息时（如返回会议列表等），把此指针实例化
	int RunResult=ARESULT_SUCCESS;//动作执行结果，发送返回信息时用，各值定义看QtWengoPhone.h文件里以“ARESULT”开头的预定义值说明
	//----ED 立即返回动作信息的相关变量----

	recvedMessFram.readLine(false);//行1，消息标记，目前认为传过来的都是“Action”，所以不做额外的检查
	TLIndex=atoi(recvedMessFram.readLine(true));//行2，对应teleController动作接收等待列表的索引号
	TLAvailable=atoi(recvedMessFram.readLine(true));//行3，对应teleController动作接收等待列表的有效标记值
	strcpy(&(CommandName[0]),recvedMessFram.readLine(true));//行4，动作名，将其复制到动作名缓冲区中
	//------开始_动作处理代码--------
	if(strcmp(CommandName,"Login")==0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_LOGOFF,ACTION_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			QtWengoPhone::SetUIState(UI_S_LOGIN_BEGIN);//用于通知遥控器界面状态变化
			QtWengoPhone::ActionLoginSignal(QString::fromUtf8(recvedMessFram.readLast()));//触发信号
		}
	}
	else if(strcmp(CommandName,"GetMeetingList")==0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			CUserProfile *cUserProfile = QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile();
			vector<ConfInfo *> currentMeetingList = cUserProfile->getCurrentConfInfoList();
			ResponseBuffer=new CMessageFrame();
			int confListSize = currentMeetingList.size();
			for (int LoopVar=0;LoopVar<confListSize;++LoopVar)
			{
				ResponseBuffer->Append((currentMeetingList[LoopVar]->Title).c_str());
				ResponseBuffer->Append((currentMeetingList[LoopVar]->URI).c_str());
				ResponseBuffer->Append((currentMeetingList[LoopVar]->ControlMode).c_str());
				ResponseBuffer->Append((currentMeetingList[LoopVar]->HostURI).c_str());
				if(currentMeetingList[LoopVar]->JoinMode == "password")
					ResponseBuffer->Append("P");
				else
					ResponseBuffer->Append("N");
			}
		}
	}
	else if (strcmp(CommandName,"RefreshMeetingList")==0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_LOGIN|UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionRefreshMeetingListSignal();//触发信号
		}
	}
	else if(strcmp(CommandName,"EnterMeeting")==0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_LOGIN,ACTION_ENTERMEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			QtWengoPhone::SetUIState(UI_S_JOINMEETING_BEGIN);//用于通知遥控器界面状态变化
			QtWengoPhone::ActionEnterMeetingSignal(QString::fromUtf8(recvedMessFram.readLast()));//触发信号
		}
	}
	else if(strcmp(CommandName,"GetMemberList")==0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			CUserProfile *cUserProfile = QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile();
			vector<MemberInfo *> currentMemberList = cUserProfile->getCurrentMemberInfoList();
			ResponseBuffer=new CMessageFrame();
			int Size = currentMemberList.size();
			int index;//位置索引值
			string nameAbstract;//分离出来的用户名
			MemberInfo *member;
			for (int LoopVar=0;LoopVar<Size;++LoopVar)
			{
				member = currentMemberList[LoopVar];
				index = member->MemberURI.find('@',0);
				nameAbstract=member->MemberURI;
				if (index!=std::string::npos)
					nameAbstract = nameAbstract.substr(0,index);
				ResponseBuffer->Append(nameAbstract.c_str());
				ResponseBuffer->Append(member->permitSpeak);
				if((member->Sdp).find("m=video")!=std::string::npos)
					ResponseBuffer->Append(1);
				else
					ResponseBuffer->Append(0);
				ResponseBuffer->Append(member->hand);
			}
		}
	}
	else if(strcmp(CommandName,"ContactMember")==0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING,ACTION_CONTACTMEMBER,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
			QtWengoPhone::ActionContactMemberSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
	}
	else if(strcmp(CommandName,"StopMember" ) == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionStopMemberSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
		}
	}
	else if(strcmp(CommandName,"QuitMeeting")==0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING,ACTION_QUITMEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			QtWengoPhone::SetUIState(UI_S_EXITMEETING_BEGIN);//用于通知遥控器界面状态变化
			QtWengoPhone::ActionQuitMeetingSignal();//触发信号
		}
	}
	else if(strcmp(CommandName,"LogOff") == 0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING|UI_S_LOGIN,ACTION_LOGOFF,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			QtWengoPhone::SetUIState(UI_S_LOGOFF_BEGIN);//用于通知遥控器界面状态变化
			QtWengoPhone::ActionLogOffSignal();//触发信号
		}
	}
	else if(strcmp(CommandName,"ShutDown") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_ALL,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			system("shutdown -s -t 120");
		}
	}
	else if(strcmp(CommandName, "LocalControl") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionLocalControlSignal();//触发信号
		}
	}
	else if (strcmp(CommandName, "GetVolume") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			StringList deviceId;
			ResponseBuffer=new CMessageFrame();
			if (strcmp(recvedMessFram.readLine(true),"out") == 0)
			{
				ResponseBuffer->Append("out");
				deviceId = ConfigManager::getInstance().getCurrentConfig().getAudioOutputDeviceId();
			}
			else
			{
				ResponseBuffer->Append("in");
				deviceId = ConfigManager::getInstance().getCurrentConfig().getAudioInputDeviceId();
			}
			AudioDevice audioDevice(deviceId);
			VolumeControl volumeControl(audioDevice);
			ResponseBuffer->Append(volumeControl.getLevel());
		}
	}
	else if(strcmp(CommandName,"SetVolume")==0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (strcmp(recvedMessFram.readLine(true),"out")==0)
				QtWengoPhone::ActionSetVolumeSignal(atoi(recvedMessFram.readLine(true)),1);//Output设备（扬声器）音量设置
			else
				QtWengoPhone::ActionSetVolumeSignal(atoi(recvedMessFram.readLine(true)),0);//Input设备（麦克风）音量设置
		}
	}
	else if (strcmp(CommandName, "GetGlobalService") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			Config & config = ConfigManager::getInstance().getCurrentConfig();
			ResponseBuffer=new CMessageFrame();
			ResponseBuffer->Append(config.getGlobalService().c_str());  //v4
			ResponseBuffer->Append(config.getGlobalService().c_str());  //v6
		}
	}
	else if(strcmp(CommandName,"SetGlobalService")==0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			string Addr=recvedMessFram.readLine(true);
			QtWengoPhone::SaveGlobalSetting(Addr,Addr);
		}
	}
	else if(strcmp(CommandName,"RaiseHand" ) == 0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING,ACTION_RAISEHAND,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
			QtWengoPhone::ActionRaiseHandSignal();//触发信号
	}
	else if(strcmp(CommandName,"PermitSpeak" ) == 0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING,ACTION_PERMITSPEAK,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
			QtWengoPhone::ActionPermitSpeakSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
	}
	else if(strcmp(CommandName,"KickMember" ) == 0)//延后响应Action
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING,ACTION_KICKMEMBER,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
			QtWengoPhone::ActionKickMemberSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
	}
	else if(strcmp(CommandName,"SetLayoutType" ) == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionSetLayoutTypeSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
		}
	}
	else if(strcmp(CommandName,"InviteMembers" ) == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionInviteMembersSignal(QString::fromUtf8(recvedMessFram.readLine(true)));//触发信号
		}
	}
	else if(strcmp(CommandName,"AcceptInvite" ) == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_ALL,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			int IdentifyNum=atoi(recvedMessFram.readLine(true));
			QtWengoPhone::ActionAcceptInviteSignal(IdentifyNum,atoi(recvedMessFram.readLine(true)));//触发信号
		}
	}
	else if(strcmp(CommandName,"CreateMeeting" ) == 0)//延后响应Action，因传送格式改变而待改
	{
		if (QtWengoPhone::CheckDelayAction(CommandName,UI_S_MEETING|UI_S_LOGIN,ACTION_CREATEMEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			QString Title=QString::fromUtf8(recvedMessFram.readLine(true));//会议名
			QString Description=QString::fromUtf8(recvedMessFram.readLine(true));//会议描述
			int ControlModeNum=atoi(recvedMessFram.readLine(true));//会议设置
			int JoinModeNum=atoi(recvedMessFram.readLine(true));//加入设置
			QString Psw=QString::fromUtf8(recvedMessFram.readLine(true));//密码
			QtWengoPhone::ActionCreateMeetingSignal(Title,Description,ControlModeNum,JoinModeNum,Psw);//触发信号
		}
	}
	else if (strcmp(CommandName, "LocalVideoSetting") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionLocalVideoSettingSignal();//触发信号
		}
	}
	else if (strcmp(CommandName, "LocalAudioSetting") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::ActionLocalAudioSettingSignal();//触发信号
		}
	}
	else if(strcmp(CommandName,"GetContactList") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (QtWengoPhone::_qtContactList)
				ResponseBuffer=new CMessageFrame(QtWengoPhone::_qtContactList->getContacts().c_str());
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"GetContactInfo") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (QtWengoPhone::_qtContactList)
				ResponseBuffer=new CMessageFrame(QtWengoPhone::_qtContactList->getContactInfo(recvedMessFram.readLine(true)).c_str());
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"EditContact") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			string ContactId=recvedMessFram.readLine(true);//联系人ID
			if (QtWengoPhone::_qtContactList)
			{
				ContactProfile ContactProfile;
				if (ContactId!="")
					ContactProfile = QtWengoPhone::_qtContactList->getCContactList().getContactProfile(ContactId);
				ContactProfile.setFirstName(recvedMessFram.readLine(true));
				ContactProfile.setLastName(recvedMessFram.readLine(true));
				//Set Group Id
				string GroupName=recvedMessFram.readLine(true);
				string GroupId = QtWengoPhone::_qtContactList->getCContactList().getContactGroupIdFromName(GroupName);
				if(GroupId == "")
				{
					QtWengoPhone::_qtContactList->getCContactList().addContactGroup(GroupName);
					GroupId = QtWengoPhone::_qtContactList->getCContactList().getContactGroupIdFromName(GroupName);
				}
				ContactProfile.setGroupId(GroupId);
				QtWengoPhone::_qtContactList->setSipAccount(recvedMessFram.readLine(true),ContactProfile);
				ContactProfile.setHomePhone(recvedMessFram.readLine(true));
				ContactProfile.setMobilePhone(recvedMessFram.readLine(true));
				ContactProfile.setPersonalEmail(recvedMessFram.readLine(true));
				StreetAddress Address;
				Address.setCity(recvedMessFram.readLine(true));
				ContactProfile.setStreetAddress(Address);
				ContactProfile.setSex((EnumSex::Sex)atoi(recvedMessFram.readLine(true)));
				QString BirthDay = recvedMessFram.readLine(true);
				QDate date = QDate::fromString(BirthDay,"yyyy-M-d");
				ContactProfile.setBirthdate(Date(date.day(),date.month(),date.year()));
				if(ContactId!="")
					QtWengoPhone::_qtContactList->getCContactList().updateContact(ContactProfile);
				else
					QtWengoPhone::_qtContactList->getCContactList().addContact(ContactProfile);
			}
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"DelContact") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (QtWengoPhone::_qtContactList)
				QtWengoPhone::_qtContactList->getCContactList().removeContact(recvedMessFram.readLine(true));
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"DelGroup") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (QtWengoPhone::_qtContactList)
			{
				string GroupId = QtWengoPhone::_qtContactList->getCContactList().getContactGroupIdFromName(recvedMessFram.readLine(true));
				if (GroupId!="")
					QtWengoPhone::_qtContactList->getCContactList().removeContactGroup(GroupId);
				else
					RunResult=ARESULT_FAIL;
			}
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"ChangeGroupName") == 0)//立即响应Action（相应信息更新通过终端主动通知实现）
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			if (QtWengoPhone::_qtContactList)
			{
				string GroupId = QtWengoPhone::_qtContactList->getCContactList().getContactGroupIdFromName(recvedMessFram.readLine(true));
				if (GroupId!="")
					QtWengoPhone::_qtContactList->getCContactList().renameContactGroup(GroupId,recvedMessFram.readLine(true));
				else
					RunResult=ARESULT_FAIL;
			}
			else
				RunResult=ARESULT_FORBIDDEN;
		}
	}
	else if(strcmp(CommandName,"CallSip") == 0)//立即响应Action
	{
		if (QtWengoPhone::CheckImitAction(CommandName,UI_S_MEETING|UI_S_LOGIN,recvedMessFram.readLine(true),TLIndex,TLAvailable)==true)
		{
			ResponseTele=true;
			QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->makeCall(recvedMessFram.readLine(true));
		}
		else
			RunResult=ARESULT_FORBIDDEN;
	}
	//------结束_动作处理代码--------
	else//收到其他不支持的动作
	{
		ResponseTele=true;
		RunResult=ARESULT_FORBIDDEN;
	}
	if (ResponseTele)
	{
		char *ResponsePointer=NULL;
		if (ResponseBuffer!=NULL)
		{
			QtWengoPhone::sendTeleResult(CommandName,RunResult,TLIndex,TLAvailable,ResponseBuffer->getString());
			delete ResponseBuffer;
		}
		else
			QtWengoPhone::sendTeleResult(CommandName,RunResult,TLIndex,TLAvailable);
	}
}

/*
 * 检查延后返回动作是否满足执行条件，当不满足时自动向teleController发送动作返回，同时，如果动作的执行模式是“E”时将释放该延后处理动作（在ActionList对应该动作的格中设置Using=false）
 * @param char *ActionName,动作名指针，当延后动作不能被执行时需要用此参数调用sendTeleResult函数
 * @param unsigned int EnabledStateCode,允许该延后返回动作执行的界面状态标志，用“或”运算组合
 * @param int ActionListIndex,该延后返回指令在ActionList中的索引值
 * @param char * RunMode,动作执行模式，“E”执行该延后响应动作，“C”取消该延后响应动作
 * @param int TIndex,teleController动作接收等待列表的索引号
 * @param int TAvailableNum,teleController动作接收等待列表的有效标记值
 */
bool QtWengoPhone::CheckDelayAction(char *ActionName,unsigned long EnabledStateCode,int ActionListIndex,char * RunMode,int TIndex,int TAvailableNum)
{
	if (strcmp(RunMode,"E")==0)
	{
		if ((QtWengoPhone::UI_State_Code & EnabledStateCode)!=0 && QtWengoPhone::ActionList[ActionListIndex].Using==false)
		{
			QtWengoPhone::ActionList[ActionListIndex].IndexNum=TIndex;
			QtWengoPhone::ActionList[ActionListIndex].AvailableNum=TAvailableNum;
			QtWengoPhone::ActionList[ActionListIndex].Using=true;
			return true;
		}
		else
		{
			if (QtWengoPhone::ActionList[ActionListIndex].Using==true)//同样的动作正在执行中
				QtWengoPhone::sendTeleResult(ActionName,ARESULT_RUNNING,TIndex,TAvailableNum);
			else//当前界面状态不能执行此动作
				QtWengoPhone::sendTeleResult(ActionName,ARESULT_FORBIDDEN,TIndex,TAvailableNum);
		}
	}
	else
		QtWengoPhone::ActionList[ActionListIndex].Using=false;
	return false;
}

/*
 * 检查立即返回动作是否满足执行条件，当不满足时自动向teleController发送动作返回
 * @param char *ActionName,动作名指针，当延后动作不能被执行时需要用此参数调用sendTeleResult函数
 * @param unsigned int EnabledStateCode,允许该延后返回动作执行的界面状态标志，用“或”运算组合
 * @param char * RunMode,动作执行模式，“E”执行该延后响应动作，“C”取消该延后响应动作
 * @param int TIndex,teleController动作接收等待列表的索引号
 * @param int TAvailableNum,teleController动作接收等待列表的有效标记值
 */
bool QtWengoPhone::CheckImitAction(char *ActionName,unsigned long EnabledStateCode,char *RunMode,int TIndex,int TAvailableNum)
{
	if (strcmp(RunMode,"E")==0 && (QtWengoPhone::UI_State_Code & EnabledStateCode)!=0)
		return true;
	else
	{
		if (strcmp(RunMode,"E")==0)
			QtWengoPhone::sendTeleResult(ActionName,ARESULT_FORBIDDEN,TIndex,TAvailableNum);
		return false;
	}
}

/*
 * 用于动作执行后返回给teleController消息的发送，此函数仅给Action在commandProcess中立即返回时调用，对于延后响应的Action，请调用sendTeleResult(const char* actionName,int ActionListIndex,int Result,const char * Response)函数
 * @param const char* actionName,要返回的动作名
 * @param int Result,动作执行结果，各值定义看QtWengoPhone.h文件中以“ARESULT”开头的预定义值的意义
 * @param int TIndex,返回给teleController时指定其在teleController的动作等待接收列表中的索引位置，函数不检查此值是否有效（不过在teleControleler程序中有相应的检查代码）
 * @param int TAvailable,返回给teleController时指定其在teleController的动作等待接收列表中的有效标记值
 * @param const char * Response=NULL,需要附带的返回信息
 * @return void
 */
void QtWengoPhone::sendTeleResult(const char* actionName,int Result,int TIndex,int TAvailable,const char * Response)
{
	CMessageFrame SendMsg;
	SendMsg.Append("Action");//行1，信息类别标志
	SendMsg.Append(TIndex);//行2，teleController中定位返回信息用的索引值
	SendMsg.Append(TAvailable);//行3，teleController中检查索引格是否仍然有效的标记
	SendMsg.Append(actionName);//行4，动作名
	SendMsg.Append(Result);//行5，动作执行结果
	if (Response!=NULL)
		SendMsg.Append(Response);//行6及后面
	QtWengoPhone::SendInfo(SendMsg.getString());//发送数据
}

/*
 * 设置界面状态标记，如果设置的状态标记与原来记录着的不一样，将发送相应信息给teleController，进而通知遥控器更新，当界面状态变化了，应该调用本函数
 * @param int StateCode,要设置成的新的状态标记，各值的说明看QtWengoPhone.h文件中以“UI_”开头的预定义值的说明
 * @return void
 */
void QtWengoPhone::SetUIState(unsigned long StateCode)
{
	unsigned long NewSStateCode=StateCode&UI_S_ALL;//分离出驻态值
	unsigned long NewDStateCode=StateCode&UI_D_ALL;//分离出瞬态值
	CMessageFrame *ReturnUIParam=NULL;//存放UI改变时需要返回给teleController的参数（含驻态号）
	//处理驻态变化
	if (NewSStateCode!=0 && NewSStateCode!=QtWengoPhone::UI_State_Code)
	{
		QtWengoPhone::UI_State_Code=NewSStateCode;
		ReturnUIParam=new CMessageFrame();
		ReturnUIParam->Append(NewSStateCode);
		switch (NewSStateCode)
		{
		//case UI_S_INIT_BEGIN:
		case UI_S_LOGIN_BEGIN:
			delete ReturnUIParam;
			ReturnUIParam=NULL;
			break;
		case UI_S_LOGIN:
			QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().ExitMeetingSuccessEvent += boost::bind(&QtWengoPhone::ExitMeetingSuccess,this);//绑定退出会议成功时触发的事件，用以更改UI_State_Code值
			ReturnUIParam->Append(QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getSipAccount()->getFullIdentity().c_str());
			//-------开始 响应驻态UI_S_LOGIN的相关Action的处理代码-------
			QtWengoPhone::ResponseDelayAction("Login",ACTION_LOGIN,ARESULT_SUCCESS);//响应Login延后处理动作，执行成功
			QtWengoPhone::ResponseDelayAction("EnterMeeting",ACTION_ENTERMEETING,ARESULT_FAIL);//响应EnterMeeting延后处理动作，执行失败
			QtWengoPhone::ResponseDelayAction("QuitMeeting",ACTION_QUITMEETING,ARESULT_SUCCESS);//响应QuitMeeting延后处理动作，执行成功
			//-------结束 响应驻态UI_S_LOGIN的相关Action的处理代码-------
			break;
		case UI_S_LOGOFF_BEGIN:
			delete ReturnUIParam;
			ReturnUIParam=NULL;
			break;
		case UI_S_LOGOFF:
			//QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().ExitMeetingSuccessEvent -= boost::bind(&QtWengoPhone::ExitMeetingSuccess,this);//不需要解绑退出会议成功时触发的事件，因为此时的UserProfile已经失效
			ReturnUIParam->Append(ConfigManager::getInstance().getCurrentConfig().getSipRealm().c_str());
			//-------开始 响应驻态UI_S_LOGOFF的相关Action的处理代码-------
			QtWengoPhone::ResponseDelayAction("Login",ACTION_LOGIN,ARESULT_FAIL);//响应Login延后处理动作，执行失败
			QtWengoPhone::ResponseDelayAction("LogOff",ACTION_LOGOFF,ARESULT_SUCCESS);//响应LogOff延后处理动作，执行成功
			//-------结束 响应驻态UI_S_LOGOFF的相关Action的处理代码-------
			break;
		case UI_S_JOINMEETING_BEGIN:
			delete ReturnUIParam;
			ReturnUIParam=NULL;
			break;
		case UI_S_MEETING:
			ReturnUIParam->Append(QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getSipAccount()->getFullIdentity().c_str());
			ReturnUIParam->Append(QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().joiningMeeting.c_str());
			//-------开始 响应驻态UI_S_MEETING的相关Action的处理代码-------
			QtWengoPhone::ResponseDelayAction("EnterMeeting",ACTION_ENTERMEETING,ARESULT_SUCCESS);//响应EnterMeeting延后处理动作，执行成功
			//-------结束 响应驻态UI_S_MEETING的相关Action的处理代码-------
			break;
		case UI_S_EXITMEETING_BEGIN:
			delete ReturnUIParam;
			ReturnUIParam=NULL;
			break;
		}
		if (ReturnUIParam!=NULL)
			QtWengoPhone::sendTeleStateChanged(CHANGE_UI,ReturnUIParam->getString());//界面状态变更主动通知
	}
	//处理瞬态变化
	if (NewDStateCode!=0)
	{
		//-------开始 瞬态值处理代码-------
		if (NewDStateCode==UI_D_CONTACTMEMBER_SUCCESS)
			QtWengoPhone::ResponseDelayAction("ContactMember",ACTION_CONTACTMEMBER,ARESULT_SUCCESS);//响应ContactMember延后处理动作，执行成功
		if (NewDStateCode==UI_D_CONTACTMEMBER_ERROR)
			QtWengoPhone::ResponseDelayAction("ContactMember",ACTION_CONTACTMEMBER,ARESULT_FAIL);//响应ContactMember延后处理动作，执行失败
		if (NewDStateCode==UI_D_RAISEHAND_SUCCESS)
			QtWengoPhone::ResponseDelayAction("RaiseHand",ACTION_RAISEHAND,ARESULT_SUCCESS);//响应RaiseHand延后处理动作，执行成功
		if (NewDStateCode==UI_D_PERMITSPEAK_ERROR)
			QtWengoPhone::ResponseDelayAction("PermitSpeak",ACTION_PERMITSPEAK,ARESULT_FAIL);//响应PermitSpeak延后处理动作，执行失败
		if (NewDStateCode==UI_D_PERMITSPEAK_SUCCESS)
			QtWengoPhone::ResponseDelayAction("PermitSpeak",ACTION_PERMITSPEAK,ARESULT_SUCCESS);//响应PermitSpeak延后处理动作，执行成功
		if (NewDStateCode==UI_D_KICKMEMBER_SUCCESS)
			QtWengoPhone::ResponseDelayAction("KickMember",ACTION_KICKMEMBER,ARESULT_SUCCESS);//响应KickMember延后处理动作，执行成功
		if (NewDStateCode==UI_D_KICKMEMBER_ERROR)
			QtWengoPhone::ResponseDelayAction("KickMember",ACTION_KICKMEMBER,ARESULT_FAIL);//响应KickMember延后处理动作，执行失败
		if (NewDStateCode==UI_D_CREATEMEETING_SUCCESS)
			QtWengoPhone::ResponseDelayAction("CreateMeeting",ACTION_CREATEMEETING,ARESULT_SUCCESS);//响应CreateMeeting延后处理动作，执行成功
		if (NewDStateCode==UI_D_LOCALAUDIO_SEND_STOP || NewDStateCode==UI_D_LOCALAUDIO_SEND_START || NewDStateCode==UI_D_LOCALVIDEO_SEND_STOP || NewDStateCode==UI_D_LOCALVIDEO_SEND_START)
		{
			string ChangeInfo;
			if (NewDStateCode==UI_D_LOCALAUDIO_SEND_STOP || NewDStateCode==UI_D_LOCALVIDEO_SEND_STOP)
				ChangeInfo="On";
			else
				ChangeInfo="Off";
			if (NewDStateCode==UI_D_LOCALAUDIO_SEND_STOP || NewDStateCode==UI_D_LOCALAUDIO_SEND_START)
				QtWengoPhone::sendTeleStateChanged(CHANGE_LOCALAUDIO,ChangeInfo.c_str());
			else
				QtWengoPhone::sendTeleStateChanged(CHANGE_LOCALVIDEO,ChangeInfo.c_str());
		}
		//-------结束 瞬态值处理代码-------
	}
}

/*
 * 处理延后响应Action的函数，函数会自动判断传入的ActionIndex所对应Action是否需要发送回复信息
 * @param string ActionName,动作名，必须与ActionIndex对应的动作相一致
 * @param int ActionIndex,动作在延后响应动作列表中的索引值，函数不会检查传入值是否有效索引号，当该索引对应的动作正等待回复时，函数会调用sendTeleResult
 * @param int Result=ARESULT_SUCCESS,动作返回的结果，默认为成功
 * @param char * ResponsePointer=NULL,回复时需要附带的信息的指针，默认为空
 * @return void
 */
void QtWengoPhone::ResponseDelayAction(string ActionName,int ActionIndex,int Result,char * ResponsePointer)
{
	if (QtWengoPhone::ActionList[ActionIndex].Using)
	{
		QtWengoPhone::sendTeleResult(ActionName.c_str(),Result,QtWengoPhone::ActionList[ActionIndex].IndexNum,QtWengoPhone::ActionList[ActionIndex].AvailableNum,ResponsePointer);
		QtWengoPhone::ActionList[ActionIndex].Using=false;
	}
}

/*
 * 发送终端主动通告的消息（"State"类型消息）
 * @param int StateIndex,主动通知索引号，与teleController中的定义相一致
 * @param const char *AddInfo=NULL,附带信息，比如界面状态改变时返回改变后的界面值
 * @return void
 */
void QtWengoPhone::sendTeleStateChanged(int StateIndex,const char *AddInfo)
{
	CMessageFrame SendMsg;
	SendMsg.Append("State");
	SendMsg.Append(StateIndex);
	if (AddInfo)
		SendMsg.Append(AddInfo);
	QtWengoPhone::SendInfo(SendMsg.getString());//发送数据
}

/*
 * 保存GlobalSetting地址值，界面与遥控器通用的下层函数
 * @param string IPv4Addr,IPv4对应的GlobalSetting地址
 * @param string IPv6Addr,IPv6对应的GlobalSetting地址，目前此参数传入后是用不上的，只使用IPv4的地址
 * @return void
 */
void QtWengoPhone::SaveGlobalSetting(string IPv4Addr,string IPv6Addr)
{
	ConfigManager::getInstance().getCurrentConfig().set(Config::SIP_GLOBAL_SERVICE,IPv4Addr);
}

/*
 * 创建会议用函数，遥控器和界面通用
 * @param QString Title,要创建会议的标题，函数不做有效性检查
 * @param QString Description,要创建会议的描述
 * @param int ControlModeNum,会议模式，0-讨论模式 1-发布会模式
 * @param int JoinModeNum,会议权限设置，0-公开会议，1-密码验证
 * @param QString Psw,会议密码，当设置为密码验证会议时此参数才有效，否则自动添入代表无密码的值“\t”
 */
void QtWengoPhone::CreateMeeting(QString Title,QString Description,int ControlModeNum,int JoinModeNum,QString Psw)
{
	string ControlMode = "";
	string JoinMode = "public";
	if (ControlModeNum==0)
		ControlMode="peer";
	else
		ControlMode="host";
	switch(JoinModeNum)
	{
	case 0:
		JoinMode = "public";
		Psw='\t';
		break;
	case 1:
		JoinMode = "password";
		break;
	default:
		break;
	}
	CUserProfile * cUserProfile = QtWengoPhone::_cWengoPhone.getCUserProfileHandler().getCUserProfile();
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	//std::string confUri = "sip:service@ser.scut.edu.cn";
	std::string confUri = config.getGlobalService();
	std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"ApplyConference\"><conference><uri></uri>";
	szContent+="<title>"+string(Title.toUtf8()) +"</title>";  //modify by li.zhenning
	//szContent+="<title>"+std::string(_ui->phoneNumber1LineEdit->text().toLocal8Bit()) +"</title>";
	szContent+="<description>"+string(Description.toUtf8()) +"</description>";
	szContent +="<hostUri></hostUri>";
	szContent +="<controlMode>"+ControlMode+"</controlMode>";
	szContent +="<joinMode>"+JoinMode+"</joinMode>";
	szContent +="<joinPassword>"+string(Psw.toUtf8())+"</joinPassword>";
	szContent +="<startTime></startTime><duration></duration>";
	szContent+="</conference></coolview>";
	cUserProfile->applyConference(confUri,szContent);
	QtWengoPhone::SetUIState(UI_D_CREATEMEETING_SUCCESS);
}

/*
 * 响应UserProfile发出的ExitMeetingSuccessEvent事件，更改UI_State_Code设置值
 * @return void
 */
void QtWengoPhone::ExitMeetingSuccess()
{
	QtWengoPhone::SetUIState(UI_S_LOGIN);
}
//-------End of Add By LZY----------