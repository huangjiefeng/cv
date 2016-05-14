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

#ifndef OWQTCHATROOMINVITEDLG_H
#define OWQTCHATROOMINVITEDLG_H

#include <QtGui/QDialog>

#include <control/contactlist/CContactList.h>
#include <model/contactlist/Contact.h>

#include <imwrapper/IMContact.h>
#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMChat.h>

namespace Ui { class ChatRoomInviteWidget; }

class QListWidget;
class QTreeWidget;
class QTreeWidgetItem;

class Contact;

/**
 *
 * @ingroup presentation
 * @author Mr K.
 */
class QtChatRoomInviteDlg : public QDialog
{
	Q_OBJECT
public:

	typedef QList<ContactProfile> SelectedContact;

	QtChatRoomInviteDlg(IMChatSession & chatSession,ContactList & contactList, QWidget * parent = 0, Qt::WFlags f = 0);

	QtChatRoomInviteDlg(IMChatSession & chatSession,CContactList & cContactList, QWidget * parent = 0, Qt::WFlags f = 0);

	~QtChatRoomInviteDlg();

	SelectedContact getSelectedContact() const;

protected:

	void setupGui();

	Ui::ChatRoomInviteWidget * _ui;

	QWidget * _chatRoomInviteWidget;

	QTreeWidget * _contactListTreeWidget;

	QListWidget * _inviteListWidget;

	QPushButton * _addPushButton;

	QPushButton * _removePushButton;

	QPushButton * _startPushButton;

//	ContactList & _contactList;
	CContactList & _cContactList;

	IMChatSession & _chatSession;

	void fillContact();

//	void fillGroup(QTreeWidgetItem * group, const ContactGroup * cgroup);
	void fillGroup(QTreeWidgetItem * group, const QString & groupId);

	QStringList getContactsInGroup(const QString & groupId) const;

	SelectedContact _selectedContact;

protected Q_SLOTS:

	void startConference();

	void addToConference();

	void removeFromConference();
};

#endif //OWQTCHATROOMINVITEDLG_H
