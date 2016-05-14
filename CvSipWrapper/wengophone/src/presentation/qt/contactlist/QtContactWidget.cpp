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

#include "QtContactWidget.h"

#include "ui_ContactWidget.h"

#include "QtContact.h"
#include "QtContactListManager.h"
#include "QtContactManager.h"

#include <presentation/qt/contactlist/QtContactListManager.h>
#include <presentation/qt/profile/QtProfileDetails.h>

#include <control/CWengoPhone.h>
#include <control/contactlist/CContactList.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/contactlist/ContactProfile.h>

#include <util/OWPicture.h>
#include <util/Logger.h>

#include <qtutil/PixmapMerging.h>
#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

static const std::string AVATAR_BACKGROUND = ":/pics/avatar_background.png";

QtContactWidget::QtContactWidget(const std::string & contactId, CWengoPhone & cWengoPhone,
	QtContactManager * qtContactManager, QWidget * parent)
	: QWidget(parent),
	_cWengoPhone(cWengoPhone) {
	_contactId = contactId;

	_ui = new Ui::ContactWidget();
	_ui->setupUi(this);

	QColor bgColor = palette().highlight().color();
	QColor fgColor = palette().highlightedText().color();

	QPalette pal(bgColor);
	pal.setColor(QPalette::Text, fgColor);
	pal.setColor(QPalette::ButtonText, fgColor);

	setPalette(pal);
	Q_FOREACH(QToolButton* button, findChildren<QToolButton*>()) {
		button->setPalette(pal);
	}

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (!config.getSmsFeatureEnabled()) {
		_ui->smsButton->hide();
	}
	updateButtons();

	SAFE_CONNECT(_ui->callButton, SIGNAL(clicked()), SLOT(callButtonClicked()));
	SAFE_CONNECT(_ui->chatButton, SIGNAL(clicked()), SLOT(chatButtonClicked()));
	SAFE_CONNECT(_ui->smsButton, SIGNAL(clicked()), SLOT(smsButtonClicked()));
	SAFE_CONNECT(_ui->landlineButton, SIGNAL(clicked()), SLOT(landlineButtonClicked()));
	SAFE_CONNECT(_ui->mobileButton, SIGNAL(clicked()), SLOT(mobileButtonClicked()));
	SAFE_CONNECT(_ui->avatarButton, SIGNAL(clicked()), SLOT(avatarButtonClicked()));
	SAFE_CONNECT(_ui->sendFileButton, SIGNAL(clicked()), SLOT(sendFileButtonClicked()));
	SAFE_CONNECT_RECEIVER_TYPE(this, SIGNAL(editContact(QString)),
		qtContactManager, SLOT(editContact(QString)), Qt::QueuedConnection);

#ifdef DISABLE_SMS
	_ui->smsButton->hide();
#endif

	//隐藏不用的按钮  zhenHua.sun 2010-09-03
	_ui->sendFileButton->hide();
	_ui->chatButton->hide();
	_ui->callButton->hide();
}

QtContactWidget::~QtContactWidget() {
	delete _ui;
}

void QtContactWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	QPalette pal = palette();

	QRect r = rect();
	QLinearGradient lg(QPointF(1, r.top()), QPointF(1, r.bottom()));
	QColor color = pal.highlight().color();
	lg.setColorAt(0, color);
	lg.setColorAt(1, color.dark(150));

	pal.setBrush(backgroundRole(), QBrush(lg));
	setPalette(pal);
}

