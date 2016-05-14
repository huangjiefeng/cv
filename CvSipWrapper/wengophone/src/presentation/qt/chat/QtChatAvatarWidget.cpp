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

#include <model/profile/AvatarList.h>

#include <util/String.h>
#include <util/Logger.h>

#include <QtGui/QPainter>

#include "QtChatAvatarWidget.h"

QtChatAvatarWidget::QtChatAvatarWidget(QWidget * parent, const QString & id,
	QPixmap picture, const QString & nickname, const QString & contactId, PictureMode pmode, NicknameMode nmode)
	: QWidget(parent), _pictureMode(pmode), _nicknameMode(nmode), _contactId(id) {

	_ui.setupUi(this);
	//setupPixmap(picture);
	setToolTip(nickname);
	//setupNickname(contactId);
	/*
	if (_nicknameMode != NONE) {
		setupNickname(nickname);
	} else {
		_ui.nicknameLabel->hide();
	}*/
}

void QtChatAvatarWidget::setupPixmap(QPixmap pixmap) {

	//TODO:: resize fond_avatar.png
	QPixmap background = QPixmap(":/pics/avatar_background.png");
	std::string data = AvatarList::getInstance().getDefaultAvatarPicture().getData();
	QPixmap defaultAvatar;
	defaultAvatar.loadFromData((uchar*) data.c_str(), data.size());
	QPainter painter(&background);

	if (!pixmap.isNull()) {
			switch (_pictureMode) {
			case HUGE:
				painter.drawPixmap(0, 0, pixmap.scaled(96, 96, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				_ui.pictureLabel->resize(96, 96);
				setMinimumSize(96, 96);
				break;
			case BIG:
				painter.drawPixmap(5, 5, pixmap.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				_ui.pictureLabel->resize(70, 70);
				setMinimumSize(70, 70);
				break;
			case MEDIUM:
				painter.drawPixmap(0, 0, pixmap.scaled(48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				_ui.pictureLabel->resize(48, 48);
				setMinimumSize(48, 48);
				break;
			case SMALL:
				painter.drawPixmap(0, 0, pixmap.scaled(24, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				_ui.pictureLabel->resize(24, 24);
				setMinimumSize(24, 24);
				break;
			case TINY:
				painter.drawPixmap(0, 0, pixmap.scaled(12, 12, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				_ui.pictureLabel->resize(12, 12);
				setMinimumSize(12, 12);
				break;
			default:
				LOG_WARN("unknown picture mode: " + String::fromNumber(_pictureMode));
		}
	} else {
		painter.drawPixmap(5, 5, defaultAvatar.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		_ui.pictureLabel->resize(70, 70);
		setMinimumSize(70, 70);
	}

	painter.end();
	_ui.pictureLabel->setPixmap(background);
}

void QtChatAvatarWidget::setupNickname(const QString & nickname) {
	//TODO: limit string length
	QFontMetrics fontMetrics(_ui.nicknameLabel->font());
	int width = 60;
	QString temp;
	for(int i = 0; i < nickname.length(); i++) {
		if (fontMetrics.width(temp) > width) {
			break;
		}
		temp += nickname[i];
	}
	_ui.nicknameLabel->setText(temp);
}

void QtChatAvatarWidget::setToolTip(const QString & nickname) {
	_ui.pictureLabel->setToolTip(nickname);
}
