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

#ifndef QTCONTACTWIDGET_H
#define QTCONTACTWIDGET_H

#include <model/contactlist/ContactProfile.h>

#include <QtGui/QWidget>
#include <QtCore/QString>

class CContact;
class CWengoPhone;
class QLabel;
class QPushButton;
class QResizeEvent;
class QtContactManager;
namespace Ui { class ContactWidget; }

/**
 * Qt user widget.
 * Represents a contact info in the contact list.
 *
 * @author Mr K
 * @author Mathieu Stute
 * @author Philippe Bernery
 */
class QtContactWidget : public QWidget {
	Q_OBJECT
public:

	QtContactWidget(const std::string & contactId, CWengoPhone & cWengoPhone,
		QtContactManager * qtContactManager, QWidget * parent);

	~QtContactWidget();

	void setText(const QString & text) {
		_text = text;
	}

	QString text() const {
		return _text;
	}

	void updateButtons();


	virtual void resizeEvent(QResizeEvent* event);

Q_SIGNALS:

	/**
	 * Emitted when a the contact icon has been clicked and so the
	 * user wants to edit the contact.
	 */
	void editContact(QString contact);

private Q_SLOTS:

	void callButtonClicked();

	void smsButtonClicked();

	void chatButtonClicked();

	void mobileButtonClicked();

	void landlineButtonClicked();

	void avatarButtonClicked();

	void sendFileButtonClicked();

private:

	void updateToolTips();

	void paintEvent(QPaintEvent *);

	void paintContact(QPainter * painter, const QRect & rect);

	std::string _contactId;

	CWengoPhone & _cWengoPhone;

	QString _text;

	Ui::ContactWidget * _ui;
};

#endif	//QTCONTACTWIDGET_H
