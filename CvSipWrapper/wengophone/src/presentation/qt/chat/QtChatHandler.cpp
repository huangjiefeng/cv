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

#include "QtChatHandler.h"

#include "QtChatWindow.h"
#include "emoticons/QtEmoticonsManager.h"

#include <control/chat/CChatHandler.h>

#include <presentation/qt/QtWengoPhone.h>

#include <util/Logger.h>

QtChatHandler::QtChatHandler(CChatHandler & cChatHandler, QtWengoPhone & qtWengoPhone)
	: QObject(NULL),
	_cChatHandler(cChatHandler),
	_qtWengoPhone(qtWengoPhone) {

	_qtChatWindow = NULL;
}

QtChatHandler::~QtChatHandler() {
	if (_qtChatWindow) {
		_qtChatWindow->hide();
		delete _qtChatWindow;
	}
}

void QtChatHandler::newIMChatSessionCreatedEvent(IMChatSession & imChatSession) {
	if (!_qtChatWindow) {
		_qtChatWindow =  new QtChatWindow(0, _cChatHandler, imChatSession, _qtWengoPhone);
	} else {
		_qtChatWindow->imChatSessionCreatedHandler(&imChatSession);
	}
}

void QtChatHandler::createSession(const QString & imAccountId, IMContactSet & imContactSet) {
	_cChatHandler.createSession(imAccountId.toStdString(), imContactSet);
}

void QtChatHandler::updatePresentation() {
}

void QtChatHandler::deletePresentation() {
	// we should delete the QtChatWindow
	// but it crashes in QtEmoticonsManager::getQtEmoticonList
	/* delete(this); */
	if (_qtChatWindow) {
		//_qtChatWindow->closeWindow();
		_qtChatWindow->close();
		_qtWengoPhone.setChatWindow(NULL);
	}
}
