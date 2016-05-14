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

#include "PresenceHandler.h"

#include <model/presence/Presence.h>
#include <model/contactlist/ContactList.h>
#include <model/connect/ConnectHandler.h>
#include <model/profile/UserProfile.h>
#include <model/WengoPhone.h>

#include <imwrapper/IMContact.h>

#include <thread/ThreadEvent.h>

#define LOGGER_COMPONENT "Presence"
#include <util/Logger.h>
#include <util/OWPicture.h>

PresenceHandler::PresenceHandler(UserProfile & userProfile)
	: _userProfile(userProfile) {

	_userProfile.getConnectHandler().connectedEvent +=
		boost::bind(&PresenceHandler::connectedEventHandler, this, _1, _2);
	_userProfile.getConnectHandler().disconnectedEvent +=
		boost::bind(&PresenceHandler::disconnectedEventHandler, this, _1, _2);

	_userProfile.getIMAccountManager().imAccountAddedEvent +=
		boost::bind(&PresenceHandler::imAccountAddedEventHandler, this, _1, _2);
	_userProfile.getIMAccountManager().imAccountRemovedEvent +=
		boost::bind(&PresenceHandler::imAccountRemovedEventHandler, this, _1, _2);
}

PresenceHandler::~PresenceHandler() {
	for (PresenceMap::const_iterator it = _presenceMap.begin();
		it != _presenceMap.end();
		++it) {
		delete (*it).second;
	}
}

void PresenceHandler::subscribeToPresenceOf(const IMContact & imContact) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imContact.getIMAccountId());
		if (it != _presenceMap.end()) {
			LOG_DEBUG("subscribing to presence of=" + imContact.getContactId());
			(*it).second->subscribeToPresenceOf(imContact.getContactId());
			_subscribedContacts.push_back(imContact);
		} else {
			//Presence for 'protocol' has not yet been created.
			// The contactId is pushed in the pending subscription list
			_pendingSubscriptions.push_back(imContact);
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::unsubscribeToPresenceOf(const IMContact & imContact) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imContact.getIMAccountId());
		if (it != _presenceMap.end()) {
			LOG_DEBUG("unsubscribing to presence of=" + imContact.getContactId());
			(*it).second->unsubscribeToPresenceOf(imContact.getContactId());
			MyIMContactList::iterator subIt =
				std::find(_subscribedContacts.begin(), _subscribedContacts.end(), imContact);
			if (subIt != _subscribedContacts.end()) {
				_subscribedContacts.erase(subIt);
			}
		} else {
			LOG_ERROR("cannot find associated Presence instance");
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::blockContact(const IMContact & imContact) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imContact.getIMAccountId());
		if (it != _presenceMap.end()) {
			LOG_DEBUG("blocking contact=" + imContact.getContactId()
				+ " of IMAccount=" + imAccount->getLogin()
				+ " of protocol=" + String::fromNumber(imAccount->getProtocol()));

			(*it).second->blockContact(imContact.getContactId());
		} else {
			LOG_ERROR("the given IMAccount has not been added yet");
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::unblockContact(const IMContact & imContact) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imContact.getIMAccountId());
		if (it != _presenceMap.end()) {
			LOG_DEBUG("unblocking contact=" + imContact.getContactId()
				+ " of IMAccount=" + imAccount->getLogin()
				+ " of protocol=" + String::fromNumber(imAccount->getProtocol()));

			(*it).second->unblockContact(imContact.getContactId());
		} else {
			LOG_ERROR("the given IMAccount has not been added yet");
		}
	}
}

