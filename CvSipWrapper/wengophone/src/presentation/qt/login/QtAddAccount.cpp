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

#include "QtAddAccount.h"
#include "QtLoginDialog.h"

#include "ui_AddAccount.h"

#include <control/profile/CUserProfileHandler.h>

#include <model/account/wengo/WengoAccount.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/webservices/url/WsUrl.h>

#include <qtutil/LanguageChangeEventFilter.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <QtGui/QtGui>

QtAddAccount::QtAddAccount(QtLoginDialog * qtLoginDialog, QWidget* parent, CUserProfileHandler & cUserProfileHandler)
	: ILogin(qtLoginDialog, cUserProfileHandler) {

	_ui = new Ui::AddAccount();
	_ui->setupUi(this);

	LANGUAGE_CHANGE(this);

	WidgetBackgroundImage::setBackgroundImage(_ui->loginLabel, ":pics/headers/login.png", WidgetBackgroundImage::AdjustHeight);

	SAFE_CONNECT(_ui->createNewAccountLabel, SIGNAL(linkActivated(const QString &)),
		SLOT(createAccountClicked(const QString &)));
	SAFE_CONNECT(_ui->nextButton, SIGNAL(clicked()), SLOT(nextClicked()));
	SAFE_CONNECT(_ui->backButton, SIGNAL(clicked()), SLOT(goBack()));
	SAFE_CONNECT_RECEIVER(_ui->cancelButton, SIGNAL(clicked()), _loginDialog, SLOT(reject()));
	SAFE_CONNECT(_ui->helpLabel, SIGNAL(linkActivated(const QString &)), SLOT(helpButtonClicked()));
}

QtAddAccount::~QtAddAccount() {
	OWSAFE_DELETE(_ui);
}

void QtAddAccount::createAccountClicked(const QString & /*url*/) {
	WsUrl::showWengoAccountCreation();
}

void QtAddAccount::helpButtonClicked() {
	WsUrl::showWikiPage();
}

void QtAddAccount::nextClicked() {
	if (_ui->newWengo->isChecked()) {
		_loginDialog->changePage(QtLoginDialog::AddWengoAccount);
	} else if (_ui->newSIP->isChecked()) {
		_loginDialog->changePage(QtLoginDialog::AddSIPAccount);
	}
	_loginDialog->currentPage()->setBackPage(QtLoginDialog::AddAccount);
}

void QtAddAccount::setInfoMessage() {
	setLoginLabel(tr("Please tell us if your account is provided<br/>by @company@ or by another service"));
}

void QtAddAccount::setErrorMessage(const QString & message) {
	setLoginLabel(QString("<font color=\"red\">%1</font>").arg(message));
}

void QtAddAccount::setLoginLabel(const QString & message) {
	QString loginLabel = QString("<span style=\"font-size:16pt;\">" + 
                                  tr("Configure your voip account") + "</span><br/>%1").arg(message);
	_ui->loginLabel->setText(loginLabel);
}

void QtAddAccount::languageChanged() {
	_ui->retranslateUi(this);
}

void QtAddAccount::load(std::string sipAccountName) {
	setInfoMessage();
}

void QtAddAccount::initPage() {
	_ui->nextButton->setDefault(true);
	setInfoMessage();
}

void QtAddAccount::keyPressEvent(QKeyEvent *event) {
     if (event->key()==Qt::Key_Return) {
		nextClicked();
     }
}
/**
* Add by: hexianfu
* for the teleController to change account
* 2009-11-11
*/
void QtAddAccount::teleCreateAccount()
{
	_ui->newSIP->setChecked(true);
	_ui->nextButton->click();
}
