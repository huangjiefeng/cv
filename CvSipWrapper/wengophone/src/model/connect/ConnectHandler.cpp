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

#include "ConnectHandler.h"

#include <model/connect/Connect.h>
#include <model/network/NetworkObserver.h>
#include <model/profile/UserProfile.h>
#include <model/WengoPhone.h>

#include <imwrapper/EnumIMProtocol.h>

#include <thread/ThreadEvent.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

ConnectHandler::ConnectHandler(UserProfile * userProfile)
	: _userProfile(userProfile) {

	_userProfile->getIMAccountManager().imAccountRemovedEvent +=
		boost::bind(&ConnectHandler::imAccountRemovedEventHandler, this, _1, _2);

	NetworkObserver::getInstance().connectionIsDownEvent +=
		boost::bind(&ConnectHandler::connectionIsDownEventHandler, this, _1);

	NetworkObserver::getInstance().connectionIsUpEvent +=
		boost::bind(&ConnectHandler::connectionIsUpEventHandler, this, _1);
		
	_freeze = false;
}

ConnectHandler::~ConnectHandler() {
	for (ConnectMap::const_iterator it = _connectMap.begin();
		it != _connectMap.end();
		++it) {
		delete (*it).second;
	}
}

void ConnectHandler::connect(const std::string & imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	Connect * connect = NULL;
	IMAccount * imAccount = 
		_userProfile->getIMAccountManager().getIMAccount(imAccountId);

	if (imAccount) {
		ConnectMap::iterator it = _connectMap.find(imAccountId);
		if (it == _connectMap.end()) {
			connect = new Connect(*imAccount, *_userProfile);

			connect->connectedEvent +=
				boost::bind(&ConnectHandler::connectedEventHandler, this, _1);
			connect->disconnectedEvent +=
				boost::bind(&ConnectHandler::disconnectedEventHandler, this, _1, _2, _3);
			connect->connectionProgressEvent +=
				boost::bind(&ConnectHandler::connectionProgressEventHandler, this, _1, _2, _3, _4);

			_connectMap.insert(std::pair<std::string, Connect*>(imAccountId, connect));
		} else {
			connect = (*it).second;
		}

		if (!(imAccount->isConnected())) {
			connect->connect();
		}
		
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccount " + imAccountId + " not found!");
	}
}

void ConnectHandler::disconnect(const std::string & imAccountId) {
	ConnectMap::iterator it = _connectMap.find(imAccountId);
	if (it != _connectMap.end()) {
		(*it).second->disconnect();
	}
}

void ConnectHandler::connectedEventHandler(IMConnect & sender) {
	typedef ThreadEvent1<void (std::string imAccountId), std::string> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&ConnectHandler::connectedEventHandlerThreadSafe, this, _1), sender.getIMAccountId());

	WengoPhone::getInstance().postEvent(event);
}

void ConnectHandler::connectedEventHandlerThreadSafe(std::string imAccountId) {
	if (_userProfile && !_freeze) {
		IMAccount * imAccount =
			_userProfile->getIMAccountManager().getIMAccount(imAccountId);
		if (imAccount) {
			imAccount->setConnected(true);
			_userProfile->updateIMAccount(*imAccount);
			OWSAFE_DELETE(imAccount);
			connectedEvent(*this, imAccountId);
		}
	}
}

void ConnectHandler::disconnectedEventHandler(IMConnect & sender, bool connectionError, std::string reason) {
/*
	typedef ThreadEvent3<void (IMAccount * imAccount, bool connectionError, std::string reason), IMAccount* , bool, std::string> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&ConnectHandler::disconnectedEventHandlerThreadSafe, this, _1, _2, _3), &sender.getIMAccount(), connectionError, reason);

	_modelThread.postEvent(event);
	FIXME: crashes when trying to change UserProfile
*/
	disconnectedEventHandlerThreadSafe(sender.getIMAccountId(), connectionError, reason);
}

