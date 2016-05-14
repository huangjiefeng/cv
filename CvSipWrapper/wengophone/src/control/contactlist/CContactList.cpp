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

#include "CContactList.h"

#include <model/WengoPhone.h>
#include <model/contactlist/ContactList.h>
#include <model/contactlist/Contact.h>
#include <model/contactlist/ContactGroup.h>

#include <control/CWengoPhone.h>

#include <presentation/PFactory.h>
#include <presentation/PContactList.h>

#include <thread/ThreadEvent.h>

#include <util/Logger.h>

CContactList::CContactList(ContactList & contactList, CWengoPhone & cWengoPhone)
	: _contactList(contactList),
	_cWengoPhone(cWengoPhone) {

	_pContactList = NULL;
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::initPresentationThreadSafe, this));
	PFactory::postEvent(event);
}

CContactList::~CContactList() {
	/*delete _pContactList;*/
	_pContactList = NULL;
}

void CContactList::initPresentationThreadSafe() {
	_pContactList = PFactory::getFactory().createPresentationContactList(*this);

	_contactList.contactGroupAddedEvent += boost::bind(&CContactList::contactGroupAddedEventHandler, this, _1, _2);
	_contactList.contactGroupRemovedEvent += boost::bind(&CContactList::contactGroupRemovedEventHandler, this, _1, _2);
	_contactList.contactAddedEvent += boost::bind(&CContactList::contactAddedEventHandler, this, _1, _2);
	_contactList.contactRemovedEvent += boost::bind(&CContactList::contactRemovedEventHandler, this, _1, _2);
	_contactList.contactMovedEvent += boost::bind(&CContactList::contactMovedEventHandler, this, _1, _2, _3, _4);
	_contactList.contactChangedEvent += boost::bind(&CContactList::contactChangedEventHandler, this, _1, _2);
}

Presentation * CContactList::getPresentation() const {
	return _pContactList;
}

CWengoPhone & CContactList::getCWengoPhone() const {
	return _cWengoPhone;
}

void CContactList::contactAddedEventHandler(ContactList & sender, Contact & contact) {
	//We do not emit the event if the Contact has no group because the GUI
	//does not support a Contact with no group
	if (!contact.getGroupId().empty()) {
		typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
		MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactAddedEventHandlerThreadSafe, this, _1), contact.getUUID());
		PFactory::postEvent(event);
	}
}

void CContactList::contactAddedEventHandlerThreadSafe(std::string contactId) {
	_pContactList->contactAddedEvent(contactId);
}

void CContactList::contactRemovedEventHandler(ContactList & sender, Contact & contact) {
	typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactRemovedEventHandlerThreadSafe, this, _1), contact.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactRemovedEventHandlerThreadSafe(std::string contactId) {
	_pContactList->contactRemovedEvent(contactId);
}

void CContactList::contactMovedEventHandler(ContactList & sender, ContactGroup & dstContactGroup,
	ContactGroup & srcContactGroup, Contact & contact) {

	typedef ThreadEvent3<void (std::string, std::string, std::string), std::string, std::string, std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactMovedEventHandlerThreadSafe, this, _1, _2, _3),
		dstContactGroup.getUUID(), srcContactGroup.getUUID(), contact.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactMovedEventHandlerThreadSafe(std::string dstGroupId, std::string srcGroupId, std::string contactId) {
	_pContactList->contactMovedEvent(dstGroupId, srcGroupId, contactId);
}

void CContactList::contactGroupAddedEventHandler(ContactList & sender, ContactGroup & contactGroup) {
	typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactGroupAddedEventHandlerThreadSafe, this, _1), contactGroup.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactGroupAddedEventHandlerThreadSafe(std::string groupId) {
	_pContactList->contactGroupAddedEvent(groupId);
}

void CContactList::contactGroupRemovedEventHandler(ContactList & sender, ContactGroup & contactGroup) {
	typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactGroupRemovedEventHandlerThreadSafe, this, _1), contactGroup.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactGroupRemovedEventHandlerThreadSafe(std::string groupId) {
	_pContactList->contactGroupRemovedEvent(groupId);
}

void CContactList::contactGroupRenamedEventHandler(ContactList & sender, ContactGroup & contactGroup) {
	typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactGroupRenamedEventHandlerThreadSafe, this, _1), contactGroup.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactGroupRenamedEventHandlerThreadSafe(std::string groupId) {
	_pContactList->contactGroupRenamedEvent(groupId);
}

void CContactList::contactChangedEventHandler(ContactList & sender, Contact & contact) {
	typedef ThreadEvent1<void (std::string), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::contactChangedEventHandlerThreadSafe, this, _1), contact.getUUID());
	PFactory::postEvent(event);
}

void CContactList::contactChangedEventHandlerThreadSafe(std::string contactId) {
	if(_pContactList)
		_pContactList->contactChangedEvent(contactId);
}

