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

#include "QtContactCallListWidget.h"

#include <control/CWengoPhone.h>
#include <control/phonecall/CPhoneCall.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/phonecall/ConferenceCall.h>
#include <model/phonecall/PhoneCall.h>
#include <model/profile/UserProfile.h>

#include <presentation/qt/phonecall/QtPhoneCall.h>

#include <QtGui/QtGui>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

QtContactCallListWidget::QtContactCallListWidget(CWengoPhone & cWengoPhone, QWidget * parent)
	: QWidget(parent),
	_cWengoPhone(cWengoPhone) {

	// Setup our sunken frame
	QFrame * frame = new QFrame(this);
	frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

	QVBoxLayout* layout = new QVBoxLayout(this);
#if defined(OS_MACOSX)
	layout->setMargin(10);
#else
	layout->setMargin(0);
#endif
	layout->addWidget(frame);

	// Create _content

	// We need to create a separate "content" widget, because if we call
	// setBackgroundRole(QPalette::Base) on "frame" the "Base" color will be
	// visible in the corners of the frame.
	_content = new QWidget(frame);
	_content->setBackgroundRole(QPalette::Base);
	_content->setAutoFillBackground(true);

	// Add _content to frame
	layout = new QVBoxLayout(frame);
	layout->setMargin(0);
	layout->addWidget(_content);

	// Create a layout for _content. This layout will hold all the QtPhoneCall
	// widgets
	layout = new QVBoxLayout(_content);
	layout->setMargin(0);
	////

	//Leads to a crash in conference call when
	//everybody hangs up
	//setAttribute(Qt::WA_DeleteOnClose, true);

	_conferenceState = CS_CallingFirst;
}

QtContactCallListWidget::~QtContactCallListWidget() {
}

void QtContactCallListWidget::addPhoneCall(QtPhoneCall * qtPhoneCall) {
	_phoneCallList.append(qtPhoneCall);

	qtPhoneCall->getWidget()->setParent(_content);

	// Add separator if necessary
	if (_content->layout()->count() > 0) {
		QFrame * separator = new QFrame(_content);
		separator->setFrameStyle(QFrame::HLine | QFrame::Sunken);
		separator->setFixedHeight(4);
		_content->layout()->addWidget(separator);
	}

	_content->layout()->addWidget(qtPhoneCall->getWidget());
	SAFE_CONNECT(qtPhoneCall, SIGNAL(destroyed(QObject*)),
		SLOT(slotQtPhoneCallDestroyed(QObject*)));

	if (qtPhoneCall->getCPhoneCall().getPhoneCall().getConferenceCall()) {
		SAFE_CONNECT(qtPhoneCall, SIGNAL(startedTalking(QtPhoneCall*)),
			SLOT(slotStartedTalking(QtPhoneCall*)) );
	}
}

void QtContactCallListWidget::slotStartedTalking(QtPhoneCall * qtPhoneCall) {
	//QMutexLocker locker(&_mutex);

	//Hack
	//Fix a crash in conference mode when QtPhoneCall comes NULL while
	//this function is not ended
	_qtPhoneCall = qtPhoneCall;

	switch (_conferenceState) {
	case CS_CallingFirst:
		QMessageBox::information(
			this,
			tr("@product@ - Proceed Conference"),
			tr("<b>You can now inform your first interlocutor he is going to be part of a conference call.</b><br><br>"
			"When it's done, click the button below. Your first interlocutor will be put on hold, while your second interlocutor will be called.<br>"),
			tr("Hold first interlocutor and call second one")
			);

		if (_qtPhoneCall) {
			_qtPhoneCall->holdOrResume();
			_conferenceState = CS_FirstOnHold;
		}
		return;

	case CS_FirstOnHold:
		QMessageBox::information(
			this,
			tr("@product@ - Proceed Conference"),
			tr("<b>You can now inform your second interlocutor he is going to be part of a conference call.</b><br><br>"
			"When it's done, click the button below to start the conference."),
			tr("Start Conference")
			);

		if (_qtPhoneCall) {
			_qtPhoneCall->holdOrResume();
			_conferenceState = CS_Started;
		}
		return;

	case CS_Started:
		// Nothing to do
		break;
	}
}

void QtContactCallListWidget::hangup() {
	QMutexLocker locker(&_mutex);

	QtPhoneCall * phone;

	QtPhoneCallList::iterator iter;

	for (iter = _phoneCallList.begin(); iter != _phoneCallList.end(); iter++) {
		phone = (*iter);

		phone->rejectCall();
	}
	_phoneCallList.clear();
}

bool QtContactCallListWidget::hasPhoneCall(PhoneCall * phoneCall) {
	QMutexLocker locker(&_mutex);

	QtPhoneCallList::iterator iter;
	QtPhoneCall * tmp = NULL;
	for (iter = _phoneCallList.begin(); iter != _phoneCallList.end(); iter++) {
		tmp = (*iter);
		if (tmp->getCPhoneCall().getPhoneCall().getCallId() == phoneCall->getCallId()) {
			return true;
		}
	}
	return false;
}

QtPhoneCall * QtContactCallListWidget::takeQtPhoneCall(PhoneCall * phoneCall) {
	QMutexLocker locker(&_mutex);

	QtPhoneCallList::iterator iter;
	for (iter = _phoneCallList.begin(); iter != _phoneCallList.end(); iter++) {
		QtPhoneCall * qtphoneCall = *iter;
		if (qtphoneCall->getCPhoneCall().getPhoneCall().getPeerSipAddress().getUserName() ==
				phoneCall->getPeerSipAddress().getUserName()) {
			_content->layout()->removeWidget(qtphoneCall->getWidget());
			_phoneCallList.erase(iter);
			return qtphoneCall;
		}
	}
	return NULL;
}

QtPhoneCall * QtContactCallListWidget::getFirstQtPhoneCall() {
	if (_phoneCallList.count() == 0) {
		return NULL;
	}
	return _phoneCallList[0];
}

bool QtContactCallListWidget::hasActivePhoneCall() {
	QMutexLocker locker(&_mutex);

	QtPhoneCallList::iterator iter;
	for (iter = _phoneCallList.begin(); iter != _phoneCallList.end(); iter++) {
		if ((*iter)->getCPhoneCall().getState() == EnumPhoneCallState::PhoneCallStateTalking) {
			return true;
		}
	}
	return false;
}

void QtContactCallListWidget::slotQtPhoneCallDestroyed(QObject * qtPhoneCall) {
	QMutexLocker locker(&_mutex);

	QtPhoneCallList::iterator iter;
	QtPhoneCall * tmp = NULL;
	for (iter = _phoneCallList.begin(); iter != _phoneCallList.end(); iter++) {
		tmp = (*iter);
		if (tmp == qtPhoneCall) {
			_phoneCallList.erase(iter);
			break;
		}
	}
	if (_phoneCallList.size() == 0) {
		
		close();
		QTabWidget * tabWidget = qobject_cast<QTabWidget *>(parentWidget()->parentWidget());
		if (!tabWidget) {
			LOG_FATAL("parent QWidget is not a QTabWidget");
		}
		int index = tabWidget->indexOf(this);
		tabWidget->removeTab(index);

		aboutToClose();
	}
	_qtPhoneCall = NULL;
}
