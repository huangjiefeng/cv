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

#include "QtPresenceMenuManager.h"

#include <model/profile/UserProfile.h>

#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <presentation/qt/QtToolBar.h>
#include <presentation/qt/QtWengoPhone.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QMenu>


QtPresenceMenuManager::QtPresenceMenuManager(QObject* parent, CUserProfileHandler* cUserProfileHandler, QtWengoPhone * qtWengoPhone)
	: QObject(parent),
	_cUserProfileHandler(cUserProfileHandler),
	_qtWengoPhone(qtWengoPhone) {
}

void QtPresenceMenuManager::addPresenceActions(QMenu* menu) {
	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateOnline,
		QPixmap(":/pics/status/online.png"), tr("Online"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateAway,
		QPixmap(":/pics/status/away.png"), tr("Away"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateDoNotDisturb,
		QPixmap(":/pics/status/donotdisturb.png"), tr("Do Not Disturb"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateInvisible,
		QPixmap(":/pics/status/invisible.png"), tr("Invisible"));

	menu->addSeparator();
	QAction* logOffAction = menu->addAction(tr("Log off"), this, SLOT(logOff()) );

	bool connected = _cUserProfileHandler->getCUserProfile()->getUserProfile().isConnected();
	logOffAction->setEnabled(connected);
}

void QtPresenceMenuManager::addGlobalPresenceAction(QMenu* menu, EnumPresenceState::PresenceState state, const QPixmap& pix, const QString& text) {
	QAction* action = menu->addAction(pix, text, this, SLOT(slotGlobalPresenceActionTriggered()) );
	_actionToState[action] = state;
}

void QtPresenceMenuManager::slotGlobalPresenceActionTriggered() {
	EnumPresenceState::PresenceState state = _actionToState[sender()];
	_cUserProfileHandler->getCUserProfile()->getUserProfile().setAllPresenceState(state);
}

void QtPresenceMenuManager::logOff() {
	_qtWengoPhone->getQtToolBar().logOff();
}
