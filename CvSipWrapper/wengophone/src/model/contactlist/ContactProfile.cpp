#include "stdafx.h"
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

#include "ContactProfile.h"

#include "ContactPresenceStateUnknown.h"
#include "ContactPresenceStateOnline.h"
#include "ContactPresenceStateOffline.h"
#include "ContactPresenceStateAway.h"
#include "ContactPresenceStateDoNotDisturb.h"
#include "ContactPresenceStateUnavailable.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/profile/AvatarList.h>

#include <imwrapper/IMAccount.h>
#include <imwrapper/IMContact.h>
#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMChat.h>

#define LOGGER_COMPONENT "ContactList"
#include <util/Logger.h>
#include <util/OWPicture.h>
#include <util/Uuid.h>

#include <iostream>

using namespace std;

ContactProfile::PresenceStates ContactProfile::_presenceStateMap;

static ContactPresenceStateUnknown contactPresenceStateUnknown;
static ContactPresenceStateOnline contactPresenceStateOnline;
static ContactPresenceStateOffline contactPresenceStateOffline;
static ContactPresenceStateAway contactPresenceStateAway;
static ContactPresenceStateDoNotDisturb contactPresenceStateDoNotDisturb;
static ContactPresenceStateUnavailable contactPresenceStateUnavailable;

ContactProfile::ContactProfile() {
	_sex = EnumSex::SexUnknown;
	_blocked = false;
	_preferredIMContact = NULL;

	_uuid = Uuid::generateString();

	//Default state (ContactPresenceStateUnknown)
	_presenceState = &contactPresenceStateUnknown;

	_presenceStateMap[contactPresenceStateUnknown.getCode()] = &contactPresenceStateUnknown;
	_presenceStateMap[contactPresenceStateOnline.getCode()] = &contactPresenceStateOnline;
	_presenceStateMap[contactPresenceStateOffline.getCode()] = &contactPresenceStateOffline;
	_presenceStateMap[contactPresenceStateAway.getCode()] = &contactPresenceStateAway;
	_presenceStateMap[contactPresenceStateDoNotDisturb.getCode()] = &contactPresenceStateDoNotDisturb;
	_presenceStateMap[contactPresenceStateUnavailable.getCode()] = &contactPresenceStateUnavailable;
}

ContactProfile::ContactProfile(const ContactProfile & contactProfile)
: Profile() {
	copy(contactProfile);
}

ContactProfile::~ContactProfile() {
}

ContactProfile & ContactProfile::operator=(const ContactProfile & contactProfile) {
	if (&contactProfile != this) {
		copy(contactProfile);
	}

	return *this;
}

void ContactProfile::copy(const ContactProfile & contactProfile) {
	Profile::copy(contactProfile);
	_uuid = contactProfile._uuid;
	_blocked = contactProfile._blocked;
	_preferredIMContact = contactProfile._preferredIMContact;
	_imContactSet = contactProfile._imContactSet;
	_groupId = contactProfile._groupId;
	_presenceState = contactProfile._presenceState;
}

bool ContactProfile::operator == (const ContactProfile & contactProfile) const {
	return (_uuid == contactProfile._uuid);
}

void ContactProfile::addIMContact(const IMContact & imContact) {
	pair<IMContactSet::const_iterator, bool> result = _imContactSet.insert(imContact);
	updatePresenceState();
}

void ContactProfile::removeIMContact(const IMContact & imContact) {
	IMContactSet::iterator it = _imContactSet.find(imContact);

	if (it != _imContactSet.end()) {
		_imContactSet.erase(it);
	}
	updatePresenceState();
}

bool ContactProfile::hasIMContact(const IMContact & imContact) const {
	if (_imContactSet.find(imContact) != _imContactSet.end()) {
		return true;
	} else {
		return false;
	}
}

IMContact * ContactProfile::getIMContact(const IMContact & imContact) const {
	IMContactSet::const_iterator it = _imContactSet.find(imContact);
	if (it != _imContactSet.end()) {
		return ((IMContact *) &(*it));
	} else {
		return NULL;
	}
}

