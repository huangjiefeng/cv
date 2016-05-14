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

#ifndef OWQTCHATLOGVIEWER_H
#define OWQTCHATLOGVIEWER_H

#include <QtGui/QMainWindow>

class ContactProfile;

class QtWengoPhone;

class QAction;
class QString;
class QStringList;
namespace Ui { class ChatLogViewer; }

/**
 * Shows saved chat logs
 *
 * @ingroup presentation
 * @author Xavier Desjardins
 */
class QtChatLogViewer : public QMainWindow {
	Q_OBJECT
public:

	QtChatLogViewer(QWidget * parent, QtWengoPhone & qtWengoPhone, QString log);

	~QtChatLogViewer();
	
public Q_SLOTS:

	void contactChangedEventSlot(QString contactId); 

	void callContact();

	void sendSmsToContact();

	void sendFileToContact();

	void createChatConference();

	void showContactInfo();

	void restartChat();
	
	void saveChatHistory();

private:

	bool canDoFileTransfer(const ContactProfile & contactProfile);

	void setupMenuBarActions();

	void setupToolBarActions();

	void updateToolBarActions();

	/**
	 * Finds an QAction and copies its properties to another QAction.
	 * Code factorization.
	 *
	 * @param actionParent where to find the original QAction
	 * @param action QAction to modify
	 */
	static void copyQAction(QObject * actionParent, QAction * action);

	QtWengoPhone & _qtWengoPhone;
	
	QStringList _cuuidList;
	
	Ui::ChatLogViewer * _ui;
};

#endif	//OWQTCHATLOGVIEWER_H

