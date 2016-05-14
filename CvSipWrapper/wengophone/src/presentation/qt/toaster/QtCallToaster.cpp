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

#include "QtCallToaster.h"

#include "QtToaster.h"

#include "ui_QtCallToaster.h"

#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

QtCallToaster::QtCallToaster()
	: QObject(NULL) {

	_callToasterWidget = new QWidget(NULL);

	_ui = new Ui::CallToaster();
	_ui->setupUi(_callToasterWidget);

	_ui->hangUpButton->setPixmaps(QPixmap(":/pics/toaster/hangup.png"),
			QPixmap(),
			QPixmap(),
			QPixmap(":/pics/toaster/hangup.png"),
			QPixmap(),
			QPixmap());

	_ui->pickUpButton->setPixmaps(QPixmap(":/pics/toaster/pickup.png"),
			QPixmap(),
			QPixmap(),
			QPixmap(":/pics/toaster/pickup.png"),
			QPixmap(),
			QPixmap());

	_ui->pickUpButton->setMinimumSize(QSize(48, 56));
	_ui->pickUpButton->setMaximumSize(QSize(48, 56));
	SAFE_CONNECT(_ui->pickUpButton, SIGNAL(clicked()), SLOT(pickUpButtonSlot()));

	_ui->hangUpButton->setMinimumSize(QSize(28, 56));
	_ui->hangUpButton->setMaximumSize(QSize(28, 56));
	SAFE_CONNECT(_ui->hangUpButton, SIGNAL(clicked()), SLOT(hangUpButtonSlot()));

	SAFE_CONNECT(_ui->closeButton, SIGNAL(clicked()), SLOT(close()));

	_toaster = new QtToaster(_callToasterWidget, _ui->windowFrame);
	_toaster->setTimeOnTop(10000);
}

QtCallToaster::~QtCallToaster() {
	OWSAFE_DELETE(_ui);
}

void QtCallToaster::setMessage(const QString & message) {
	_ui->messageLabel->setText(message);
}

void QtCallToaster::setPixmap(const QPixmap & pixmap) {
	_ui->pixmapLabel->setPixmap(pixmap);
}

void QtCallToaster::show() {
	_toaster->show();
}

void QtCallToaster::close() {
	_toaster->close();
}

void QtCallToaster::hangUpButtonSlot() {
	hangUpButtonClicked();
	close();
}

void QtCallToaster::pickUpButtonSlot() {
	pickUpButtonClicked();
	close();
}
