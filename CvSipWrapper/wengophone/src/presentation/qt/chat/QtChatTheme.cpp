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
#include "QtChatTheme.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

#include <util/Logger.h>

static const char KEYWORD_CHAR = '%';

QString QtChatTheme::getFooter() const {
	return _footer;
}

QString QtChatTheme::getStatusMessage() const {
	return _statusMessage;
}

QString QtChatTheme::getContent(Direction direction, Position position) const {
	QString html;
	if (direction == Incoming && position == First) {
		html = _incomingContent;
	} else if (direction == Outgoing && position == First) {
		html = _outgoingContent;
	} else if (direction == Incoming && position == Next) {
		html = _incomingNextContent;
	} else if (direction == Outgoing && position == Next) {
		html = _outgoingNextContent;
	} else {
		LOG_FATAL("Unknown combination of direction and position");
	}

	return html;
}

QString QtChatTheme::getStyleSheet() const {
	return _styleSheet;
}

static QString loadFile(const QString& path) {
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QString data = QString::fromUtf8(file.readAll());
	return data.trimmed();
}

static QString themeBaseDir() {
	static QString baseDir;
	if (baseDir.isEmpty()) {
		Config & config = ConfigManager::getInstance().getCurrentConfig();
		baseDir = QString::fromUtf8(config.getResourcesDir().c_str())
			+ "/chat/";
	}
	return baseDir;
}

void QtChatTheme::load(const QString & themeDir) {
	QString themePath = themeBaseDir() + themeDir + "/";

	_incomingContent = loadFile(themePath + "incoming/content.html");
	_incomingNextContent = loadFile(themePath + "incoming/nextcontent.html");
	_outgoingContent = loadFile(themePath + "outgoing/content.html");
	_outgoingNextContent = loadFile(themePath + "outgoing/nextcontent.html");
	_footer = loadFile(themePath + "footer.html");
	_statusMessage = loadFile(themePath + "status.html");
	_styleSheet = loadFile(themePath + "main.css");
}

void QtChatTheme::setKeywordValue(QString & html, const QString & keyword, const QString & value) {
	html.replace(KEYWORD_CHAR + keyword + KEYWORD_CHAR, value);
}

QStringList QtChatTheme::getThemeList() {
	QDir dir(themeBaseDir());

	QStringList list;
	Q_FOREACH(QFileInfo info, dir.entryInfoList()) {
		if (info.isDir()) {
			QString name = info.fileName();
			if (name[0] != '.') {
				list << name;
			}
		}
	}

	return list;
}
