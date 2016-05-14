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

#ifndef OWQTCHATAVATARFRAME_H
#define OWQTCHATAVATARFRAME_H

#include "ui_AvatarFrame.h"

#include <QtGui/QFrame>
#include <QtCore/QList>
#include <QtCore/QStringList>

class QVBoxLayout;
class QtChatAvatarWidget;

/**
 * Avatar frame widget.
 *
 * @author Mathieu Stute
 */
class QtChatAvatarFrame : public QFrame {
	Q_OBJECT
public:

	QtChatAvatarFrame(QWidget * parent);

	void setUserPixmap(QPixmap pixmap);

	void addRemoteContact(const QString & id, const QString & displayName, const QString & contactId, QPixmap avatar);

	void removeRemoteContact(const QString & contactId);

	void updateContact(const QString & id, QPixmap avatar, const QString & displayName);

	QStringList getContactIDList();

private:

	typedef QList<QtChatAvatarWidget*> WidgetList;

	WidgetList _widgetList;

	QVBoxLayout * _layout;

	Ui::AvatarFrame _ui;

	QStringList _contactIdList;
};

#endif	//OWQTCHATAVATARFRAME_H
