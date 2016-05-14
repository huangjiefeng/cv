#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
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

#include "QtUserProfile.h"

//#include <presentation/qt/QtBrowserWidget.h>		¾«¼ò zhenHua.sun
#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/login/QtLoginDialog.h>
#include <presentation/qt/profile/QtProfileDetails.h>
#include <presentation/qt/profile/QtUserProfileHandler.h>

#include <presentation/qt/QtSystray.h>
#include <presentation/qt/statusbar/QtStatusBar.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/contactlist/ContactProfile.h>
#include <model/presence/PresenceHandler.h>
#include <model/profile/UserProfile.h>

#include <imwrapper/IMContact.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

QtUserProfile::QtUserProfile(CUserProfile & cUserProfile, QtWengoPhone & qtWengoPhone)
	: QObjectThreadSafe(NULL),
	_qtWengoPhone(qtWengoPhone),
	_cUserProfile(cUserProfile) {

	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtUserProfile::initThreadSafe, this));
	postEvent(event);
}

void QtUserProfile::initThreadSafe() {
	qRegisterMetaType<IMContact>("IMContact");

	SAFE_CONNECT_TYPE(this, SIGNAL(loginStateChangedEventHandlerSignal(SipAccount *, int)),
		SLOT(loginStateChangedEventHandlerSlot(SipAccount *, int)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(networkDiscoveryStateChangedEventHandlerSignal(SipAccount *, int)),
		SLOT(networkDiscoveryStateChangedEventHandlerSlot(SipAccount *, int)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(authorizationRequestEventHandlerSignal(PresenceHandler *, IMContact, QString)),
		SLOT(authorizationRequestEventHandlerSlot(PresenceHandler *, IMContact, QString)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(incomingSubscribeEventHandlerSignal(PresenceHandler *,const QString&,int,const QString&, const QString&)),
		SLOT(incomingSubscribeEventHandlerSlot(PresenceHandler *,const QString&,int,const QString&,const QString&)), Qt::QueuedConnection);
/* zhenHua.sun 2010-10-29
	QtBrowserWidget* browserWidget = _qtWengoPhone.getQtBrowserWidget();
	if (browserWidget) {
		browserWidget->loadAccountURL();
	}
	*/
}

QtUserProfile::~QtUserProfile() {
	LOG_DEBUG("QtUserProfile");
}

void QtUserProfile::updatePresentation() {
}

void QtUserProfile::updatePresentationThreadSafe() {
}

void QtUserProfile::loginStateChangedEventHandler(SipAccount & sender,
	EnumSipLoginState::SipLoginState state) {
	loginStateChangedEventHandlerSignal(&sender, (int) state);
}

void QtUserProfile::networkDiscoveryStateChangedEventHandler(SipAccount & sender,
	SipAccount::NetworkDiscoveryState state) {
	networkDiscoveryStateChangedEventHandlerSignal(&sender, (int) state);
}

void QtUserProfile::authorizationRequestEventHandler(PresenceHandler & sender,
	const IMContact & imContact, const std::string & message) {
	authorizationRequestEventHandlerSignal(&sender, imContact, QString::fromStdString(message));
}

void QtUserProfile::incomingSubscribeEventHandler(PresenceHandler & sender,const std::string & imaccountId,int sid,const std::string & from,const std::string & evtType) 
{
	incomingSubscribeEventHandlerSignal(&sender, QString::fromStdString(imaccountId), sid,QString::fromStdString(from), QString::fromStdString(evtType));
}

void QtUserProfile::loginStateChangedEventHandlerSlot(SipAccount * sender, int iState) {
	EnumSipLoginState::SipLoginState state = (EnumSipLoginState::SipLoginState) iState;

	switch (state) {
	case EnumSipLoginState::SipLoginStateReady:
		//_qtWengoPhone.getQtBrowserWidget()->loadDefaultURL();
		break;

	case EnumSipLoginState::SipLoginStateConnected:
		//_qtWengoPhone.getQtBrowserWidget()->loadAccountURL();
		break;

	case EnumSipLoginState::SipLoginStateDisconnected:
		//_qtWengoPhone.getQtBrowserWidget()->loadDefaultURL();
		break;

	case EnumSipLoginState::SipLoginStatePasswordError:
		LOG_DEBUG("SipLoginStatePasswordError");
		break;

	case EnumSipLoginState::SipLoginStateNetworkError:
	{
		CUserProfileHandler & handler = _qtWengoPhone.getCWengoPhone().getCUserProfileHandler();
		QtLoginDialog dlg(&_qtWengoPhone, handler);
		dlg.setValidAccount((WengoAccount &) *sender);
		dlg.setErrorMessage(tr("Network error."));
		dlg.exec();
	}
		break;
	case EnumSipLoginState::SipLoginStateProgress:
		_qtWengoPhone.getQtStatusBar().updatePhoneLineState(EnumPhoneLineState::PhoneLineStateProgress);
		_qtWengoPhone.getQtSystray().phoneLineStateChanged(EnumPhoneLineState::PhoneLineStateProgress);
		break;
	
	default:
		LOG_FATAL("unknown state=" + String::fromNumber(state));
	}

	_qtWengoPhone.updatePresentation();
}

void QtUserProfile::networkDiscoveryStateChangedEventHandlerSlot(SipAccount * /*sender*/, int /*iState*/) {

	//SipAccount::NetworkDiscoveryState state = (SipAccount::NetworkDiscoveryState) iState;
}

void QtUserProfile::incomingSubscribeEventHandlerSlot(PresenceHandler * sender,const QString & imaccountId,int sid,const QString & from, const QString& evtType) 
{
	QString request = QString("%1 wants to see the presence state.\n")
			.arg(from);


		int buttonClicked = QMessageBox::question(_qtWengoPhone.getWidget(),
			tr("@product@ - Authorization Request"), request,
			tr("&Authorize"), tr("&Block"));

		if (buttonClicked == 0) {
			//TODO: give a personal message

			/*if (_cUserProfile.getCContactList().findContactThatOwns(imContact).empty()) {
				//If the contact is not in our ContactList
				ContactProfile contactProfile;
				contactProfile.addIMContact(imContact);
				QtProfileDetails qtProfileDetails(_cUserProfile, contactProfile, _qtWengoPhone.getWidget(), tr("Edit Contact"));
				if (qtProfileDetails.show()) {
					_cUserProfile.getCContactList().addContact(contactProfile);
				}
			}*/
			//sender->authorizeContact(imContact, true, String::null);
			//_cUserProfile.getUserProfile()
			sender->acceptSubscription(sid,imaccountId.toStdString());

		} else {
			sender->rejectSubscription(sid,imaccountId.toStdString());
			//TODO: give a personal message
			//TODO: avoid direct access to model (as we are in the GUI thread)
			//sender->authorizeContact(imContact, false, String::null);
		}
}

void QtUserProfile::authorizationRequestEventHandlerSlot(PresenceHandler * sender,
	IMContact imContact, QString message) {

	IMAccount * imAccount = _cUserProfile.getUserProfile().getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		QString request = QString("%1 (from %2) wants to see the presence state of %3.\n")
			.arg(QString::fromStdString(imContact.getContactId()))
			.arg(QString::fromStdString(EnumIMProtocol::toString(imContact.getProtocol())))
			.arg(QString::fromStdString(imAccount->getLogin()));

		if (!message.isEmpty()) {
			request += QString("%1\n").arg(message);
		}

		int buttonClicked = QMessageBox::question(_qtWengoPhone.getWidget(),
			tr("@product@ - Authorization Request"), request,
			tr("&Authorize"), tr("&Block"));

		if (buttonClicked == 0) {
			//TODO: give a personal message

			if (_cUserProfile.getCContactList().findContactThatOwns(imContact).empty()) {
				//If the contact is not in our ContactList
				ContactProfile contactProfile;
				contactProfile.addIMContact(imContact);
				QtProfileDetails qtProfileDetails(_cUserProfile, contactProfile, _qtWengoPhone.getWidget(), tr("Edit Contact"));
				if (qtProfileDetails.show()) {
					_cUserProfile.getCContactList().addContact(contactProfile);
				}
			}
			sender->authorizeContact(imContact, true, String::null);

		} else {
			//TODO: give a personal message
			//TODO: avoid direct access to model (as we are in the GUI thread)
			sender->authorizeContact(imContact, false, String::null);
		}
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccount not found!");
	}
}
