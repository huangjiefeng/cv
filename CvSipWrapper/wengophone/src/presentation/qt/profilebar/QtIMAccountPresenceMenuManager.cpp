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

#include "QtIMAccountPresenceMenuManager.h"

#include <control/profile/CUserProfile.h>

#include <model/connect/ConnectHandler.h>
#include <model/profile/UserProfile.h>

#include <qtutil/SafeConnect.h>

#include <memory>

#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>

QtIMAccountPresenceMenuManager::QtIMAccountPresenceMenuManager(QObject* parent, CUserProfile& cUserProfile, std::string imAccountId)
	: QObject(parent)
	, _cUserProfile(cUserProfile)
	, _imAccountId(imAccountId)
{
	_disconnectAction = new QAction(this);
	_disconnectAction->setText(tr("Disconnect"));
	SAFE_CONNECT(_disconnectAction, SIGNAL(triggered(bool)), SLOT(disconnect()) );
}

void QtIMAccountPresenceMenuManager::addPresenceActions(QMenu* menu) {
	addPresenceAction(
		menu, EnumPresenceState::PresenceStateOnline,
		QPixmap(":/pics/status/online.png"), tr("Online"));

	addPresenceAction(
		menu, EnumPresenceState::PresenceStateAway,
		QPixmap(":/pics/status/away.png"), tr("Away"));

	addPresenceAction(
		menu, EnumPresenceState::PresenceStateDoNotDisturb,
		QPixmap(":/pics/status/donotdisturb.png"), tr("Do Not Disturb"));

	addPresenceAction(
		menu, EnumPresenceState::PresenceStateInvisible,
		QPixmap(":/pics/status/invisible.png"), tr("Invisible"));

	menu->addAction(_disconnectAction);

	// We don't want the user to delete his Wengo or SIP account, so add "delete
	// account entry" for all protocols, except Wengo or SIP
	std::auto_ptr<IMAccount> imAccount ( _cUserProfile.getUserProfile().getIMAccountManager().getIMAccount(_imAccountId) );
	EnumIMProtocol::IMProtocol protocol = imAccount->getProtocol();
	if (protocol != EnumIMProtocol::IMProtocolWengo && protocol != EnumIMProtocol::IMProtocolSIP) {
		menu->addSeparator();
		menu->addAction(tr("Delete Account"), this, SLOT(deleteAccount()) );
	}

	SAFE_CONNECT(menu, SIGNAL(aboutToShow()), SLOT(updateActions()) );
}

void QtIMAccountPresenceMenuManager::updateActions() {
	std::auto_ptr<IMAccount> imAccount ( _cUserProfile.getUserProfile().getIMAccountManager().getIMAccount(_imAccountId) );
	_disconnectAction->setEnabled(imAccount->isConnected());
}

void QtIMAccountPresenceMenuManager::addPresenceAction(QMenu* menu, EnumPresenceState::PresenceState state, const QPixmap& pix, const QString& text) {
	QAction* action = menu->addAction(pix, text, this, SLOT(slotPresenceActionTriggered()) );
	_actionToState[action] = state;
}

void QtIMAccountPresenceMenuManager::slotPresenceActionTriggered() {
	EnumPresenceState::PresenceState state = _actionToState[sender()];
	_cUserProfile.getUserProfile().setPresenceState(state, _imAccountId);
}

void QtIMAccountPresenceMenuManager::disconnect() {
	_cUserProfile.getUserProfile().getConnectHandler().disconnect(_imAccountId);
}

void QtIMAccountPresenceMenuManager::deleteAccount() {
	std::auto_ptr<IMAccount> imAccount ( _cUserProfile.getUserProfile().getIMAccountManager().getIMAccount(_imAccountId) );
	QString login = QString::fromStdString(imAccount->getLogin());
	QString message =
		tr("Are sure you want to delete the account <b>%1</b>?")
		.arg(login);

	int result = QMessageBox::question(QApplication::activeWindow(),
				tr("@product@"), message,
				tr("&Delete"), tr("Cancel"));

	if (result == 0) {
		_cUserProfile.getUserProfile().removeIMAccount(*imAccount);
	}
}
