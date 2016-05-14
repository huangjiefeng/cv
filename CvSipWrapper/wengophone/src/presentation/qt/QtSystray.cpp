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

#include "QtSystray.h"

#include "QtWengoPhone.h"
#include "QtPresenceMenuManager.h"
#include "QtPresencePixmapHelper.h"
#include "QtIMAccountMonitor.h"
#include "QtNoWengoAlert.h"
#include "conference/QtToolWidget.h"

#include "contactlist/QtContactListManager.h"
#include "contactlist/QtContactMenu.h"
//#include "webservices/sms/QtSms.h"

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/network/NetworkObserver.h>
#include <model/profile/UserProfile.h>

#include <cutil/global.h>
#include <util/Logger.h>

#include <qtutil/SafeConnect.h>
#include <qtutil/LanguageChangeEventFilter.h>

#include <QtGui/QtGui>

#include <trayicon.h>

#if defined(OS_MACOSX)
	#include "macosx/QtMacApplication.h"
#endif

#ifdef OS_WINDOWS
	#include <windows.h>
#endif

static const char* SYSTRAY_ICON_TEMPLATE=":/pics/systray/%1.png";
static const int SYSTRAY_ICON_CENTERX=8;
static const int SYSTRAY_ICON_CENTERY=9;
static const int SYSTRAY_ICON_RADIUS=16;

QtSystray::QtSystray(QObject * parent)
	: QObjectThreadSafe(NULL) {

	_qtWengoPhone = (QtWengoPhone *) parent;

	_callMenu = NULL;

	//Check Internet connection status
	NetworkObserver::getInstance().connectionIsDownEvent +=
		boost::bind(&QtSystray::connectionIsDownEventHandler, this);

	NetworkObserver::getInstance().connectionIsUpEvent +=
		boost::bind(&QtSystray::connectionIsUpEventHandler, this);

	//trayMenu
	_trayMenu = new QMenu(_qtWengoPhone->getWidget());
	SAFE_CONNECT(_trayMenu, SIGNAL(aboutToShow()), SLOT(setTrayMenu()));

	//trayIcon
	_trayIcon = new TrayIcon(QPixmap(":pics/status/online.png"), tr("@product@"), _trayMenu, _qtWengoPhone->getWidget());

#if defined(OS_LINUX)
	// On Linux, the main window is shown using only one click
	SAFE_CONNECT(_trayIcon, SIGNAL(clicked( const QPoint&, int)), SLOT(toggleMainWindow()));
#else
	SAFE_CONNECT(_trayIcon, SIGNAL(doubleClicked(const QPoint &)), SLOT(toggleMainWindow()));
#endif
	phoneLineStateChanged(EnumPhoneLineState::PhoneLineStateProgress);
	_trayIcon->show();

	//systray re-initialization
	SAFE_CONNECT(_qtWengoPhone, SIGNAL(userProfileDeleted()), SLOT(userProfileDeleted()));

	setTrayMenu();

	LANGUAGE_CHANGE(_trayIcon);
}

QtSystray::~QtSystray() {
}

void QtSystray::setTrayMenu() {
	_trayMenu->clear();

	//openAction
#if defined(OS_MACOSX)
	// On Mac OS X, clicking the Dock icon should show the application thus the
	// 'Open WengoPhone' entry is not necessary. We get the Dock Icon click event
	// from our QtMacApplication class.
	QtMacApplication * qMacApp = dynamic_cast<QtMacApplication *>(QApplication::instance());
	SAFE_CONNECT_RECEIVER(qMacApp, SIGNAL(applicationMustShow()), _qtWengoPhone->getWidget(), SLOT(show()));
#endif

	//Start a call session
	_callMenu = new QMenu(tr("Call"));
	_callMenu->setIcon(QIcon(":/pics/contact/call.png"));
	updateMenu();
	_trayMenu->addMenu(_callMenu);

	//Change status menu
	/*2009-12-9 ע��by zhuang
	if (_qtWengoPhone->getCWengoPhone().getCUserProfileHandler().getCUserProfile()) {
		_trayMenu->addMenu(createStatusMenu());
	}
	*/

	//quitAction
#if !defined(OS_MACOSX)
	// There is already a Quit on Mac OS X
	QAction * quitAction = _trayMenu->addAction(QIcon(":/pics/exit.png"), tr("Quit @product@"));
#endif

#if !defined(OS_MACOSX)
	SAFE_CONNECT_RECEIVER(quitAction, SIGNAL(triggered()), _qtWengoPhone, SLOT(prepareToExitApplication()));
#endif

	_trayIcon->setPopup(_trayMenu);
}

