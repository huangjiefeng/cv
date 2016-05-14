#ifndef _COMMONSTRUTS_H_
#define _COMMONSTRUTS_H_

#include <QString>

enum UploadStatus
{
	Pending,
	Uploading,
	Failed,
	Error,
	Stop,
	Done
};

enum ItemOperate
{
	AddItem,
	DeleteItem,
	RefreshItem
};

struct ErrorInfo
{
	bool error;
	QString info;
};

struct WeblibSettingInfo
{
	QString url;
	QString account;
	QString password;
	QString filepath;
	int		scanlevel;
	QString	groupname;
	int		groupid;
	qint64	connecttimeout;
	qint64	scaninterval;
	qint64	uploadspeed;
	int		threads;
	bool	iscreateudir;
};

struct UploadItem
{
	UploadStatus status;
	QString filename;
	qint64 bytesuploaded;
	qint64 size;
	int pecent;
	int groupid;
	int parentid;
	QString speed;
	QString lefttime;

	bool IsUploadable()
	{
		switch(status)
		{
		case Pending:
		case Failed:
			return true;
		case Uploading:
		case Done:
		case Error:
			return false;
		default:
			return false;
		}
	}
};

static QString SizeConverter(qint64 size)
{
	double filesize = size;
	if(filesize < 1024)
		return QString("%1B").arg(filesize, 0, 'f', 2);
	else if(filesize < 1048576)
		return QString("%1KB").arg(filesize / 1024, 0, 'f', 2);
	else if(filesize < 1073741824)
		return QString("%1MB").arg(filesize / 1048576, 0, 'f', 2);
	else
		return QString("%1GB").arg(filesize / 1073741824, 0, 'f', 2);
}

static QString SpeedConverter(double speed)
{
	if(speed < 1024)
		return QString("%1B/s").arg(speed, 0, 'f', 1);
	else if(speed < 1048576)
		return QString("%1KB/s").arg(speed / 1024, 0, 'f', 1);
	else if(speed < 1073741824)
		return QString("%1MB/s").arg(speed / 1048576, 0, 'f', 1);
	else
		return QString("%1GB/s").arg(speed / 1073741824, 0, 'f', 1);
}

static QString TimeConverter(qint64 time)
{
	int hours, mins, secs;
	hours = mins = secs = 0;
	if(time >= 3600)
	{
		hours = time / 3600;
		time %= 3600;
	}
	if(time >= 60)
	{
		mins = time / 60;
		time %= 60;
	}
	secs = time;
	return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10,  QChar('0'));
}

#endif