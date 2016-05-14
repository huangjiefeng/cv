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

#include "ContactList.h"

#include "Contact.h"
#include "IMContactListHandler.h"

#include <model/presence/PresenceHandler.h>
#include <model/profile/UserProfile.h>

#include <thread/Thread.h>

#define LOGGER_COMPONENT "ContactList"
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <util/StringList.h>

using namespace std;

ContactList::ContactList(UserProfile & userProfile)
	: _userProfile(userProfile),
	_imContactListHandler(userProfile.getIMContactListHandler()) {

	_imContactListHandler.newIMContactAddedEvent +=
		boost::bind(&ContactList::newIMContactAddedEventHandler, this, _1, _2, _3);
	_imContactListHandler.imContactRemovedEvent +=
		boost::bind(&ContactList::imContactRemovedEventHandler, this, _1, _2, _3);
	_imContactListHandler.newContactGroupAddedEvent +=
		boost::bind(&ContactList::newContactGroupAddedEventHandler, this, _1, _2);
	_imContactListHandler.contactGroupRemovedEvent +=
		boost::bind(&ContactList::contactGroupRemovedEventHandler, this, _1, _2);
	_imContactListHandler.imContactMovedEvent +=
		boost::bind(&ContactList::imContactMovedEventHandler, this, _1, _2, _3);
	_userProfile.getPresenceHandler().presenceStateChangedEvent +=
		boost::bind(&ContactList::presenceStateChangedEventHandler, this, _1, _2, _3, _4);
	_userProfile.getPresenceHandler().contactIconChangedEvent +=
		boost::bind(&ContactList::contactIconChangedEventHandler, this, _1, _2, _3);
	_userProfile.getIMAccountManager().imAccountRemovedEvent +=
		boost::bind(&ContactList::imAccountRemovedEventHandler, this, _1, _2);
}

ContactList::~ContactList() {
}

void ContactList::addContactGroup(const string & groupName) {
	RecursiveMutex::ScopedLock lock(_mutex);

	_addContactGroup(groupName);
	_imContactListHandler.addGroup(groupName);
}

void ContactList::removeContactGroup(const string & groupId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(groupId);
	if (contactGroup) {
		string groupName = contactGroup->getName();
		_removeContactGroup(groupId);
		_imContactListHandler.removeGroup(groupName);
	}
}

void ContactList::renameContactGroup(const std::string & groupId, const std::string & name) {
	RecursiveMutex::ScopedLock lock(_mutex);

	if (!name.empty()) {
		ContactGroup * contactGroup = getContactGroup(groupId);

		if (contactGroup) {
			std::string oldName = contactGroup->getName();
			contactGroup->setName(name);
			_imContactListHandler.changeGroupName(oldName, name);
			contactGroupRenamedEvent(*this, *contactGroup);
		}
	}
}

Contact & ContactList::createContact() {
	RecursiveMutex::ScopedLock lock(_mutex);

	Contact contact(_userProfile);
	Contacts::const_iterator it;

	_contacts.push_back(contact);
	//Look for the added Contact
	for (it = _contacts.begin();
		it != _contacts.end(); ++it) {
		if (contact == (*it)) {
			((Contact &) *it).contactChangedEvent +=
				boost::bind(&ContactList::contactChangedEventHandler, this, _1);
			contactAddedEvent(*this, (Contact &) *it);
			break;
		}
	}

	return (Contact &) *it;
}

void ContactList::removeContact(Contact & contact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	//Remove the Contact from its group
	ContactGroup * contactGroup = getContactGroup(contact.getGroupId());

	if (contactGroup) {
		contactGroup->removeContact(contact);
	}
	////

	//Remove all IMContacts

	//We get a copy of the IMContactSet as it will modified while browsing it
	IMContactSet imContactSet = contact.getIMContactSet();

	for (IMContactSet::const_iterator it = imContactSet.begin();
		it != imContactSet.end(); ++it) {
		removeIMContact(contact, *contact.getIMContact(*it));
	}
	////

	//Remove the Contact from the ContactList
	for (Contacts::iterator it = _contacts.begin();
		it != _contacts.end(); ++it) {
		if (contact == (*it)) {
			contactRemovedEvent(*this, (Contact &)*it);
			_contacts.erase(it);
			break;
		}
	}
	/////
}