void PresenceHandler::connectedEventHandler(ConnectHandler & sender, std::string imAccountId) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imAccountId);
		if (it != _presenceMap.end()) {
			LOG_DEBUG("an account is connected, login=" + imAccount->getLogin()
				+ " protocol=" + String::fromNumber(imAccount->getProtocol()));

			EnumPresenceState::PresenceState presenceState = imAccount->getPresenceState();
			if (presenceState == EnumPresenceState::PresenceStateOffline) {
				presenceState =  EnumPresenceState::PresenceStateOnline;
			}

			(*it).second->changeMyPresence(presenceState, String::null);

			// Launch subscriptions to all already subscribed contacts
			for (MyIMContactList::const_iterator subIt = _subscribedContacts.begin();
				subIt != _subscribedContacts.end();
				++subIt) {
				if ((*subIt).getIMAccountId() == imAccountId) {
					LOG_DEBUG("subscribing to presence of=" + (*subIt).getContactId());
					(*it).second->subscribeToPresenceOf((*subIt).getContactId());
				}
			}
			////

			//Launch all pending subscriptions
			for (MyIMContactList::iterator pendIt = _pendingSubscriptions.begin();
				pendIt != _pendingSubscriptions.end();) {
				if ((*pendIt).getIMAccountId() == imAccountId) {
					LOG_DEBUG("subscribing to presence of=" + (*pendIt).getContactId());
					(*it).second->subscribeToPresenceOf((*pendIt).getContactId());
					_subscribedContacts.push_back(*pendIt);
					_pendingSubscriptions.erase(pendIt++);
				} else {
					++pendIt;
				}
			}
			////

			changeMyAlias(_userProfile.getAlias(), imAccount);
			changeMyIcon(_userProfile.getIcon(), imAccount);
		} else {
			LOG_FATAL("the given IMAccount has not been added yet");
		}

		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::disconnectedEventHandler(ConnectHandler & sender, std::string imAccountId) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		LOG_DEBUG("an account is disconnected, login=" + imAccount->getLogin()
			+ " protocol=" + String::fromNumber(imAccount->getProtocol()));

		PresenceMap::iterator it = _presenceMap.find(imAccountId);
		if (it != _presenceMap.end()) {
			setPresenceStateToUnknown((*it).second);
		} else {
			LOG_FATAL("the given IMAccount has not been added yet");
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::changeMyPresenceState(EnumPresenceState::PresenceState state,
	const std::string & note, IMAccount * imAccount) {

	LOG_DEBUG("changing MyPresenceState for "
		+ ((!imAccount) ? "all" : imAccount->getLogin() + ", of protocol=" + String::fromNumber(imAccount->getProtocol()))
		+ " with state=" + String::fromNumber(state) + " and note=" + note);

	if (!imAccount) {
		for (PresenceMap::const_iterator it = _presenceMap.begin();
			it != _presenceMap.end();
			it++) {
			//TODO: this code should be in a event handler
			IMAccount * imAccount =
				_userProfile.getIMAccountManager().getIMAccount((*it).first);
			if (imAccount) {
				imAccount->setPresenceState(state);
				_userProfile.updateIMAccount(*imAccount);
				OWSAFE_DELETE(imAccount);
			}
			(*it).second->changeMyPresence(state, note);
		}
	} else {
		//Find the desired Protocol
		PresenceMap::iterator it = _presenceMap.find(imAccount->getUUID());

		if (it != _presenceMap.end()) {
			//TODO: this code should be in a event handler
			IMAccount * tmp =
				_userProfile.getIMAccountManager().getIMAccount(imAccount->getUUID());
			if (tmp) {
				tmp->setPresenceState(state);
				_userProfile.updateIMAccount(*tmp);
				OWSAFE_DELETE(tmp);
			}
			(*it).second->changeMyPresence(state, note);
		}
	}
}

void PresenceHandler::changeMyAlias(const std::string & alias, IMAccount * imAccount) {
	LOG_DEBUG("changing alias for "
		+ ((!imAccount) ? "all" : imAccount->getLogin() + ", of protocol=" + String::fromNumber(imAccount->getProtocol()))
		+ " with alias=" + alias);

	if (!imAccount) {
		for (PresenceMap::const_iterator it = _presenceMap.begin();
			it != _presenceMap.end();
			it++) {
			(*it).second->changeMyAlias(alias);
		}
	} else {
		//Find the desired Protocol
		PresenceMap::iterator it = _presenceMap.find(imAccount->getUUID());

		if (it != _presenceMap.end()) {
			(*it).second->changeMyAlias(alias);
		}
	}
}

void PresenceHandler::changeMyIcon(const OWPicture & picture, IMAccount * imAccount) {
	LOG_DEBUG("changing icon for "
		+ ((!imAccount) ? "all" : imAccount->getLogin() + ", of protocol=" + String::fromNumber(imAccount->getProtocol())));

	if (!imAccount) {
		for (PresenceMap::const_iterator it = _presenceMap.begin();
			it != _presenceMap.end();
			it++) {
			(*it).second->changeMyIcon(picture);
		}
	} else {
		//Find the desired Protocol
		PresenceMap::iterator it = _presenceMap.find(imAccount->getUUID());

		if (it != _presenceMap.end()) {
			(*it).second->changeMyIcon(picture);
		}
	}
}

void PresenceHandler::presenceStateChangedEventHandler(IMPresence & sender,
	EnumPresenceState::PresenceState state, std::string alias, std::string from) {

/*
	typedef ThreadEvent4<void (IMAccount * imAccount, EnumPresenceState::PresenceState state, std::string alias, std::string from),
		IMAccount *, EnumPresenceState::PresenceState, std::string, std::string> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&PresenceHandler::presenceStateChangedEventHandlerThreadSafe, this, _1, _2, _3, _4), &sender.getIMAccount(), state, alias, from);

	_modelThread.postEvent(event);
	FIXME: crashes when trying to change UserProfile
*/
	presenceStateChangedEventHandlerThreadSafe(sender.getIMAccountId(), state, alias, from);
}

void PresenceHandler::presenceStateChangedEventHandlerThreadSafe(std::string imAccountId,
	EnumPresenceState::PresenceState state, std::string note, std::string from) {

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		LOG_DEBUG("presence of=" + imAccount->getLogin() + " changed=" + EnumPresenceState::toString(state));
		if (!imAccount->isConnected()) {
			state = EnumPresenceState::PresenceStateUnknown;
		}
		presenceStateChangedEvent(*this, state, note, IMContact(*imAccount, from));
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::myPresenceStatusEventHandler(IMPresence & sender,
	EnumPresenceState::MyPresenceStatus status) {
/*
	typedef ThreadEvent2<void (IMAccount * imAccount, EnumPresenceState::MyPresenceStatus status),
		IMAccount *, EnumPresenceState::MyPresenceStatus> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&PresenceHandler::myPresenceStatusEventHandlerThreadSafe, this, _1, _2), &sender.getIMAccount(), status);

	_modelThread.postEvent(event);
	FIXME: crashes when trying to change UserProfile
*/
	if ((status == EnumPresenceState::MyPresenceStatusOk) || (status == EnumPresenceState::MyPresenceStatusError)) {
		myPresenceStatusEventHandlerThreadSafe(sender.getIMAccountId(), status);
	} else {
		LOG_WARN("bad EnumPresenceState::MyPresenceStatus" + String::fromNumber(status));
	}
}

void PresenceHandler::myPresenceStatusEventHandlerThreadSafe(std::string imAccountId,
	EnumPresenceState::MyPresenceStatus status) {

	myPresenceStatusEvent(*this, imAccountId, status);
}

void PresenceHandler::authorizationRequestEventHandler(IMPresence & sender,
	std::string contactId, std::string message) {

	typedef ThreadEvent3<void (std::string imAccountId, std::string contactId, std::string message),
		std::string, std::string, std::string> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&PresenceHandler::authorizationRequestEventHandlerThreadSafe, this, _1, _2, _3), sender.getIMAccountId(), contactId, message);

	WengoPhone::getInstance().postEvent(event);
}

