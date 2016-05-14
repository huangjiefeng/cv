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

#include "CChatHandler.h"

#include <presentation/PChatHandler.h>
#include <presentation/PFactory.h>

#include <control/profile/CUserProfile.h>
#include <control/CWengoPhone.h>

#include <model/chat/ChatHandler.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <thread/ThreadEvent.h>

CChatHandler::CChatHandler(ChatHandler & chatHandler, CUserProfile & cUserProfile)
	: _chatHandler(chatHandler),
	_cUserProfile(cUserProfile) {

	_pChatHandler = NULL;
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CChatHandler::initPresentationThreadSafe, this));
	PFactory::postEvent(event);
}

CChatHandler::~CChatHandler() {

	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&PChatHandler::deletePresentation, _pChatHandler));
	PFactory::postEvent(event);
}

void CChatHandler::initPresentationThreadSafe() {

	_pChatHandler = PFactory::getFactory().createPresentationChatHandler(*this);
	_chatHandler.newIMChatSessionCreatedEvent += boost::bind(&CChatHandler::newIMChatSessionCreatedEventHandler, this, _1, _2);
}

Presentation * CChatHandler::getPresentation() const {
	return _pChatHandler;
}

CWengoPhone & CChatHandler::getCWengoPhone() const {
	return _cUserProfile.getCWengoPhone();
}

CUserProfile & CChatHandler::getCUserProfile() const {
	return _cUserProfile;
}

void CChatHandler::newIMChatSessionCreatedEventHandler(ChatHandler & sender, IMChatSession & imChatSession) {
	typedef ThreadEvent1<void (IMChatSession &), IMChatSession &> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CChatHandler::newIMChatSessionCreatedEventHandlerThreadSafe, this, _1), imChatSession);
	PFactory::postEvent(event);
}

void CChatHandler::newIMChatSessionCreatedEventHandlerThreadSafe(IMChatSession & imChatSession) {
	LOG_DEBUG("new IMChatSessionCreatedEvent");
	if (_pChatHandler) {
		_pChatHandler->newIMChatSessionCreatedEvent(imChatSession);
	}
}

void CChatHandler::createSession(const std::string & imAccountId, const IMContactSet & imContactSet) {
	_chatHandler.createSession(imAccountId, imContactSet);
}


