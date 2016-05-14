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

#include "StreetAddress.h"

using namespace std;

StreetAddress::StreetAddress() {
}

StreetAddress::StreetAddress(const StreetAddress & streetAddress) {
	initialize(streetAddress);
}

StreetAddress::~StreetAddress() {
}

StreetAddress & StreetAddress::operator = (const StreetAddress & streetAddress) {
	if (&streetAddress != this) {
		initialize(streetAddress);
	}

	return *this;
}

bool StreetAddress::operator == (const StreetAddress & streetAddress) const {
	return ((_street == streetAddress._street)
			&& (_stateProvince == streetAddress._stateProvince)
			&& (_city == streetAddress._city)
			&& (_zipCode == streetAddress._zipCode)
			&& (_country == streetAddress._country));
}

void StreetAddress::initialize(const StreetAddress & streetAddress) {
	_street = streetAddress._street;
	_stateProvince = streetAddress._stateProvince;
	_city = streetAddress._city;
	_zipCode = streetAddress._zipCode;
	_country = streetAddress._country;
}
