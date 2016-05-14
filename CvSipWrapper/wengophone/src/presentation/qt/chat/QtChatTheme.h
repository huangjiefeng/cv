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
#ifndef QTCHATUTILS_H
#define QTCHATUTILS_H

#include <QtCore/QString>

/**
 * Embryonic implementation of theme support. Severely limited by Qt HTML
 * renderer for now. Hope WebKit will save us.
 */
class QtChatTheme {
public:
	enum Direction {
		Incoming, Outgoing
	};

	enum Position {
		First, Next
	};

	QString getFooter() const;

	QString getStatusMessage() const;

	QString getContent(Direction, Position) const;

	QString getStyleSheet() const;

	void load(const QString & themeDir);

	static void setKeywordValue(QString & html, const QString & keyword, const QString & value);

	static QStringList getThemeList();

private:
	QString _incomingContent;
	QString _incomingNextContent;
	QString _outgoingContent;
	QString _outgoingNextContent;
	QString _footer;
	QString _statusMessage;
	QString _styleSheet;
};

#endif // QTCHATUTILS_H