QMenu * QtSystray::createStatusMenu() {
	CWengoPhone & cWengoPhone = _qtWengoPhone->getCWengoPhone();

	CUserProfileHandler& cUserProfileHandler = cWengoPhone.getCUserProfileHandler();
	QMenu * menu = new QMenu(_qtWengoPhone->getWidget());
	menu->setTitle(tr("Status"));
	QtPresenceMenuManager * manager = new QtPresenceMenuManager(menu, 
		&cUserProfileHandler,_qtWengoPhone);
	manager->addPresenceActions(menu);

	return menu;
}

void QtSystray::updateMenu() {
	CWengoPhone & cWengoPhone = _qtWengoPhone->getCWengoPhone();

#ifndef DISABLE_SMS
	//sendSmsMenu
	QMenu * sendSmsMenu = new QMenu(_qtWengoPhone->getWidget());
	sendSmsMenu->setTitle(tr("Send a SMS"));
	sendSmsMenu->setIcon(QIcon(":/pics/contact/sms.png"));
	SAFE_CONNECT(sendSmsMenu, SIGNAL(triggered(QAction *)), SLOT(sendSms(QAction *)));
	sendSmsMenu->clear();

	QAction * sendSmsBlankAction = sendSmsMenu->addAction(QIcon(":/pics/contact/sms.png"), tr("Send SMS"));
	sendSmsBlankAction->setData(String::null);
	_trayMenu->addMenu(sendSmsMenu);
	QtContactMenu::populateMobilePhoneMenu(sendSmsMenu, cWengoPhone);
	///
#endif
	//startChatMenu
	/*2009-12-9 ע�� by zhuang
	QMenu * startChatMenu = new QMenu(_qtWengoPhone->getWidget());
	startChatMenu->setIcon(QIcon(":/pics/contact/chat.png"));
	SAFE_CONNECT(startChatMenu, SIGNAL(triggered(QAction *)), SLOT(startChat(QAction *)));
	startChatMenu->clear();
	startChatMenu->setTitle(tr("Start a chat"));
	_trayMenu->addMenu(startChatMenu);
	QtContactMenu::populateChatMenu(startChatMenu, cWengoPhone);
	*/
	///
/*2009-12-9 ע�� by zhuang
	//call menu
	QAction * placeCallBlankAction = _callMenu->addAction(QIcon(":/pics/contact/call.png"), tr("Place Call"));
	placeCallBlankAction->setData(String::null);

	//callWengoMenu
	QMenu * callWengoMenu = new QMenu(_qtWengoPhone->getWidget());
	callWengoMenu->setTitle(tr("Call @company@"));
	SAFE_CONNECT(callWengoMenu, SIGNAL(triggered(QAction *)), SLOT(makeCall(QAction *)));
	callWengoMenu->clear();
	_callMenu->addMenu(callWengoMenu);
	QtContactMenu::populateWengoCallMenu(callWengoMenu, cWengoPhone);
	///

	//callMobileMenu
	QMenu * callMobileMenu = new QMenu(_qtWengoPhone->getWidget());
	callMobileMenu->setTitle(tr("Call Mobile"));
	SAFE_CONNECT(callMobileMenu, SIGNAL(triggered(QAction *)), SLOT(makeCall(QAction *)));
	callMobileMenu->clear();
	_callMenu->addMenu(callMobileMenu);
	QtContactMenu::populateMobilePhoneMenu(callMobileMenu, cWengoPhone);
	///

	//callLandlineMenu
	QMenu * callLandlineMenu = new QMenu(_qtWengoPhone->getWidget());
	callLandlineMenu->setTitle(tr("Call land line"));
	SAFE_CONNECT(callLandlineMenu, SIGNAL(triggered(QAction *)), SLOT(makeCall(QAction *)));
	callLandlineMenu->clear();
	_callMenu->addMenu(callLandlineMenu);
	QtContactMenu::populateHomePhoneMenu(callLandlineMenu, cWengoPhone);
	///
*/
	//callSIPMenu
	QMenu * callSIPMenu = new QMenu(_qtWengoPhone->getWidget());
	callSIPMenu->setTitle(tr("Call SIP"));
	SAFE_CONNECT(callSIPMenu, SIGNAL(triggered(QAction *)), SLOT(makeCall(QAction *)));
	callSIPMenu->clear();
	_callMenu->addMenu(callSIPMenu);
	QtContactMenu::populateSipCallMenu(callSIPMenu, cWengoPhone);
	///
/*2009-12-9 ע�� by zhuang	
	//sendFileMenu
	QMenu * sendFileMenu = new QMenu(_qtWengoPhone->getWidget());
	sendFileMenu->setTitle(tr("Send File"));
	sendFileMenu->setIcon(QIcon(":/pics/actions/send_file.png"));
	SAFE_CONNECT(sendFileMenu, SIGNAL(triggered(QAction *)), SLOT(sendFile(QAction *)));
	sendFileMenu->clear();
	_trayMenu->addMenu(sendFileMenu);
	//TODO: call a new method populateFreeCallMenu with contactId
	QtContactMenu::populateWengoUsersContactId(sendFileMenu, cWengoPhone);
	///
	*/
}

