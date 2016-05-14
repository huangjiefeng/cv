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

#include "ChatHandler.h"

#include <model/profile/UserProfile.h>

#include <imwrapper/IMAccount.h>
#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMWrapperFactory.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

ChatHandler::ChatHandler(UserProfile & userProfile)
	: _userProfile(userProfile) {

	_userProfile.getIMAccountManager().imAccountAddedEvent +=
		boost::bind(&ChatHandler::imAccountAddedEventHandler, this, _1, _2);
	_userProfile.getIMAccountManager().imAccountRemovedEvent +=
		boost::bind(&ChatHandler::imAccountRemovedEventHandler, this, _1, _2);
}

ChatHandler::~ChatHandler() {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (IMChatSessionList::const_iterator it = _imChatSessionList.begin();
		it != _imChatSessionList.end();
		it++) {
		delete (*it);
	}

	for (IMChatMap::const_iterator it = _imChatMap.begin();
		it != _imChatMap.end();
		++it) {
		delete (*it).second;
	}
}

void ChatHandler::createSession(const std::string & imAccountId, const IMContactSet & imContactSet) {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMAccount * imAccount = 
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);

	if (imAccount) {
		LOG_DEBUG("creating new IMChatSession for: " + imAccount->getLogin());
		IMChatMap::iterator it = _imChatMap.find(imAccount->getUUID());
		if (it != _imChatMap.end()) {
			(*it).second->createSession(imContactSet);
		} else {
			LOG_ERROR("No IMChat created for this IMAccount");
		}
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccount not found!");
	}
}

void ChatHandler::newIMChatSessionCreatedEventHandler(IMChat & sender, IMChatSession & imChatSession) {
	RecursiveMutex::ScopedLock lock(_mutex);

	LOG_DEBUG("a new IMChatSession has been created");
	_imChatSessionList.push_back(&imChatSession);
	imChatSession.imChatSessionWillDieEvent +=
		boost::bind(&ChatHandler::imChatSessionWillDieEventHandler, this, _1);
	newIMChatSessionCreatedEvent(*this, imChatSession);
}

void ChatHandler::imChatSessionWillDieEventHandler(IMChatSession & sender) {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMChatSessionList::iterator it =
		std::find(_imChatSessionList.begin(), _imChatSessionList.end(), &sender);

	if (it != _imChatSessionList.end()) {
		_imChatSessionList.erase(it);
	} else {
		LOG_ERROR("IMChatSession not in ChatHandler");
	}
}

void ChatHandler::imAccountAddedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		LOG_DEBUG("new account added: login: " + imAccount->getLogin()
			+ " protocol: " + EnumIMProtocol::toString(imAccount->getProtocol()));

		IMChatMap::iterator it = _imChatMap.find(imAccountId);
		if (it == _imChatMap.end()) {
			IMChat * imChat = IMWrapperFactory::getFactory().createIMChat(*imAccount);
			imChat->newIMChatSessionCreatedEvent +=
				boost::bind(&ChatHandler::newIMChatSessionCreatedEventHandler, this, _1, _2);

			_imChatMap.insert(std::pair<std::string, IMChat *>(imAccountId, imChat));
		} else {
			LOG_ERROR("this IMAccount has already been added " + imAccount->getLogin());
		}
		OWSAFE_DELETE(imAccount);
	}
}

void ChatHandler::imAccountRemovedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMChatMap::iterator it = _imChatMap.find(imAccountId);

	if (it != _imChatMap.end()) {
		//TODO close all IMChatSession opened with this IMAccount
		delete (*it).second;
		_imChatMap.erase(it);
	} else {
		LOG_ERROR("this IMAccount has not been added: " + imAccountId);
	}
}

void ChatHandler::unregisterAllEvents() {
	_userProfile.getIMAccountManager().imAccountAddedEvent -=
		boost::bind(&ChatHandler::imAccountAddedEventHandler, this, _1, _2);
	_userProfile.getIMAccountManager().imAccountRemovedEvent -=
		boost::bind(&ChatHandler::imAccountRemovedEventHandler, this, _1, _2);

	for (IMChatMap::const_iterator it = _imChatMap.begin();
		it != _imChatMap.end();
		++it) {
		(*it).second->newIMChatSessionCreatedEvent -=
			boost::bind(&ChatHandler::newIMChatSessionCreatedEventHandler, this, _1, _2);
	}
}
