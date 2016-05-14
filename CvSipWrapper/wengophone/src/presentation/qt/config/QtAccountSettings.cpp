#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
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

#include "QtAccountSettings.h"

#include "ui_AccountSettings.h"

//#include <presentation/qt/imaccount/QtIMAccountManager.h>		¾«¼ò zhenHua.sun 2010-09-03
#include <presentation/qt/QtToolBar.h>
#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/profile/QtProfileDetails.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/WengoPhone.h>
#include <model/profile/UserProfile.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

QtAccountSettings::QtAccountSettings(CWengoPhone & cWengoPhone, QWidget * parent)
	: QWidget(parent) {

	_ui = new Ui::AccountSettings();
	_ui->setupUi(this);

	// Edit profile button
	QtWengoPhone *qtWengoPhone = cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28
	QtToolBar *toolbar = &qtWengoPhone->getQtToolBar();
	SAFE_CONNECT_RECEIVER(_ui->editProfileButton, SIGNAL(clicked()), 
		toolbar, SLOT(editMyProfile()));

	/* ¾«¼ò zhenHua.sun 2010-09-03
	// IM Account list
	QtIMAccountManager * imAccountManager = new QtIMAccountManager(cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile(),
		false, NULL);

	QWidget* widget = imAccountManager->getWidget();
	widget->setParent(_ui->imAccountFrame);
	QVBoxLayout* layout = new QVBoxLayout(_ui->imAccountFrame);
	layout->setMargin(0);
	layout->addWidget(widget);
	*/
}

QtAccountSettings::~QtAccountSettings() {
	delete _ui;
}

QString QtAccountSettings::getName() const {
	return tr("Accounts");
}

QString QtAccountSettings::getTitle() const {
	return tr("Accounts Settings");
}

QString QtAccountSettings::getIconName() const {
	return "imaccounts";
}
