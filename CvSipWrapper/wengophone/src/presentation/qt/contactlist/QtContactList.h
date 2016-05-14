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

#ifndef OWQTCONTACTLIST_H
#define OWQTCONTACTLIST_H

#include <presentation/PContactList.h>

#include <QtCore/QObject>
#include <QtCore/QString>

class CContactList;
class Contact;
class ContactPopupMenu;
class ContactGroupPopupMenu;
class CWengoPhone;
class QtContactManager;
class ContactProfile;  //Add by li.zhennig  2010-08-09
class CUserProfile;   //Add by li.zhennig
class UserProfile;   //Add by li.zhennig

class QTreeWidgetItem;
namespace Ui { class ContactList; }

/**
 * Qt Presentation component for ContactList.
 *
 * @author Tanguy Krotoff
 * @author Philippe Bernery
 */
class QtContactList : public QObject, public PContactList {
	Q_OBJECT
public:

	static const QString DEFAULT_GROUP_NAME;
	static const QString STATUS_UNKNOW_PIXMAP;
	static const QString STATUS_NOSTATUS_PIXMAP;
	static const QString STATUS_NOSTATUS_SIP_PIXMAP;
	static const QString STATUS_ONLINE_PIXMAP;
	static const QString STATUS_OFFLINE_PIXMAP;
	static const QString STATUS_DND_PIXMAP;
	static const QString STATUS_INVISIBLE_PIXMAP;
	static const QString STATUS_AWAY_PIXMAP;
	static const QString STATUS_WDEAL_PIXMAP;
	static const QString STATUS_TESTCALL_PIXMAP;
	static const QString STATUS_CONTACT_IM_PIXMAP;
	static const QString STATUS_CONTACT_CALL_PIXMAP;
	static const QString STATUS_CONTACT_VIDEO_PIXMAP;
	static const QString STATUS_GROUP_OPEN_PIXMAP;
	static const QString STATUS_GROUP_CLOSE_PIXMAP;

	QtContactList(CContactList & cContactList, CWengoPhone & cWenghoPhone);

	virtual ~QtContactList();

	QWidget * getWidget() const {
		return _contactListWidget;
	}

	QtContactManager * getContactManager() const {
		return _contactManager;
	}

	void updatePresentation();

	void contactGroupAddedEvent(const std::string & groupId);

	void contactGroupRemovedEvent(const std::string & groupId);

	void contactGroupRenamedEvent(const std::string & groupId);

	void contactAddedEvent(const std::string & contactId);

	void contactRemovedEvent(const std::string & contactId);

	void contactMovedEvent(const std::string & dstGroupId,
		const std::string & srcGroupId, const std::string & contactId);

	void contactChangedEvent(const std::string & contactId);

	CContactList & getCContactList() const;

	/***
	* Add by li.zhennig at 2010-08-04
	**/
	std::string getContacts();
	std::string getContactInfo(std::string contactId);
	void setSipAccount(std::string sipAccount,ContactProfile& contactProfile);
	std::string getSipAccount(std::string contactId);
	/****************************************/

Q_SIGNALS:

	void contactChangedEventSignal(QString contactId);

private Q_SLOTS:

	void groupRightClickedSlot(const QString & groupName);

	void mergeContactsSlot(QString dstContact, QString srcContact);

private:

	/**
	 * Initializes the Content of the QtContactList.
	 */
	void initContent();

	void addGroup(QString contactGroupId);

	QtContactManager * _contactManager;

	CContactList & _cContactList;

	Ui::ContactList * _ui;

	QWidget * _contactListWidget;

	ContactPopupMenu * _contactPopupMenu;

	ContactGroupPopupMenu * _contactGroupPopupMenu;

	/** True when model is doing some things on contacts. */
	bool _waitingForModel;

	CWengoPhone & _cWengoPhone;
};

#endif	//OWQTCONTACTLIST_H

