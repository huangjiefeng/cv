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

#include "QtContactMenu.h"

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/profile/UserProfile.h>
#include <model/contactlist/ContactProfile.h>

#include <util/Logger.h>

#include <QtGui/QtGui>

void QtContactMenu::populateMobilePhoneMenu(QMenu * menu, CWengoPhone & cWengoPhone) {

	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getCompleteName().c_str());

			if (!tmpContactProfile.getMobilePhone().empty()) {
				QString mobilePhone = QString::fromStdString(tmpContactProfile.getMobilePhone());
				//Call mobile action
				QAction * tmpAction;
				if (displayName == " ") {
					tmpAction = menu->addAction(mobilePhone);
				} else {
					tmpAction = menu->addAction(displayName + ": " + mobilePhone);
				}
				tmpAction->setData(QVariant(mobilePhone));
			}
		}
	}
}

void QtContactMenu::populateWengoCallMenu(QMenu * menu, CWengoPhone & cWengoPhone) {

	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getDisplayName().c_str());
			
			if (tmpContactProfile.hasAvailableWengoId() &&
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateOffline && 
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateUnknown && 
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateUnavailable) {

				QString freePhoneNumber = 
					QString::fromStdString(tmpContactProfile.getFirstAvailableWengoIMContact().getDisplayContactId());
				QAction * tmpAction;
				tmpAction = menu->addAction(displayName);
				tmpAction->setData(QVariant(freePhoneNumber));
				EnumPresenceState::PresenceState presenceState = tmpContactProfile.getPresenceState();
				setPresenceIcon(tmpAction, presenceState);
			}
		}
	}
}

void QtContactMenu::populateSipCallMenu(QMenu * menu, CWengoPhone & cWengoPhone) {
	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getDisplayName().c_str());
			if (tmpContactProfile.hasAvailableSIPNumber() ) {

				QString freePhoneNumber = QString::fromStdString(tmpContactProfile.getFirstFreePhoneNumber());
				QAction * tmpAction;
				tmpAction = menu->addAction(displayName);
				tmpAction->setData(QVariant(freePhoneNumber));
				EnumPresenceState::PresenceState presenceState = tmpContactProfile.getPresenceState();
				//setPresenceIcon(tmpAction, presenceState);
				
				if (presenceState == EnumPresenceState::PresenceStateUnknown) {
					setPresenceIcon(tmpAction, QIcon(":/pics/status/unknown-sip.png"));
				} else {
					setPresenceIcon(tmpAction, presenceState);
				}
			}
		}
	}
}

void QtContactMenu::populateHomePhoneMenu(QMenu * menu, CWengoPhone & cWengoPhone) {

	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getCompleteName().c_str());

			if (!tmpContactProfile.getHomePhone().empty()) {
				QString homePhone = QString::fromStdString(tmpContactProfile.getHomePhone());
				QAction * tmpAction;
				if (displayName == " ") {
					tmpAction = menu->addAction(homePhone);
				} else {
					tmpAction = menu->addAction(displayName + ": " + homePhone);
				}
				tmpAction->setData(QVariant(homePhone));
			}
		}
	}
}

void QtContactMenu::populateChatMenu(QMenu * menu, CWengoPhone & cWengoPhone) {

	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getDisplayName().c_str());

			if (tmpContactProfile.getPreferredIMContact() != NULL &&
				tmpContactProfile.hasIM()) {

				QAction * tmpAction = menu->addAction(displayName);
				tmpAction->setData(QVariant(QString::fromStdString(*it)));
				EnumPresenceState::PresenceState presenceState = tmpContactProfile.getPresenceState();
				setPresenceIcon(tmpAction, presenceState);
			}
		}
	}
}

void QtContactMenu::setPresenceIcon(QAction * action, QIcon icon) {
	action->setIcon(icon);
}
					
void QtContactMenu::setPresenceIcon(QAction * action, EnumPresenceState::PresenceState presenceState) {

	switch (presenceState) {
	case EnumPresenceState::PresenceStateOnline:
		action->setIcon(QIcon(":/pics/status/online.png"));
		break;
	case EnumPresenceState::PresenceStateOffline:
		action->setIcon(QIcon(":/pics/status/offline.png"));
		break;
	case EnumPresenceState::PresenceStateInvisible:
		action->setIcon(QIcon(":/pics/status/invisible.png"));
		break;
	case EnumPresenceState::PresenceStateAway:
		action->setIcon(QIcon(":/pics/status/away.png"));
		break;
	case EnumPresenceState::PresenceStateDoNotDisturb:
		action->setIcon(QIcon(":/pics/status/donotdisturb.png"));
		break;
	case EnumPresenceState::PresenceStateUnknown:
		action->setIcon(QIcon(":/pics/status/unknown.png"));
		break;
	case EnumPresenceState::PresenceStateUnavailable:
		action->setIcon(QIcon(":/pics/status/unknown-wengo.png"));
		break;
	default:
		LOG_FATAL("unknown presenceState=" + String::fromNumber(presenceState));
		break;
	}
}

void QtContactMenu::populateWengoUsersContactId(QMenu * menu, CWengoPhone & cWengoPhone) {

	CUserProfile * currentCUserProfile = cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (currentCUserProfile) {

		CContactList & currentCContactList = currentCUserProfile->getCContactList();
		StringList currentContactsIds = currentCContactList.getContactIds();

		for (StringList::const_iterator it = currentContactsIds.begin(); it != currentContactsIds.end(); ++it) {

			ContactProfile tmpContactProfile = currentCContactList.getContactProfile(*it);
			QString displayName = QString::fromUtf8(tmpContactProfile.getDisplayName().c_str());

			if (tmpContactProfile.hasAvailableWengoId() &&
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateOffline && 
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateUnknown && 
				tmpContactProfile.getPresenceState() != EnumPresenceState::PresenceStateUnavailable) {

				QAction * tmpAction = menu->addAction(displayName);
				tmpAction->setData(QVariant(QString::fromStdString(*it)));
				EnumPresenceState::PresenceState presenceState = tmpContactProfile.getPresenceState();
				setPresenceIcon(tmpAction, presenceState);
			}
		}
	}
}
