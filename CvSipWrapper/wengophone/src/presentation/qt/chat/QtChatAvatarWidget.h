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

#ifndef OWQTCHATAVATARWIDGET_H
#define OWQTCHATAVATARWIDGET_H

#include "ui_AvatarWidget.h"

#include <QtGui/QWidget>

/**
 * Chat contact list.
 *
 * @author Mathieu Stute
 */
class QtChatAvatarWidget : public QWidget {
	Q_OBJECT
public:
#ifdef HUGE
#undef HUGE
#endif
	enum PictureMode {
		HUGE,			// 96x96
		BIG,			// 60x60
		MEDIUM,			// 48x48
		SMALL,			// 24x24
		TINY,			// 12x12
	};

	enum NicknameMode {
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
		NONE,
	};

	QtChatAvatarWidget(QWidget * parent, const QString & id, QPixmap picture,
		const QString & nickname, const QString & contactId, PictureMode pmode = SMALL, NicknameMode nmode = NONE);

	const QString getContactId() {
		return _contactId;
	}

	void setupPixmap(QPixmap pixmap);

	void setToolTip(const QString & nickname);

private:

	void setupNickname(const QString & nickname);

	PictureMode _pictureMode;

	NicknameMode _nicknameMode;

	QString _contactId;

	Ui::AvatarWidget _ui;
};

#endif	//OWQTCHATAVATARWIDGET_H
