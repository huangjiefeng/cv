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

#ifndef OWENUMIMPROTOCOL_H
#define OWENUMIMPROTOCOL_H

#include <util/NonCopyable.h>

#include <string>

/**
 * Instant Messaging protocols.
 *
 * @author Philippe Bernery
 */
class EnumIMProtocol : NonCopyable {
public:

	enum IMProtocol {
		IMProtocolUnknown,
		IMProtocolAll,
		IMProtocolMSN,
		IMProtocolYahoo,
		IMProtocolAIM,
		IMProtocolICQ,
		IMProtocolJabber,
		IMProtocolSIPSIMPLE, // Kept for compatibility with previous version of WengoPhone. @see ConfigImporter
		IMProtocolSIP,
		IMProtocolWengo
	};

	/**
	 * Converts a protocol into a string.
	 *
	 * @return the string
	 */
	static std::string toString(IMProtocol protocol);

	/**
	 * Converts a string into a protocol.
	 *
	 * @return the protocol
	 */
	static IMProtocol toIMProtocol(const std::string & protocol);
};

#endif	//OWENUMIMPROTOCOL_H
