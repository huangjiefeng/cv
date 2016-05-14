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

#include "WengoWebService.h"

#include <model/account/wengo/WengoAccount.h>
#include <model/config/Config.h>
#include <model/config/ConfigManager.h>
#include <WengoPhoneBuildId.h>

#include <util/Logger.h>

WengoWebService::WengoWebService(WengoAccount * wengoAccount)
	: _wengoAccount(wengoAccount) {

	_https = false;
	_get = false;
	_auth = false;
	_port = 0;
}

WengoWebService::~WengoWebService() {

}

void WengoWebService::setHttps(bool https) {
	_https = https;
}

void WengoWebService::setGet(bool get) {
	_get = get;
}

void WengoWebService::setHostname(const std::string & hostname) {
	_hostname= hostname;
}

void WengoWebService::setPort(int port) {
	_port = port;
}

void WengoWebService::setServicePath(const std::string & servicePath) {
	_servicePath = servicePath;
}

void WengoWebService::setParameters(const std::string & parameters) {
	_parameters = parameters;
}

void WengoWebService::setWengoAuthentication(bool auth) {
	_auth = auth;
}

int WengoWebService::sendRequest() {
	HttpRequest * httpRequest = new HttpRequest();
	httpRequest->answerReceivedEvent += boost::bind(&WengoWebService::answerReceivedEventHandler, this, _1, _2, _3, _4);
	return httpRequest->sendRequest(_https, _hostname, _port, _servicePath, _parameters, _get);
}

int WengoWebService::call(WengoWebService * caller) {
	//Set caller
	_caller = caller;

	//Add wengo parameters
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	std::string language = config.getLanguage();

	std::string data = "lang=" + language;
	data += "&wl=" + std::string(WengoPhoneBuildId::getSoftphoneName());

	//Add authentication parameters
	if ((_auth) && (_wengoAccount)) {
		String login = String::encodeUrl(_wengoAccount->getWengoLogin());
		login.replace("%2e", ".", false);
		String password = String::encodeUrl(_wengoAccount->getWengoPassword());
		password.replace("%2e", ".", false);
		data += "&login=" + login + "&password=" + password;
	}

	if (!_parameters.empty()) {
		_parameters = data + "&" + _parameters;
	} else {
		_parameters = data;
	}

	return sendRequest();
}

void WengoWebService::answerReceivedEventHandler(IHttpRequest * sender, int requestId, const std::string & answer, HttpRequest::Error error) {
	if (_caller) {
		if (error == HttpRequest::NoError) {
			_caller->answerReceived(answer, requestId);
		} else {
			_caller->answerReceived(String::null, requestId);
		}
	}
}
