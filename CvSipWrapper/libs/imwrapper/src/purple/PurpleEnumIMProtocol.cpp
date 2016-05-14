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

#include "PurpleEnumIMProtocol.h"

#include <util/Logger.h>

#include <cstring>

#define PRPL_ALL	"prpl-all"
#define PRPL_MSN	"prpl-msn"
#define PRPL_YAHOO	"prpl-yahoo"
#define PRPL_AIM	"prpl-aim"
#define PRPL_ICQ	"prpl-icq"
#define PRPL_JABBER	"prpl-jabber"
#define PRPL_SIMPLE	"prpl-simple"
#define PRPL_UNK	"prpl-unknown"

const char * PurpleIMPrcl::GetPrclId(EnumIMProtocol::IMProtocol protocol) {
	switch (protocol) {
	case EnumIMProtocol::IMProtocolAll:
		return PRPL_ALL;

	case EnumIMProtocol::IMProtocolMSN:
		return PRPL_MSN;

	case EnumIMProtocol::IMProtocolYahoo:
		return PRPL_YAHOO;

	case EnumIMProtocol::IMProtocolAIM:
		return PRPL_AIM;

	case EnumIMProtocol::IMProtocolICQ:
		return PRPL_ICQ;

	case EnumIMProtocol::IMProtocolJabber:
		return PRPL_JABBER;

	case EnumIMProtocol::IMProtocolSIPSIMPLE:
		return PRPL_SIMPLE;

	default:
		return PRPL_UNK;
	}
}

EnumIMProtocol::IMProtocol PurpleIMPrcl::GetEnumIMProtocol(const char * PurplePrclId) {
	if (!PurplePrclId || *PurplePrclId == '\0') {
		LOG_DEBUG("unknown purple protocol ID");
		return EnumIMProtocol::IMProtocolUnknown;
	}

	if (!strcmp(PurplePrclId, PRPL_ALL)) {
		return EnumIMProtocol::IMProtocolAll;
	}

	else if (!strcmp(PurplePrclId, PRPL_MSN)) {
		return EnumIMProtocol::IMProtocolMSN;
	}

	else if (!strcmp(PurplePrclId, PRPL_YAHOO)) {
		return EnumIMProtocol::IMProtocolYahoo;
	}

	else if (!strcmp(PurplePrclId, PRPL_AIM)) {
		return EnumIMProtocol::IMProtocolAIM;
	}

	else if (!strcmp(PurplePrclId, PRPL_ICQ)) {
		return EnumIMProtocol::IMProtocolICQ;
	}

	else if (!strcmp(PurplePrclId, PRPL_JABBER)) {
		return EnumIMProtocol::IMProtocolJabber;
	}

	else if (!strcmp(PurplePrclId, PRPL_SIMPLE)) {
		return EnumIMProtocol::IMProtocolSIPSIMPLE;
	}

	else {
		return EnumIMProtocol::IMProtocolUnknown;
	}
}
