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
#include "QtAddAccountUtils.h"

#include <model/profile/UserProfile.h>
#include <model/contactlist/Contact.h>
#include <model/contactlist/ContactList.h>

#include <QtCore/QCoreApplication>

namespace QtAddAccountUtils {

static void addContact(UserProfile* userProfile, const std::string& groupId, const std::string& name, const std::string& sipAddress) {
	Contact& contact = userProfile->getContactList().createContact();
	contact.setGroupId(groupId);
	contact.setFirstName(name);

	IMContact imContact(EnumIMProtocol::IMProtocolSIP, sipAddress);
	contact.addIMContact(imContact);
}

void addTestContacts(UserProfile* userProfile, const std::string& audioTestCallAddress, const std::string& videoTestCallAddress) {
	if (audioTestCallAddress.empty() && videoTestCallAddress.empty()) {
		return;
	}

	std::string groupName = QCoreApplication::translate("QtAddAccountUtils", "Test").toStdString();
	userProfile->getContactList().addContactGroup(groupName);
	std::string groupId = userProfile->getContactList().getContactGroupIdFromName(groupName);

	if (!audioTestCallAddress.empty()) {
		std::string name = QCoreApplication::translate("QtAddAccountUtils", "Audio Test Call").toStdString();
		addContact(userProfile, groupId, name, audioTestCallAddress);
	}

	if (!videoTestCallAddress.empty()) {
		std::string name = QCoreApplication::translate("QtAddAccountUtils", "Video Test Call").toStdString();
		addContact(userProfile, groupId, name, videoTestCallAddress);
	}
}

} // namespace