void QtContactWidget::updateButtons() {
	ContactProfile contactProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList().getContactProfile(_contactId);
	std::string foregroundPixmapData = contactProfile.getIcon().getData();
	_ui->avatarButton->setIcon(PixmapMerging::merge(foregroundPixmapData, AVATAR_BACKGROUND));

	QString str = QString::fromUtf8(contactProfile.getHomePhone().c_str());
	if (str.isEmpty()) {
		_ui->landlineButton->setText(tr("No landline phone number set"));
	} else {
		_ui->landlineButton->setText(str);
	}

	str = QString::fromUtf8(contactProfile.getMobilePhone().c_str());
	if (str.isEmpty()) {
		_ui->mobileButton->setText(tr("No mobile phone number set"));
		_ui->smsButton->setEnabled(false);
	} else {
		_ui->mobileButton->setText(str);
		_ui->smsButton->setEnabled(true);
	}

	_ui->callButton->setEnabled(contactProfile.hasFreeCall() || contactProfile.hasVoiceMail());

	_ui->chatButton->setEnabled(contactProfile.hasIM());

	_ui->sendFileButton->setEnabled(contactProfile.hasFileTransfer());
}

void QtContactWidget::callButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	ul->startFreeCall(QString::fromStdString(_contactId));
}

void QtContactWidget::smsButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	ul->startSMS(QString::fromStdString(_contactId));
}

void QtContactWidget::chatButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	ul->startChat(QString::fromStdString(_contactId));
}

void QtContactWidget::sendFileButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	ul->sendFile(QString::fromStdString(_contactId));
}

void QtContactWidget::updateToolTips() {

	QtContactListManager * ul = QtContactListManager::getInstance();

	if (!ul->getMobilePhone(QString::fromStdString(_contactId)).isEmpty()) {
		_ui->mobileButton->setToolTip(tr("Click here to call"));
	} else {
		_ui->mobileButton->setToolTip(tr("Click here to set a phone number"));
	}

	if (!ul->getHomePhone(QString::fromStdString(_contactId)).isEmpty()) {
		_ui->landlineButton->setToolTip(tr("Click here to call"));
	} else {
		_ui->landlineButton->setToolTip(tr("Click here to set a phone number"));
	}

	_ui->avatarButton->setToolTip(tr("Click here to edit the contact"));

	if (_ui->smsButton->isEnabled()) {
		_ui->smsButton->setToolTip(tr("Click here to send a SMS"));
	} else {
		_ui->smsButton->setToolTip(QString::null);
	}

	if (_ui->chatButton->isEnabled()) {
		_ui->chatButton->setToolTip(tr("Click here to start a chat"));
	} else {
		_ui->chatButton->setToolTip(QString::null);
	}

	if (_ui->callButton->isEnabled()) {
		_ui->callButton->setToolTip(tr("Click here to start a free call"));
	} else {
		_ui->callButton->setToolTip(QString::null);
	}

	if (_ui->sendFileButton->isEnabled()) {
		_ui->sendFileButton->setToolTip(tr("Click here to send a file"));
	} else {
		_ui->sendFileButton->setToolTip(QString::null);
	}
}

void QtContactWidget::mobileButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	if (!ul->getMobilePhone(QString::fromStdString(_contactId)).isEmpty()) {
		ul->startCall(QString::fromStdString(_contactId), _ui->mobileButton->text());
	} else {
		editContact(_text);
	}
}

void QtContactWidget::landlineButtonClicked() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	if (!ul->getHomePhone(QString::fromStdString(_contactId)).isEmpty()) {
		ul->startCall(QString::fromStdString(_contactId), _ui->landlineButton->text());
	} else {
		editContact(_text);
	}
}

void QtContactWidget::avatarButtonClicked() {
	editContact(_text);
}

void QtContactWidget::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	paintContact(&painter, rect());
	painter.end();
}

void QtContactWidget::paintContact(QPainter * painter, const QRect & rect) {
	QtContactListManager * ul = QtContactListManager::getInstance();

	QtContact * qtContact = ul->getContact(QString::fromStdString(_contactId));
	if (!qtContact) {
		return;
	}

	QStyleOptionViewItem option;
	option.initFrom(ul->getTreeWidget());
	option.state = QStyle::State_Selected;
	qtContact->paintForeground(painter, option);
	updateToolTips();
}
