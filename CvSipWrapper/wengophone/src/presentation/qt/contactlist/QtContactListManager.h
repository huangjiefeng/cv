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

#ifndef OWCONTACTLISTMANAGER_H
#define OWCONTACTLISTMANAGER_H

#include "QtContact.h"
#include "QtContactPixmap.h"

#include <QtCore/QString>

class CContact;
class QMutex;
class QTreeWidget;

/**
 * QtContact storage class for fast lookup
 *
 * @author Mr K.
 */
class QtContactListManager {
public:

	static QtContactListManager * getInstance();

	~QtContactListManager();

	void addContact(QtContact * qtContact);

	void removeContact(QtContact * qtContact);

	void paintContact(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index);

	void mouseOn(const QString & contactId);

	bool contains(const QString & contactId) const;

	void contactUpdated(const QString & contactId);

	bool hasIM(const QString & contactId) const;

	bool hasCall(const QString & contactId) const;

	bool hasVideo(const QString & contactId) const;

	bool hasPhoneNumber(const QString & contactId) const;

	bool hasFileTransfer(const QString & contactId) const;

	QtContact * getContact(const QString & contactId) const;

	QString getPreferredNumber(const QString & contactId) const;

	QString getMobilePhone(const QString & contactId) const;

	QString getHomePhone(const QString & contactId) const;

	QString getWorkPhone(const QString & contactId) const;

	QString getWebsite(const QString & contactId) const;

	void setButton(const QString & contactId, const Qt::MouseButton button);

	void setOpenStatus(const QString & contactId, bool value);

	Qt::MouseButton getButton(const QString & contactId) const;

	int getHeight(const QString & contactId) const;

	QString getWengoPhoneNumber(const QString & contactId) const;

	QTreeWidget * getTreeWidget() const { return _tree; }

	void setTreeWidget(QTreeWidget * tree) { _tree = tree; }

	void resetMouseStatus();

	void startChat(const QString & contactId);

	void startSMS(const QString & contactId);

	void startCall(const QString & contactId, const QString & number);

	void startCall(const QString & contactId);

	void sendFile(const QString & contactId);

	void clear();

	/**
	 * Will start a call with a free phone number.
	 */
	void startFreeCall(const QString & contactId);

private:

	QtContactListManager();

	QtContactListManager(const QtContactListManager & other)
		: _tree(other._tree),
		_contactList(other._contactList) { }

	QtContactListManager & operator=(const QtContactListManager & other) {
		_tree = other._tree;
		_contactList = other._contactList;
		return *this;
	}

	QTreeWidget * _tree;

	QHash <QString, QtContact *> _contactList;

	static QtContactListManager * _instance;

	QString _lastMouseOn;

	mutable QMutex * _mutex;
};

#endif	//OWCONTACTLISTMANAGER_H