void QtSystray::updateSystrayIcon() {
	CWengoPhone & cWengoPhone = _qtWengoPhone->getCWengoPhone();
	CUserProfile * cUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();

	if(cUserProfile)
	{
		IMAccountList accountList = cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
		QtPresencePixmapHelper helper(accountList);
		QPixmap icon = helper.createPixmap(SYSTRAY_ICON_TEMPLATE, 
			SYSTRAY_ICON_CENTERX, 
			SYSTRAY_ICON_CENTERY,
			SYSTRAY_ICON_RADIUS);
		_trayIcon->setIcon(icon);

		EnumPresenceState::PresenceState presenceState = cUserProfile->getUserProfile().getPresenceState();

		switch (presenceState) {
		case EnumPresenceState::PresenceStateAway:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Away"));
			break;
		case EnumPresenceState::PresenceStateOnline:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Online"));
			break;
		case EnumPresenceState::PresenceStateOffline:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Offline"));
			break;
		case EnumPresenceState::PresenceStateInvisible:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Invisible"));
			break;
		case EnumPresenceState::PresenceStateDoNotDisturb:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Do Not Disturb"));
			break;
		case EnumPresenceState::PresenceStateMulti:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Multiple Status"));
			break;
		case EnumPresenceState::PresenceStateUnknown:
			_trayIcon->setToolTip(tr("@product@") + " - " + tr("Unknown Status"));
			break;
		default:
			LOG_FATAL("unknown presenceState=" + String::fromNumber(presenceState));
			break;
		}
	}
}

void QtSystray::userProfileDeleted() {
	connectionStateEventHandlerThreadSafe(false);
}

void QtSystray::phoneLineStateChanged(EnumPhoneLineState::PhoneLineState state) {
	bool connected = false;

	switch (state) {
	case EnumPhoneLineState::PhoneLineStateUnknown:
		break;
	case EnumPhoneLineState::PhoneLineStateServerError:
		break;
	case EnumPhoneLineState::PhoneLineStateTimeout:
		break;
	case EnumPhoneLineState::PhoneLineStateOk:
		connected = true;
		break;
	case EnumPhoneLineState::PhoneLineStateClosed:
		break;
	case EnumPhoneLineState::PhoneLineStateProgress:
		_trayIcon->setIcon(QPixmap(":/pics/systray/connecting.png"));
		_trayIcon->setToolTip(tr("@product@") + " - " + tr("Connecting..."));
		return;
	default:
		LOG_FATAL("unknown state=" + EnumPhoneLineState::toString(state));
	};

	//connectionStateEventHandlerThreadSafe(connected);
}

void QtSystray::connectionIsDownEventHandler() {
	//typedef PostEvent1<void (bool), bool> MyPostEvent;
	//MyPostEvent * event = new MyPostEvent(boost::bind(&QtSystray::connectionStateEventHandlerThreadSafe, this, _1), false);
	//FIXME Replaced by phoneLineStateChanged()
	//postEvent(event);
}

