#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006 Wengo
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

#include "Profile.h"

#include <util/String.h>
#include <util/Logger.h>

Profile::Profile() {
	_sex = EnumSex::SexUnknown;
	_firstName = String::null;
	_lastName = String::null;
	_website = String::null;
	_company = String::null;
	_mobilePhone = String::null;
	_homePhone = String::null;
	_workPhone = String::null;
	_wengoPhoneNumber = String::null;
	_otherPhone = String::null;
	_fax = String::null;
	_personalEmail = String::null;
	_workEmail = String::null;
	_otherEmail = String::null;
	_notes = String::null;
	_alias = String::null;
	_wengoPhoneId = String::null;
	_smsSignature = String::null;
}

Profile::Profile(const Profile & profile)
	: Interface() {
	copy(profile);
}

void Profile::copy(const Profile & profile) {
	_firstName = profile._firstName;
	_lastName = profile._lastName;
	_sex = profile._sex;
	_birthdate = profile._birthdate;
	_website = profile._website;
	_company = profile._company;
	_mobilePhone = profile._mobilePhone;
	_homePhone = profile._homePhone;
	_workPhone = profile._workPhone;
	_wengoPhoneNumber = profile._wengoPhoneNumber;
	_otherPhone = profile._otherPhone;
	_fax = profile._fax;
	_personalEmail = profile._personalEmail;
	_workEmail = profile._workEmail;
	_otherEmail = profile._otherEmail;
	_streetAddress = profile._streetAddress;
	_notes = profile._notes;
	_wengoPhoneId = profile._wengoPhoneId;
	_alias = profile._alias;
	_icon = profile._icon;
	_smsSignature = profile._smsSignature;
}

bool Profile::operator==(const Profile & profile) const {
	return ((_firstName == profile._firstName)
		&& (_lastName == profile._lastName)
		&& (_sex == profile._sex)
		&& (_birthdate == profile._birthdate)
		&& (_website == profile._website)
		&& (_company == profile._company)
		&& (_mobilePhone == profile._mobilePhone)
		&& (_homePhone == profile._homePhone)
		&& (_workPhone == profile._workPhone)
		&& (_wengoPhoneNumber == profile._wengoPhoneNumber)
		&& (_otherPhone == profile._otherPhone)
		&& (_fax == profile._fax)
		&& (_personalEmail == profile._personalEmail)
		&& (_workEmail == profile._workEmail)
		&& (_otherEmail == profile._otherEmail)
		&& (_streetAddress == profile._streetAddress)
		&& (_notes == profile._notes)
		&& (_wengoPhoneId == profile._wengoPhoneId)
		&& (_smsSignature == profile._smsSignature));
}
