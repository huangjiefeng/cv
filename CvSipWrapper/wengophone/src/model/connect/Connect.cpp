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

#include "Connect.h"

#include <imwrapper/IMAccount.h>
#include <imwrapper/IMWrapperFactory.h>

#include <model/network/NetworkObserver.h>
#include <model/profile/IMAccountManager.h>
#include <model/profile/UserProfile.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

const unsigned RECONNECT_INTERVAL = 200000;
const unsigned RECONNECT_RETRY = 10;

Connect::Connect(IMAccount account, UserProfile & userProfile)
	: _imAccount(account),
	_userProfile(userProfile) {

	_imConnect = IMWrapperFactory::getFactory().createIMConnect(account);

	_imConnect->connectedEvent +=
		boost::bind(&Connect::connectedEventHandler, this, _1);
	_imConnect->disconnectedEvent +=
		boost::bind(&Connect::disconnectedEventHandler, this, _1, _2, _3);
	_imConnect->connectionProgressEvent +=
		boost::bind(&Connect::connectionProgressEventHandler, this, _1, _2, _3, _4);

	_timerIsRunning = false;
	_timer.timeoutEvent +=
		boost::bind(&Connect::timeoutEventHandler, this, _1);

	_isConnecting = false;
	_isDisconnecting = false;
}

Connect::~Connect() {
	_timer.stop();
	delete _imConnect;
}

void Connect::connect() {
	_connectionRetryCount = 0;
	_isDisconnecting = false;

	autoConnect();
}

void Connect::disconnect(bool force) {
	_timer.stop();
	_timerIsRunning = false;
	_isConnecting = false;

	// Gets up-to-date IMAccount
	IMAccount * imAccount = _userProfile.getIMAccountManager().getIMAccount(_imAccount.getUUID());

	if (imAccount) {
		if (imAccount->isConnected()) {
			_isDisconnecting = true;
			if ((_imAccount.getProtocol() == EnumIMProtocol::IMProtocolSIPSIMPLE)
				|| (_imAccount.getProtocol() == EnumIMProtocol::IMProtocolSIP)
				|| (_imAccount.getProtocol() == EnumIMProtocol::IMProtocolWengo)) {
				_userProfile.disconnectSipAccounts(force);
			}
			_imConnect->disconnect(force);
		}
		OWSAFE_DELETE(imAccount);
	}
}

void Connect::timeoutEventHandler(Timer & sender) {
	Mutex::ScopedLock lock(_mutex);
	bool stopToggle = false;
	if (_isConnecting) {
		return;
	}

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(_imAccount.getUUID());
	if (imAccount) {
		// Check if the IMAccount is not already connected. This can happen
		// if a timeout event is emitted during a connection process.
		if (!imAccount->isConnected()) {
			if (_connectionRetryCount < RECONNECT_RETRY && NetworkObserver::getInstance().isConnected()) {
				++_connectionRetryCount;
				_isConnecting = true;
				if ((_imAccount.getProtocol() == EnumIMProtocol::IMProtocolSIPSIMPLE)
					|| (_imAccount.getProtocol() == EnumIMProtocol::IMProtocolSIP)
					|| (_imAccount.getProtocol() == EnumIMProtocol::IMProtocolWengo)) {
					if (!_userProfile.connectSipAccounts()) {
						LOG_ERROR("Couldn't connect sip accounts");
						stopToggle = true;
					}
				} else {
					_imConnect->connect();
				}
			} else {
				if (_connectionRetryCount >= RECONNECT_RETRY) {
					LOG_ERROR("Too many attempts to connect");
				} else {
					LOG_ERROR("Not connected");
				}
				stopToggle = true;
			}

			if (stopToggle) {
				// Either the user as cancelled the connection
				// nor the retry count has been exceeded
				_timer.stop();
				_timerIsRunning = false;
				_connectionRetryCount = 0;
			}
		}
		OWSAFE_DELETE(imAccount);
	}
}

void Connect::autoConnect() {
	if (!_timerIsRunning && (_connectionRetryCount < RECONNECT_RETRY)) {
		_timerIsRunning = true;
		_timer.start(0, RECONNECT_INTERVAL, 0);
	}
}

void Connect::connectedEventHandler(IMConnect & sender) {
	connectedEvent(sender);
	
	_timer.stop();
	_timerIsRunning = false;
	_isConnecting = false;
	_connectionRetryCount = 0;
}

void Connect::disconnectedEventHandler(IMConnect & sender, bool connectionError, std::string reason) {
	disconnectedEvent(sender, connectionError, reason);

	_isConnecting = false;

	if (_isDisconnecting) {
		return;
	}

	// We check if this is a user disconnection or a error disconnection.
	// If so, we try to reconnect
	if (connectionError) {
		autoConnect();
	} else {
		_timer.stop();
		_timerIsRunning = false;
		_connectionRetryCount = 0;
	}
}

void Connect::connectionProgressEventHandler(IMConnect & sender,
	int currentStep, int totalSteps, std::string infoMessage) {

	connectionProgressEvent(sender, currentStep, totalSteps, infoMessage);
}

void Connect::unregisterAllEvents() {
	_imConnect->connectedEvent -=
		boost::bind(&Connect::connectedEventHandler, this, _1);
	_imConnect->disconnectedEvent -=
		boost::bind(&Connect::disconnectedEventHandler, this, _1, _2, _3);
	_imConnect->connectionProgressEvent -=
		boost::bind(&Connect::connectionProgressEventHandler, this, _1, _2, _3, _4);

	_timer.timeoutEvent -=
		boost::bind(&Connect::timeoutEventHandler, this, _1);
}
