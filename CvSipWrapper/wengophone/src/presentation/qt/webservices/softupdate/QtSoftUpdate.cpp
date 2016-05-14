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

#include "QtSoftUpdate.h"

#include "ui_SoftUpdateWindow.h"

#include "QtBrowserSoftUpdate.h"

#include <presentation/qt/QtWengoPhone.h>

#include <control/CWengoPhone.h>
#include <control/webservices/softupdate/CSoftUpdate.h>

#include <softupdater/SoftUpdater.h>

#include <util/String.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <cutil/global.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

static const char * UPDATE_PROGRAM = "update.exe";

QtSoftUpdate::QtSoftUpdate(CSoftUpdate & cSoftUpdate)
	: QObjectThreadSafe(NULL),
	_cSoftUpdate(cSoftUpdate) {

	_softUpdater = NULL;
	_ui = NULL;

	_qtWengoPhone = QtSoftUpdate::_cSoftUpdate.getCWengoPhone()._qtWengoPhone;//Modify By LZY 2010-09-28

	updateWengoPhoneEvent += boost::bind(&QtSoftUpdate::updateWengoPhoneEventHandler, this, _1, _2, _3, _4);

	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtSoftUpdate::initThreadSafe, this));
	postEvent(event);
}

void QtSoftUpdate::initThreadSafe() {
}

QtSoftUpdate::~QtSoftUpdate() {
	OWSAFE_DELETE(_softUpdater);
	OWSAFE_DELETE(_ui);
}

void QtSoftUpdate::updateWengoPhoneEventHandler(const std::string & downloadUrl,
				unsigned long long buildId,
				const std::string & version,
				unsigned fileSize) {

	typedef PostEvent4<void (std::string, unsigned long long, std::string, unsigned),
					std::string, unsigned long long, std::string, unsigned> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtSoftUpdate::updateWengoPhoneEventHandlerThreadSafe,
					this, _1, _2, _3, _4), downloadUrl, buildId, version, fileSize);
	postEvent(event);
}

void QtSoftUpdate::updateWengoPhoneEventHandlerThreadSafe(const std::string & downloadUrl,
				unsigned long long buildId,
				const std::string & version,
				unsigned fileSize) {

#ifdef OS_WINDOWS
	_softUpdateWindow = new QDialog(_qtWengoPhone->getWidget());

	_ui = new Ui::SoftUpdateWindow();
	_ui->setupUi(_softUpdateWindow);

	SAFE_CONNECT(_softUpdateWindow, SIGNAL(rejected()), SLOT(abortDownload()));

	//updateTextLabel
	_originalLabelText = _ui->updateTextLabel->text()
				.arg(QString::fromStdString(version))
				.arg(QString::fromStdString(downloadUrl))
				.arg(fileSize);
	_ui->updateTextLabel->setText(_originalLabelText);

	//Deletes previous update program
	QFile file(UPDATE_PROGRAM);
	file.remove();
	file.close();

	_softUpdater = new SoftUpdater(downloadUrl, UPDATE_PROGRAM);
	_softUpdater->dataReadProgressEvent += boost::bind(&QtSoftUpdate::dataReadProgressEventHandler, this, _1, _2, _3);
	_softUpdater->downloadFinishedEvent += boost::bind(&QtSoftUpdate::downloadFinishedEventHandler, this, _1);
	_softUpdater->start();

	_softUpdateWindow->exec();
#else
	QtBrowserSoftUpdate(downloadUrl, buildId, version, fileSize, _qtWengoPhone->getWidget());
#endif
}

void QtSoftUpdate::dataReadProgressEventHandler(double bytesDone, double bytesTotal, unsigned downloadSpeed) {
	typedef PostEvent3<void (double, double, unsigned), double, double, unsigned> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtSoftUpdate::dataReadProgressEventHandlerThreadSafe, this, _1, _2, _3), bytesDone, bytesTotal, downloadSpeed);
	postEvent(event);
}

void QtSoftUpdate::dataReadProgressEventHandlerThreadSafe(double bytesDone, double bytesTotal, unsigned downloadSpeed) {
	LOG_DEBUG("progress=" + String::fromDouble(bytesDone));

	//progressBar
	_ui->progressBar->setRange(0, (int)bytesTotal);
	_ui->progressBar->setValue((int)bytesDone);

	_ui->updateTextLabel->setText(_originalLabelText.arg(downloadSpeed));

	QApplication::processEvents();
}

void QtSoftUpdate::downloadFinishedEventHandler(HttpRequest::Error error) {
	typedef PostEvent1<void (HttpRequest::Error), HttpRequest::Error> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtSoftUpdate::downloadFinishedEventHandlerThreadSafe, this, _1), error);
	postEvent(event);
}

void QtSoftUpdate::downloadFinishedEventHandlerThreadSafe(HttpRequest::Error error) {
	LOG_DEBUG("download finished");

	if (error == HttpRequest::NoError) {
		//Terminates the thread from the model
		_cSoftUpdate.getCWengoPhone().terminate();

		launchUpdateProcess();
	} else {
		disconnect(_softUpdateWindow, SIGNAL(rejected()), this, SLOT(abortDownload()));
		_softUpdateWindow->reject();
	}
}

void QtSoftUpdate::abortDownload() {
	_softUpdater->abort();
}

void QtSoftUpdate::launchUpdateProcess() {
	QProcess * updateProcess = new QProcess();
	SAFE_CONNECT(updateProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(updateProcessError(QProcess::ProcessError)));
	updateProcess->start(UPDATE_PROGRAM);
}

void QtSoftUpdate::updateProcessError(QProcess::ProcessError error) {
	QMessageBox::critical(_qtWengoPhone->getWidget(),
			tr("@product@ - Update failed"),
			tr("@product@ update failed to start: try to update @product@ manually"));
}