std::vector< std::pair<std::string, std::string> > CContactList::getContactGroups() const {
	std::vector< std::pair<std::string, std::string> > result;

	_contactList.lock();
	ContactList::ContactGroupSet contactGroups = _contactList.getContactGroupSet();
	_contactList.unlock();

	for (ContactList::ContactGroupSet::const_iterator it = contactGroups.begin();
		it != contactGroups.end(); ++it) {
		result.push_back(std::pair<std::string, std::string>((*it).getUUID(), (*it).getName()));
	}

	return result;
}

std::string CContactList::getContactGroupName(const std::string & groupId) const {
	std::string result;

	_contactList.lock();

	ContactGroup * contactGroup = _contactList.getContactGroup(groupId);
	if (contactGroup) {
		result = contactGroup->getName();
	}

	_contactList.unlock();

	return result;
}

std::string CContactList::getContactGroupIdFromName(const std::string & groupName) const {
	return _contactList.getContactGroupIdFromName(groupName);
}

ContactProfile CContactList::getContactProfile(const std::string & contactId) const {
	ContactProfile result;
	Contact * contact = getContact(contactId);
	if (contact) {
		result = *contact;
	}

	return result;
}

Contact * CContactList::getContact(const std::string & contactId) const {
	return _contactList.getContact(contactId);
}

StringList CContactList::getContactIds() const {
	StringList result;

	_contactList.lock();

	const ContactList::Contacts & contacts = _contactList.getContacts();
	for (ContactList::Contacts::const_iterator it = contacts.begin();
		it != contacts.end(); ++it) {
		result += (*it).getUUID();
	}

	_contactList.unlock();

	return result;
}

void CContactList::addContact(const ContactProfile & contactProfile) {
	typedef ThreadEvent1<void (ContactProfile contactProfile), ContactProfile> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::addContactThreadSafe, this, _1), contactProfile);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::addContactThreadSafe(ContactProfile contactProfile) {
	Contact & contact = _contactList.createContact();
	contact = contactProfile;

	LOG_DEBUG("adding new contact=" + contact.getFirstName() + " in group=" + contact.getGroupId());
}

void CContactList::removeContact(const std::string & contactId) {
	typedef ThreadEvent1<void (std::string contactId), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::removeContactThreadSafe, this, _1), contactId);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::removeContactThreadSafe(std::string contactId) {
	Contact * contact = getContact(contactId);
	if (contact) {
		_contactList.removeContact(*contact);
	}
}

void CContactList::updateContact(const ContactProfile & contactProfile) {
	typedef ThreadEvent1<void (ContactProfile contactProfile), ContactProfile> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::updateContactThreadSafe, this, _1), contactProfile);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::updateContactThreadSafe(ContactProfile contactProfile) {
	Contact * contact = getContact(contactProfile.getUUID());
	if (contact) {
		(*contact) = contactProfile;
	}
}

void CContactList::addContactGroup(const std::string & name) {
	/*
	typedef ThreadEvent1<void (std::string contactId), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::addContactGroupThreadSafe, this, _1), name);
	_modelThread.postEvent(event);
	*/
	//FIXME: Here we do not change the thread because QtProfileDetails needs this method to be blocking
	//We should change the code in QtProfileDetails
	_contactList.addContactGroup(name);
}

void CContactList::addContactGroupThreadSafe(std::string name) {
	_contactList.addContactGroup(name);
}

void CContactList::removeContactGroup(const std::string & groupId) {
	typedef ThreadEvent1<void (std::string contactId), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::removeContactGroupThreadSafe, this, _1), groupId);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::removeContactGroupThreadSafe(std::string groupId) {
	_contactList.removeContactGroup(groupId);
}

void CContactList::renameContactGroup(const std::string & groupId, const std::string & name) {
	typedef ThreadEvent2<void (std::string contactId, std::string name), std::string, std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::renameContactGroupThreadSafe, this, _1, _2), groupId, name);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::renameContactGroupThreadSafe(std::string groupId, std::string name) {
	_contactList.renameContactGroup(groupId, name);
}

void CContactList::merge(const std::string & dstContactId, const std::string & srcContactId) {
	typedef ThreadEvent2<void (std::string dstContactId, std::string srcContactId), std::string, std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CContactList::mergeThreadSafe, this, _1, _2), dstContactId, srcContactId);
	WengoPhone::getInstance().postEvent(event);
}

void CContactList::mergeThreadSafe(std::string dstContactId, std::string srcContactId) {
	Contact * dstContact = getContact(dstContactId);
	Contact * srcContact = getContact(srcContactId);
	if (dstContact && srcContact) {
		_contactList.mergeContacts(*dstContact, *srcContact);
	}
}

std::string CContactList::findContactThatOwns(const IMContact & imContact) {
	std::string result;

	Contact * contact = _contactList.findContactThatOwns(imContact);
	if (contact) {
		result = contact->getUUID();
	}

	return result;
}