void ContactList::addIMContact(Contact & contact, const IMContact & imContact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(contact.getGroupId());

	if (contactGroup) {
		contact._addIMContact(imContact);
		_imContactListHandler.addIMContact(contactGroup->getName(), imContact);
	}
}

void ContactList::removeIMContact(Contact & contact, const IMContact & imContact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(contact.getGroupId());

	if (contactGroup) {
		_imContactListHandler.removeIMContact(contactGroup->getName(), imContact);
		contact._removeIMContact(imContact);
	}
}

void ContactList::newIMContactAddedEventHandler(IMContactListHandler & sender,
	const std::string & groupName, IMContact & newIMContact) {

	RecursiveMutex::ScopedLock lock(_mutex);

	if (!groupName.empty()) {
		LOG_DEBUG("adding a new IMContact in group " + groupName + ": " + newIMContact.getContactId());

		//Find the Contact that owns the IMContact. Creating a new one if needed
		Contact * contact = findContactThatOwns(newIMContact);
		if (!contact) {
			LOG_DEBUG("IMContact " + newIMContact.getContactId() + " not found. Adding a new Contact");
			contact = &(createContact());
		}

		if (!contact->hasIMContact(newIMContact)) {
			_addToContactGroup(groupName, *contact);
			contact->_addIMContact(newIMContact);

			LOG_DEBUG("IMContact added in group " + groupName + ": " + newIMContact.getContactId());
		} else {
			//This event can be received although the IMContact is already present
			//in the ContactList. We assume that this is a move event.
			_moveContactToGroup(groupName, *contact);
		}
	}
}

void ContactList::imContactRemovedEventHandler(IMContactListHandler & sender,
	const std::string & groupName, IMContact & imContact) {

	RecursiveMutex::ScopedLock lock(_mutex);

	LOG_DEBUG("IMContact removed from group " + groupName + ": "
		+ imContact.getContactId());

	//Find the Contact that owns the IMContact. Creating a new one if needed
	Contact * contact = findContactThatOwns(imContact);
	if (!contact) {
		contact = &(createContact());
	}

	contact->_removeIMContact(imContact);

	LOG_DEBUG("IMContact removed: " + imContact.getContactId());
}

void ContactList::newContactGroupAddedEventHandler(IMContactList & sender, const std::string & groupName) {
	RecursiveMutex::ScopedLock lock(_mutex);

	_addContactGroup(groupName);
}

void ContactList::contactGroupRemovedEventHandler(IMContactList & sender, const std::string & groupName) {
	RecursiveMutex::ScopedLock lock(_mutex);

	_removeContactGroup(groupName);
}

void ContactList::presenceStateChangedEventHandler(PresenceHandler & sender,
	EnumPresenceState::PresenceState state,
	const std::string & alias, const IMContact & imContact) {

	RecursiveMutex::ScopedLock lock(_mutex);

	//Find the Contact that owns the IMContact. Creating a new one if needed
	Contact * contact = findContactThatOwns(imContact);
	if (contact) {
		//The PresenceState must not be changed if the PresenceState is
		//UserDefined (used by PhApi to set the alias)
		if (!contact->getIMContact(imContact)) {
			LOG_FATAL("bad algorithm in findContactThatOwns or in getIMContact");
		}

		if (state != EnumPresenceState::PresenceStateUserDefined) {
			contact->getIMContact(imContact)->setPresenceState(state);
			IMAccount * imAccount = 
				_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
			if (imAccount->getProtocol() != EnumIMProtocol::IMProtocolWengo) {
				contact->getIMContact(imContact)->setAlias(alias);
			}
			OWSAFE_DELETE(imAccount);
		} else {
			contact->getIMContact(imContact)->setPresenceState(EnumPresenceState::PresenceStateOnline);
			contact->getIMContact(imContact)->setAlias(alias);
		}
	}
}

