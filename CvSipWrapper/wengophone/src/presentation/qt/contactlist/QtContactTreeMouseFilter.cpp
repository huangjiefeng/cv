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

#include "QtContactTreeMouseFilter.h"
#include "QtContactListManager.h"

#include <control/contactlist/CContactList.h>

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <util/Logger.h>

#include <QtCore/QEvent>
#include <QtCore/QMimeData>
#include <QtCore/QTimer>

#include <QtGui/QApplication>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QTreeWidget>

const QString QtContactTreeMouseFilter::WENGO_MIME_TYPE = "application/x-wengo-user-data";

QtContactTreeMouseFilter::QtContactTreeMouseFilter(CContactList & cContactList, QObject * parent, QTreeWidget * treeWidget)
	: QObject(parent),
	_cContactList(cContactList) {
	_tree = treeWidget;

	//The event filter must be installed in the viewport of the QTreeWidget
	_tree->viewport()->installEventFilter(this);
	_dropDestinationItem = 0;
}

bool QtContactTreeMouseFilter::eventFilter(QObject * obj, QEvent * event) {
	switch (event->type()) {
	case QEvent::MouseButtonPress:
		mousePressEvent((QMouseEvent *) event);
		return false;
	case QEvent::MouseButtonRelease:
		mouseReleaseEvent((QMouseEvent *) event);
		return false;
	case QEvent::MouseMove:
		mouseMoveEvent((QMouseEvent *) event);
		event->accept();
		return true;
	case QEvent::DragEnter:
		dragEnterEvent((QDragEnterEvent *) event);
		event->accept();
		return true;
	case QEvent::Drop:
		dropEvent((QDropEvent *) event);
		event->accept();
		return true;
	case QEvent::DragMove:
		dragMoveEvent((QDragMoveEvent *) event);
		event->accept();
		return true;
	default:
		return QObject::eventFilter(obj, event);
	}
}

void QtContactTreeMouseFilter::mousePressEvent(QMouseEvent * event) {

	mouseClicked(event->button());
	QTreeWidgetItem * item = _tree->itemAt(event->pos());
	QtContactListManager * ul = QtContactListManager::getInstance();
	if (item) {
		if (!item->parent()) {
			return;
		}
		ul->resetMouseStatus();
		_selectedContactId = item->text(0);
		ul->setButton(_selectedContactId, event->button());
		if (event->button() == Qt::LeftButton) {
			_dstart = event->pos();
		}
	}
}

void QtContactTreeMouseFilter::mouseReleaseEvent(QMouseEvent * event) {
	if (event->button() == Qt::RightButton) {
		mouseClicked(Qt::RightButton);
	} else {
		mouseClicked(Qt::NoButton);
	}
}

void QtContactTreeMouseFilter::mouseMoveEvent(QMouseEvent * event) {
	if (_selectedContactId.isNull()) {
		return;
	}

	if (!(event->buttons() & Qt::LeftButton)) {
		return;
	}

	if ((event->pos() - _dstart).manhattanLength() < QApplication::startDragDistance()) {
		return;
	}

	//Define a new empty custom data
	QByteArray custom;
	QDrag * drag = new QDrag(_tree);
	QMimeData * mimeData = new QMimeData;
	QtContactListManager * ul = QtContactListManager::getInstance();

	QtContact * contact = ul->getContact(_selectedContactId);
	if (contact) {
		mimeData->setText(contact->getUserName());
		mimeData->setData(WENGO_MIME_TYPE, custom);
		drag->setMimeData(mimeData);
		drag->start(Qt::MoveAction);
	}
}

void QtContactTreeMouseFilter::dragEnterEvent(QDragEnterEvent * event) {
	if (event->mimeData()->hasFormat(WENGO_MIME_TYPE)) {
		event->acceptProposedAction();
	}
}

void QtContactTreeMouseFilter::dropEvent(QDropEvent * event) {
	QTreeWidgetItem * item = _tree->itemAt(event->pos());

	if (!event->mimeData()->hasFormat(WENGO_MIME_TYPE)) {
		return;
	}
	event->acceptProposedAction();

	if (_selectedContactId.isNull()) {
		// No contact selected. Should not happen.
		LOG_WARN("No contact selected. This should not happen.");
		return;
	}

	if (item) {
		if (_selectedContactId == item->text(0)) {
			//Dropped on same item. Nothing to do.
			_selectedContactId = QString::null;
			return;
		}

		_dropSourceContactId = _selectedContactId;
		_dropDestinationItem = item;

		// Call handleDrop() through a QTimer::singleShot, so that we are out
		// of the event loop. This is necessary because code in handleDrop()
		// can show dialog messages, causing crashes under X11.
		QTimer::singleShot(0, this, SLOT(handleDrop()));
	}

	_selectedContactId = QString::null;
}

void QtContactTreeMouseFilter::handleDrop() {
	Q_ASSERT(_dropDestinationItem);
	QString destinationContactId = _dropDestinationItem->text(0);

	ContactProfile sourceProfile = _cContactList.getContactProfile(_dropSourceContactId.toStdString());
	ContactProfile destinationProfile = _cContactList.getContactProfile(destinationContactId.toStdString());

	if ( sourceProfile.getGroupId() == destinationProfile.getGroupId() || _tree->topLevelItemCount() == 1 ) {
		//The destination and the source groups are the same
		//This is a contact combination
		mergeContacts(_dropSourceContactId, destinationContactId);
	} else {
		//This is a Contact move
		QString groupId;
		if (_dropDestinationItem->parent()) {
			//The destination is a contact, move the contact to the destination
			//group
			groupId = _dropDestinationItem->parent()->text(0);
		} else {
			//The destination is a group
			groupId = destinationContactId;
		}
		sourceProfile.setGroupId(groupId.toStdString());
		_cContactList.updateContact(sourceProfile);
	}

	_dropDestinationItem = 0;
}

void QtContactTreeMouseFilter::dragMoveEvent(QDragMoveEvent * event) {
	QTreeWidgetItem * item = _tree->itemAt(event->pos());
	event->setDropAction(Qt::IgnoreAction);
	if (item) {
		event->setDropAction(Qt::MoveAction);
		if (event->mimeData()->hasFormat(WENGO_MIME_TYPE)) {
			event->acceptProposedAction();
		}
	}
}
