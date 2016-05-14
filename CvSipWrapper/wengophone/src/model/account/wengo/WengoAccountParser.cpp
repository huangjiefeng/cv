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

#include "WengoAccountParser.h"

#include <util/StringList.h>
#include <util/String.h>
#include <util/Logger.h>

#include <tinyxml.h>

#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

static const std::string STATUS_CODE_OK = "200";

WengoAccountParser::WengoAccountParser(WengoAccount & account, const std::string & data) {

	TiXmlDocument doc;
	doc.Parse(data.c_str());

	TiXmlHandle docHandle(& doc);

	TiXmlHandle sso = docHandle.FirstChild("sso");

	//sso status code
	TiXmlElement * elem = sso.FirstChild("status").Element();
	if (elem) {
		std::string statusCode = elem->Attribute("code");
		LOG_DEBUG("SSO status code=" + statusCode);
		if (statusCode != STATUS_CODE_OK) {
			_loginPasswordOk = false;
			return;
		} else {
			_loginPasswordOk = true;
		}
	}

	//iterate over "d" element
	TiXmlElement * element = sso.FirstChild("d").Element();
	while (element) {

		std::string key = std::string(element->Attribute("k"));
		std::string value;
		const char * tmp = element->Attribute("v");
		if (tmp) {
			value = std::string(tmp);
		}

		if (key == "sip.auth.userid") {
			account._identity = value;
		} else if (key == "sip.auth.password") {
			account._password = value;
		} else if (key == "sip.auth.realm") {
			account._realm = value;
		} else if (key == "sip.address.name") {
			account._username = value;
		} else if (key == "sip.address.displayname") {
			account._displayName = value;
		} else if (key == "sip.address.server.host") {
			account._registerServerHostname = value;
		} else if (key == "sip.address.server.port") {
			account._registerServerPort = String(value).toInteger();
		} else if (key == "sip.outbound") {
			//TODO: pass this parameter to phapi by phconfig
		} else if (key == "sip.outbound.proxy.host") {
			account._sipProxyServerHostname = value;
		} else if (key == "sip.outbound.proxy.port") {
			account._sipProxyServerPort = String(value).toInteger();
		} else if (key == "netlib.stun.host") {
			account._stunServer = value;
		}

		else if (key == "netlib.tunnel.http") {
			StringList httpTunnels;
			TiXmlElement * elt = element->FirstChildElement("l");
			while (elt) {

				const char * tmp = elt->Attribute("v");
				if (tmp) {
					httpTunnels += std::string(tmp);
				}

				elt = elt->NextSiblingElement("l");
			}
			account._httpTunnelServerHostname = chooseHttpTunnel(httpTunnels);

		} else if (key == "netlib.tunnel.https") {
			StringList httpsTunnels;
			TiXmlElement * elt = element->FirstChildElement("l");
			while (elt) {

				const char * tmp = elt->Attribute("v");
				if (tmp) {
					httpsTunnels += std::string(tmp);
				}

				elt = elt->NextSiblingElement("l");
			}
			account._httpsTunnelServerHostname = chooseHttpTunnel(httpsTunnels);
		}

		element = element->NextSiblingElement("d");
	}
}

std::string WengoAccountParser::chooseHttpTunnel(const StringList & httpTunnels) {
	srand(time(NULL));
	return httpTunnels[rand() % httpTunnels.size()];
}