void PresenceHandler::authorizationRequestEventHandlerThreadSafe(std::string imAccountId,
	std::string contactId, std::string message) {

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		authorizationRequestEvent(*this, IMContact(*imAccount, contactId), message);
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccout not found!");
	}
}


void PresenceHandler::incomingSubscribeEventHandler(IMPresence & sender,
	int sid, const std::string& from, const std::string& evtType) {

	typedef ThreadEvent4<void (IMPresence & sender,int sid, const std::string& from, const std::string& evtType),
		IMPresence &,int, std::string, std::string> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&PresenceHandler::incomingSubscribeEventHandlerThreadSafe, this, _1, _2, _3, _4),sender, sid, from, evtType);

	WengoPhone::getInstance().postEvent(event);
}

void PresenceHandler::incomingSubscribeEventHandlerThreadSafe(IMPresence & sender,int sid,std::string from, std::string evtType) 
{
	/*IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		authorizationRequestEvent(*this, IMContact(*imAccount, contactId), message);
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccout not found!");
	}*/
	incomingSubscribeEvent(*this,sender.getIMAccountId(),sid,from,evtType);
}

void PresenceHandler::acceptSubscription(int sid,std::string imaccountId)
{
	IMAccount * imAccount = _userProfile.getIMAccountManager().getIMAccount(imaccountId);
	if (imAccount) 
	{
		PresenceMap::iterator it = _presenceMap.find(imaccountId);
		if (it != _presenceMap.end()) 
		{
			(*it).second->acceptSubscription(sid);
		} 
		else 
		{
			LOG_FATAL("unknown IMAccount");
		}

		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::rejectSubscription(int sid,std::string imaccountId)
{
	IMAccount * imAccount = _userProfile.getIMAccountManager().getIMAccount(imaccountId);
	if (imAccount) 
	{
		PresenceMap::iterator it = _presenceMap.find(imaccountId);
		if (it != _presenceMap.end()) 
		{
			(*it).second->rejectSubscription(sid);
		} 
		else 
		{
			LOG_FATAL("unknown IMAccount");
		}

		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::subscribeStatusEventHandler(IMPresence & sender, 
	std::string contactId, IMPresence::SubscribeStatus status) {
/*
	typedef ThreadEvent3<void (IMAccount * imAccount, std::string contactId, IMPresence::SubscribeStatus status),
		IMAccount *, std::string, IMPresence::SubscribeStatus> MyThreadEvent;
	MyThreadEvent * event =
		new MyThreadEvent(boost::bind(&PresenceHandler::subscribeStatusEventHandlerThreadSafe, this, _1, _2, _3), &sender.getIMAccount(), contactId, status);

	_modelThread.postEvent(event);
*/
	subscribeStatusEventHandlerThreadSafe(sender.getIMAccountId(), contactId, status);
}

void PresenceHandler::subscribeStatusEventHandlerThreadSafe(std::string imAccountId,
	std::string contactId, IMPresence::SubscribeStatus status) {

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		subscribeStatusEvent(*this, IMContact(*imAccount, contactId), status);
		OWSAFE_DELETE(imAccount);
	} else {
		LOG_ERROR("IMAccount not found!");
	}
}

void PresenceHandler::imAccountAddedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imAccountId);
		if (it == _presenceMap.end()) {
			//Presence for this IMAccount has not yet been created
			Presence * presence = new Presence(*imAccount);
			_presenceMap.insert(std::pair<std::string, Presence *>(imAccountId, presence));

			presence->presenceStateChangedEvent +=
				boost::bind(&PresenceHandler::presenceStateChangedEventHandler, this, _1, _2, _3, _4);
			presence->myPresenceStatusEvent +=
				boost::bind(&PresenceHandler::myPresenceStatusEventHandler, this, _1, _2);
			presence->subscribeStatusEvent +=
				boost::bind(&PresenceHandler::subscribeStatusEventHandler, this, _1, _2, _3);
			presence->authorizationRequestEvent +=
				boost::bind(&PresenceHandler::authorizationRequestEventHandler, this, _1, _2, _3);
			presence->incomingSubscribeEvent +=
				boost::bind(&PresenceHandler::incomingSubscribeEventHandler, this, _1, _2, _3, _4);
			presence->contactIconChangedEvent +=
				boost::bind(&PresenceHandler::contactIconChangedEventHandler, this, _1, _2, _3);
		} else {
			LOG_ERROR("this IMAccount has already been added=" + imAccount->getLogin());
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::imAccountRemovedEventHandler(IMAccountManager & sender, std::string imAccountId) {
	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imAccountId);
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imAccountId);
		if (it != _presenceMap.end()) {
			setPresenceStateToUnknown((*it).second);
			delete (*it).second;
			_presenceMap.erase(it);
		} else {
			LOG_ERROR("this IMAccount has not been added=" + imAccount->getLogin());
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::authorizeContact(const IMContact & imContact, bool authorized,
	const std::string & message) {

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(imContact.getIMAccountId());
	if (imAccount) {
		PresenceMap::iterator it = _presenceMap.find(imContact.getIMAccountId());
		if (it != _presenceMap.end()) {
			return (*it).second->authorizeContact(imContact.getContactId(), authorized, message);
		} else {
			LOG_FATAL("unknown IMAccount");
		}
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::contactIconChangedEventHandler(IMPresence & sender,
	std::string contactId, OWPicture icon) {

	IMAccount * imAccount =
		_userProfile.getIMAccountManager().getIMAccount(sender.getIMAccountId());
	if (imAccount) {
		contactIconChangedEvent(*this, IMContact(*imAccount, contactId), icon);
		OWSAFE_DELETE(imAccount);
	}
}

void PresenceHandler::unregisterAllEvents() {
	_userProfile.getConnectHandler().connectedEvent -=
		boost::bind(&PresenceHandler::connectedEventHandler, this, _1, _2);
	_userProfile.getConnectHandler().disconnectedEvent -=
		boost::bind(&PresenceHandler::disconnectedEventHandler, this, _1, _2);

	_userProfile.getIMAccountManager().imAccountAddedEvent -=
		boost::bind(&PresenceHandler::imAccountAddedEventHandler, this, _1, _2);
	_userProfile.getIMAccountManager().imAccountRemovedEvent -=
		boost::bind(&PresenceHandler::imAccountRemovedEventHandler, this, _1, _2);

	for (PresenceMap::const_iterator it = _presenceMap.begin();
		it != _presenceMap.end();
		++it) {
		(*it).second->unregisterAllEvents();

		(*it).second->presenceStateChangedEvent -=
			boost::bind(&PresenceHandler::presenceStateChangedEventHandler, this, _1, _2, _3, _4);
		(*it).second->myPresenceStatusEvent -=
			boost::bind(&PresenceHandler::myPresenceStatusEventHandler, this, _1, _2);
		(*it).second->subscribeStatusEvent -=
			boost::bind(&PresenceHandler::subscribeStatusEventHandler, this, _1, _2, _3);
		(*it).second->authorizationRequestEvent -=
			boost::bind(&PresenceHandler::authorizationRequestEventHandler, this, _1, _2, _3);
		(*it).second->incomingSubscribeEvent -=
			boost::bind(&PresenceHandler::incomingSubscribeEventHandler, this, _1, _2, _3, _4);
		(*it).second->contactIconChangedEvent -=
			boost::bind(&PresenceHandler::contactIconChangedEventHandler, this, _1, _2, _3);
	}
}

void PresenceHandler::setPresenceStateToUnknown(Presence * presence) {
	if (presence) {
		for (MyIMContactList::const_iterator subIt = _subscribedContacts.begin();
			subIt != _subscribedContacts.end();
			++subIt) {
			if ((*subIt).getIMAccountId() == presence->getIMAccountId()) {
				LOG_DEBUG("unsubscribing to presence of=" + (*subIt).getContactId());
				presence->unsubscribeToPresenceOf((*subIt).getContactId());
				presenceStateChangedEvent(*this, 
					EnumPresenceState::PresenceStateUnknown, String::null, *subIt);
			}
		}
	}
}
