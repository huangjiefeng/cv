#include "stdafx.h"
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

#include "QtChatRoomInviteDlg.h"
#include "QtChatRoomTreeWidgetItem.h"
#include "QtChatRoomListWidgetItem.h"

#include "ui_ChatRoomInviteWidget.h"

#include <presentation/qt/contactlist/QtContactPixmap.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include <qtutil/WidgetFactory.h>
#include <qtutil/Object.h>

#include <model/contactlist/ContactList.h>
#include <model/contactlist/ContactProfile.h>
#include <model/contactlist/ContactGroup.h>

#include <QtGui/QHeaderView>

QtChatRoomInviteDlg::QtChatRoomInviteDlg(IMChatSession & chatSession, CContactList & cContactList, QWidget * parent , Qt::WFlags f ) :
QDialog(parent,f), _cContactList(cContactList), _chatSession(chatSession) {

	_chatRoomInviteWidget = new QWidget(this);
	_ui = new Ui::ChatRoomInviteWidget();
	_ui->setupUi(_chatRoomInviteWidget);

	QGridLayout * layout = new QGridLayout();
	layout->addWidget(_chatRoomInviteWidget);
	layout->setMargin(0);
	setLayout(layout);
	setupGui();
}

QtChatRoomInviteDlg::~QtChatRoomInviteDlg() {
	OWSAFE_DELETE(_ui);
	OWSAFE_DELETE(_chatRoomInviteWidget);
}

void QtChatRoomInviteDlg::setupGui() {
	_contactListTreeWidget = _ui->contactListTreeWidget;
	_inviteListWidget = _ui->inviteListWidget;
	_addPushButton = _ui->addPushButton;
	_removePushButton = _ui->removePushButton;
	_startPushButton = _ui->startPushButton;

	SAFE_CONNECT (_addPushButton, SIGNAL(clicked()), SLOT(addToConference()));
	SAFE_CONNECT (_removePushButton, SIGNAL(clicked()), SLOT(removeFromConference()));
	SAFE_CONNECT (_startPushButton, SIGNAL(clicked()), SLOT(startConference()));
	// Remove the column header
	_contactListTreeWidget->header()->hide();
	fillContact();
}

void QtChatRoomInviteDlg::startConference() {
	// _chatSession
	QList<QListWidgetItem *>selectList =  _inviteListWidget->findItems("*",Qt::MatchWildcard);
	QList<QListWidgetItem *>::iterator it;
	for (it = selectList.begin(); it!= selectList.end(); it++) {
		QtChatRoomListWidgetItem * item = dynamic_cast<QtChatRoomListWidgetItem *> (*it);
		_chatSession.addIMContact(*(item->getContact().getFirstAvailableIMContact(_chatSession)));
		_selectedContact.append((item->getContact()));
	}
	accept();
 }

void QtChatRoomInviteDlg::addToConference() {
	// Get list of selected item in the treeview
	QList<QTreeWidgetItem *> selectList =  _contactListTreeWidget->selectedItems ();
	QList<QTreeWidgetItem *>::iterator it;

	for (it = selectList.begin(); it!= selectList.end(); it++) {
		if ( (*it)->childCount() == 0 ) {
			QtChatRoomTreeWidgetItem * item = dynamic_cast<QtChatRoomTreeWidgetItem *>((*it));
			_contactListTreeWidget->setItemSelected(item,false);
			if (_inviteListWidget->findItems(item->text(0),Qt::MatchExactly).isEmpty()) {
				QtChatRoomListWidgetItem * listItem = new QtChatRoomListWidgetItem(item->getContact(), _inviteListWidget);
				listItem->setText(item->text(0));
				listItem->setIcon(item->icon(0));
			}
		}
	}
}

void QtChatRoomInviteDlg::removeFromConference() {
	QList<QListWidgetItem *>selectList = _inviteListWidget->selectedItems();
	QList<QListWidgetItem *>::iterator it;

	for (it= selectList.begin(); it!= selectList.end(); it++ ) {
		_inviteListWidget->takeItem(_inviteListWidget->row((*it)));
		delete (*it);
	}
}

void QtChatRoomInviteDlg::fillContact() {
	std::vector< std::pair<std::string, std::string> >groupList=_cContactList.getContactGroups();
	std::vector< std::pair<std::string, std::string> >::iterator it;

	for (it = groupList.begin(); it != groupList.end(); it++) {
		QTreeWidgetItem * groupItem = new QTreeWidgetItem( _contactListTreeWidget);
		groupItem->setFlags(Qt::ItemIsEnabled);
		QString groupName = QString::fromUtf8((*it).second.c_str());
		QString groupId = QString::fromStdString((*it).first);
		groupItem->setText(0,groupName);
		fillGroup(groupItem, groupId);
	}
}

void QtChatRoomInviteDlg::fillGroup(QTreeWidgetItem * group, const QString & groupId) {

	QStringList contactsIdList = getContactsInGroup(groupId);
	QStringList::iterator it;

	QtContactPixmap::ContactPixmap status;

	for (it=contactsIdList.begin();it!=contactsIdList.end();it++) {
		//(*it) is a contactId
		ContactProfile contactProfile = _cContactList.getContactProfile((*it).toStdString());
		if (contactProfile.getFirstAvailableIMContact(_chatSession) != NULL) {
			QtChatRoomTreeWidgetItem * item = new QtChatRoomTreeWidgetItem (contactProfile,group );
			item->setText(0, QString::fromStdString (contactProfile.getDisplayName()));
			switch ( contactProfile.getPresenceState()) {
				case EnumPresenceState::PresenceStateOnline:
					status = QtContactPixmap::ContactOnline;
					break;
				case EnumPresenceState::PresenceStateOffline:
					status = QtContactPixmap::ContactOffline;
					break;
				case EnumPresenceState::PresenceStateDoNotDisturb:
					status = QtContactPixmap::ContactDND;
					break;
				case EnumPresenceState::PresenceStateAway:
					status = QtContactPixmap::ContactAway;
					break;
				case EnumPresenceState::PresenceStateInvisible:
					status = QtContactPixmap::ContactInvisible;
					break;
				case EnumPresenceState::PresenceStateUserDefined:
					status = QtContactPixmap::ContactOnline;
					break;
				case EnumPresenceState::PresenceStateUnknown:
					status = QtContactPixmap::ContactUnknown;
					break;
				default:
					status = QtContactPixmap::ContactUnknown;
			}
		item->setIcon(0,QIcon(QtContactPixmap::getInstance()->getPixmap(status)));
		}
	}
}

QStringList QtChatRoomInviteDlg::getContactsInGroup(const QString & groupId) const {
	StringList contactIdList = _cContactList.getContactIds();
	StringList::iterator it;
	QStringList tmp;

	for (it=contactIdList.begin();it!=contactIdList.end();it++) {
		ContactProfile contactProfile = _cContactList.getContactProfile((*it));
		if (contactProfile.getGroupId() == groupId.toStdString()) {
			tmp << QString::fromStdString((*it));
		}
	}
	return tmp;
}
