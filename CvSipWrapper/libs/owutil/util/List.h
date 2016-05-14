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

#ifndef OWLIST_H
#define OWLIST_H

#include <vector>
#include <algorithm>

/**
 * List.
 *
 * @see java.util.List<E>
 * @see QList
 * @author Tanguy Krotoff
 */
template<typename T>
class List : public std::vector<T> {
public:

	/**
	 * Appends the specified element to the end of this list.
	 *
	 * @param element element to be appended to this list
	 */
	void operator+=(const T & element) {
		this->push_back(element);
	}

	/**
	 * Removes the first occurrence in this list of the specified element.
	 *
	 * Does not delete the element, just remove it from the list.
	 *
	 * @param element to remove from the list
	 * @return true if the element was removed; false otherwise
	 */
	bool remove(const T & element) {
		typename std::vector<T>::iterator it = std::find(this->begin(), this->end(), element);
		if (it != this->end()) {
			this->erase(it);
			return true;
		}
		return false;
	}

	/**
	 * @see remove()
	 */
	bool operator-=(const T & element) {
		return remove(element);
	}

	/**
	 * Gets the number of occurrences of an element contained in this list.
	 *
	 * @param element element to find inside this list
	 * @return number of occurrences of the specified element contained in this list
	 */
	unsigned contains(const T & element) const {
		unsigned j = 0;
		for (unsigned i = 0; i < this->size(); i++) {
			if ((*this)[i] == element) {
				j++;
			}
		}

		return j;
	}
};

#endif	//OWLIST_H
