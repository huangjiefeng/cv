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

#include "ContactGroupPopupMenu.h"
#include "QtRenameGroup.h"

#include <control/contactlist/CContactList.h>

#include <model/contactlist/ContactGroup.h>

#include <util/Logger.h>
#include <util/StringList.h>

#include <QtGui/QtGui>

#include <iostream>

using namespace std;

ContactGroupPopupMenu::ContactGroupPopupMenu(CContactList & cContactList, QWidget * parent)
	: QMenu(parent),
	_cContactList(cContactList) {

	addAction(tr("Remove Contact Group"), this, SLOT(removeContactGroup()));
	addAction(tr("Rename Contact Group"), this, SLOT(renameContactGroup()));
	//addAction(tr("Send SMS to group"), this, SLOT(sendSms()));
}

void ContactGroupPopupMenu::showMenu(const QString & groupId) {
	_groupId = groupId;
	exec(QCursor::pos());
}

void ContactGroupPopupMenu::removeContactGroup() const {
	if (QMessageBox::question(qobject_cast<QWidget *>(parent()),
		tr("Delete group"),
		tr("Do you really want to delete this group?"),
		tr("&Yes"),
		tr("&No"),
		QString(), 0, 1) == 0) {
			_cContactList.removeContactGroup(_groupId.toStdString());
		}
}

void ContactGroupPopupMenu::renameContactGroup() const {
	std::string tmp = _cContactList.getContactGroupName(_groupId.toStdString());
	QString groupName = QString::fromUtf8(tmp.c_str());
	QtRenameGroup dialog(groupName, qobject_cast<QWidget *>(parent()));
	if (dialog.exec()) {
		_cContactList.renameContactGroup(_groupId.toStdString(), dialog.getGroupName().toUtf8().data());
	}
}

void ContactGroupPopupMenu::sendSms() {
/*
	StringList list = _contactGroup->getMobilePhoneList();
	for(unsigned int i = 0; i < list.size(); i++) {
		LOG_DEBUG("mobile number: " + list[i] + "\n");
	}
*/
}