void ContactProfile::setGroupId(const std::string & groupId) {
	_groupId = groupId;
	profileChangedEvent(*this);
}

bool ContactProfile::hasIM() const 
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	if(config.getSipChatWithoutPresence())
	{
	  String imContact = getFirstAvailableSIPNumber();
	  if(!imContact.empty())
	    return true;
	}

	return (getPresenceState() != EnumPresenceState::PresenceStateOffline &&
		getPresenceState() != EnumPresenceState::PresenceStateUnknown && 
		getPresenceState() != EnumPresenceState::PresenceStateUnavailable);
}

bool ContactProfile::hasCall() const {
	if (!getPreferredNumber().empty()) {
		return true;
	} else {
		return false;
	}
}

bool ContactProfile::hasVideo() const {
	return hasAvailableWengoId();
}

bool ContactProfile::hasVoiceMail() const {
	// For now, we assume only Wengo accounts support voice mail
	return !getFirstWengoId().empty();
}

bool ContactProfile::hasFileTransfer() const {
	if (!getFirstWengoId().empty() && isAvailable()) {
		IMContact imContact = getFirstAvailableWengoIMContact();
		if ((imContact.getPresenceState() != EnumPresenceState::PresenceStateOffline) &&
			(imContact.getPresenceState() != EnumPresenceState::PresenceStateUnknown) &&
			(imContact.getPresenceState() != EnumPresenceState::PresenceStateUnavailable)) {
				return true;
		}
	}
	return false;
}

std::string ContactProfile::getPreferredNumber() const {
	string result;

	if (!_preferredNumber.empty()) {
		result = _preferredNumber;
	} else if (hasAvailableWengoId()) {
			result = getFirstAvailableWengoId();
	} else if (hasAvailableSIPNumber()) {
			result = getFirstAvailableSIPNumber();
	} else if (!_mobilePhone.empty()) {
		result = _mobilePhone;
	} else if (!_homePhone.empty()) {
		result = _homePhone;
	} else if (!_workPhone.empty()) {
		result = _workPhone;
	} else if (!_otherPhone.empty()) {
		result = _otherPhone;
	}

	return result;
}

/**
 * Helper to assign scores to presence states
 * FIXME: This should be moved in EnumPresenceState, as a getScore() method
 */
static int getStateScore(EnumPresenceState::PresenceState state) {
	typedef std::map<EnumPresenceState::PresenceState, int> StateScoreMap;
	static StateScoreMap stateScoreMap;

	if (stateScoreMap.empty()) {
		stateScoreMap[EnumPresenceState::PresenceStateOffline] = 1;
		stateScoreMap[EnumPresenceState::PresenceStateDoNotDisturb] = 2;
		stateScoreMap[EnumPresenceState::PresenceStateAway] = 3;
		stateScoreMap[EnumPresenceState::PresenceStateOnline] = 4;
	}

	StateScoreMap::const_iterator it = stateScoreMap.find(state);
	if (it == stateScoreMap.end()) {
		return 0;
	}
	return it->second;
};

IMContact * ContactProfile::getPreferredIMContact() const {
	const int offlineScore = getStateScore(EnumPresenceState::PresenceStateOffline);

	IMContact * bestIMContact = 0;
	int bestIMContactScore = -1;
	IMContact * bestWengoContact = 0;
	int bestWengoContactScore = -1;

	// Find best IM contact and best Wengo contact
	IMContactSet::const_iterator
		it = _imContactSet.begin(),
		end = _imContactSet.end();

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	bool ignorePresence = config.getSipChatWithoutPresence();

	for (; it!=end; ++it) {
		EnumPresenceState::PresenceState state = it->getPresenceState();
		int contactScore = getStateScore(state);



		if (!ignorePresence && (contactScore <= offlineScore))
		  continue;


		IMContact* contact = const_cast<IMContact*>( &(*it) );

		if (contact->getProtocol() == EnumIMProtocol::IMProtocolWengo) {
			if (contactScore > bestWengoContactScore) {
				bestWengoContact = contact;
				bestWengoContactScore = contactScore;
			}
		} else {
			if (contactScore > bestIMContactScore) {
				bestIMContact = contact;
				bestIMContactScore = contactScore;
			}
		}
	}

	// Get score of _preferredIMContact, if any
	int preferredIMContactScore;
	if (_preferredIMContact) {
		preferredIMContactScore = getStateScore(_preferredIMContact->getPresenceState());
	} else {
		preferredIMContactScore = -1;
	}

	// Choose the best one
	IMContact* bestContact = 0;
	if (bestWengoContactScore >= bestIMContactScore) {
		if (preferredIMContactScore >= bestWengoContactScore) {
			bestContact =  _preferredIMContact;
		} else {
			bestContact = bestWengoContact;
		}
	} else {
		if (preferredIMContactScore >= bestIMContactScore) {
			bestContact = _preferredIMContact;
		} else {
			bestContact = bestIMContact;
		}
	}
	////

	return bestContact;
}

