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

#include "ContactGroup.h"

#include "Contact.h"
#include "IMContactListHandler.h"

#define LOGGER_COMPONENT "ContactList"
#include <util/Logger.h>
#include <util/Uuid.h>

ContactGroup::ContactGroup(const std::string & groupName)
	: _groupName(groupName) {
	_uuid = Uuid::generateString();
}

ContactGroup::ContactGroup(const ContactGroup & contactGroup)
	: _groupName(contactGroup._groupName),
	_contactList(contactGroup._contactList) {

	_uuid = contactGroup._uuid;
}

void ContactGroup::addContact(Contact & contact) {
	Mutex::ScopedLock lock(_mutex);

	_contactList.push_back(&contact);
	contactAddedEvent(*this, contact);
	LOG_DEBUG("Contact added to group " + _groupName);
}

void ContactGroup::removeContact(Contact & contact) {
	Mutex::ScopedLock lock(_mutex);

	for (ContactVector::iterator it = _contactList.begin();
		it != _contactList.end();
		++it) {
		if ((*(*it)) == contact) {
			contactRemovedEvent(*this, *(*it));
			_contactList.erase(it);
			break;
		}
	}
}

Contact * ContactGroup::operator[](unsigned int i) const {
	Mutex::ScopedLock lock(_mutex);

	if (i <= size()) {
		return _contactList[i];
	} else {
		return NULL;
	}
}

bool ContactGroup::operator==(const ContactGroup & contactGroup) const {
	return (_groupName == contactGroup._groupName);
}

bool ContactGroup::operator<(const ContactGroup & contactGroup) const {
	return (_groupName < contactGroup._groupName);
}

void ContactGroup::setName(const std::string & groupName) {
	_groupName = groupName;
	contactGroupModifiedEvent(*this);
}

std::list<std::string> ContactGroup::getMobilePhoneList() const {
	std::list<std::string> list;
	for (ContactVector::const_iterator it = _contactList.begin();
		it != _contactList.end(); ++it) {

		std::string getMobilePhone = (*it)->getMobilePhone();
		if( getMobilePhone != "" ) {
			list.push_front(getMobilePhone);
		}
	}
	return list;
}