void ContactList::imContactMovedEventHandler(IMContactListHandler & sender,
	const std::string & groupName, IMContact & imContact) {

	RecursiveMutex::ScopedLock lock(_mutex);

	Contact * contact = findContactThatOwns(imContact);
	if (contact) {
		//This method can be called when a Contact has changed, not only
		//because the Contact has moved.
		ContactGroup * contactGroup = getContactGroup(contact->getGroupId());
		if (contactGroup && (contactGroup->getName() == groupName)) {
			contactChangedEvent(*this, *contact);
		} else {
			_moveContactToGroup(groupName, *contact);
		}
	}
}

Contact * ContactList::findContactThatOwns(const IMContact & imContact) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	Contact * result = NULL;

	for (Contacts::const_iterator it = _contacts.begin();
		it != _contacts.end();
		++it) {
		Contact & contact = (Contact &)*it;
		if (contact.checkAndSetIMContact(imContact)) {
			result = &contact;
			break;
		}
	}

	return result;
}

void ContactList::_addContactGroup(const std::string & groupName) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup contactGroup(groupName);
	pair<ContactGroupSet::const_iterator, bool> result = _contactGroupSet.insert(contactGroup);

	//If the ContactGroup was actually inserted (e.g did not exist)
	if (result.second) {
		contactGroupAddedEvent(*this, (ContactGroup &)*result.first);
		LOG_DEBUG("new ContactGroup added " + groupName);
	} else {
		LOG_DEBUG("this ContactGroup already exists: " + groupName);
	}
}

void ContactList::_removeContactGroup(const std::string & groupId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(groupId);

	if (contactGroup) {
		//Deleting every Contacts in the ContactGroup
		ContactGroup contactGroupCopy = *contactGroup;
		for (ContactGroup::ContactVector::const_iterator vectIt = contactGroupCopy._contactList.begin();
			vectIt != contactGroupCopy._contactList.end();
			++vectIt) {
			removeContact(*(*vectIt));
		}
		////

		//Deleting the group
		ContactGroupSet::iterator it = _contactGroupSet.find(contactGroupCopy);
		contactGroupRemovedEvent(*this, contactGroupCopy);
		_contactGroupSet.erase(it);
		////
		LOG_DEBUG("ContactGroup removed. UUID: " + groupId);
	}
}

void ContactList::_addToContactGroup(const std::string & groupName, Contact & contact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	if (!groupName.empty()) {
		addContactGroup(groupName);

		ContactGroupSet::const_iterator it = _contactGroupSet.find(ContactGroup(groupName));
		if (it != _contactGroupSet.end()) {
			LOG_DEBUG("adding a Contact to group " + groupName);
			((ContactGroup &)(*it)).addContact(contact);
			contact.setGroupId((*it).getUUID());
		} else {
			LOG_FATAL("the group " + groupName + " has not been added");
		}
	}
}

void ContactList::imAccountRemovedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (Contacts::const_iterator it = _contacts.begin();
		it != _contacts.end();
		++it) {
		for (IMContactSet::const_iterator imContactIt = ((Contact &) *it).getIMContactSet().begin();
			imContactIt != ((Contact &) *it).getIMContactSet().end();
			++imContactIt) {
			if (((*imContactIt).getIMAccountId()) == imAccountId) {
				((IMContact &)*imContactIt).setIMAccount(NULL);
			}
		}
	}
}

ContactGroup * ContactList::getContactGroup(const std::string & groupId) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * result = NULL;

	for (ContactGroupSet::const_iterator it = _contactGroupSet.begin();
		it != _contactGroupSet.end();
		++it) {
		if ((*it).getUUID() == groupId) {
			result = &(ContactGroup &)(*it);
			break;
		}
	}

	return result;
}

void ContactList::contactIconChangedEventHandler(PresenceHandler & sender, const IMContact & imContact, OWPicture icon) {
	RecursiveMutex::ScopedLock lock(_mutex);

	Contact * contact = findContactThatOwns(imContact);
	if (contact) {
		contact->getIMContact(imContact)->setIcon(icon);
	} else {
		LOG_DEBUG("IMContact not found: " + imContact.getContactId());
	}
}

