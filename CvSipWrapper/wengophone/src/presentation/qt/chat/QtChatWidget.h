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

#ifndef OWQTCHATWIDGET_H
#define OWQTCHATWIDGET_H

#include "ui_ChatWidget.h"

#include "emoticons/QtEmoticon.h"

#include <control/chat/CChatHandler.h>

#include <imwrapper/IMContact.h>
#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMChat.h>

#include <util/Trackable.h>

#include <QtCore/QTime>

class Profile;
class QtWengoPhone;
class QtWengoStyleLabel;
class EmoticonsWidget;
class QFont;
class QUrl;
class QTimer;

/**
 *
 * @ingroup presentation
 * @author Mr K.
 * @author Mathieu Stute
 */
class QtChatWidget : public QWidget, public Trackable {
	Q_OBJECT
public:

	QtChatWidget(CChatHandler & cChatHandler, QtWengoPhone * qtWengoPhone,
		int sessionId, QWidget * parent);

	virtual ~QtChatWidget();

	void setNickName(const QString & nickname) {_nickName = nickname;}

	const QString & nickName() const {return _nickName;}

	bool canDoMultiChat() {return _imChatSession->canDoMultiChat();}

	void setContactId(QString contactId) {_contactId = contactId;}

	int getSessionId() const {return _sessionId;}

	IMChatSession * getIMChatSession() {return _imChatSession;}

	QString getContactId() const {return _contactId;}

	// called from the model's thread
	void contactAddedEventHandler(IMChatSession & sender, const IMContact & imContact);

	// called from the model's thread
	void contactRemovedEventHandler(IMChatSession & sender, const IMContact & imContact);

	void setIMChatSession(IMChatSession * imChatSession);

	/**
	 * Adds a message to the history and display it.
	 *
     * @param contactId id of the contact who sends the message, or "self" if the sender is the local user
	 * @param senderName name of the contact who sends the message
	 * @param str the message
	 * @param time the time of the message, will use the current time if empty
	 */
	void addToHistory(const QString & contactId, const QString & senderName, const QString & str, const QTime & time = QTime());

	/**
	 * Displays a satus message.
	 *
	 * Behaves same as addToHistory but set the text color to the status message
	 * color and does not display a contact name.
	 */
	void addStatusMessage(const QString & statusMessage);

	void saveHistoryAsHtml();

	/**
	 * Sets the Contact state.
	 *
	 * If not connected and the last status was connected,
	 * the ChatEditWidget will be disabled and a status message
	 * will be displayed saying the contact went offline.
	 *
	 * If connect and the last status was disconnected,
	 * the ChatEditWidget will be enabled and a status message
	 * will be displayed saying the contact when online.
	 **/
	void setContactConnected(bool connected);

	bool canDoFileTransfer();

public Q_SLOTS:

	/**
	 * QtChatEditWidget text has changed
	 */
	void chatEditTextChanged();

	void changeFont();

	void chooseEmoticon();

	void emoticonSelected(QtEmoticon emoticon);

	void showInviteDialog();

	void contactAddedEventSlot(const IMContact & imContact);

	void contactRemovedEventSlot(const IMContact & imContact);

	virtual void setVisible(bool visible);

	void sendFileToSession(const QString & filename);

Q_SIGNALS:

	void newMessage(IMChatSession* session,const QString & msg);

	void contactAddedEventSignal(const IMContact & imContact);

	void contactRemovedEventSignal(const IMContact & imContact);

	void ctrlTabPressed();

	void profileChangedEventHandlerSignal();

protected:
	virtual bool eventFilter(QObject* object, QEvent* event);

private Q_SLOTS:

	void avatarFrameButtonClicked();

	void changeFontColor();

	void boldClickedSlot();

	void italicClickedSlot();

	void underlineClickedSlot();

	void contactChangedSlot(QString contactId);

	void sendMessage();
	
	void stoppedTypingSlot();

	void updateUserAvatar();

private:
	void profileChangedEventHandler();
	
	void updateAvatarFrame();

	void showAvatarFrame();

	void hideAvatarFrame();

	bool historyKeyPressEventFilter(QKeyEvent* event);

	bool editKeyPressEventFilter(QKeyEvent* event);

	CChatHandler & _cChatHandler;

	QtWengoPhone * _qtWengoPhone;

	IMChatSession * _imChatSession;

	QWidget * _widget;

	QString _contactId;

	int _sessionId;

	QString _nickName;

	mutable QMutex _mutex;

	EmoticonsWidget * _emoticonsWidget;

	Ui::ChatWidget _ui;

	/** True if the Contact in this Widget is connected. */
	bool _isContactConnected;

	// font style settings
	QFont _currentFont;
	QColor _currentColor;
	bool _bold;
	bool _italic;
	bool _underline;
	////
	
	/** stopped typing timer */
	QTimer * _stoppedTypingTimer;
	
	bool _isTyping;
};

#endif //OWQTCHATWIDGET_H
