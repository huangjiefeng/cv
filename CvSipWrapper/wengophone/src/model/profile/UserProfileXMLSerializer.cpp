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

#include "UserProfileXMLSerializer.h"

#include <model/account/SipAccount.h>
#include <model/account/SipAccountXMLSerializer.h>
#include <model/account/wengo/WengoAccount.h>
#include <model/account/wengo/WengoAccountXMLSerializer.h>

#include "UserProfile.h"

#include <tinyxml.h>

UserProfileXMLSerializer::UserProfileXMLSerializer(UserProfile & userProfile) 
: ProfileXMLSerializer(userProfile), _userProfile(userProfile) {
}

std::string UserProfileXMLSerializer::serialize() {
	std::string result;

	result += "<userprofile>\n";

	result += ProfileXMLSerializer::serialize();

	if (_userProfile.hasWengoAccount()) {
		WengoAccountXMLSerializer wserializer(*(dynamic_cast<WengoAccount*>(_userProfile.getSipAccount())));
		result += wserializer.serialize();
	} else {
		//for now only a "generic" sip account
		SipAccountXMLSerializer sserializer(*_userProfile.getSipAccount());
		result += sserializer.serialize();
	}

	result += "</userprofile>\n";

	return result;
}

bool UserProfileXMLSerializer::unserialize(const std::string & data) {

	bool result = false;
	TiXmlDocument doc;

	doc.Parse(data.c_str());

	TiXmlHandle docHandle(&doc);
	TiXmlHandle userprofile = docHandle.FirstChild("userprofile");

	result = ProfileXMLSerializer::unserializeContent(userprofile);
	if (result) {
		// Retrieving wengoaccount
		TiXmlNode * wengoaccount = userprofile.FirstChild("wengoaccount").Node();
		if (wengoaccount) {
			TiXmlElement * wengoaccountElt = wengoaccount->ToElement();
			std::string wengoaccountData;
			wengoaccountData << *wengoaccountElt;
			WengoAccount wengoAccount;
			WengoAccountXMLSerializer wserializer(wengoAccount);
			result = wserializer.unserialize(wengoaccountData);
			//if (result && !wengoAccount.getWengoLogin().empty()) {
			if (result && !wengoAccount.isEmpty()) {
				_userProfile._sipAccount = new WengoAccount(wengoAccount);
				_userProfile.computeName();
				_userProfile._sipAccount->setUserProfileName(_userProfile.getName());
			}
			return result;
		}
		/////
		
		// Retrieving "generic" sipaccount
		TiXmlNode * sipaccount = userprofile.FirstChild("sipaccount").Node();
		if (sipaccount) {
			TiXmlElement * sipaccountElt = sipaccount->ToElement();
			std::string sipaccountData;
			sipaccountData << *sipaccountElt;
			SipAccount sipAccount;
			SipAccountXMLSerializer sserializer(sipAccount);
			result = sserializer.unserialize(sipaccountData);
			if (result && !sipAccount.isEmpty()) {
				_userProfile._sipAccount = new SipAccount(sipAccount);
				_userProfile.computeName();
				_userProfile._sipAccount->setUserProfileName(_userProfile.getName());
			}
			return result;
		}
		/////
	}

	return result;
}
