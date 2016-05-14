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

#ifndef OWQTCHATHISTORYWIDGET_H
#define OWQTCHATHISTORYWIDGET_H

#include <imwrapper/EnumIMProtocol.h>

#include <QtCore/QTime>
#include <QtCore/QUrl>
#include <QtGui/QTextBrowser>

class QtChatTheme;

/**
 * Implements the chat text zone with a custom context menu.
 *
 * @author Nicolas Couturier
 * @author Mathieu Stute
 * @author Xavier Desjardins
 */
class QtChatHistoryWidget : public QTextBrowser {
	Q_OBJECT
public:

	QtChatHistoryWidget(QWidget * parent);

	~QtChatHistoryWidget();

	void setTheme(const QString& themeDir);

	void insertMessage(const QString & contactId, const QString & senderName, const QString & str, const QTime & time);
	
	void insertStatusMessage(const QString & statusMessage, const QTime& time);
	
	void setProtocol(EnumIMProtocol::IMProtocol newProtocol) {
		_protocol = newProtocol;
	}

	void saveHistoryAsHtml();

	/**
	 * Overloaded to implement avatar loading
	 */
	virtual QVariant loadResource(int type, const QUrl& url);

	void setAvatarPixmap(const QString& name, const QPixmap& pixmap);

private Q_SLOTS:

	/**
	 * An URL has been clicked we must open a web browser
	 *
	 * @param link the clicked url
	 */
	void urlClicked(const QUrl & link);

	void scrollToBottom();
	
private:
	EnumIMProtocol::IMProtocol _protocol;

	QString _lastSenderName;

	QTime _lastMessageTime;

	QMap <QString, QPixmap> _avatarMap;

	QMap <QUrl, QPixmap> _scaledAvatarCache;

	QtChatTheme* _theme;
};

#endif	//OWQTCHATHISTORYWIDGET_H
