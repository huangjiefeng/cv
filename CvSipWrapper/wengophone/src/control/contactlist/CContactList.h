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

#ifndef OWCCONTACTLIST_H
#define OWCCONTACTLIST_H

#include <control/Control.h>

#include <util/StringList.h>

#include <vector>
#include <string>

class Presentation;
class ContactList;
class Contact;
class ContactGroup;
class ContactProfile;
class CWengoPhone;
class PContactList;
class IMContact;

/**
 *
 * @ingroup control
 * @author Tanguy Krotoff
 */
class CContactList : public Control {
	friend class CUserProfile;
public:

	CContactList(ContactList & contactList, CWengoPhone & cWengoPhone);

	~CContactList();

	Presentation * getPresentation() const;

	CWengoPhone & getCWengoPhone() const;

	/**
	 * Gets a list of pair of <group UUID, group name>.
	 */
	std::vector< std::pair<std::string, std::string> > getContactGroups() const;

	/**
	 * Gets the name of a ContactGroup given its UUID.
	 *
	 * @param groupId the UUID of the desired group
	 * @return the group name, a null string if not found
	 */
	std::string getContactGroupName(const std::string & groupId) const;

	/**
	 * @see ContactList::getContactGroupIdFromName()
	 */
	std::string getContactGroupIdFromName(const std::string & groupName) const;

	/**
	 * Constructs a ContactProfile from a Contact contained in the model.
	 *
	 * If no Contact with the given contactId exists an empty ContactProfile
	 * is returned.
	 *
	 * Used by the GUI to get data from the model.
	 *
	 * @param contactId the Contact UUID od the desired Contact
	 * @return a ContactProfile based on the found Contact
	 */
	ContactProfile getContactProfile(const std::string & contactId) const;

	/**
	 * @see ContactList::findContactThatOwns()
	 */
	std::string findContactThatOwns(const IMContact & imContact);

	/**
	 * Gets all existing Contact Ids.
	 *
	 * @return a string list containing the UUID of the Contacts.
	 */
	StringList getContactIds() const;

	/**
	 * Adds a Contact to the ContactList.
	 *
	 * This method is used by GUI to add a Contact into the model.
	 *
	 * The Contact will be created from the given ContactProfile.
	 *
	 * @param contactProfile the ContactProfile that will be used to create the Contact
	 */
	void addContact(const ContactProfile & contactProfile);

	/**
	 * @see ContactList::removeContact()
	 */
	void removeContact(const std::string & contactId);

	/**
	 * Updates a Contact from a ContactProfile.
	 *
	 * @param contactProfile the ContactProfile which will full the Contact
	 */
	void updateContact(const ContactProfile & contactProfile);

	/**
	 * Adds a contact group.
	 *
	 * @param name name of the ContactGroup to add
	 */
	void addContactGroup(const std::string & name);

	/**
	 * @see ContactList::removeContactGroup()
	 */
	void removeContactGroup(const std::string & groupId);

	/**
	 * @see ContactList::renameContactGroup()
	 */
	void renameContactGroup(const std::string & groupId, const std::string & name);

	/**
	 * @see ContactList::mergeContacts()
	 */
	void merge(const std::string & dstContactId, const std::string & srcContactId);

	/**
	 * FIXME: this part should not exist. It is here until QtChatRoomInviteDlg
	 * will be refactored.
	 */
	ContactList & getContactList() { return _contactList; }

private:

	void initPresentationThreadSafe();

	/**
	 * Returns a pointer to the Contact associated with the given contactId
	 * or NULL if not found.
	 *
	 * Used by the Control to retrieve a Contact grom a contact UUID given by
	 * the GUI.
	 *
	 * @param contactId the UUID of the desired Contact
	 * @return the found Contact
	 */
	Contact * getContact(const std::string & contactId) const;

	/**
	 * @see ContactList::contactAddedEvent
	 */
	void contactAddedEventHandler(ContactList & sender, Contact & contact);

	void contactAddedEventHandlerThreadSafe(std::string contactId);

	/**
	 * @see ContactList::contactRemovedEvent
	 */
	void contactRemovedEventHandler(ContactList & sender, Contact & contact);

	void contactRemovedEventHandlerThreadSafe(std::string contactId);

	/**
	 * @see ContactList::contactGroupAddedEvent
	 */
	void contactGroupAddedEventHandler(ContactList & sender, ContactGroup & contactGroup);

	void contactGroupAddedEventHandlerThreadSafe(std::string groupId);

	/**
	 * @see ContactList::contactGroupRemovedEvent
	 */
	void contactGroupRemovedEventHandler(ContactList & sender, ContactGroup & contactGroup);

	void contactGroupRemovedEventHandlerThreadSafe(std::string groupId);

	/**
	 * @see ContactList::contactGroupRenamedEvent
	 */
	void contactGroupRenamedEventHandler(ContactList & sender, ContactGroup & contactGroup);

	void contactGroupRenamedEventHandlerThreadSafe(std::string groupId);

	/**
	 * @see ContactList::contactGroupMovedEvent
	 */
	void contactMovedEventHandler(ContactList & sender, ContactGroup & dstContactGroup,
		ContactGroup & srcContactGroup, Contact & contact);

	void contactMovedEventHandlerThreadSafe(std::string dstGroupId, std::string srcGroupId, std::string contactId);

	/**
	 * @see ContactList::contactChangedEvent
	 */
	void contactChangedEventHandler(ContactList & sender, Contact & contact);

	void contactChangedEventHandlerThreadSafe(std::string contactId);

	/**
	 * @see addContact
	 */
	void addContactThreadSafe(ContactProfile contactProfile);

	/**
	 * @see removeContact
	 */
	void removeContactThreadSafe(std::string contactId);

	/**
	 * @see updateContact
	 */
	void updateContactThreadSafe(ContactProfile contactProfile);

	/**
	 * @see addContactGroup
	 */
	void addContactGroupThreadSafe(std::string name);

	/**
	 * @see removeContactGroup
	 */
	void removeContactGroupThreadSafe(std::string groupId);

	/**
	 * @see renameContactGroup
	 */
	void renameContactGroupThreadSafe(std::string groupId, std::string name);

	/**
	 * @see merge
	 */
	void mergeThreadSafe(std::string dstContactId, std::string srcContactId);

	/** Direct link to the model. */
	ContactList & _contactList;

	/** Direct link to the presentation via an interface. */
	PContactList * _pContactList;

	CWengoPhone & _cWengoPhone;
};

#endif	//OWCCONTACTLIST_H
