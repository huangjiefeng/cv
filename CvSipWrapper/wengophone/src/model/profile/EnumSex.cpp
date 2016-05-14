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

#include "EnumSex.h"

#include <util/Logger.h>

using namespace std;

string EnumSex::toString(EnumSex::Sex sex) {
	string result;

	switch (sex) {
	case SexUnknown:
		result = "unknown";
		break;
	case SexFemale:
		result = "female";
		break;
	case SexMale:
		result = "male";
		break;
	default:
		LOG_FATAL("unknown case");
	}

	return result;
}

EnumSex::Sex EnumSex::toSex(const string & sex) {
	if (sex == "male") {
		return SexMale;
	} else if (sex == "female") {
		return SexFemale;
	} else {
		return SexUnknown;
	}
}
