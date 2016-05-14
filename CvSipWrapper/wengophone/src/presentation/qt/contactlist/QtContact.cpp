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

#include "QtContact.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/profile/UserProfileHandler.h>
#include <model/profile/UserProfile.h>

#include <control/CWengoPhone.h>
#include <control/contactlist/CContactList.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/filetransfer/QtFileTransfer.h>
#include <presentation/qt/webservices/sms/QtSms.h>
#include <presentation/qt/QtNoWengoAlert.h>

#include <util/Logger.h>

#include <QtGui/QtGui>

QtContact::QtContact(const ContactProfile * contactProfile, CWengoPhone & cWengoPhone, QObject * parent)
	: QObject(parent),
	_cWengoPhone(cWengoPhone) {

	_contactProfile = *contactProfile;
	_contactId = _contactProfile.getUUID();
	_mouseOn = false;
	_openStatus = false;
}

QtContact::QtContact(const std::string & contactId, CWengoPhone & cWengoPhone, QObject * parent)
	: QObject(parent),
	_cWengoPhone(cWengoPhone) {

	_contactId = contactId;
	contactUpdated();
	_mouseOn = false;
	_openStatus = false;
}

void QtContact::paint(QPainter * painter, const QStyleOptionViewItem & option) {
	if ((option.state & QStyle::State_Selected) == QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
	}
	paintForeground(painter, option);
}

void QtContact::paintForeground(QPainter * painter, const QStyleOptionViewItem & option) {
	QMutexLocker locker(&_mutex);

	QtContactPixmap * spx = QtContactPixmap::getInstance();

	if ((option.state & QStyle::State_Selected) == QStyle::State_Selected) {
		painter->setPen(option.palette.highlightedText().color());
	} else {
		painter->setPen(option.palette.text().color());
	}

	// Draw the status pixmap
	QtContactPixmap::ContactPixmap status = getStatus();

	QPixmap px = spx->getPixmap(status);
	QRect painterRect = option.rect;
	painter->drawPixmap(painterRect.left(), painterRect.top() + 2, px);

	painterRect.setLeft(painterRect.left() + px.width() + 5);

	// Draw the text
	painter->setFont(option.font);
	// Center the text vertically
	QRect textRect = painterRect;
	int textY = (UserSize - QFontMetrics(option.font).height()) / 2;
	textRect.setTop(textY + textRect.top());

	QString text = QString::fromUtf8(_contactProfile.getDisplayName().c_str());
	painter->drawText(textRect, Qt::AlignLeft, text, 0);
}

QString QtContact::getId() const {
	return QString::fromStdString(_contactId);
}

QString QtContact::getUserName() const {
	return QString::fromUtf8(_contactProfile.getDisplayName().c_str());
}

QString QtContact::getGroupName() const {
	std::string groupId = _contactProfile.getGroupId();
	std::string groupName = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList().getContactGroupName(groupId);
	return QString::fromUtf8(groupName.c_str());
}

bool QtContact::isTestCall() const {
	std::string sipAddress = _contactProfile.getFirstFreePhoneNumber();
	if (sipAddress.empty()) {
		return false;
	}

	// Check if the sipAddress is one of the test calls. We check all test
	// calls because we don't have the information about whether we are
	// connected with a SIP or a Wengo account here.
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (sipAddress == config.getWengoAudioTestCall()) {
		return true;
	}
	if (sipAddress == config.getWengoVideoTestCall()) {
		return true;
	}
	if (sipAddress == config.getSipAudioTestCall()) {
		return true;
	}
	if (sipAddress == config.getSipVideoTestCall()) {
		return true;
	}
	return false;
}

QtContactPixmap::ContactPixmap QtContact::getStatus() const {
	// Rules are defined here:
	// http://dev.openwengo.org/trac/openwengo/trac.cgi/wiki/ContactPresenceGrid
	// version 9
	if (getGroupName() == "WDeal") {
		return QtContactPixmap::ContactWDeal;
	}

	if (isTestCall()) {
		return QtContactPixmap::ContactTestCall;
	}

	QtContactPixmap::ContactPixmap status;
	bool hasPSTNCall = _contactProfile.hasCall() && !_contactProfile.hasFreeCall();

	switch (_contactProfile.getPresenceState()) {
	case EnumPresenceState::PresenceStateUnavailable:
		LOG_INFO("Received PresenceStateUnavailable state");
		// Fall through intended
	case EnumPresenceState::PresenceStateUnknown:
		if (_contactProfile.hasAvailableSIPNumber()) {
			// SIP
			status = QtContactPixmap::ContactNoStatusSIP;
		} else if (hasPSTNCall) {
			status = QtContactPixmap::ContactNoStatus;
		} else {
			status = QtContactPixmap::ContactUnknown;
		}
		break;
	case EnumPresenceState::PresenceStateOnline:
		status = QtContactPixmap::ContactOnline;
		break;
	case EnumPresenceState::PresenceStateDoNotDisturb:
		status = QtContactPixmap::ContactDND;
		break;
	case EnumPresenceState::PresenceStateAway:
		status = QtContactPixmap::ContactAway;
		break;
	case EnumPresenceState::PresenceStateInvisible:
		status = QtContactPixmap::ContactInvisible;
		break;
	case EnumPresenceState::PresenceStateOffline:
		if (hasPSTNCall) {
			status = QtContactPixmap::ContactNoStatus;
		} else {
			status = QtContactPixmap::ContactOffline;
		}
		break;
	default:
		status = QtContactPixmap::ContactUnknown;
		LOG_FATAL("unknown state=" + String::fromNumber(_contactProfile.getPresenceState()));
	}
	return status;
}

