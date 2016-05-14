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

#include <imwrapper/IMAccountXMLSerializer.h>

#include <imwrapper/IMAccount.h>
#include <imwrapper/IMAccountParametersXMLSerializer.h>

#include <util/Base64.h>

#include <tinyxml.h>

using namespace std;

IMAccountXMLSerializer::IMAccountXMLSerializer(IMAccount & imAccount)
	: _imAccount(imAccount) {
}

void IMAccountXMLSerializer::setIMAccountParametersData(const std::string & data) {
	_imAccountParametersData = data;
}

std::string IMAccountXMLSerializer::serialize() {
	string result;
	EnumIMProtocol enumIMProtocol;

	result += "<account protocol=\"" + enumIMProtocol.toString(_imAccount._protocol) + "\">\n";
	result += "<uuid>" + _imAccount.getUUID() + "</uuid>\n";
	result += ("<login>" + _imAccount._login + "</login>\n");
	result += ("<password>" + Base64::encode(_imAccount._password) + "</password>\n");
	result += ("<presence>" + EnumPresenceState::toString(_imAccount._presenceState) + "</presence>\n");
	IMAccountParametersXMLSerializer serializer(_imAccount._imAccountParameters);
	result += serializer.serialize();
	result += "</account>\n";

	return result;
}

bool IMAccountXMLSerializer::unserialize(const std::string & data) {
	TiXmlDocument doc;
	EnumIMProtocol imProtocol;

	doc.Parse(data.c_str());

	TiXmlHandle docHandle(&doc);
	TiXmlHandle account = docHandle.FirstChild("account");

	// Retrieving protocol
	TiXmlElement * lastChildElt = account.Element();
	if (lastChildElt) {
		_imAccount._protocol = imProtocol.toIMProtocol(lastChildElt->Attribute("protocol"));
	} else {
		return false;
	}

	//Retrieving uuid
	TiXmlText * uuid= account.FirstChild("uuid").FirstChild().Text();
	if (uuid) {
		_imAccount.setUUID(uuid->Value());
	} else {
		return false;
	}

	//Retrieving login
	TiXmlText * login = account.FirstChild("login").FirstChild().Text();
	if (login) {
		_imAccount._login = login->Value();
	} else {
		return false;
	}

	//Retrieving password
	TiXmlText * password = account.FirstChild("password").FirstChild().Text();
	if (password) {
		_imAccount._password = Base64::decode(password->Value());
	}

	//Retireving Presence state
	TiXmlText * presence = account.FirstChild("presence").FirstChild().Text();
	if (presence) {
		_imAccount._presenceState = EnumPresenceState::fromString(presence->Value());
	}

	//Retrieving IMAccountParameters
	IMAccountParameters imAccountParameters;
	IMAccountParametersXMLSerializer serializer(imAccountParameters);
	serializer.unserializeSystemSettings(_imAccountParametersData);
	TiXmlNode * settingsNode = account.FirstChild("settings").Node();
	if (settingsNode) {
		string nodeData;
		nodeData << *settingsNode;
		serializer.unserializeUserSettings(nodeData);
	}
	_imAccount._imAccountParameters = imAccountParameters;

	return true;
}
