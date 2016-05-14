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

#ifndef QTIMACCOUNTPRESENCEMENUMANAGER_H
#define QTIMACCOUNTPRESENCEMENUMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>

#include <model/presence/Presence.h>
#include <imwrapper/IMAccount.h>

class CUserProfile;
class QAction;
class QIcon;
class QMenu;
class QPixmap;
class QString;


/**
 * This class manages the menu actions for an IM account
 *
 * @author Aurelien Gateau
 */
class QtIMAccountPresenceMenuManager : public QObject {
	Q_OBJECT
public:
	QtIMAccountPresenceMenuManager(QObject* parent, CUserProfile&, std::string imAccountId);

	/**
	 * Fills menu with actions to modify the IM account presence
	 */
	void addPresenceActions(QMenu* menu);

private Q_SLOTS:
	void slotPresenceActionTriggered();
	void disconnect();
	void deleteAccount();
	void updateActions();

private:
	void addPresenceAction(QMenu* menu, EnumPresenceState::PresenceState state, const QPixmap& pix, const QString& text);

	QMap<QObject*, EnumPresenceState::PresenceState> _actionToState;
	CUserProfile& _cUserProfile;
	std::string _imAccountId;
	QAction* _disconnectAction;
};

#endif	// QTIMACCOUNTPRESENCEMENUMANAGER_H