void QtContact::startChat() {
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->startIM(_contactId);
}

void QtContact::startSMS() {
	QtWengoPhone * qtWengoPhone = QtContact::_cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28
	UserProfile * userprofile = _cWengoPhone.getCUserProfileHandler().getUserProfileHandler().getCurrentUserProfile();
	if (userprofile && userprofile->hasWengoAccount()) {
		QtSms * sms = qtWengoPhone->getQtSms();
		if (sms) {
			sms->setPhoneNumber(getMobilePhone());
			sms->setText(QString::null);
			sms->setSignature(QString::null);
			sms->getWidget()->show();
		}
	} else {
		QtNoWengoAlert alert(qtWengoPhone->getWidget(), *qtWengoPhone);
		alert.getQDialog()->exec();
	}
}

void QtContact::setButton(const Qt::MouseButton button) {
	_mouseButton = button;
}

Qt::MouseButton QtContact::getButton() const {
	return _mouseButton;
}

void QtContact::setOpenStatus(bool value) {
	_openStatus = value;
}

int QtContact::getHeight() const {
	if (_openStatus) {
		return 95;
	}
	return UserSize;
}

QString QtContact::getMobilePhone() const {
	return QString::fromStdString(_contactProfile.getMobilePhone());
}

QString QtContact::getHomePhone() const {
	return QString::fromStdString(_contactProfile.getHomePhone());
}

QString QtContact::getWebsite() const {
	return QString::fromStdString(_contactProfile.getWebsite());
}

QString QtContact::getWorkPhone() const {
	return QString::fromStdString(_contactProfile.getWorkPhone());
}

QString QtContact::getWengoPhoneNumber() const {
	return QString::fromStdString(_contactProfile.getFirstAvailableWengoId());
}

QString QtContact::getPreferredNumber() const {
	return QString::fromStdString(_contactProfile.getPreferredNumber());
}

bool QtContact::hasPhoneNumber() const {
	if (!getMobilePhone().isEmpty()) {
		return true;
	}
	if (!getHomePhone().isEmpty()) {
		return true;
	}
	if (!getWorkPhone().isEmpty()) {
		return true;
	}
	if (!getWengoPhoneNumber().isEmpty()) {
		return true;
	}
	return false;
}

bool QtContact::hasIM() const {
	return _contactProfile.hasIM();
}

bool QtContact::hasFreeCall() const {
	return _contactProfile.hasFreeCall();
}

bool QtContact::hasCall() const {
	return _contactProfile.hasCall();
}

bool QtContact::hasVideo() const {
	return _contactProfile.hasVideo();
}

bool QtContact::hasFileTransfer() const {
	return _contactProfile.hasFileTransfer();
}

void QtContact::startCall(const QString & number) {
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->makeCall(number.toStdString());
}

void QtContact::startCall() {
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->makeContactCall(_contactId);
}

void QtContact::sendFile() {

	if (_contactProfile.hasAvailableWengoId()) {
		QtWengoPhone * qtWengoPhone = QtContact::_cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28
		IMContactSet contactList;

		IMContact imContact = _contactProfile.getFirstAvailableWengoIMContact();
		contactList.insert(imContact);

		QtFileTransfer * qtFileTransfer = qtWengoPhone->getFileTransfer();
		if (qtFileTransfer) {

			QString filename = qtFileTransfer->getChosenFile();
			if (!filename.isEmpty()) {

				qtFileTransfer->createSendFileSession(contactList, filename,
					_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList());
			}
		}
	}
}

void QtContact::contactUpdated() {
	_contactProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList().getContactProfile(_contactId);
}

void QtContact::startFreeCall() {
	std::string phoneNumber = _contactProfile.getFirstFreePhoneNumber();
	if (phoneNumber.empty()) {
		phoneNumber = _contactProfile.getFirstVoiceMailNumber();
	}
	if (phoneNumber.empty()) {
		LOG_WARN("Couldn't find neither a free phone number, nor a voice mail number");
		return;
	}
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->makeCall(phoneNumber);
}

QString QtContact::getDisplayName() const {
	return QString::fromStdString(_contactProfile.getDisplayName());
}

EnumPresenceState::PresenceState QtContact::getPresenceState() const {
	return _contactProfile.getPresenceState();
}
