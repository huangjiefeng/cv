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

#include "QtEmoticonsManager.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <QtGui/QtGui>
#include <QtXml/QtXml>

QtEmoticonsManager * QtEmoticonsManager::_instance = NULL;

QtEmoticonsManager::QtEmoticonsManager() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	loadFromFile(QString::fromStdString(config.getResourcesDir() + "pics/emoticons/icondef.xml"));
}

QtEmoticonsManager * QtEmoticonsManager::getInstance() {
	if (!_instance) {
		_instance = new QtEmoticonsManager();
	}
	return _instance;
}

int QtEmoticonsManager::getProtocolCount() {
	return _protocolList.size();
}

int QtEmoticonsManager::getEmoticonCount(const QString & protocol) {
	if (_protocolList.find(protocol) == _protocolList.end()) {
		return 0;
	}
	return _protocolList[protocol].size();
}

QtEmoticon QtEmoticonsManager::getEmoticon(const QString & text, const QString & protocol) {
	QtEmoticon emoticon;

	if (_protocolList.find(protocol) == _protocolList.end()) {
		return emoticon;
	}
	QtEmoticonList emoticonList = _protocolList[protocol];

	for (QtEmoticonList::iterator it = emoticonList.begin(); it != emoticonList.end(); it++) {
		QStringList stringList = (*it).getText();
		for (QStringList::iterator it2 = stringList.begin(); it2 != stringList.end(); it2++) {
			if ((*it2).toUpper() == text.toUpper()) {
				return (*it);
			}
		}
	}
	return emoticon;
}

QtEmoticonsManager::QtEmoticonList QtEmoticonsManager::getQtEmoticonList(const QString & protocol) {
	QtEmoticonList emoticonList;
	if (_protocolList.find(protocol) == _protocolList.end()) {
		return emoticonList;
	}
	emoticonList = _protocolList[protocol];
	return emoticonList;
}

void QtEmoticonsManager::loadFromFile(const QString & filename) {
	QFile file(filename);
	QString ErrorMsg;
	int ErrorLine = 0;
	int ErrorCol = 0;
	QDomDocument doc("wengoIcons");

	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	if (!doc.setContent(&file, &ErrorMsg, &ErrorLine, &ErrorCol)) {
		file.close();
		return;
	}
	file.close();

	QDomElement tmpElement;
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	QString tagName;

	while (!n.isNull()) {
		tmpElement = n.toElement();
		if (!tmpElement.isNull()) {
			tagName = tmpElement.tagName();
			if (tagName == "protocol") {
				readProtocol(n);
			}
		}
		n = n.nextSibling();
	}
}

void QtEmoticonsManager::readProtocol(const QDomNode & node) {
	QDomNode n = node.firstChild();
	QDomElement element;
	QString attributeName;
	while (!n.isNull()) {
		if (!n.toElement().isNull()) {
			if (n.toElement().tagName() == "icon") {
				element = node.toElement();
				if (!element.isNull()) {
					attributeName = element.attribute("name");
					readIcon(n, attributeName);
					readIcon(n, "all");
				}
			}
		}
		n = n.nextSibling();
	}
}

void QtEmoticonsManager::readIcon(const QDomNode & node, const QString & protocol) {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	QDomNode n1 = node.firstChild();
	QStringList textList;
	QtEmoticon emoticon;
	while (!n1.isNull()) {
		QDomElement e1 = n1.toElement();
		if (!e1.isNull()) {
			if (e1.tagName() == "text") {
				textList << e1.text();
			}
			if (e1.tagName() == "object") {
				QString emoticonPath = QString::fromStdString(config.getResourcesDir() + "pics/emoticons/") + e1.text();
				emoticon.setPath(emoticonPath);
				QPixmap emoticonPix = QPixmap(emoticonPath);
				emoticon.setPixmap(emoticonPix);
				emoticon.setButtonPixmap(emoticonPix);
			}
		}
		n1 = n1.nextSibling();
	}
	emoticon.setText(textList);
	_protocolList[protocol] << emoticon;
}

QString QtEmoticonsManager::text2Emoticon(const QString & text, const QString & protocol) {
	if (_protocolList.find(protocol) == _protocolList.end()) {
		return text;
	}

	QtEmoticonList emoticonList = _protocolList[protocol];
	QString result;

	// Match full tags (<bla>) or text
	QRegExp rx("([^><]+|<[^>]+>)");
	int pos = 0;
	while (true) {
		pos = rx.indexIn(text, pos);
		if (pos == -1) {
			break;
		}

		QString token = rx.cap(0);
		if (token[0] != '<') {
			// Not a tag, replace smileys in it
			for (QtEmoticonList::iterator it = emoticonList.begin(); it != emoticonList.end(); it++) {
				QStringList stringList = it->getText();
				for (QStringList::iterator it2 = stringList.begin(); it2 != stringList.end(); it2++) {
					token.replace(*it2, it->getHtml(), Qt::CaseInsensitive);
				}
			}
		}

		result += token;
		pos += rx.matchedLength();
	}

	return result;
}

QString QtEmoticonsManager::emoticons2Text(const QString & text, const QString & protocol) {
	QString ret = text;

	if (_protocolList.find(protocol) == _protocolList.end()) {
		return text;
	}
	QtEmoticonList emoticonList = _protocolList[protocol];

	for (QtEmoticonList::iterator it = emoticonList.begin(); it != emoticonList.end(); it++) {
		QStringList stringList = (*it).getText();
		for (QStringList::iterator it2 = stringList.begin(); it2 != stringList.end(); it2++) {
			ret.replace((*it).getHtml(), encode((*it2)), Qt::CaseInsensitive);
		}
	}
	return ret;
}

QString QtEmoticonsManager::encode(const QString & text) const {
	QByteArray ba;

	for (int i = 0; i < text.size(); i++) {
		switch(text[i].toAscii()) {
		case '<':
			ba += "&lt;";
			break;
		case '>':
			ba += "&gt;";
			break;
		case '&':
			ba += "&amp;";
			break;
		case '\'':
			ba += "&apos;";
			break;
		case '"':
			ba += "&apos;";
			break;
		default:
			ba += text[i];
		}
	}
	return QString(ba);
}
