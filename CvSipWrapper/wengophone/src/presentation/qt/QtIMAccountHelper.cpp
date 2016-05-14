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

#include "QtIMAccountHelper.h"

#include <util/Logger.h>

namespace QtIMAccountHelper {

void copyListToPtrVector(const IMAccountList& list, QtIMAccountPtrVector* vector) {
	IMAccountList::const_iterator
		it = list.begin(),
		end = list.end();

	for (; it!=end; ++it) {
		vector->push_back(& (*it) );
	}
}

static int protocolOrder(EnumIMProtocol::IMProtocol protocol) {
	switch (protocol) {
	case EnumIMProtocol::IMProtocolWengo:
		return 0;

	case EnumIMProtocol::IMProtocolSIPSIMPLE:
	case EnumIMProtocol::IMProtocolSIP:
		return 10;

	case EnumIMProtocol::IMProtocolYahoo:
		return 20;

	case EnumIMProtocol::IMProtocolMSN:
		return 30;

	case EnumIMProtocol::IMProtocolAIM:
		return 40;

	case EnumIMProtocol::IMProtocolICQ:
		return 50;

	case EnumIMProtocol::IMProtocolJabber:
		return 60;

	case EnumIMProtocol::IMProtocolUnknown:
	case EnumIMProtocol::IMProtocolAll:
		LOG_FATAL("Protocol should not be " + EnumIMProtocol::toString(protocol));
		return -1;
	}

	LOG_FATAL("Unhandled protocol: " + EnumIMProtocol::toString(protocol));
	return -1;
}

bool compareIMAccountPtrs(const IMAccount* account1, const IMAccount* account2) {
	int protocol1Order = protocolOrder(account1->getProtocol());
	int protocol2Order = protocolOrder(account2->getProtocol());
	
	if (protocol1Order != protocol2Order) {
		return protocol1Order < protocol2Order;
	}

	return account1->getLogin() < account2->getLogin();
}

} // namespace
