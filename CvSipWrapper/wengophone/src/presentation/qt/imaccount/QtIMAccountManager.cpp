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

#include "QtIMAccountManager.h"

#include "ui_IMAccountManager.h"

#include "QtIMAccountSettings.h"

#include <model/connect/ConnectHandler.h>
#include <model/profile/UserProfile.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

QtIMAccountManager::QtIMAccountManager(UserProfile & userProfile, bool showAsDialog, QWidget * parent)
	: QObject(parent),
	_userProfile(userProfile) {

	if (showAsDialog) {
		_imAccountManagerWidget = new QDialog(parent);
	} else {
		_imAccountManagerWidget = new QWidget(parent);
	}

	_ui = new Ui::IMAccountManager();
	_ui->setupUi(_imAccountManagerWidget);

	QMenu * addIMAccountMenu = new QMenu(_ui->addIMAccountButton);
	SAFE_CONNECT(addIMAccountMenu, SIGNAL(triggered(QAction *)), SLOT(addIMAccount(QAction *)));

	addIMAccountMenu->addAction(QIcon(":pics/protocols/msn.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolMSN));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/aim.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolAIM));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/icq.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolICQ));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/yahoo.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolYahoo));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/jabber.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolJabber));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/jabber.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolGoogleTalk));
	_ui->addIMAccountButton->setMenu(addIMAccountMenu);

	SAFE_CONNECT(_ui->modifyIMAccountButton, SIGNAL(clicked()), SLOT(modifyIMAccount()));

	SAFE_CONNECT(_ui->deleteIMAccountButton, SIGNAL(clicked()), SLOT(deleteIMAccount()));

	if (showAsDialog) {
		SAFE_CONNECT_RECEIVER(_ui->closeButton, SIGNAL(clicked()), _imAccountManagerWidget, SLOT(accept()));
	} else {
		_imAccountManagerWidget->layout()->setMargin(0);
		_imAccountManagerWidget->layout()->setSpacing(0);
		_ui->closeButton->hide();
	}

	SAFE_CONNECT(_ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));

	SAFE_CONNECT(_ui->treeWidget, SIGNAL(itemSelectionChanged()),
			SLOT(updateButtons()));

	loadIMAccounts();
	updateButtons();
	
	if (showAsDialog) {
		((QDialog *) _imAccountManagerWidget)->exec();
	}
}

QtIMAccountManager::~QtIMAccountManager() {
	OWSAFE_DELETE(_ui);
}

void QtIMAccountManager::loadIMAccounts() {
	_ui->treeWidget->clear();

	IMAccountList imAccountList = _userProfile.getIMAccountManager().getIMAccountListCopy();

	for (IMAccountList::iterator it = imAccountList.begin(); it != imAccountList.end(); it++) {
		IMAccount imAccount = (IMAccount &) (*it);
		QStringList accountStrList;
		
		// protocol
		EnumIMProtocol::IMProtocol imProtocol = imAccount.getProtocol();
		if ((imProtocol == EnumIMProtocol::IMProtocolWengo) ||
			(imProtocol == EnumIMProtocol::IMProtocolSIP)){
			//only IM account should be shown
			continue;
		}
		accountStrList << QString::fromStdString(EnumIMProtocol::toString(imProtocol));
		////
		
		// login
		accountStrList << QString::fromStdString(imAccount.getLogin());
		////
		accountStrList << QString::null;

		QTreeWidgetItem * item = new QTreeWidgetItem(_ui->treeWidget, accountStrList);
		QString id = QString::fromStdString(imAccount.getUUID());
		item->setData(0, Qt::UserRole, id);
	}
}

void QtIMAccountManager::addIMAccount(QAction * action) {
	QString protocolName = action->text();
	LOG_DEBUG(protocolName.toStdString());

	QtEnumIMProtocol::IMProtocol imProtocol = QtEnumIMProtocol::toIMProtocol(protocolName);
	QtIMAccountSettings * imAccountSettings = new QtIMAccountSettings(_userProfile, imProtocol, _imAccountManagerWidget);
	loadIMAccounts();
	OWSAFE_DELETE(imAccountSettings);
}

std::auto_ptr<IMAccount> QtIMAccountManager::getIMAccountFromItem(QTreeWidgetItem* item) {
	if (!item) {
		return std::auto_ptr<IMAccount>(0);
	}
	std::string id = item->data(0, Qt::UserRole).toString().toStdString();
	return std::auto_ptr<IMAccount>(_userProfile.getIMAccountManager().getIMAccount(id));
}

void QtIMAccountManager::deleteIMAccount() {
	QTreeWidgetItem* item = _ui->treeWidget->currentItem();
	std::auto_ptr<IMAccount> imAccount = getIMAccountFromItem(item);
	if (!imAccount.get()) {
		return;
	}
	int buttonClicked = 
		QMessageBox::question(_imAccountManagerWidget,
			tr("@product@"),
			tr("Are sure you want to delete this account?\n") +
			QString::fromStdString(imAccount->getLogin()),
			tr("&Delete"), tr("Cancel"));

	//Button delete clicked
	if (buttonClicked == 0) {
		_userProfile.removeIMAccount(*imAccount);
	}
	delete item;
}

void QtIMAccountManager::modifyIMAccount() {
	QTreeWidgetItem* item = _ui->treeWidget->currentItem();
	std::auto_ptr<IMAccount> imAccount = getIMAccountFromItem(item);
	if (!imAccount.get()) {
		return;
	}
	QtIMAccountSettings * imAccountSettings =
		new QtIMAccountSettings(_userProfile, *imAccount.get(), _imAccountManagerWidget);
	OWSAFE_DELETE(imAccountSettings);

	imAccount = getIMAccountFromItem(item);
	item->setText(1, QString::fromStdString(imAccount->getLogin()));
}

void QtIMAccountManager::itemDoubleClicked(QTreeWidgetItem * item, int column) {
	_ui->treeWidget->setCurrentItem(item);
	modifyIMAccount();
}

void QtIMAccountManager::updateButtons() {
	bool itemSelected = _ui->treeWidget->selectedItems().count() != 0;
	_ui->modifyIMAccountButton->setEnabled(itemSelected);
	_ui->deleteIMAccountButton->setEnabled(itemSelected);
}
