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

#include "QtChatAvatarFrame.h"
#include "QtChatAvatarWidget.h"

#include <util/Logger.h>

#include <stdio.h>

#include <QtGui/QPainter>

QtChatAvatarFrame::QtChatAvatarFrame(QWidget * parent)
	: QFrame(parent) {

	_ui.setupUi(this);
	setMinimumWidth(70);
	_layout = new QVBoxLayout(_ui.remoteAvatarFrame);
	_layout->setMargin(0);
	_layout->setSpacing(0);
}

void QtChatAvatarFrame::setUserPixmap(QPixmap pixmap) {

	QPixmap background = QPixmap(":pics/avatar_background.png");

	if (!pixmap.isNull()) {
		QPainter painter(&background);
		painter.drawPixmap(5, 5, pixmap.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.end();
		_ui.userAvatar->setPixmap(background);
	} else {
		_ui.userAvatar->setPixmap(background);
	}
}

void QtChatAvatarFrame::addRemoteContact(const QString & id, const QString & displayName, const QString & contactId, QPixmap avatar) {

	_contactIdList.append(id);

	QtChatAvatarWidget * avatarWidget = new QtChatAvatarWidget(
		_ui.remoteAvatarFrame, id, avatar, displayName, contactId, QtChatAvatarWidget::BIG);
	avatarWidget->setupPixmap(avatar);
	
	_widgetList.append(avatarWidget);
	_layout->addWidget(avatarWidget);

	//_ui.remoteAvatarFrame->setMinimumSize(0, glayout->count() * 70);
}

void QtChatAvatarFrame::removeRemoteContact(const QString & contactId) {

	for (int i = 0; i < _widgetList.size(); i++) {
		if (_widgetList[i]->getContactId() == contactId) {
			delete _widgetList[i];
			_widgetList.removeAt(i);
		}
	}
	_contactIdList.removeAll(contactId);
}

void QtChatAvatarFrame::updateContact(const QString & id, QPixmap avatar, const QString & displayName) {
	for(int i = 0; i < _widgetList.size(); i++) {
		QtChatAvatarWidget * chatAvatarWidget = _widgetList[i];
		if (chatAvatarWidget->getContactId() == id) {
			chatAvatarWidget->setupPixmap(avatar);
			chatAvatarWidget->setToolTip(displayName);
		}
	}
}

QStringList QtChatAvatarFrame::getContactIDList() {
	return _contactIdList;
}
