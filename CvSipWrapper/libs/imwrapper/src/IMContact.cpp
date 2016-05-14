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

#include <imwrapper/IMContact.h>

#include <imwrapper/IMAccount.h>

#include <util/String.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

IMContact::IMContact() {
	_imAccount = NULL;
	_presenceState = EnumPresenceState::PresenceStateUnknown;
	_protocol = EnumIMProtocol::IMProtocolUnknown;
}

IMContact::IMContact(const IMAccount & imAccount, const std::string & contactId) {
	_imAccount = imAccount.clone();
	_contactId = contactId;
	_presenceState = EnumPresenceState::PresenceStateUnknown;
	_protocol = imAccount.getProtocol();
}

IMContact::IMContact(EnumIMProtocol::IMProtocol protocol, const std::string & contactId) {
	_imAccount = NULL;
	_contactId = contactId;
	_presenceState = EnumPresenceState::PresenceStateUnknown;
	_protocol = protocol;
}

IMContact::IMContact(const IMContact & imContact)
	: Trackable() {
	copy(imContact);
}

IMContact & IMContact::operator = (const IMContact & imContact) {
	OWSAFE_DELETE(_imAccount);

	copy(imContact);

	return *this;
}

void IMContact::copy(const IMContact & imContact) {
	if (imContact._imAccount) {
		_imAccount = imContact._imAccount->clone();
	} else {
		_imAccount = NULL;
	}
	_contactId = imContact._contactId;
	_presenceState = imContact._presenceState;
	_protocol = imContact._protocol;
	_icon = imContact._icon;
	_alias = imContact._alias;
}

IMContact::~IMContact() {
	OWSAFE_DELETE(_imAccount);
}

bool IMContact::operator == (const IMContact & imContact) const {
	bool result = false;

	if (_imAccount && imContact._imAccount) {
		if ((*_imAccount) == (*(imContact._imAccount))) {
			if (cleanContactId() == imContact.cleanContactId()) {
				result = true;
			}
		}
	} else {
		if (getProtocol() == imContact.getProtocol()) {
			if (cleanContactId() == imContact.cleanContactId()) {
				result = true;
			}
		}
	}

	return result;
}

bool IMContact::operator < (const IMContact & imContact) const {
	bool result = false;

	if (cleanContactId() < imContact.cleanContactId()) {
		result = true;
	} else {
		if (cleanContactId() == imContact.cleanContactId()) {
			if (_imAccount && imContact._imAccount) {
				if (_imAccount->getUUID() < imContact._imAccount->getUUID()) {
					result = true;
				}
			} else {
				if (getProtocol() < imContact.getProtocol()) {
					result = true;
				}
			}
		}
	}

	return result;
}

std::string IMContact::cleanContactId() const {

	if (_protocol == EnumIMProtocol::IMProtocolWengo) {
		return getDisplayContactId();
	}

	std::string result;
	std::string::size_type index = _contactId.find('/');

	if (index != std::string::npos) {
		result = _contactId.substr(0, index);
	} else {
		result = _contactId;
	}

	return result;
}

std::string IMContact::getIMAccountId() const {
	std::string result;

	if (_imAccount) {
		result = _imAccount->getUUID();
	}

	return result;
}

void IMContact::setIMAccount(const IMAccount * imAccount) {
	OWSAFE_DELETE(_imAccount);

	if (imAccount) {
		_imAccount = imAccount->clone();
		_protocol = imAccount->getProtocol();
	} else {
		_imAccount = NULL;
	}
}

void IMContact::setContactId(const std::string & contactId) {
	_contactId = contactId;
}

void IMContact::setPresenceState(EnumPresenceState::PresenceState presenceState) {
	if (_presenceState == presenceState) {
		return;
	}
	_presenceState = presenceState;
	imContactChangedEvent(*this);
}

void IMContact::setAlias(const std::string& alias) {
	if (_alias == alias) {
		return;
	}
	_alias = alias;
	imContactChangedEvent(*this);
}

void IMContact::setIcon(const OWPicture& icon) {
	_icon = icon;
	imContactChangedEvent(*this);
}

std::string IMContact::getDisplayContactId() const {
	if (_protocol == EnumIMProtocol::IMProtocolWengo ) {
		size_t posFirstOf = _contactId.find_first_of("@");
		if (posFirstOf < _contactId.npos) {
			return _contactId.substr(0, posFirstOf);
		}
	}
	return _contactId;
}
