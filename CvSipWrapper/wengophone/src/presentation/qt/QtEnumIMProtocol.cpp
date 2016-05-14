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

#include "QtEnumIMProtocol.h"

#include <util/String.h>
#include <util/Logger.h>

#include <map>

typedef std::map<QtEnumIMProtocol::IMProtocol, QString> ProtocolMap;
static ProtocolMap _protocolMap;

static void init() {
	if (!_protocolMap.empty()) {
		return;
	}
	_protocolMap[QtEnumIMProtocol::IMProtocolMSN] = "MSN";
	_protocolMap[QtEnumIMProtocol::IMProtocolYahoo] = "Yahoo";
	_protocolMap[QtEnumIMProtocol::IMProtocolAIM] = "AIM";
	_protocolMap[QtEnumIMProtocol::IMProtocolICQ] = "ICQ";
	_protocolMap[QtEnumIMProtocol::IMProtocolJabber] = "Jabber";
	_protocolMap[QtEnumIMProtocol::IMProtocolGoogleTalk] = "GoogleTalk";
	_protocolMap[QtEnumIMProtocol::IMProtocolSIPSIMPLE] = "SIP/SIMPLE";
	_protocolMap[QtEnumIMProtocol::IMProtocolWengo] = "Wengo";
}

QString QtEnumIMProtocol::toString(IMProtocol protocol) {
	init();
	QString tmp = _protocolMap[protocol];
	if (tmp.isEmpty()) {
		LOG_FATAL("unknown IMProtocol=" + String::fromNumber(protocol));
	}
	return tmp;
}

QtEnumIMProtocol::IMProtocol QtEnumIMProtocol::toIMProtocol(const QString & protocol) {
	init();
	for (ProtocolMap::const_iterator it = _protocolMap.begin();
		it != _protocolMap.end(); ++it) {

		if ((*it).second == protocol) {
			return (*it).first;
		}
	}

	LOG_FATAL("unknown protocol=" + protocol.toStdString());
	return IMProtocolUnknown;
}

QtEnumIMProtocol::IMProtocol QtEnumIMProtocol::toIMProtocol(EnumIMProtocol::IMProtocol protocol) {
	IMProtocol imProtocol = IMProtocolUnknown;

	switch (protocol) {
	case EnumIMProtocol::IMProtocolMSN:
		imProtocol = IMProtocolMSN;
		break;

	case EnumIMProtocol::IMProtocolYahoo:
		imProtocol = IMProtocolYahoo;
		break;

	case EnumIMProtocol::IMProtocolAIM:
		imProtocol = IMProtocolAIM;
		break;

	case EnumIMProtocol::IMProtocolICQ:
		imProtocol = IMProtocolICQ;
		break;

	case EnumIMProtocol::IMProtocolJabber:
		imProtocol = IMProtocolJabber;
		break;

	case EnumIMProtocol::IMProtocolSIPSIMPLE:
		imProtocol = IMProtocolSIPSIMPLE;
		break;

	case EnumIMProtocol::IMProtocolWengo:
		imProtocol = IMProtocolWengo;
		break;

	default:
		LOG_FATAL("unknown IM protocol=" + String::fromNumber(protocol));
	}

	return imProtocol;
}

EnumIMProtocol::IMProtocol QtEnumIMProtocol::toModelIMProtocol(IMProtocol protocol) {
	switch (protocol) {
	case IMProtocolUnknown:
		return EnumIMProtocol::IMProtocolUnknown;

	case IMProtocolMSN:
		return EnumIMProtocol::IMProtocolMSN;

	case IMProtocolYahoo:
		return EnumIMProtocol::IMProtocolYahoo;

	case IMProtocolAIM:
		return EnumIMProtocol::IMProtocolAIM;

	case IMProtocolICQ:
		return EnumIMProtocol::IMProtocolICQ;

	case IMProtocolJabber:
	case IMProtocolGoogleTalk:
		return EnumIMProtocol::IMProtocolJabber;
	
	case IMProtocolSIPSIMPLE:
		return EnumIMProtocol::IMProtocolSIPSIMPLE;

	case IMProtocolWengo:
		return EnumIMProtocol::IMProtocolWengo;
	}

	LOG_FATAL("unknown IM protocol=" + String::fromNumber(protocol));
	return EnumIMProtocol::IMProtocolUnknown;
}
