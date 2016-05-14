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

#ifndef CONTACTGROUP_H
#define CONTACTGROUP_H

#include <util/Event.h>
#include <thread/Mutex.h>

#include <string>
#include <list>

class Contact;
class ContactList;

/**
 * A group of contacts (i.e familly, friends ect...).
 *
 * Contains a list of Contact.
 *
 * @ingroup model
 * @author Tanguy Krotoff
 * @author Philippe Bernery
 */
class ContactGroup {
	friend class ContactList;
public:

	/**
	 * A ContactGroup has been modified/updated.
	 *
	 * @param sender ContactGroup modified/updated
	 */
	Event<void(ContactGroup & sender)> contactGroupModifiedEvent;

	/**
	 * A Contact has been added.
	 *
	 * @param sender this class
	 * @param contact Contact added
	 */
	Event<void(ContactGroup & sender, Contact & contact)> contactAddedEvent;

	/**
	 * A Contact has been removed.
	 *
	 * @param sender this class
	 * @param contact Contact removed
	 */
	Event<void(ContactGroup & sender, Contact & contact)> contactRemovedEvent;

	ContactGroup(const std::string & groupName);

	ContactGroup(const ContactGroup & contactGroup);

	std::string getName() const {
		return _groupName;
	}

	/**
	 * Permits to use ContactGroup as an array.
	 *
	 * @see List::operator[]
	 * @param i index inside the array
	 * @return the Contact that corresponds to the index i inside the ContactGroup or NULL
	 */
	Contact * operator[](unsigned i) const;

	/**
	 * @return the number of Contact in this ContactGroup
	 */
	unsigned size() const {
		return _contactList.size();
	}

	/**
	 * @return a list containing all mobile phone number
	 */
	std::list<std::string> getMobilePhoneList() const;

	/**
	 * Gets the UUID of this ContactGroup.
	 *
	 * @return the UUID of this ContactGroup
	 */
	std::string getUUID() const {
		return _uuid;
	}

	/**
	 * Compare two groups.
	 *
	 * @param contactGroup group to compare
	 * @return true if equal
	 */
	bool operator==(const ContactGroup & contactGroup) const;

	bool operator<(const ContactGroup & contactGroup) const;

	std::string serialize();

	bool unserialize(const std::string & data);

private:

	/**
	 * Sets the name of the group.
	 *
	 * @param groupName the desired name
	 */
	void setName(const std::string & groupName);

	/**
	 * Add a Contact to the ContactGroup.
	 *
	 * Must only be called by ContactList
	 *
	 * @param contact the Contact to add
	 */
	void addContact(Contact & contact);

	/**
	 * Remove a Contact from the ContactGroup.
	 *
	 * Must only be called by ContactList
	 *
	 * @param contact the Contact to remove
	 */
	void removeContact(Contact & contact);

	/** Defines the vector of Contact. */
	typedef std::vector<Contact *> ContactVector;

	/** Name of the ContactGroup. */
	std::string _groupName;

	/** List of Contact associated with the ContactGroup. */
	ContactVector _contactList;

	/** The UUID of this ContactGroup. */
	std::string _uuid;

	mutable Mutex _mutex;
};

#endif	//CONTACTGROUP_H
