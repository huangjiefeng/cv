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

#ifndef OWQTCRASHREPORT_H
#define OWQTCRASHREPORT_H

#include "FtpUpload.h"

#include <settings/Settings.h>

#include <qtutil/QObjectThreadSafe.h>

class QDialog;
namespace Ui { class CrashReport; }

/**
 * Crash report widget
 *
 * @author Mathieu Stute
 */
class QtCrashReport : public QObjectThreadSafe {
	Q_OBJECT
public:

	QtCrashReport(const std::string & dumpfile, const std::string& logfile,
		const std::string & applicationName, const std::string & lang = "",
		const std::string & info = "");

	~QtCrashReport();

	void show();

private Q_SLOTS:

	void sendButtonClicked();

private:

	void ftpProgressEventHandler(FtpUpload * sender, double ultotal, double ulnow);

	void ftpStatusEventHandler(FtpUpload * sender, FtpUpload::Status status);

	void updatePresentationThreadSafe();

	void initThreadSafe();

	void createDescriptionFile() const;

	void readConfig();

	FtpUpload * _ftpUpload;

	FtpUpload * _ftpUpload2;

	FtpUpload::Status _status;

	double _progressNow;

	double _progressTotal;

	std::string _descfile;

	std::string _logfile;

	std::string _info;

	QDialog * _dialog;

	Ui::CrashReport * _ui;

	bool _firstFileUploaded;

	Settings _config;
};

#endif	//OWQTCRASHREPORT_H
