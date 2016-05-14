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

#include "QtIMAccountPlugin.h"

#include <model/connect/ConnectHandler.h>
#include <model/profile/UserProfile.h>

#include <util/SafeDelete.h>

#include <QtGui/QtGui>

QtIMAccountPlugin::QtIMAccountPlugin(UserProfile & userProfile, IMAccount & imAccount, QDialog * parent)
	: QObject(parent),
	_imAccount(imAccount),
	_userProfile(userProfile) {

	_parentDialog = parent;
}

void QtIMAccountPlugin::checkAndSave() {
	if (isValid()) {
		save();
		_parentDialog->accept();
	} else {
		QMessageBox::critical(_parentDialog, tr("@product@"),
			tr("Please enter a login"), QMessageBox::Ok, QMessageBox::NoButton);
	}
}

void QtIMAccountPlugin::save() {
	// Check if this an IMAccount update or an IMAccount creation
	bool update = !_imAccount.getLogin().empty();

	setIMAccount();

	if (update) {
		_userProfile.getConnectHandler().disconnect(_imAccount.getUUID());
		_userProfile.updateIMAccount(_imAccount);
		_userProfile.getConnectHandler().connect(_imAccount.getUUID());
	} else {
		_userProfile.addIMAccount(_imAccount);
		_userProfile.getConnectHandler().connect(_imAccount.getUUID());
	}
}
