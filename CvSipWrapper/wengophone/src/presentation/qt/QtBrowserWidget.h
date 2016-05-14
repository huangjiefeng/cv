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

#ifndef OWQTBROWSERWIDGET_H
#define OWQTBROWSERWIDGET_H

#include <QtCore/QObject>

#include <util/NonCopyable.h>

#include <string>

class QtBrowser;
class QtWengoPhone;
class QWidget;

/**
 * Embedded HTML browser widget inside the main window.
 *
 * Works only under Windows for the moment using a ActiveX control.
 *
 * @author Tanguy Krotoff
 * @author Mathieu Stute
 */
class QtBrowserWidget : public QObject, NonCopyable {
	Q_OBJECT
public:

	QtBrowserWidget(QtWengoPhone & qtWengoPhone);

	~QtBrowserWidget();

	QWidget * getWidget() const;

	/**
	 * Loads the default URL, loaded at startup.
	 */
	void loadDefaultURL();

	/**
	 * Loads the URL when the user is connected.
	 */
	void loadAccountURL();

private Q_SLOTS:

	/**
	 * delete the browser.
	 */
	void slotUserProfileDeleted();

private:

	QtWengoPhone & _qtWengoPhone;

	QtBrowser * _qtBrowser;
};

#endif	//OWQTBROWSERWIDGET_H