void ConnectHandler::disconnectedEventHandlerThreadSafe(std::string imAccountId, bool connectionError, std::string reason) {
	if (_userProfile) {
		IMAccount * imAccount =
			_userProfile->getIMAccountManager().getIMAccount(imAccountId);
		if (imAccount) {
			imAccount->setConnected(false);
			_userProfile->updateIMAccount(*imAccount);
			OWSAFE_DELETE(imAccount);
			disconnectedEvent(*this, imAccountId, connectionError, reason);
		}
	}
}

void ConnectHandler::connectionProgressEventHandler(IMConnect & sender,
	int currentStep, int totalSteps, std::string infoMessage) {

	connectionProgressEventHandlerThreadSafe(sender.getIMAccountId(), currentStep, totalSteps, infoMessage);
}

void ConnectHandler::connectionProgressEventHandlerThreadSafe(std::string imAccountId,
	int currentStep, int totalSteps, std::string infoMessage) {

	connectionProgressEvent(*this, imAccountId, currentStep, totalSteps, infoMessage);
}

void ConnectHandler::imAccountRemovedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ConnectMap::iterator it = _connectMap.find(imAccountId);

	if (it != _connectMap.end()) {
		delete (*it).second;
		_connectMap.erase(it);
	} else {
		LOG_DEBUG("No Connect constructed for IMAccount " + imAccountId);
	}
}

void ConnectHandler::connectionIsUpEventHandler(NetworkObserver & sender) {
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&ConnectHandler::connectionIsUpEventHandlerThreadSafe, this));

	WengoPhone::getInstance().postEvent(event);
}

void ConnectHandler::connectionIsUpEventHandlerThreadSafe() {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (ConnectMap::const_iterator it = _connectMap.begin();
		it != _connectMap.end();
		++it) {

		IMAccount * imAccount = 
			_userProfile->getIMAccountManager().getIMAccount((*it).first);
		if (imAccount) {
			if (!imAccount->isConnected()) {
				(*it).second->connect();
			}
			OWSAFE_DELETE(imAccount);
		} else {
			LOG_ERROR("IMAccount " + (*it).first + " not found!");
		}
	}
}

void ConnectHandler::connectionIsDownEventHandler(NetworkObserver & sender) {
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&ConnectHandler::connectionIsDownEventHandlerThreadSafe, this));

	WengoPhone::getInstance().postEvent(event);
}

void ConnectHandler::connectionIsDownEventHandlerThreadSafe() {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (ConnectMap::const_iterator it = _connectMap.begin();
		it != _connectMap.end();
		++it) {

		IMAccount * imAccount = 
			_userProfile->getIMAccountManager().getIMAccount((*it).first);
		if (imAccount) {
			if (imAccount->isConnected()) {
				(*it).second->disconnect(true);
			}
			OWSAFE_DELETE(imAccount);
		} else {
			LOG_ERROR("IMAccount " + (*it).first + " not found!");
		}
	}
}

void ConnectHandler::userProfileIsLoggingOff() {
	// hack to avoid crashes when logging off while an IMAccount is connecting
	_freeze = true;
}

void ConnectHandler::unregisterAllEvents() {
	_userProfile->getIMAccountManager().imAccountRemovedEvent -=
		boost::bind(&ConnectHandler::imAccountRemovedEventHandler, this, _1, _2);

	NetworkObserver::getInstance().connectionIsDownEvent -=
		boost::bind(&ConnectHandler::connectionIsDownEventHandler, this, _1);

	NetworkObserver::getInstance().connectionIsUpEvent -=
		boost::bind(&ConnectHandler::connectionIsUpEventHandler, this, _1);

	for (ConnectMap::const_iterator it = _connectMap.begin();
		it != _connectMap.end();
		++it) {
		(*it).second->unregisterAllEvents();

		(*it).second->connectedEvent -=
			boost::bind(&ConnectHandler::connectedEventHandler, this, _1);
		(*it).second->disconnectedEvent -=
			boost::bind(&ConnectHandler::disconnectedEventHandler, this, _1, _2, _3);
		(*it).second->connectionProgressEvent -=
			boost::bind(&ConnectHandler::connectionProgressEventHandler, this, _1, _2, _3, _4);
	}
}
