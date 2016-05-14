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

#ifndef OWCONTACTLIST_H
#define OWCONTACTLIST_H

#include "ContactGroup.h"

#include <imwrapper/EnumPresenceState.h>

#include <thread/RecursiveMutex.h>

#include <util/Event.h>
#include <util/OWPicture.h>
#include <util/Trackable.h>

#include <set>
#include <list>

class Contact;
class IMAccount;
class IMAccountManager;
class IMContact;
class IMContactList;
class IMContactListHandler;
class IMPresence;
class PresenceHandler;
class StringList;
class UserProfile;

/**
 * Contact list.
 *
 * List of ContactGroup.
 *
 * @ingroup model
 * @author Tanguy Krotoff
 * @author Philippe Bernery
 */
class ContactList : public Trackable {
	friend class Contact;
	friend class ContactListXMLSerializer;
	friend class ContactListXMLSerializer1;
	friend class ContactXMLSerializer;
	friend class ContactXMLSerializer1;
public:

	/** Set of ContactGroup. */
	typedef std::set<ContactGroup> ContactGroupSet;

	/** List of Contact. */
	typedef std::list<Contact> Contacts;

	ContactList(UserProfile & userProfile);

	~ContactList();

	/**
	 * A ContactGroup has been added.
	 *
	 * @param sender this class
	 * @param contactGroup ContactGroup added
	 */
	Event<void (ContactList & sender, ContactGroup & contactGroup)> contactGroupAddedEvent;

	/**
	 * A ContactGroup has been removed.
	 *
	 * @param sender this class
	 * @param contactGroup ContactGroup removed
	 */
	Event<void (ContactList & sender, ContactGroup & contactGroup)> contactGroupRemovedEvent;

	/**
	 * A ContactGroup has been renamed.
	 *
	 * @param sender this class
	 * @param contactGroup ContactGroup renamed
	 */
	Event<void (ContactList & sender, ContactGroup & contactGroup)> contactGroupRenamedEvent;

	/**
	 * A Contact has been added to a ContactGroup.
	 *
	 * @param sender this class
	 * @param contactGroup the ContactGroup where the Contact has been added to
	 * @param contact the added Contact
	 */
	Event<void (ContactList & sender, Contact & contact)> contactAddedEvent;

	/**
	 * A Contact has been removed from a ContactGroup.
	 *
	 * @param sender this class
	 * @param contactGroup the ContactGroup where the Contact has been removed from
	 * @param contact the removed Contact
	 */
	Event<void (ContactList & sender, Contact & contact)> contactRemovedEvent;

	/**
	 * @see contactProfilePresenceChangedEvent
	 * FIXME not finished yet, this is for notification/toaster
	 */
	Event<void (ContactList & sender, Contact & contact)> contactPresenceChangedEvent;

	/**
	 * A Contact has moved.
	 *
	 * @param sender this class
	 * @param groupName the destinaton group
	 * @param contact the removed Contact
	 */
	Event<void (ContactList & sender, ContactGroup & dstContactGroup,
		ContactGroup & srcContactGroup, Contact & contact)> contactMovedEvent;

	/**
	 * Emitted when a Contact has been changed.
	 *
	 * @param sender this class
	 * @param contact the changed Contact
	 */
	Event<void (ContactList & sender, Contact & contact)> contactChangedEvent;

	/**
	 * Adds a ContactGroup.
	 *
	 * If a ContactGroup with the given name exists, no ContactGroup is created
	 *
	 * @param name name of the ContactGroup to add.
	 */
	void addContactGroup(const std::string & name);

	/**
	 * Removes a ContactGroup.
	 *
	 * If no ContactGroup with the given name exists, nothing happens
	 *
	 * @param groupId the UUID of the ContactGroup to remove.
	 */
	void removeContactGroup(const std::string & groupId);

	/**
	 * Renames a ContactGroup.
	 *
	 * @param groupId the UUID of the group to rename
	 * @param name the desired name
	 */
	void renameContactGroup(const std::string & groupId, const std::string & name);

	/**
	 * Creates and adds a Contact to the ContactList.
	 *
	 * @return a reference to the new Contact
	 */
	Contact & createContact();

	/**
	 * Removes a Contact.
	 *
	 * @param the contact to remove
	 */
	void removeContact(Contact & contact);

	/**
	 * Merges two Contacts.
	 *
	 * Merging two Contact will result in a deletation of the source Contact
	 * after fulling the destination Contact with missing information that
	 * come from the source Contact.
	 *
	 * @param src the source Contact
	 * @param dst the destination Contact
	 */
	void mergeContacts(Contact & dst, Contact & src);

	/**
	 * Moves a Contact from to a group.
	 *
	 * @param contact the Contact to move
	 * @param dst the destination group
	 */
	void moveContactToGroup(const std::string & dst, Contact & contact);

	/**
	 * Gets a ContactGroup given its UUID.
	 *
	 * @param groupId the group UUID of the desired ContactGroup
	 * @return a pointer to the ContactGroup, NULL if not found
	 */
	ContactGroup * getContactGroup(const std::string & groupId) const;