EnumPresenceState::PresenceState ContactProfile::getPresenceState() const {
	return _presenceState->getCode();
}

EnumPresenceState::PresenceState ContactProfile::computePresenceState() const {
	// Find best state
	EnumPresenceState::PresenceState bestState = EnumPresenceState::PresenceStateUnknown;
	int bestStateScore = 0;

	IMContactSet::const_iterator
		it = _imContactSet.begin(),
		end = _imContactSet.end();

	for (; it!=end; ++it) {
		EnumPresenceState::PresenceState contactState = it->getPresenceState();
		int contactStateScore = getStateScore(contactState);

		if (contactStateScore > bestStateScore) {
			bestState = contactState;
			bestStateScore = contactStateScore;
		}
	}

	return bestState;
}

void ContactProfile::updatePresenceState() {
	EnumPresenceState::PresenceState presenceState = computePresenceState();

	LOG_DEBUG("PresenceState=" + String::fromNumber(presenceState));

	PresenceStates::iterator it = _presenceStateMap.find(presenceState);
	if (it == _presenceStateMap.end()) {
		LOG_FATAL("unknown PresenceState=" + String::fromNumber(presenceState));
	}

	ContactPresenceState * state = it->second;
	if (state->getCode() == presenceState) {
		if (_presenceState->getCode() != state->getCode()) {
			_presenceState = state;
			_presenceState->execute(*this);
			LOG_DEBUG("presence state changed=" + EnumPresenceState::toString(_presenceState->getCode()));
			profileChangedEvent(*this);
			return;
		}
	}
}

OWPicture ContactProfile::getIcon() const {
	OWPicture icon;
	for (IMContactSet::const_iterator it = _imContactSet.begin() ;
		it != _imContactSet.end() ;
		it++) {
		icon = it->getIcon();
		if (!icon.getData().empty()) {
			return icon;
		}
	}

	return AvatarList::getInstance().getDefaultAvatarPicture();
}

string ContactProfile::getDisplayName() const {
	string result;
	string pseudo;
	string contactId;

	if (!_firstName.empty() || !_lastName.empty()) {
		result += _firstName;

		if (!result.empty() && !_lastName.empty()) {
			result += " ";
		}

		result += _lastName;
	}

	// Take the alias of the first IMContact
	for (IMContactSet::const_iterator it = _imContactSet.begin();
		it != _imContactSet.end();
		++it) {
		if (!(*it).getAlias().empty()) {
			pseudo = (*it).getAlias();
			contactId = (*it).getDisplayContactId();
			break;
		}
	}

	// If no alias set, we take the first contact id:
	if (contactId.empty()) {
		IMContactSet::const_iterator it = _imContactSet.begin();
		if (it != _imContactSet.end()) {
			contactId = (*it).getDisplayContactId();
		}
	}

	if (result.empty()) {
		result = contactId;
	}

	// FIXME: I don't know how this happens, but sometimes the pseudo is set to
	// "(null)"! Since I don't have time to investigate that bug (and don't
	// know how to reproduce it), I resort to this hack for now.
	// See ticket #1495
	if (!pseudo.empty() && pseudo != "(null)") {
		result += " - (" + pseudo + ")";
	}

	return result;
}

