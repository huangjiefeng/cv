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

#include "IMAccountManager.h"

#include <imwrapper/IMWrapperFactory.h>

class IMAccountEqualsPredicate : public std::binary_function<IMAccount, IMAccount, bool> {
public:
	bool operator () (IMAccount c1, IMAccount c2) const {
		return ((c1.getLogin() == c2.getLogin())
			&& (c1.getProtocol() == c2.getProtocol()));
	}
};

IMAccountManager::IMAccountManager() {
}

IMAccountManager::~IMAccountManager() {
}

IMAccountList IMAccountManager::getIMAccountsOfProtocol(EnumIMProtocol::IMProtocol protocol) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMAccountList result;

	for (IMAccountList::const_iterator it = _imAccountList.begin();
		it != _imAccountList.end();
		++it) {

		// FIXME: line 50 and 51 should not be there.
		// There are here for compatibility with previous version of WengoPhone.
		EnumIMProtocol::IMProtocol accountProtocol = (*it).getProtocol();
		if ((accountProtocol == protocol) ||
			((protocol == EnumIMProtocol::IMProtocolSIPSIMPLE) &&
			 (accountProtocol == EnumIMProtocol::IMProtocolSIP)) ||
			((accountProtocol == EnumIMProtocol::IMProtocolSIPSIMPLE) &&
			 (protocol == EnumIMProtocol::IMProtocolSIP)))
		{
			result.push_back(*it);
		}
	}

	return result;
}

IMAccountList IMAccountManager::getIMAccountsOfProtocolVector(std::vector<EnumIMProtocol::IMProtocol> protocols) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMAccountList result;

	for (IMAccountList::const_iterator it = _imAccountList.begin();
		it != _imAccountList.end();
		++it) {

		for(std::vector<EnumIMProtocol::IMProtocol>::const_iterator itv = protocols.begin();
			itv != protocols.end();
			++itv) {

			if ((*it).getProtocol() == (*itv)) {
				result.push_back(*it);
				break;
			}

		}

	}
	return result;
}

IMAccountList IMAccountManager::getIMAccountListCopy() const {
	return _imAccountList;
}

IMAccount * IMAccountManager::getIMAccount(const std::string & imAccountId) const {
	RecursiveMutex::ScopedLock lock(_mutex);

	IMAccount * result = NULL;

	for (IMAccountList::const_iterator it = _imAccountList.begin();
		it != _imAccountList.end();
		++it) {
		if ((*it).getUUID() == imAccountId) {
			result = (*it).clone();
			break;
		}
	}

	return result;
}

bool IMAccountManager::contains(const std::string & imAccountId) {
	RecursiveMutex::ScopedLock lock(_mutex);

	bool result = false;

	for (IMAccountList::const_iterator it = _imAccountList.begin();
		it != _imAccountList.end();
		++it) {
		if ((*it).getUUID() == imAccountId) {
			result = true;
			break;
		}
	}

	return result;
}

bool IMAccountManager::addIMAccount(const IMAccount & imAccount) {
	RecursiveMutex::ScopedLock lock(_mutex);

	bool result = false;

	IMAccountList::const_iterator it = 
		std::find_if(_imAccountList.begin(), _imAccountList.end(), 
			std::bind2nd(IMAccountEqualsPredicate(), imAccount));
	if (it == _imAccountList.end()) {
		_imAccountList.push_back(imAccount);
		imAccountAddedEvent(*this, imAccount.getUUID());
		result = true;
	}

	return result;
}

bool IMAccountManager::removeIMAccount(const IMAccount & imAccount) {
	RecursiveMutex::ScopedLock lock(_mutex);

	bool result = false;

	IMAccountList::iterator it = 
		std::find(_imAccountList.begin(), _imAccountList.end(), imAccount);
	if (it != _imAccountList.end()) {

		// TODO: Hack. See IMWrapperFactory::removeIMAccount doc
		IMWrapperFactory::getFactory().removeIMAccount(imAccount);

		imAccountRemovedEvent(*this, imAccount.getUUID());
		_imAccountList.erase(it);

		result = true;
	}

	return result;
}

bool IMAccountManager::updateIMAccount(const IMAccount & imAccount) {
	RecursiveMutex::ScopedLock lock(_mutex);

	bool result = false;

	IMAccountList::iterator it = 
		std::find(_imAccountList.begin(), _imAccountList.end(), imAccount);
	if (it != _imAccountList.end()) {
		(*it) = imAccount;
		IMWrapperFactory::getFactory().imAccountUpdated(imAccount);
		imAccountUpdatedEvent(*this, imAccount.getUUID());
		result = true;
	}

	return result;
	
}

void IMAccountManager::emitIMAccountAddedEvent() {
	RecursiveMutex::ScopedLock lock(_mutex);

	for (IMAccountList::const_iterator it = _imAccountList.begin();
		it != _imAccountList.end();
		++it) {
		imAccountAddedEvent(*this, (*it).getUUID());
	}
}
