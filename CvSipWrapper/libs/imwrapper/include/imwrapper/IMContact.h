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

#ifndef OWIMCONTACT_H
#define OWIMCONTACT_H

#include <imwrapper/EnumPresenceState.h>
#include <imwrapper/EnumIMProtocol.h>

#include <util/Event.h>
#include <util/OWPicture.h>
#include <util/Trackable.h>

#include <string>

class IMAccount;

/**
 * An Instant Message Contact.
 *
 * Associates a contact identifier (bob@hotmail.com) with a IMAccount.
 * This is needed since several accounts using the same protocol can used in parallel.
 *
 * @author Philippe Bernery
 */
class IMContact : public Trackable {
	friend class IMContactXMLSerializer;
	friend class IMContactXMLSerializer1;
public:

	/**
	 * Emitted when this IMContact has changed (e.g. its presence state
	 * could have changed or its alias).
	 *
	 * @param sender this class
	 */
	Event< void (IMContact & sender) > imContactChangedEvent;

	/**
	 * Constructs a new IMContact.
	 *
	 * @param imAccount the imAccount that the IMContact is associated with.
	 * @param contactId id of the IMContact to create
	 * @param presenceHandler the PresenceHandler that will receive presence message
	 */
	IMContact(const IMAccount & imAccount, const std::string & contactId);

	/**
	 * Constructs a new IMContact.
	 *
	 * @param protocol the protocol of the IMContact
	 * @param contactId id of the IMContact to create
	 * @param presenceHandler the PresenceHandler that will receive presence message
	 */
	IMContact(EnumIMProtocol::IMProtocol protocol, const std::string & contactId);

	IMContact(const IMContact & imContact);

	IMContact & operator = (const IMContact & imContact);

	IMContact();

	~IMContact();

	/**
	 * Checks if imContact is equal to this IMContact.
	 *
	 * @param imContact the IMContact to test
	 */
	bool operator == (const IMContact & imContact) const;

	bool operator < (const IMContact & imContact) const;

	std::string getIMAccountId() const;

	void setIMAccount(const IMAccount * imAccount);

	/** Used by the ConfigImporter. */
	void setContactId(const std::string & contactId);
	
	std::string getContactId() const {
		return _contactId;
	}
	
	std::string getDisplayContactId() const;

	EnumIMProtocol::IMProtocol getProtocol() const {
		return _protocol;
	}

	EnumPresenceState::PresenceState getPresenceState() const {
		return _presenceState;
	}

	void setPresenceState(EnumPresenceState::PresenceState presenceState);

	const std::string & getAlias() const {
		return _alias;
	}

	void setAlias(const std::string & alias);

	void setIcon(const OWPicture & icon);

	const OWPicture & getIcon() const {
		return _icon;
	}

	void setBlocked(bool blocked) {
		_blocked = blocked;
	}

	bool isBlocked() const {
		return _blocked;
	}

	/**
	 * Used to get a cleaned contact id.
	 *
	 * When chatting with a Jabber contact, its id changed to
	 * our_contact_id/something wich made some strange behaviour
	 * in our model. So we strip everything after the '/'.
	 *
	 * @return the clean contact id
	 */
	std::string cleanContactId() const;

private:

	void copy(const IMContact & imContact);

	/**
	 * Pointer to the associated IMAccount.
	 * If NULL, no IMAccount is currently associated with this IMAccount
	 */
	const IMAccount * _imAccount;

	std::string _contactId;

	EnumIMProtocol::IMProtocol _protocol;

	std::string _alias;
	
	//std::string _displayContactId;

	OWPicture _icon;

	/** True if this IMContact is blocked. */
	bool _blocked;

	EnumPresenceState::PresenceState _presenceState;
};

#endif	//OWIMCONTACT_H
