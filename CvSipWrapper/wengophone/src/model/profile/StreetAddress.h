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

#ifndef STREETADDRESS_H
#define STREETADDRESS_H

#include <string>

/**
 * Class for the living address of a Contact.
 *
 * @ingroup model
 * @author Tanguy Krotoff
 */
class StreetAddress {
	friend class StreetAddressXMLSerializer;
public:

	StreetAddress();

	StreetAddress(const StreetAddress & streetAddress);

	~StreetAddress();

	StreetAddress & operator=(const StreetAddress & streetAddress);

	bool operator == (const StreetAddress & streetAddress) const;

	void setStreet(const std::string & street) { _street = street; }
	const std::string & getStreet() const { return _street; }

	void setStateProvince(const std::string & stateProvince) { _stateProvince = stateProvince; }
	const std::string & getStateProvince() const { return _stateProvince; }

	void setCity(const std::string & city) { _city = city; }
	const std::string & getCity() const { return _city; }

	void setZipCode(const std::string & zipCode) { _zipCode = zipCode; }
	const std::string & getZipCode() const { return _zipCode; }

	void setCountry(const std::string & country) { _country = country; }
	const std::string & getCountry() const { return _country; }

private:

	/** Factorizes code between contructor and copy contructor. */
	void initialize(const StreetAddress & streetAddress);

	std::string _street;
	std::string _stateProvince;
	std::string _city;
	std::string _zipCode;
	std::string _country;
};

#endif	//STREETADDRESS_H
