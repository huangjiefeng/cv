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

#ifndef OWQTSYSTRAY_H
#define OWQTSYSTRAY_H

#include <sipwrapper/EnumPhoneLineState.h>

#include <util/Trackable.h>

#include <qtutil/QObjectThreadSafe.h>

#include <QtCore/QVariant>

class QtWengoPhone;
class TrayIcon;

class QMenu;
class QAction;
class QPoint;

/**
 * Systray class.
 *
 * @author Tanguy Krotoff
 * @author Mathieu Stute
 */
class QtSystray : public QObjectThreadSafe, public Trackable {
	Q_OBJECT
public:

	QtSystray(QObject * parent);

	~QtSystray();

	void phoneLineStateChanged(EnumPhoneLineState::PhoneLineState state);

public Q_SLOTS:

	void updateSystrayIcon();

	void setTrayMenu();

	void hide();

private Q_SLOTS:

	void toggleMainWindow();

	void makeCall(QAction * action);

	void startChat(QAction * action);

	void sendSms(QAction * action);

	void sendFile(QAction * action);

	/**
	 * Re-initializes the systray.
	 */
	void userProfileDeleted();

	void languageChanged();

private:

	void connectionIsUpEventHandler();

	void connectionIsDownEventHandler();

	void connectionStateEventHandlerThreadSafe(bool connected);

	QMenu * createStatusMenu();

	void updateMenu();

	void initThreadSafe() { }

	QtWengoPhone * _qtWengoPhone;

	QMenu * _trayMenu;

	QMenu * _callMenu;

	TrayIcon * _trayIcon;
};

#endif	//OWQTSYSTRAY_H
