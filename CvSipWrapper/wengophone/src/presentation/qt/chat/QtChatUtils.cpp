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

#include "QtChatUtils.h"

#include "emoticons/QtEmoticonsManager.h"

#include <QtGui/QTextDocument>

const QString QtChatUtils::replaceUrls(const QString & str, const QString & htmlstr) {
	int beginPos = 0;
	QString tmp = htmlstr;
	int endPos;
	int repCount = 0;

	QStringList urls;
	QStringList reps;
	while (true) {

		beginPos = str.indexOf(QRegExp("(http://|https://|ftp://|^www.)", Qt::CaseInsensitive), beginPos);
		if (beginPos == -1) {
			break;
		}

		for (endPos = beginPos; endPos < str.size(); endPos++) {
			if ((str[endPos] == ' ') || (str[endPos] == '\r') || (str[endPos] == '\n')) {
				break;
			}
		}
		QString url = str.mid(beginPos, endPos - beginPos);
		urls << url;
		QString r = QString("$$_$$*_WENGOSTRUTILS_|KB|%1").arg(repCount);
		reps << r;
		repCount++;
		tmp.replace(url, r);
		beginPos = endPos;
	}
	for (int i = 0; i < reps.size(); i++) {
		QString url = QString("<a href='" + urls[i]+ "'>" + urls[i] + "</a>");
		tmp.replace(reps[i], url);
	}
	return tmp;
}

const QString QtChatUtils::text2Emoticon(const QString & htmlstr, const QString & protocol) {
	QtEmoticonsManager * emoticonsManager = QtEmoticonsManager::getInstance();
	return emoticonsManager->text2Emoticon(htmlstr, protocol);
}

const QString QtChatUtils::emoticons2Text(const QString & htmlstr, const QString & protocol) {
	QtEmoticonsManager * emoticonsManager = QtEmoticonsManager::getInstance();
	return emoticonsManager->emoticons2Text(htmlstr, protocol);
}

const QString QtChatUtils::getProtocol(EnumIMProtocol::IMProtocol protocol) {
	switch (protocol) {
		case EnumIMProtocol::IMProtocolMSN:
			return "msn";
		case EnumIMProtocol::IMProtocolYahoo:
			return "yahoo";
		case EnumIMProtocol::IMProtocolWengo:
			return "wengo";
		case EnumIMProtocol::IMProtocolAll:
			return "all";
		case EnumIMProtocol::IMProtocolJabber:
		case EnumIMProtocol::IMProtocolAIM:
		case EnumIMProtocol::IMProtocolICQ:
		case EnumIMProtocol::IMProtocolUnknown:
		default:
			return "default";
	}
}

const QString QtChatUtils::encodeMessage(EnumIMProtocol::IMProtocol protocol, const QString & message) {
	return emoticons2Text(message, getProtocol(protocol));
}

const QString QtChatUtils::decodeMessage(EnumIMProtocol::IMProtocol protocol, const QString & message) {
	QTextDocument tmp;
	tmp.setHtml(message);
	QString toReturn = text2Emoticon(
		replaceUrls(tmp.toPlainText(), message),
		getProtocol(protocol)
	);
	return toReturn;
}
