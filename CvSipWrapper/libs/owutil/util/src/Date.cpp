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

#include <util/Date.h>

#include <util/Logger.h>
#include <util/String.h>

Date::Date() {
	std::time_t curTime = time(NULL);
	struct std::tm * timeinfo = std::localtime(&curTime);
	setDay(timeinfo->tm_mday);
	setMonth(timeinfo->tm_mon + 1);
	setYear(timeinfo->tm_year + 1900);
}

Date::Date(const Date & date) {
	setDay(date._day);
	setMonth(date._month);
	setYear(date._year);
}

Date::Date(unsigned day, unsigned month, unsigned year) {
	setDay(day);
	setMonth(month);
	setYear(year);
}

Date::~Date() {
}

bool Date::operator==(const Date & date) const {
	return ((_day == date._day)
		&& (_month == date._month)
		&& (_year == date._year));
}

unsigned Date::getDay() const {
	return _day;
}

void Date::setDay(unsigned day) {
	if (day < 1) {
		LOG_FATAL("day cannot be < 1");
	} else if (day > 31) {
		LOG_FATAL("day cannot be > 31");
	}

	_day = day;
}

unsigned Date::getMonth() const {
	return _month;
}

void Date::setMonth(unsigned month) {
	if (month < 1) {
		LOG_FATAL("month cannot be < 1");
	} else if (month > 12) {
		LOG_FATAL("month cannot be > 12");
	}

	_month = month;
}

unsigned Date::getYear() const {
	return _year;
}

void Date::setYear(unsigned year) {
	_year = year;
}

std::string Date::toString() const {
	std::string month = String::fromNumber(_month);
	std::string day = String::fromNumber(_day);

	if (month.size() == 1) {
		month = "0" + month;
	}

	if (day.size() == 1) {
		day = "0" + day;
	}

	return String::fromNumber(_year) + "-" + month + "-" + day;
}
