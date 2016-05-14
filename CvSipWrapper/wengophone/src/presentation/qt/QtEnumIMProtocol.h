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

#ifndef OWQTENUMIMPROTOCOL_H
#define OWQTENUMIMPROTOCOL_H

#include <util/NonCopyable.h>

#include <imwrapper/EnumIMProtocol.h>

#include <QtCore/QString>

/**
 * Instant Messaging protocols.
 *
 * This class duplicates EnumIMProtocol because
 * protocols at network level are not always the one that we want to show
 * to the user.
 *
 * For example at network level there is no GoogleTalk protocol but just Jabber protocol;
 * at graphical level there is a GoogleTalk protocol for the user because it's easier.
 *
 * @see EnumIMProtocol
 * @author Philippe Bernery
 * @author Tanguy Krotoff
 */
class QtEnumIMProtocol : NonCopyable {
public:

	enum IMProtocol {
		IMProtocolUnknown,
		IMProtocolMSN,
		IMProtocolYahoo,
		IMProtocolAIM,
		IMProtocolICQ,
		IMProtocolJabber,
		IMProtocolGoogleTalk,
		IMProtocolSIPSIMPLE,
		IMProtocolWengo
	};

	/**
	 * Converts a protocol into a string.
	 *
	 * @return the string
	 */
	static QString toString(IMProtocol protocol);

	/**
	 * Converts a string into a protocol.
	 *
	 * @return the protocol
	 */
	static IMProtocol toIMProtocol(const QString & protocol);

	//static QPixmap getPixmap(IMProtocol protocol);

	/**
	 * Converts an EnumIMProtocol into a QtEnumIMProtocol.
	 *
	 * @return the protocol
	 */
	static IMProtocol toIMProtocol(EnumIMProtocol::IMProtocol protocol);

	/**
	 * Converts a QtEnumIMProtocol to an EnumIMProtocol.
	 *
	 * @return the protocol
	 */
	static EnumIMProtocol::IMProtocol toModelIMProtocol(IMProtocol protocol);
};

#endif	//OWQTENUMIMPROTOCOL_H
