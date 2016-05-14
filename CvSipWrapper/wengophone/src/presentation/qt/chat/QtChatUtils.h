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

#ifndef OWQTCHATUTILS_H
#define OWQTCHATUTILS_H

#include <imwrapper/EnumIMProtocol.h>

class QColor;
class QFont;
class QString;

/**
 * Helper class that format message
 *
 * @ingroup presentation
 * @author Mr K.
 * @author Mathieu Stute
 */
class QtChatUtils {
public:
	/**
	 * Gets a string representing the given protocol.
	 *
	 * @param protocol the protocol.
	 * @return a string representing the protocol.
	 */
	static const QString getProtocol(EnumIMProtocol::IMProtocol protocol);

	/**
	 * Prepares a message to be sent to the network.
	 *
	 * @param font the font to use.
	 * @param protocol used protocol.
	 * @param message the message to prepare.
	 * @return the prepared message.
	 */
	static const QString encodeMessage(EnumIMProtocol::IMProtocol protocol, const QString & message);

	/**
	 * Decodes a message from the network.
	 *
	 * @param protocol used protocol.
	 * @param message the message to decode.
	 * @return the prepared message.
	 */
	static const QString decodeMessage(EnumIMProtocol::IMProtocol protocol, const QString & message);

private:

	/**
	 * Replaces urls by href html code.
	 *
	 * @param str the message in plain text.
	 * @param htmlstr the message in html.
	 * @return the changed message.
	 */
	static const QString replaceUrls(const QString & str, const QString & htmlstr);

	/**
	 * @see QtEmoticonsManager::text2Emoticon.
	 */
	static const QString text2Emoticon(const QString & htmlstr, const QString & protocol);

	/**
	 * @see QtEmoticonsManager::emoticon2Text.
	 */
	static const QString emoticons2Text(const QString & htmlstr, const QString & protocol);
};

#endif	//OWQTCHATUTILS_H
