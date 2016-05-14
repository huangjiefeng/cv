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

#include "QtCrashReport.h"

#include "ui_CrashReport.h"

#include <cutil/global.h>
#include <util/Date.h>
#include <util/Time.h>
#include <util/Path.h>
#include <util/File.h>
#include <thread/Thread.h>
#include <util/SafeDelete.h>
#include <util/Logger.h>

#include <settings/SettingsXMLSerializer.h>

#ifdef OS_WINDOWS
	#include <system/WindowsVersion.h>
#endif

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

#include <iostream>
#include <cstdio>


static const std::string FTP_SERVER_KEY = "ftp.server";
static const std::string FTP_LOGIN_KEY = "ftp.login";
static const std::string FTP_PASSWORD_KEY = "ftp.password";
#if defined(OS_WINDOWS)
	static const std::string FTP_PATH = ".";
#elif defined(OS_LINUX)
	static const std::string FTP_PATH = "linux";
#else
	static const std::string FTP_PATH = "macosx";
#endif


QtCrashReport::QtCrashReport(const std::string & dumpfile, const std::string & logfile,
	const std::string & applicationName_, const std::string & lang, const std::string & info)
	: QObjectThreadSafe(NULL),
	_info(info) {
	readConfig();

	_progressTotal = 0;
	_progressNow = 0;
	_status = FtpUpload::None;
	_firstFileUploaded = false;
	_logfile = logfile;
	_descfile = dumpfile + ".txt";

	_ftpUpload = new FtpUpload(
		_config.getStringKeyValue(FTP_SERVER_KEY),
		_config.getStringKeyValue(FTP_LOGIN_KEY),
		_config.getStringKeyValue(FTP_PASSWORD_KEY),
		FTP_PATH, dumpfile);
	_ftpUpload->progressionEvent += boost::bind(&QtCrashReport::ftpProgressEventHandler, this, _1, _2, _3);
	_ftpUpload->statusEvent += boost::bind(&QtCrashReport::ftpStatusEventHandler, this, _1, _2);

	//Adds the translation file
	QTranslator * translator = new QTranslator(NULL);
	translator->load(QString::fromStdString(lang));
	QApplication::installTranslator(translator);

	_dialog = new QDialog(NULL);

	_ui = new Ui::CrashReport();
	_ui->setupUi(_dialog);

	SAFE_CONNECT(_ui->sendButton, SIGNAL(clicked()), SLOT(sendButtonClicked()));

	// Do minimal branding
	QString applicationName = QString::fromStdString(applicationName_);
	_dialog->setWindowTitle(tr("%1 Crash Report").arg(applicationName));
	QString text = _ui->mainLabel->text();
	text = text.replace("@product@", applicationName);
	_ui->mainLabel->setText(text);

	_ui->progressBar->setValue(0);
}

QtCrashReport::~QtCrashReport() {
	OWSAFE_DELETE(_ui);
}

void QtCrashReport::readConfig() {
	// Init resourcesDir
	std::string resourcesDir;
#if defined(OS_WINDOWS)
	resourcesDir = Path::getApplicationDirPath();
#elif defined(OS_MACOSX)
	resourcesDir = Path::getApplicationResourcesDirPath();
#elif defined(OS_LINUX) and defined(OW_RESOURCEDIR)
	resourcesDir = OW_RESOURCEDIR + File::getPathSeparator();

	//Check if resourcesDir exist, if not then Path::getApplicationDirPath()
	if (!File::exists(resourcesDir)) {
		resourcesDir = Path::getApplicationDirPath();
	}
#endif

	std::string configFileName = resourcesDir + "config/crashreport.xml";
	FileReader configFile(configFileName);
	if (!configFile.open()) {
		LOG_FATAL("Could not open " + configFileName);
	}

	// Load from XML
	SettingsXMLSerializer serializer(_config);
	serializer.unserialize(configFile.read());
}

void QtCrashReport::sendButtonClicked() {
	_ui->sendButton->setEnabled(false);
	//start the upload
	createDescriptionFile();
	_ftpUpload->start();
}

void QtCrashReport::initThreadSafe() {
}

void QtCrashReport::show() {
	_dialog->show();
}

void QtCrashReport::ftpProgressEventHandler(FtpUpload * sender, double ultotal, double ulnow) {
	_progressNow = ulnow;
	_progressTotal = ultotal;

	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtCrashReport::updatePresentationThreadSafe, this));
	postEvent(event);
}

void QtCrashReport::ftpStatusEventHandler(FtpUpload * sender, FtpUpload::Status status) {
	_status = status;

	typedef PostEvent0<void ()> MyPostEvent;
	MyPostEvent * event = new MyPostEvent(boost::bind(&QtCrashReport::updatePresentationThreadSafe, this));
	postEvent(event);
}

void QtCrashReport::updatePresentationThreadSafe() {
	_ui->progressBar->setMaximum((int) _progressTotal);
	_ui->progressBar->setValue((int) _progressNow);

	if ((_status == FtpUpload::Ok) || (_status == FtpUpload::Error)) {
		if (!_firstFileUploaded) {

			_firstFileUploaded  = true;
			_ui->progressBar->setValue(0);

			//delete _ftpUpload;
			_ftpUpload2 = new FtpUpload(
				_config.getStringKeyValue(FTP_SERVER_KEY),
				_config.getStringKeyValue(FTP_LOGIN_KEY),
				_config.getStringKeyValue(FTP_PASSWORD_KEY),
				FTP_PATH, _descfile);
			_ftpUpload2->progressionEvent += boost::bind(&QtCrashReport::ftpProgressEventHandler, this, _1, _2, _3);
			_ftpUpload2->statusEvent += boost::bind(&QtCrashReport::ftpStatusEventHandler, this, _1, _2);
			_ftpUpload2->start();

		} else {
			//sleep: only for the feeling
			Thread::sleep(1);
			_dialog->close();
		}
	}
}

static std::string readLogFile(const std::string& name) {
	std::string data;

	FileReader file(name);
	if (file.open()) {
		data = file.read();
		file.close();
	} else {
		return "failed to open log file=" + name;
	}
	return data;
}

void QtCrashReport::createDescriptionFile() const {
	std::ofstream file;
	file.open(_descfile.c_str(), std::ios::out);

	file
		<< "Date: " << Date().toString() << std::endl
		<< "Time: " << Time().toString() << std::endl;

#ifdef OS_WINDOWS
	file << "Windows version: " << WindowsVersion::getVersion() << std::endl;
#endif

	if (_ui->mailLineEdit) {
		file << "From: " << _ui->mailLineEdit->text().toStdString() << std::endl;
	}

	if (_ui->descTextEdit) {
		file << "Description: " << _ui->descTextEdit->toPlainText().toStdString() << std::endl;
	}

	file << "User info:" << std::endl << _info << std::endl;
	file << _logfile << " content:" << std::endl << readLogFile(_logfile) << std::endl;
}
