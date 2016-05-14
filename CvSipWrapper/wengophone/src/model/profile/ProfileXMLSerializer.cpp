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

#include "ProfileXMLSerializer.h"

#include "Profile.h"

#include <model/profile/StreetAddressXMLSerializer.h>
#include <model/profile/StreetAddress.h>

#include <serializer/DateXMLSerializer.h>

#include <util/Base64.h>

#include <tinyxml.h>

using namespace std;

ProfileXMLSerializer::ProfileXMLSerializer(Profile & profile)
	: _profile(profile) {
}

string ProfileXMLSerializer::serialize() {
	string result;

	// Serializing Wengo ID 
	if (!_profile._wengoPhoneId.empty()) { 
		result += ("<wengoid>" + _profile._wengoPhoneId + "</wengoid>\n"); 
	} 
	//// 

	// Serializing names
	result += "<name>\n";
	if (!_profile._firstName.empty()) {
		result += ("<first><![CDATA[" + _profile._firstName + "]]></first>\n");
	}
	if (!_profile._lastName.empty()) {
		result += ("<last><![CDATA[" + _profile._lastName + "]]></last>\n");
	}
	result += "</name>\n";
	////

	//Serializing sex
	if (_profile._sex != EnumSex::SexUnknown) {
		result += ("<sex>" + EnumSex::toString(_profile._sex) + "</sex>\n");
	}
	////

	// Serializing alias
	if (!_profile._alias.empty()) {
		result += ("<alias><![CDATA[" + _profile._alias + "]]></alias>");
	}
	////

	// Serializing urls
	if (!_profile._website.empty()) {
		//result += ("<url type=\"website\">" + _profile._website + "</url>\n");
		result += ("<url type=\"website\"><![CDATA[" + _profile._website + "]]></url>\n");
	}
	////

	// Serializing birthday
	result += "<birthday>\n";

	DateXMLSerializer serializer(_profile._birthdate);
	result += serializer.serialize();

	result += "</birthday>\n";
	////

	// Serializing organization
	/*
	result += "<organization>\n";

	result += "</organization>\n";
	*/
	////

	// Serializing phone numbers
	if (!_profile._mobilePhone.empty()) {
		result += ("<tel type=\"cell\">" + _profile._mobilePhone + "</tel>\n");
	}

	if (!_profile._workPhone.empty()) {
		result += ("<tel type=\"work\">" + _profile._workPhone + "</tel>\n");
	}

	if (!_profile._homePhone.empty()) {
		result += ("<tel type=\"home\">" + _profile._homePhone + "</tel>\n");
	}

	if (!_profile._wengoPhoneNumber.empty()) {
		result += ("<tel type=\"wengo\">" + _profile._wengoPhoneNumber + "</tel>");
	}
	////

	// Serializing street address
	StreetAddressXMLSerializer streetSerializer(_profile._streetAddress);
	result += streetSerializer.serialize();
	////

	// Serializing emails
	if (!_profile._personalEmail.empty()) {
		result += ("<email type=\"home\">" + _profile._personalEmail + "</email>\n");
	}

	if (!_profile._workEmail.empty()) {
		result += ("<email type=\"work\">" + _profile._workEmail + "</email>\n");
	}

	if (!_profile._otherEmail.empty()) {
		result += ("<email type=\"other\">" + _profile._otherEmail + "</email>\n");
	}
	////

	// Serializing SMS signature
	if (!_profile._smsSignature.empty()) {
		result += ("<smssignature type=\"other\">" + _profile._smsSignature + "</smssignature>\n");
	}
	////

	// Serializing photo
	if (!_profile._icon.getData().empty()) {
		result += ("<photo filename=\"" + _profile._icon.getFilename()
			+ "\"><![CDATA[" + Base64::encode(_profile._icon.getData()) + "]]></photo>");
	}
	////

	// Serializing notes
	if (!_profile._notes.empty()) {
		result += ("<notes><![CDATA[" + _profile._notes + "]]></notes>"); 
	}
	////

	return result;
}

bool ProfileXMLSerializer::unserialize(const std::string & data) {
	return false;
}

