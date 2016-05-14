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

#include "QtChatToaster.h"

#include "ui_QtChatToaster.h"

#include "QtToaster.h"

#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

QtChatToaster::QtChatToaster()
	: QObject(NULL) {

	_chatToasterWidget = new QWidget(NULL);

	_ui = new Ui::ChatToaster();
	_ui->setupUi(_chatToasterWidget);

	_ui->chatButton->setPixmaps(QPixmap(":/pics/toaster/chat.png"),
			QPixmap(),
			QPixmap(),
			QPixmap(":/pics/toaster/chat.png"),
			QPixmap(),
			QPixmap());

	SAFE_CONNECT(_ui->chatButton, SIGNAL(clicked()), SLOT(chatButtonSlot()));

	SAFE_CONNECT(_ui->closeButton, SIGNAL(clicked()), SLOT(close()));

	_toaster = new QtToaster(_chatToasterWidget, _ui->windowFrame);
	_toaster->setTimeOnTop(5000);
}

QtChatToaster::~QtChatToaster() {
	OWSAFE_DELETE(_ui);
}

void QtChatToaster::setMessage(const QString & message) {
	_ui->messageLabel->setText(message);
}

void QtChatToaster::setPixmap(const QPixmap & pixmap) {
	_ui->pixmapLabel->setPixmap(pixmap);
}

void QtChatToaster::show() {
	_toaster->show();
}

void QtChatToaster::close() {
	_toaster->close();
}

void QtChatToaster::chatButtonSlot() {
	chatButtonClicked();
	close();
}
