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

#include "QtContactListManager.h"

#include <util/Logger.h>

#include <QtGui/QtGui>

QtContactListManager * QtContactListManager::_instance = NULL;

QtContactListManager * QtContactListManager::getInstance() {
	if (!_instance) {
		_instance = new QtContactListManager;
	}
	return _instance;
}

QtContactListManager::QtContactListManager() {
	_mutex = new QMutex(QMutex::Recursive);
}

QtContactListManager::~QtContactListManager() {
	delete _mutex;
}

void QtContactListManager::paintContact(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) {
	QMutexLocker locker(_mutex);
	QtContact * qtContact;
	if (contains(index.data().toString())) {
		qtContact = _contactList[index.data().toString()];
		qtContact->paint(painter, option);
	}
}

void QtContactListManager::addContact(QtContact * qtContact) {
	QMutexLocker locker(_mutex);
	if (qtContact) {
		_contactList[qtContact->getId()] = qtContact;
	}
}

void QtContactListManager::removeContact(QtContact  * qtContact) {
	QMutexLocker locker(_mutex);
	if (qtContact) {
		_contactList.remove(qtContact->getId());
		delete qtContact;
	}
}

void QtContactListManager::mouseOn(const QString & contactId) {
	QMutexLocker locker(_mutex);
	QtContact * qtContact;
	if (contains(contactId)) {
		qtContact = _contactList[contactId];
		if (qtContact) {
			qtContact->setMouseOn(true);
		}
		qtContact = _contactList[_lastMouseOn];
		if (qtContact) {
			qtContact->setMouseOn(false);
		}
		_lastMouseOn = contactId;
	}
}

QtContact * QtContactListManager::getContact(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId];
	}
	return NULL;
}

QString QtContactListManager::getPreferredNumber(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getPreferredNumber();
	}
	return QString::null;
}

QString QtContactListManager::getMobilePhone(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getMobilePhone();
	}
	return QString::null;
}

QString QtContactListManager::getHomePhone(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getHomePhone();
	}
	return QString::null;
}

QString QtContactListManager::getWorkPhone(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getHomePhone();
	}
	return QString::null;
}

QString QtContactListManager::getWebsite(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getWebsite();
	}
	return QString::null;
}

bool QtContactListManager::contains(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	return _contactList.contains(contactId);
}

void QtContactListManager::contactUpdated(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		_contactList[contactId]->contactUpdated();
	}
}

QString QtContactListManager::getWengoPhoneNumber(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getWengoPhoneNumber();
	}
	return QString::null;
}

bool QtContactListManager::hasIM(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->hasIM();
	}
	return false;
}

bool QtContactListManager::hasFileTransfer(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->hasFileTransfer();
	}
	return false;
}

bool QtContactListManager::hasCall(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->hasCall();
	}
	return false;
}

bool QtContactListManager::hasVideo(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->hasVideo();
	}
	return false;
}

bool QtContactListManager::hasPhoneNumber(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->hasPhoneNumber();
	}
	return false;
}

void QtContactListManager::setButton(const QString & contactId, const Qt::MouseButton button) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		_contactList[contactId]->setButton(button);
	}
}

Qt::MouseButton QtContactListManager::getButton(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getButton();
	}
	return Qt::NoButton;
}

void QtContactListManager::setOpenStatus(const QString & contactId, bool value) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		_contactList[contactId]->setOpenStatus(value);
	}
}

int QtContactListManager::getHeight(const QString & contactId) const {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		return _contactList[contactId]->getHeight();
	}
	else {
		return 0;
	}
}

void QtContactListManager::resetMouseStatus() {
	QMutexLocker locker(_mutex);
	QHash <QString,QtContact *>::iterator it;
	for (it = _contactList.begin(); it != _contactList.end(); it++) {
		QtContact * qtContact = it.value();
		if (qtContact) {
			qtContact->setButton(Qt::NoButton);
		}
	}
}

void QtContactListManager::startChat(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->startChat();
		locker.relock();
	}
}

void QtContactListManager::startSMS(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->startSMS();
		locker.relock();
	}
}

void QtContactListManager::startCall(const QString & contactId, const QString & number) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->startCall(number);
		locker.relock();
	}
}

void QtContactListManager::startCall(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->startCall();
		locker.relock();
	}
}

void QtContactListManager::startFreeCall(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->startFreeCall();
		locker.relock();
	}
}

void QtContactListManager::sendFile(const QString & contactId) {
	QMutexLocker locker(_mutex);
	if (contains(contactId)) {
		locker.unlock();
		_contactList[contactId]->sendFile();
		locker.relock();
	}
}

void QtContactListManager::clear() {
	QMutexLocker locker(_mutex);
	QHash <QString, QtContact *>::iterator it;
	for (it = _contactList.begin(); it != _contactList.end(); it++) {
		delete((*it));
	}
	_contactList.clear();
}