bool ProfileXMLSerializer::unserializeContent(TiXmlHandle & rootElt) {
	// Retrieving Wengo ID 
	TiXmlNode * wengoPhoneId = rootElt.FirstChild("wengoid").FirstChild().Node(); 
	if (wengoPhoneId) { 
		// Here we need to call setWengoPhoneId because each class inherited
		// from Profile handles the wengophone ID differently.
		_profile.setWengoPhoneId(wengoPhoneId->Value());
	}
	//// 

	// Retrieving names
	TiXmlNode * firstName = rootElt.FirstChild("name").FirstChild("first").FirstChild().Node();
	if (firstName) {
		_profile._firstName = firstName->Value();
	}
	TiXmlNode * lastName = rootElt.FirstChild("name").FirstChild("last").FirstChild().Node();
	if (lastName) {
		_profile._lastName = lastName->Value();
	}
	////

	// Retrieving alias
	TiXmlNode * alias = rootElt.FirstChild("alias").FirstChild().Node();
	if (alias) {
		_profile._alias = alias->Value();
	}
	////

	// Retrieving SMS Signature
	TiXmlNode * smsSignature = rootElt.FirstChild("smssignature").FirstChild().Node();
	if (smsSignature) {
		_profile._smsSignature = smsSignature->Value();
	}
	////

	// Retrieving sex
	TiXmlNode * sex = rootElt.FirstChild("sex").FirstChild().Node();
	if (sex) {
		_profile._sex = EnumSex::toSex(sex->Value());
	}
	////

	// Retrieving URLs
	if (rootElt.Node()) {
		TiXmlNode * url = NULL;
		while ((url = rootElt.Node()->IterateChildren("url", url))) {
			TiXmlElement * urlElt = url->ToElement();
			string typeAttr = string(urlElt->Attribute("type"));
			if (typeAttr == "website") {
				TiXmlNode * website = url->FirstChild();
				if (website) {
					_profile._website = website->Value();
				}
			}
		}
	}
	////

	// Retrieving birthday
	TiXmlNode * birthday = rootElt.FirstChild("birthday").FirstChild("date").Node();
	if (birthday) {
		TiXmlElement * birthdayElt = birthday->ToElement();
		string birthdayData;
		birthdayData << *birthdayElt;
		Date date;
		DateXMLSerializer dateSerializer(date);
		dateSerializer.unserialize(birthdayData);
		_profile._birthdate = date;
	}
	/////

	// Retrieving organization
	////

	// Retrieving address
	TiXmlNode * address = rootElt.FirstChild("address").Node();
	if (address) {
		TiXmlElement * addressElt = address->ToElement();
		string addressData;
		addressData << *addressElt;
		StreetAddress streetAddress;
		StreetAddressXMLSerializer addressSerializer(streetAddress);
		addressSerializer.unserialize(addressData);
		_profile._streetAddress = streetAddress;
	}
	////

	// Retrieving phone numbers
	TiXmlNode * tel = NULL;
	while ((rootElt.Node()) && (tel = rootElt.Node()->IterateChildren("tel", tel))) {
		TiXmlElement * telElt = tel->ToElement();
		string typeAttr = string(telElt->Attribute("type"));
		if (typeAttr == "home") {
			TiXmlNode * homePhone = tel->FirstChild();
			if (homePhone) {
				_profile._homePhone = homePhone->Value();
			}
		} else if (typeAttr == "work") {
			TiXmlNode * workPhone = tel->FirstChild();
			if (workPhone) {
				_profile._workPhone = workPhone->Value();
			}
		} else if (typeAttr == "cell") {
			TiXmlNode * mobilePhone = tel->FirstChild();
			if (mobilePhone) {
				_profile._mobilePhone = mobilePhone->Value();
			}
		} else if (typeAttr == "wengo") {
			TiXmlNode * wengoPhoneNumber = tel->FirstChild();
			if (wengoPhoneNumber) {
				_profile._wengoPhoneNumber = wengoPhoneNumber->Value();
			}
		}
	}
	////

	// Retrieving emails
	TiXmlNode * email = NULL;
	while ((rootElt.Node()) && (email = rootElt.Node()->IterateChildren("email", email))) {
		TiXmlElement * emailElt = email->ToElement();
		string typeAttr = string(emailElt->Attribute("type"));
		if (typeAttr == "home") {
			TiXmlNode * personalEmail = email->FirstChild();
			if (personalEmail) {
				_profile._personalEmail = personalEmail->Value();
			}
		} else if (typeAttr == "work") {
			TiXmlNode * workEmail = email->FirstChild();
			if (workEmail) {
				_profile._workEmail = workEmail->Value();
			}
		} else if (typeAttr == "other") {
			TiXmlNode * otherEmail = email->FirstChild();
			if (otherEmail) {
				_profile._otherEmail = otherEmail->Value();
			}
		}
	}
	////

	// Retrieving icon
	TiXmlNode * photo = rootElt.FirstChild("photo").Node();
	if (photo) {
		TiXmlElement * photoElt = photo->ToElement();
		string filename;
		const char * filenameAttr = photoElt->Attribute("filename");
		if (filenameAttr) {
			filename = string(filenameAttr);
		}

		TiXmlNode * photoNode = photo->FirstChild();
		if (photoNode) {
			OWPicture picture = OWPicture::pictureFromData(Base64::decode(photoNode->Value()));
			picture.setFilename(filename);
			_profile._icon = picture;
		}
	}
	////

	// Retrieving notes
	TiXmlNode * notes = rootElt.FirstChild("notes").FirstChild().Node();
	if (notes) {
		_profile._notes = notes->Value();
	}
	////

	return true;
}