std::string ContactProfile::getShortDisplayName() const {
	std::string toReturn = getFirstName();
	if (toReturn.empty()) {
		toReturn = getLastName();
	}

	return std::string(toReturn);
}

IMContact * ContactProfile::getFirstAvailableIMContact(IMChatSession & imChatSession) const {
	IMContact * result = NULL;

	for (IMContactSet::const_iterator it = _imContactSet.begin() ; it != _imContactSet.end() ; ++it) {
		if (((*it).getPresenceState() != EnumPresenceState::PresenceStateOffline)
			&& (((*it).getIMAccountId()) == imChatSession.getIMChat().getIMAccountId())) {
			result = (IMContact *)&(*it);
			break;
		}
	}

	return result;
}

std::string ContactProfile::getFirstAvailableSIPNumber() const {
	std::string result;

	for (IMContactSet::const_iterator it = _imContactSet.begin();
		it != _imContactSet.end();
		++it) {
		if (((*it).getProtocol() == EnumIMProtocol::IMProtocolSIPSIMPLE) ||
			((*it).getProtocol() == EnumIMProtocol::IMProtocolSIP)) {
			result = (*it).getContactId();
			break;
		}
	}

	return result;
}

bool ContactProfile::hasAvailableSIPNumber() const {
	return (!getFirstAvailableSIPNumber().empty());
}

IMContact ContactProfile::getFirstAvailableWengoIMContact() const {
	IMContact result;

	for (IMContactSet::const_iterator it = _imContactSet.begin();
		it != _imContactSet.end();
		++it) {
			
		if (((*it).getProtocol() == EnumIMProtocol::IMProtocolWengo)
			&& ((*it).getPresenceState() != EnumPresenceState::PresenceStateOffline)) {
			result = *it;
			break;
		}
	}

	return result;
}

std::string ContactProfile::getFirstAvailableWengoId() const {
	return getFirstAvailableWengoIMContact().getContactId();
}

std::string ContactProfile::getFirstWengoId() const {
	std::string result;

	for (IMContactSet::const_iterator it = _imContactSet.begin();
		it != _imContactSet.end();
		++it) {
		if (((*it).getProtocol() == EnumIMProtocol::IMProtocolWengo)) {
			result = (*it).getContactId();
			break;
		}
	}

	return result;
}

bool ContactProfile::hasAvailableWengoId() const {
	return (!getFirstAvailableWengoId().empty());
}

bool ContactProfile::hasAvailableSIPNumberAndNoWengoId() const {

	bool hasSIP = false;
	
	for (IMContactSet::const_iterator it = _imContactSet.begin();
		it != _imContactSet.end();
		++it) {
		if (((*it).getProtocol() == EnumIMProtocol::IMProtocolWengo)) {
			return false;
		} else if (((*it).getProtocol() == EnumIMProtocol::IMProtocolSIPSIMPLE) ||
			((*it).getProtocol() == EnumIMProtocol::IMProtocolSIP)) {
			hasSIP = true;
		}
	}
	
	return hasSIP;
}

std::string ContactProfile::getFirstFreePhoneNumber() const {
	std::string result;

	if (hasAvailableWengoId()) {
		result = getFirstAvailableWengoId();
	} else if (hasAvailableSIPNumber()) {
		result = getFirstAvailableSIPNumber();
	}

	return result;
}

std::string ContactProfile::getFirstVoiceMailNumber() const {
	return getFirstWengoId();
}

bool ContactProfile::hasFreeCall() const {
	return (!getFirstFreePhoneNumber().empty());
}

void ContactProfile::setWengoPhoneId(const std::string & wengoPhoneId) {
	Profile::setWengoPhoneId(wengoPhoneId);
	addIMContact(IMContact(EnumIMProtocol::IMProtocolWengo, wengoPhoneId));
}

bool ContactProfile::isAvailable() const {
	return ((getPresenceState() != EnumPresenceState::PresenceStateOffline)
		&& ((getPresenceState() != EnumPresenceState::PresenceStateUnknown)));
}

void ContactProfile::setUUID(const std::string & newUUID) {
	_uuid = newUUID;
}
