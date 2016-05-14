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

#include "QtUserProfileHandler.h"
#include <model/config/config.h>
#include <model/config/ConfigManager.h>
#include <control/profile/CUserProfileHandler.h>

#include <presentation/PFactory.h>
#include <presentation/qt/QtLanguage.h>
#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/login/QtLoginDialog.h>
#include <presentation/qt/QtSystray.h>
#include <presentation/qt/statusbar/QtStatusBar.h>

#include <qtutil/SafeConnect.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <QtCore/QTimer>
#include <QtGui/QMessageBox>

QtUserProfileHandler::QtUserProfileHandler(CUserProfileHandler & cUserProfileHandler,
	QtWengoPhone & qtWengoPhone)
	: QObject(&qtWengoPhone),
	_cUserProfileHandler(cUserProfileHandler),
	_qtWengoPhone(qtWengoPhone),_qtLoginDialog(0) {

	//Connection for UserProfile change
	SAFE_CONNECT_TYPE(this, SIGNAL(setCurrentUserProfileEventHandlerSignal()),
		SLOT(setCurrentUserProfileEventHandlerSlot()), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(noCurrentUserProfileSetEventHandlerSignal()),
		SLOT(noCurrentUserProfileSetEventHandlerSlot()), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(currentUserProfileWillDieEventHandlerSignal()),
		SLOT(currentUserProfileWillDieEventHandlerSlot()), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(userProfileInitializedEventHandlerSignal()),
		SLOT(userProfileInitializedEventHandlerSlot()), Qt::QueuedConnection);
	qRegisterMetaType<SipAccount>("SipAccount");
	qRegisterMetaType<WengoAccount>("WengoAccount");
	qRegisterMetaType<EnumSipLoginState::SipLoginState>("EnumSipLoginState::SipLoginState");
	SAFE_CONNECT_TYPE(this, SIGNAL(sipAccountConnectionFailedEventHandlerSignal(SipAccount*, EnumSipLoginState::SipLoginState)),
		SLOT(sipAccountConnectionFailedEventHandlerSlot(SipAccount*, EnumSipLoginState::SipLoginState)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(profileLoadedFromBackupsEventHandlerSignal(QString)),
		SLOT(profileLoadedFromBackupsEventHandlerSlot(QString)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(profileCannotBeLoadedEventHandlerSignal(QString)),
		SLOT(profileCannotBeLoadedEventHandlerSlot(QString)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(sipNetworkErrorEventHandlerSignal(SipAccount*)),
		SLOT(sipNetworkErrorEventHandlerSlot(SipAccount*)), Qt::QueuedConnection);
}

void QtUserProfileHandler::updatePresentation() {
}

void QtUserProfileHandler::noCurrentUserProfileSetEventHandler() {
	noCurrentUserProfileSetEventHandlerSignal();
}

void QtUserProfileHandler::currentUserProfileWillDieEventHandler() {
	currentUserProfileWillDieEventHandlerSignal();
}

void QtUserProfileHandler::userProfileInitializedEventHandler() {
	userProfileInitializedEventHandlerSignal();
}

void QtUserProfileHandler::sipAccountConnectionFailedEventHandler(const SipAccount & sipAccount, EnumSipLoginState::SipLoginState state) {
	// We need to clone the SipAccount because we don't want to pass a
	// reference of the original, and doing a simple copy would produce a
	// SipAccount instance, even if the reference is in fact a WengoAccount.
	sipAccountConnectionFailedEventHandlerSignal(sipAccount.clone(), state);
}

void QtUserProfileHandler::profileLoadedFromBackupsEventHandler(std::string profileName) {
	profileLoadedFromBackupsEventHandlerSignal(QString::fromStdString(profileName));
}

void QtUserProfileHandler::profileCannotBeLoadedEventHandler(std::string profileName) {

	//-------Modify By LZY 2010-09-20-------
	profileCannotBeLoadedEventHandlerSignal(QString::fromStdString(profileName));
	/*if(!_qtWengoPhone.isSendLoginCommand) //Add by hexianfu 10-1-15   if teleLogin,do not emit the signal;
	{
		profileCannotBeLoadedEventHandlerSignal(QString::fromStdString(profileName));
	}
	else  //Add by hexianfu 10-1-15   if teleLogin,login failed;
	{
		_qtWengoPhone.TeleLoginTimeOut();
	}*/
	//-------End of Modify By LZY----------
}

void QtUserProfileHandler::networkErrorEventHandler(const SipAccount & sipAccount) {
	sipNetworkErrorEventHandlerSignal(sipAccount.clone());
}

void QtUserProfileHandler::noCurrentUserProfileSetEventHandlerSlot() {
	_qtWengoPhone.uninstallQtBrowserWidget();
	showLoginWindow();
}

void QtUserProfileHandler::setCurrentUserProfileEventHandler() {
	setCurrentUserProfileEventHandlerSignal();
}

void QtUserProfileHandler::setCurrentUserProfileEventHandlerSlot() {
	_qtWengoPhone.setCurrentUserProfileEventHandlerSlot();
}

void QtUserProfileHandler::currentUserProfileWillDieEventHandlerSlot() {
	if(!_qtLoginDialog)
	{
		_qtWengoPhone.currentUserProfileWillDieEventHandlerSlot();
		_cUserProfileHandler.currentUserProfileReleased();

	// For some reason, if we call showLoginWindow directly, the CPU is at 100%
	// on MacOS X. Delaying showLoginWindow with a timer solves it.
		QTimer::singleShot(0, this, SLOT(showLoginWindow()));
	}
}

void QtUserProfileHandler::userProfileInitializedEventHandlerSlot() {
	_qtWengoPhone.userProfileInitializedEventHandlerSlot();
}

void QtUserProfileHandler::profileLoadedFromBackupsEventHandlerSlot(QString profileName) {
	QString profileloadingerror = QString::fromLocal8Bit("加载用户信息时发生错误，\n已读取最后备份的用户信息，\n您最后的修改将会丢失。");
	QMessageBox::warning(_qtWengoPhone.getWidget(), tr("@product@"),
		profileloadingerror,
		QMessageBox::Ok, QMessageBox::NoButton);
}

void QtUserProfileHandler::profileCannotBeLoadedEventHandlerSlot(QString profileName) {
	QString profileunloaderror = QString::fromLocal8Bit("无法加载您的用户信息，\n请选择其他用户或创建一个新的用户。");
	QMessageBox::warning(_qtWengoPhone.getWidget(), tr("@product@"),
		profileunloaderror,
		QMessageBox::Ok, QMessageBox::NoButton);
}

/**
* modified by: hexianfu
* for the telecontroller to control the Login commmand
* 2009-11-9
*/
void QtUserProfileHandler::showLoginWindow() {

	if(!_qtLoginDialog)
	{
		if(!_qtLoginDialog)
		_qtLoginDialog = new QtLoginDialog(&_qtWengoPhone, _cUserProfileHandler);

		Config & config = ConfigManager::getInstance().getCurrentConfig();
		if(config.getPasswordRemember())
		{
			std::string login = config.getProfileLastUsedName();
			_qtLoginDialog->changePage(QtLoginDialog::AddSIPAccount,login);
		}
		else
			_qtLoginDialog->changePage(QtLoginDialog::AddSIPAccount);

		_qtWengoPhone.setQtLoginDialog(_qtLoginDialog); //Add by hexianfu  09-11-9

		if (_qtLoginDialog->exec() == QDialog::Accepted) {
			_qtWengoPhone.installQtBrowserWidget();
			_qtWengoPhone.getQtStatusBar().updatePhoneLineState(EnumPhoneLineState::PhoneLineStateProgress);
			_qtWengoPhone.getQtSystray().phoneLineStateChanged(EnumPhoneLineState::PhoneLineStateProgress);
		}else
		{
			//如果取消登录的话则推出程序   add by zhenHua.sun 2010-09-03
			_qtWengoPhone.prepareToExitApplication();			
		}
		delete _qtLoginDialog;
		_qtLoginDialog = 0;
	}
}

void QtUserProfileHandler::showLoginWindowWithWengoAccount(const WengoAccount & wengoAccount) {

	if(!_qtLoginDialog )
	{
		_qtLoginDialog = new QtLoginDialog(&_qtWengoPhone, _cUserProfileHandler);

		_qtWengoPhone.setQtLoginDialog(_qtLoginDialog); //Add by hexianfu  09-11-9

		_qtLoginDialog->setValidAccount(wengoAccount);
		if (_qtLoginDialog->exec() == QDialog::Accepted) {
			_qtWengoPhone.installQtBrowserWidget();
			_qtWengoPhone.getQtStatusBar().updatePhoneLineState(EnumPhoneLineState::PhoneLineStateProgress);
			_qtWengoPhone.getQtSystray().phoneLineStateChanged(EnumPhoneLineState::PhoneLineStateProgress);
		}
		delete _qtLoginDialog;
		_qtLoginDialog = 0;
	}
}

void QtUserProfileHandler::sipNetworkErrorEventHandlerSlot(SipAccount* sipAccount) {
	/*_qtWengoPhone.uninstallQtBrowserWidget();

	QtLoginDialog dlg(&_qtWengoPhone, _cUserProfileHandler);
	dlg.setInvalidAccount(*sipAccount);
	dlg.setErrorMessage(tr("Network error."));
	if (dlg.exec() == QDialog::Accepted) {
		_qtWengoPhone.installQtBrowserWidget();
	}
	delete sipAccount;*/
}

/**
* modified by: hexianfu
* for the telecontroller to control the Login commmand
* 2009-11-24
*/
void QtUserProfileHandler::sipAccountConnectionFailedEventHandlerSlot(SipAccount* sipAccount, EnumSipLoginState::SipLoginState state) {
	LOG_DEBUG("error: %d", state);
	// FIXME: Why do we do different things depending on the account type?
	if (sipAccount->getType() == SipAccount::SipAccountTypeWengo) {
		_qtWengoPhone.uninstallQtBrowserWidget();
	} else {
		_qtWengoPhone.currentUserProfileWillDieEventHandlerSlot();
		_cUserProfileHandler.currentUserProfileReleased();
	}

	if(!_qtLoginDialog)
	{
		_qtLoginDialog =new QtLoginDialog(&_qtWengoPhone, _cUserProfileHandler);
		_qtLoginDialog->setInvalidAccount(*sipAccount);

		_qtWengoPhone.setQtLoginDialog(_qtLoginDialog); //Add by hexianfu  09-11-9
		QString message;
		if (state == EnumSipLoginState::SipLoginStatePasswordError) {
			if (sipAccount->getType() == SipAccount::SipAccountTypeWengo) {
				message = QString::fromLocal8Bit("email或密码输入错误");//tr("Wrong email/password entered");
			} else {
				message = QString::fromLocal8Bit("帐号或密码输入错误");//tr("Wrong login/password entered");
			}
		} else if (state == EnumSipLoginState::SipLoginStateNetworkError) {
			message = QString::fromLocal8Bit("连接错误");//tr("Connection error");
		} else {
			QString unexpectederror = QString::fromLocal8Bit("无法识别的错误");
			message = unexpectederror+tr(" (code: %1").arg(int(state));
		}
		_qtLoginDialog->setErrorMessage(message);
		
		delete sipAccount;

		if (_qtLoginDialog->exec() == QDialog::Accepted) {
			_qtWengoPhone.installQtBrowserWidget();
			_qtWengoPhone.getQtStatusBar().updatePhoneLineState(EnumPhoneLineState::PhoneLineStateProgress);
			_qtWengoPhone.getQtSystray().phoneLineStateChanged(EnumPhoneLineState::PhoneLineStateProgress);
		}else
		{
			//如果取消登录的话则推出程序   add by zhenHua.sun 2010-09-03
			_qtWengoPhone.prepareToExitApplication();			
		}
		delete _qtLoginDialog;
		_qtLoginDialog = 0;
	}
}
