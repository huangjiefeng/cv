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

#ifndef OWQTIMPROFILEWIDGET_H
#define OWQTIMPROFILEWIDGET_H

#include <imwrapper/EnumIMProtocol.h>
#include <imwrapper/IMAccount.h>

#include <util/Trackable.h>

#include <QtCore/QObject>
#include <Windows.h>

class CWengoPhone;
class CUserProfile;
class QtIMAccountMonitor;

class QAction;
class QFrame;
class QHBoxLayout;
class QToolButton;
class QWidget;

namespace Ui { class IMProfileWidget; }

/**
 * Shows a summary of the user profile + its IM accounts.
 *
 * Inside the profile bar at the top of the main window.
 *
 * @author Tanguy Krotoff
 * @author Mathieu Stute
 */
class QtIMProfileWidget : public QObject, public Trackable {
	Q_OBJECT
public:
	QtIMProfileWidget(QWidget * parent);

	~QtIMProfileWidget();

	QWidget * getWidget() const;

	void init(CUserProfile*, QtIMAccountMonitor*);
	
	void resetAccountButton();

public Q_SLOTS:
	void addAccountButton(QString imAccountId);

	void removeAccountButton(QString imAccountId);

	void updateAccountButton(QString imAccountId);

	void reset();
	

private Q_SLOTS:

	void changeAvatarClicked();

	void aliasTextChanged();

	void languageChanged();

	void addIMAccount(QAction * action);

private:
	void addAccountButton(const IMAccount* imAccount);

	/**
	 * @see UserProfile::profileChangedEvent
	 */
	void profileChangedEventHandler();

	void updateAvatar();

	void createAccountButtons();

	/**
	 * Internal code factorization.
	 */
	void showImAccountManager();

	void updateWidgets();

	/**
	 * Show/hide text on the IM account button depending on the number of IM
	 * accounts
	 */
	void updateAddIMAccountButton();

	Ui::IMProfileWidget * _ui;
	QFrame * _accountFrame;
	QHBoxLayout * _accountFrameLayout;

	typedef std::map<QString, QToolButton*> AccountIdToButton;
	AccountIdToButton _accountIdToButton;

	QWidget * _imProfileWidget;

	CUserProfile * _cUserProfile;

	QtIMAccountMonitor* _qtImAccountMonitor;

public:
	HWND getAvatarButtonWnd();

};

#endif	//OWQTIMPROFILEWIDGET_H
