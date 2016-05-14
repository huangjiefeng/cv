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

#ifndef OWQTCONTACTPIXMAP_H
#define OWQTCONTACTPIXMAP_H

#include <QtCore/QHash>
#include <QtGui/QPixmap>

/**
 * Store status pixmaps
 *
 * @author Mr K
 */
class QtContactPixmap {
public:

	enum ContactPixmap {
		ContactUnknown,
		ContactNoStatus,
		ContactOnline,
		ContactOffline,
		ContactDND,
		ContactInvisible,
		ContactAway,
		ContactGroupOpen,
		ContactGroupClose,
		ContactWDeal,
		ContactNoStatusSIP,
		ContactTestCall
	};

	static QtContactPixmap * getInstance();

	void setPixmap(ContactPixmap status, const QPixmap & px);

	QPixmap & getPixmap(ContactPixmap status);

private:

	QtContactPixmap();

	QtContactPixmap(const QtContactPixmap & other)
		: _contactPixmap(other._contactPixmap) { }

	QtContactPixmap & operator=(const QtContactPixmap & other) {
		_contactPixmap = other._contactPixmap;
		return * this;
	}

	QHash < ContactPixmap, QPixmap > _contactPixmap;

	static QtContactPixmap * _instance;
};

#endif	//OWQTCONTACTPIXMAP_H
