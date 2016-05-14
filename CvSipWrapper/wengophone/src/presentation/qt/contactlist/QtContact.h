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

#ifndef OWQTCONTACT_H
#define OWQTCONTACT_H

#include <model/contactlist/ContactProfile.h>

#include "QtContactPixmap.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtGui/QPixmap>

class QPainter;
class CWengoPhone;
class QModelIndex;
class QStyleOptionViewItem;
class QPainter;
class QRect;

/**
 * Qt Presentation component for Contact
 *
 * @author Mr K
 */
class QtContact : QObject {
	Q_OBJECT
public:

	enum SizeHint { UserSize = 22 };

	QtContact(const ContactProfile * contactProfile, CWengoPhone & cWengoPhone, QObject * parent);
	QtContact(const std::string & contactId, CWengoPhone & wengoPhone, QObject * parent);

	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option);
	void paintForeground(QPainter * painter, const QStyleOptionViewItem & option);

	QString getId() const;

	QString getUserName() const;

	QString getGroupName() const;

	bool hasIM() const;

	bool hasCall() const;

	bool hasFreeCall() const;

	bool hasVideo() const;

	bool hasFileTransfer() const;

	void setMouseOn(bool value) { _mouseOn = value; }

	/**
	 * Returns the pixmap to draw for this contact.
	 */
	QtContactPixmap::ContactPixmap getStatus() const;

	void setButton(const Qt::MouseButton button);

	void setOpenStatus(bool value);

	bool isOpen() const {
		return _openStatus;
	}

	int getHeight() const;

	Qt::MouseButton getButton() const;

	void setAvatar(QString path) { _avatarPath = path; }

	QString getAvatar() const { return _avatarPath; }

	void startChat();

	void startSMS();

	void startCall(const QString & number);

	void startCall();

	void startFreeCall();

	void sendFile();

	QString getMobilePhone() const;

	QString getHomePhone() const;

	QString getWorkPhone() const;

	QString getWengoPhoneNumber() const;

	QString getPreferredNumber() const;

	QString getWebsite() const;

	bool hasPhoneNumber() const;

	QString getDisplayName() const;

	EnumPresenceState::PresenceState getPresenceState() const;

public Q_SLOTS:

	void contactUpdated();

Q_SIGNALS:

	void clicked(QtContact * qtContact, int prt);

private:
	bool isTestCall() const;

	QString _userName;

	QPixmap _imPixmap;

	QPixmap _callPixmap;

	QPixmap _videoPixmap;

	QtContactPixmap::ContactPixmap _status;

	bool _mouseOn;

	bool _openStatus;

	Qt::MouseButton _mouseButton;

	QString _avatarPath;

	/**
	 * The UUID of the Contact associated with this QtContact.
	 */
	std::string _contactId;

	ContactProfile _contactProfile;

	CWengoPhone & _cWengoPhone;

	QMutex _mutex;
};

#endif	//OWQTCONTACT
