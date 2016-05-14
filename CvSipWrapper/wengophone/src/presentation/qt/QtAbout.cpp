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

#include "QtAbout.h"

#include "ui_AboutWindow.h"

#include <WengoPhoneBuildId.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <cutil/global.h>

#include <QtGui/QtGui>

QtAbout::QtAbout(QWidget * parent) {
	_aboutDialog = new QDialog(parent);

	_ui = new Ui::AboutWindow();
	_ui->setupUi(_aboutDialog);

	/* Set a formatted text to the build id label. */
	_ui->wengoPhoneBuildIdStringLabel->setText("<b>" +
					QString(QObject::tr("@product@")) + " " +
					QString(WengoPhoneBuildId::getVersion()) + " rev" +
					QString(WengoPhoneBuildId::getSvnRevision()) + "-" +
					QString::number(WengoPhoneBuildId::getBuildId()) + "<b>");

	QFile file(":/AUTHORS");
	if (file.open(QFile::ReadOnly)) {
		QString authors = file.readAll();
		file.close();
		_ui->authorsTextEdit->setPlainText(authors);
	} else {
		LOG_ERROR("couldn't locate file=" + file.fileName().toStdString());
	}

	/* Set a formatted text to dependencies version label. */
	_ui->versionLabel->setText("<i>Qt: </i>" + QString(qVersion()) + "<BR>" +
				"<i>Boost: </i>" + QString(WengoPhoneBuildId::getBoostVersion()) + "<BR>" +
				"<i>LibPurple: </i>" + QString(WengoPhoneBuildId::getPurpleVersion()) + "<BR>" +
				"<i>GLib: </i>" + QString(WengoPhoneBuildId::getGLibVersion()) + "<BR>" +
				"<i>cURL: </i>" + QString(WengoPhoneBuildId::getCurlVersion()) + "<BR>" +
				"<i>TinyXML: </i>" + QString(WengoPhoneBuildId::getTinyXmlVersion()) +  "<BR>" +
				"<i>FFmpeg's libavcodec: </i>" + QString(WengoPhoneBuildId::getAvcodecVersion())
#ifndef OS_MACOSX
				+ "<BR>" + "<i>PortAudio: </i>" + QString(WengoPhoneBuildId::getPortaudioVersion())
#endif
				);
}

QtAbout::~QtAbout() {
	OWSAFE_DELETE(_ui);
}
