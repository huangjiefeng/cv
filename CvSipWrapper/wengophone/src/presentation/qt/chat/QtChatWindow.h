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

#ifndef OWQTCHATWINDOW_H
#define OWQTCHATWINDOW_H

#include <QtGui/QMainWindow>

#include <control/chat/CChatHandler.h>

#include <imwrapper/IMChat.h>

#include <util/Trackable.h>

#include <string>
#include <map>

class QtChatTabWidget;
class QtWengoPhone;
class QtChatWidget;
class IMContact;
class IMChatSession;
class ContactProfile;

class QAction;
namespace Ui { class ChatMainWindow; }

/**
 *
 * @ingroup presentation
 * @author Mathieu Stute.
 */
class QtChatWindow : public QMainWindow, public Trackable {
	Q_OBJECT
public:

	QtChatWindow(QWidget * parent, CChatHandler & cChatHandler, IMChatSession & imChatSession, QtWengoPhone & qtWengoPhone);

	~QtChatWindow();

	void imChatSessionCreatedHandler(IMChatSession * imChatSession);

	void closeEvent(QCloseEvent *event);

Q_SIGNALS:

	void messageReceivedSignal(IMChatSession * sender);

	void typingStateChangedSignal(const IMChatSession * sender, const IMContact * imContact, const IMChat::TypingState * state);

public Q_SLOTS:

	void activeTabChanged(int index);

	void messageReceivedSlot(IMChatSession * sender);

	void show();

	void typingStateChangedThreadSafe(const IMChatSession * sender, const IMContact * imContact, const IMChat::TypingState * state);

	void callActiveTabContact();

	void sendSmsToActiveTabContact();

	void sendFileToActiveTabContact();

	void createChatConference();

	void showActiveTabContactInfo();

	//TODO
	void blockActiveTabContact() {}

	void closeActiveTab();

	void closeAllTabs();

	void statusChangedSlot(QString contactId);

	void saveActiveTabChatHistory();

protected:
	/**
	 * Installed on QTextEdit widgets. Used to update clipboard actions.
	 */
	virtual bool eventFilter(QObject* object, QEvent* event);

private Q_SLOTS:
	void cut();

	void copy();

	void paste();

	void updateClipboardActions();

private:

	void addChatWidgetTab(IMChatSession * session, const IMContact & from);

	void showToaster(IMChatSession * imChatSession);

	QtChatWidget * getActiveTabWidget();

	const QString getActiveTabContactId();

	ContactProfile getContactProfileFromContactId(const QString & contactId);

	void setupMenuBarActions();

	void setupToolBarActions();

	void updateToolBarActions();

	void messageReceivedEventHandler(IMChatSession & sender);

	void typingStateChangedEventHandler(IMChatSession & sender, const IMContact & imContact, IMChat::TypingState state);

	void imChatSessionWillDieEventHandler(IMChatSession & sender);

	void flashWindow();

	void showMinimized();

	QString getShortDisplayName(const QString & contactId, const QString & defaultName) const;

	QtChatWidget * _chatWidget;

	QtChatTabWidget * _tabWidget;

	CChatHandler & _cChatHandler;

	IMChatSession * _imChatSession;

	QtWengoPhone & _qtWengoPhone;

	Ui::ChatMainWindow * _ui;

	/** Index of the last message printed on screen for ecah sessionID **/
	std::map<int, int> _lastReceivedMessageIndex;
};

#endif	//OWQTCHATWINDOW_H