void QtSystray::connectionIsUpEventHandler() {
	//typedef PostEvent1<void (bool), bool> MyPostEvent;
	//MyPostEvent * event = new MyPostEvent(boost::bind(&QtSystray::connectionStateEventHandlerThreadSafe, this, _1), true);
	//FIXME Replaced by phoneLineStateChanged()
	//postEvent(event);
}

void QtSystray::connectionStateEventHandlerThreadSafe(bool connected) {
	if (connected) 
	{
		//_trayIcon->setIcon(QPixmap(":/pics/systray/connecting.png"));
		//_trayIcon->setToolTip(tr("@product@") + " - " + tr("Connecting..."));
		//updateSystrayIcon();
	} 
	else {
		_trayIcon->setIcon(QPixmap(":/pics/systray/disconnected.png"));
		_trayIcon->setToolTip(tr("@product@") + " - " + tr("Internet Connection Error"));
	}
}

void QtSystray::toggleMainWindow() {
	// If the wengophone is hidden or minimized, show & raise it, otherwise hide it
	if (_qtWengoPhone->getWidget()->isHidden() || _qtWengoPhone->getWidget()->isMinimized() ) {
		//_qtWengoPhone->getWidget()->showMaximized();		//双击图标的时候最大化，防止调整画面出现僵死  zhenHua.sun2010-09-14
		_qtWengoPhone->getWidget()->raise();
		
		//在这里显示工具栏
		if( _qtWengoPhone->getQtToolWidget() )
		{
			_qtWengoPhone->getQtToolWidget()->hideToolWidget();
		}
		
	} else {
		_qtWengoPhone->getWidget()->hide();
	}
}

void QtSystray::hide() {
	_trayIcon->hide();
}

void QtSystray::makeCall(QAction * action) {
	CWengoPhone & cWengoPhone = _qtWengoPhone->getCWengoPhone();
	if (action) {
		LOG_DEBUG("call=" + action->data().toString().toStdString());
		cWengoPhone.getCUserProfileHandler().getCUserProfile()->makeCall(action->data().toString().toStdString());
	} else {
		LOG_FATAL("QAction cannot be NULL");
	}
}

void QtSystray::startChat(QAction * action) {
	if (action) {
		QtContactListManager * contactListManager = QtContactListManager::getInstance();
		if (contactListManager) {
			LOG_DEBUG("chat=" + action->data().toString().toStdString());
			contactListManager->startChat(action->data().toString());
		} else {
			LOG_FATAL("QtContactListManager cannot be NULL");
		}
	} else {
		LOG_FATAL("QAction cannot be NULL");
	}
}

void QtSystray::sendSms(QAction * action) {
	CWengoPhone & cWengoPhone = _qtWengoPhone->getCWengoPhone();
	if (action) {
		CUserProfile * cuserprofile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
		if (cuserprofile) {
			if (_qtWengoPhone->getQtSms() &&
				cuserprofile->getUserProfile().hasWengoAccount() &&
				cuserprofile->getUserProfile().getWengoAccount()->isConnected()) {

				//_qtWengoPhone->getQtSms()->getWidget()->show();	zhenHua.sun 2010-10-27
				_qtWengoPhone->getWidget()->show();
				_qtWengoPhone->getWidget()->setWindowState(_qtWengoPhone->getWidget()->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
				//_qtWengoPhone->getQtSms()->setPhoneNumber(action->data().toString());		zhenHua.sun 2010-10-27
			} else {
				QtNoWengoAlert noWengoAlert(NULL, *_qtWengoPhone);
				noWengoAlert.getQDialog()->exec();
			}
		}
	} else {
		LOG_FATAL("QAction cannot be NULL");
	}
}

void QtSystray::sendFile(QAction * action) {
	if (action) {
		QtContactListManager * contactListManager = QtContactListManager::getInstance();
		if (contactListManager) {
			LOG_DEBUG("sendfile=" + action->data().toString().toStdString());
			contactListManager->sendFile(action->data().toString());
		}
	} else {
		LOG_FATAL("QAction cannot be NULL");
	}
}

void QtSystray::languageChanged() {
	updateMenu();
}
