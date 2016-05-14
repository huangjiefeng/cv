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

#include "QtContactInfo.h"

#include "QtContactListManager.h"
#include "QtContact.h"

#include <QtGui/QtGui>

QtContactInfo::QtContactInfo(QTreeWidgetItem * item, QTreeWidgetItem * parentItem, const QString & contactId, int index) {
	_contactId = contactId;
	_item = item;
	_parentItem = parentItem;
	_index = index;
	_clear = false;
}

QtContactInfo::QtContactInfo(const QtContactInfo & other) {
	_contactId = other._contactId;
	_item = other._item;
	_parentItem = other._parentItem;
	_index = other._index;
	_clear = other._clear;
}

QtContactInfo::~QtContactInfo() {
}

void QtContactInfo::clear() {
	_clear = true;
}

bool QtContactInfo::isCleared() const {
	return _clear;
}

QTreeWidgetItem * QtContactInfo::getItem() const {
	return _item;
}

QTreeWidgetItem * QtContactInfo::getParentItem() const {
	return _parentItem;
}

QString QtContactInfo::getContactId() const {
	return _contactId;
}

int QtContactInfo::getIndex() const {
	return _index;
}

bool QtContactInfo::operator<(const QtContactInfo & other) const {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QtContact * qtContact = ul->getContact(_contactId);
	QtContact * otherQtContact = ul->getContact(other._contactId);
	if (qtContact && otherQtContact) {
		return qtContact->getUserName().toUpper() < otherQtContact->getUserName().toUpper();
	}
	return false;
}

QtContactInfo & QtContactInfo::operator=(const QtContactInfo & other) {
	_contactId = other._contactId;
	_item = other._item;
	_parentItem = other._parentItem;
	_index = other._index;
	_clear = other._clear;
	return *this;
}

QtContactPixmap::ContactPixmap QtContactInfo::getStatus() const {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QtContact * qtContact = ul->getContact(_contactId);
	return qtContact->getStatus();
}