void ContactList::mergeContacts(Contact & dst, Contact & src) {
	RecursiveMutex::ScopedLock lock(_mutex);

	dst.merge(src);

	//Remove the source Contact without removing it from linked IMContactLists
	//Remove the Contact from its group
	ContactGroup * contactGroup = getContactGroup(src.getGroupId());
	if (contactGroup) {
		contactGroup->removeContact(src);
	}
	////

	for (Contacts::iterator it = _contacts.begin();
		it != _contacts.end(); ++it) {
		if (src == (*it)) {
			contactRemovedEvent(*this, (Contact &) *it);
			_contacts.erase(it);
			break;
		}
	}
	////
}

void ContactList::_moveContactToGroup(const string & dst, Contact & contact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(contact.getGroupId());
	std::string oldGroupName;

	if (contactGroup) {
		oldGroupName = contactGroup->getName();
	}

	if (oldGroupName != dst) {
		addContactGroup(dst);

		ContactGroupSet::iterator oldIt = _contactGroupSet.find(ContactGroup(oldGroupName));
		ContactGroupSet::iterator newIt = _contactGroupSet.find(ContactGroup(dst));

		if ((newIt != _contactGroupSet.end())
			&& (oldIt != _contactGroupSet.end())) {
			((ContactGroup &)(*oldIt)).removeContact(contact);
			((ContactGroup &)(*newIt)).addContact(contact);

			contact.setGroupId((*newIt).getUUID());
			contactMovedEvent(*this, (ContactGroup &) *newIt, (ContactGroup &) *oldIt, contact);
		}
	}
}

void ContactList::moveContactToGroup(const string & dst, Contact & contact) {
	RecursiveMutex::ScopedLock lock(_mutex);

	ContactGroup * contactGroup = getContactGroup(contact.getGroupId());
	std::string oldGroupName;

	if (contactGroup) {
		oldGroupName = contactGroup->getName();
	}

	if (oldGroupName != dst) {
		addContactGroup(dst);

		ContactGroupSet::iterator oldIt = _contactGroupSet.find(ContactGroup(oldGroupName));
		ContactGroupSet::iterator newIt = _contactGroupSet.find(ContactGroup(dst));

		for (IMContactSet::const_iterator it = contact.getIMContactSet().begin();
			it != contact.getIMContactSet().end();
			++it) {
			_imContactListHandler.moveContactToGroup((*newIt).getName(), oldGroupName, (IMContact &)*it);
		}

		if ((newIt != _contactGroupSet.end())
			&& (oldIt != _contactGroupSet.end())) {
			((ContactGroup &)(*oldIt)).removeContact(contact);
			((ContactGroup &)(*newIt)).addContact(contact);

			contact.setGroupId((*newIt).getUUID());
			contactMovedEvent(*this, (ContactGroup &)*newIt, (ContactGroup &)*oldIt, contact);
		}
	}
}

Contact * ContactList::getContact(const std::string & contactId) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (Contacts::const_iterator it = _contacts.begin();
		it != _contacts.end();
		++it) {
		if ((*it).getUUID() == contactId) {
			return &(Contact &)(*it);
			break;
		}
	}

	return NULL;
}

void ContactList::contactChangedEventHandler(Contact & sender) {
	RecursiveMutex::ScopedLock lock(_mutex);

	contactChangedEvent(*this, sender);
}

void ContactList::lock() {
	_mutex.lock();
}

void ContactList::unlock() {
	_mutex.unlock();
}

std::string ContactList::getContactGroupIdFromName(const std::string & groupName) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	std::string result;

	for (ContactGroupSet::const_iterator it = _contactGroupSet.begin();
		it != _contactGroupSet.end(); it++) {
		LOG_DEBUG("groupName: " + groupName + ", current group name: " + (*it).getName());
		if ((*it).getName() == groupName) {
			result = (*it).getUUID();
			break;
		}
	}

	return result;
}