	/**
	 * @return a copy of the set of ContactGroups.
	 */
	ContactGroupSet getContactGroupSet() const {
		return ContactGroupSet(_contactGroupSet);
	}

	/**
	 * Gets the list of Contacts.
	 *
	 * @return the list of Contacts.
	 */
	const Contacts & getContacts() const {
		return _contacts;
	}

	/**
	 * Finds the first Contact that owns an IMContact.
	 *
	 * @param imContact the IMContact to look for
	 * @return the Contact or NULL if not found
	 */
	Contact * findContactThatOwns(const IMContact & imContact) const;

	/**
	 * Gets the Contact of given UUID.
	 *
	 * @param contactId the contact UUID
	 * @return the Contact or NULL if no found
	 */
	Contact * getContact(const std::string & contactId) const;

	/**
	 * Gets the UUID of a ContactGroup from its name.
	 *
	 * @param groupName the name of the group to find
	 * @return the UUID of the group. An empty string if not found
	 */
	std::string getContactGroupIdFromName(const std::string & groupName) const;

	/**
	 * Locks the ContactList.
	 *
	 * A locked ContactList can't be modified.
	 * The ContactList must be unlocked by calling unlock.
	 *
	 * This method is used by the model and the control.
	 */
	void lock();

	/**
	 * Unlocks the ContactList.
	 *
	 * This method is used by the model and the control.
	 */
	void unlock();

private:

	/**
	 * @see IMAccountManager::imAccountRemovedEvent
	 */
	void imAccountRemovedEventHandler(IMAccountManager & sender, std::string imAccountId);

	/**
	 * @see IMContactListHandler::newIMContactAddedEvent
	 */
	void newIMContactAddedEventHandler(IMContactListHandler & sender,
		const std::string & groupName, IMContact & newIMContact);

	/**
	 * @see IMContactListHandler::imContactRemovedEvent
	 */
	void imContactRemovedEventHandler(IMContactListHandler & sender,
		const std::string & groupName, IMContact & imContact);

	/**
	 * @see IMContactListHandler::imContactMovedEvent
	 */
	void imContactMovedEventHandler(IMContactListHandler & sender,
		const std::string & groupName, IMContact & imContact);

	/**
	 * @see IMContactListHandler::newContactGroupAddedEvent
	 */
	void newContactGroupAddedEventHandler(IMContactList & sender,
		const std::string & groupName);

	/**
	 * @see IMContactListHandler::contactGroupRemovedEvent
	 */
	void contactGroupRemovedEventHandler(IMContactList & sender,
		const std::string & groupName);

	/**
	 * @see PresenceHandler::presenceStateChangedEventHandler
	 */
	void presenceStateChangedEventHandler(PresenceHandler & sender,
		EnumPresenceState::PresenceState state,
		const std::string & alias, const IMContact & imContact);

	/**
	 * @see PresenceHandler::contactIconChangedEvent
	 */
	void contactIconChangedEventHandler(PresenceHandler & sender,
		const IMContact & imContact, OWPicture icon);

	/**
	 * @see Contact::contactChangedEvent
	 */
	void contactChangedEventHandler(Contact & sender);

	/**
	 * Add an IMContact to a Contact.
	 *
	 * It sends an asynchronous command to IMContactListHandler.
	 *
	 * Must only be called by Contact
	 *
	 * @param contact the Contact that wants to add an IMContact
	 * @param imContact the IMContact to add
	 */
	void addIMContact(Contact & contact, const IMContact & imContact);

	/**
	 * Remove an IMContact from a Contact.
	 *
	 * It sends an asynchronous command to IMContactListHandler.
	 *
	 * Must only be called by Contact
	 *
	 * @param contact the Contact that wants to remove an IMContact
	 * @param imContact the IMContact to remove
	 */
	void removeIMContact(Contact & contact, const IMContact & imContact);

	/**
	 * Moves a Contact to a group without modifying IM lists.
	 */
	void _moveContactToGroup(const std::string & dst, Contact & contact);

	/**
	 * Actually add a ContactGroup.
	 *
	 * @param groupName the group name
	 */
	void _addContactGroup(const std::string & groupName);

	/**
	 * Actually remove a ContactGroup.
	 *
	 * @param groupId the group UUID
	 */
	void _removeContactGroup(const std::string & groupId);

	/**
	 * Actually add the Contact to a ContactGroup.
	 *
	 * @param contact the Contact
	 * @param groupName the ContactGroup
	 */
	void _addToContactGroup(const std::string & groupName, Contact & contact);

	/** Set of ContactGroup. */
	ContactGroupSet _contactGroupSet;

	/** Vector of Contact. */
	Contacts _contacts;

	UserProfile & _userProfile;

	IMContactListHandler & _imContactListHandler;

	mutable RecursiveMutex _mutex;
};

#endif	//OWCONTACTLIST_H
