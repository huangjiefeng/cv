#ifndef _FILERUPLOADER_H_
#define _FILERUPLOADER_H_

#include <QList>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QModelIndex>
#include <QMutex>
#include "uploadworker.h"
#include "weblibsetting.h"
#include "commonstruts.h"
#include "filescaner.h"


class FileUploader : public QObject
{
	Q_OBJECT
public:
	FileUploader();
	virtual ~FileUploader();

public:
	void stopAll();
	bool removeFile(const QString &filepath);
public slots:
	void startAll();

signals:
	void error(const QString &info);
	void signalItemStateChanged(ItemOperate oper, const UploadItem &item);

private:
	void startNextUpload(bool erritem);
	void stopUpload(const QString &filepath);
	UploadItem *checkFileContained(const QString &filepath);

private slots:
	void refreshItemStatus(UploadItem *iteminfo);
	void fileComplete(const QString &filename);
	void fileStop(const QString &filename);
	void addFile(const QString &filepath, int groupid, int parentid);
private:
	bool m_stop;
	//map用于快速定位
	QMap<QString, UploadItem *> m_indexlist;
	//list用于按顺序上传文件
	QList<UploadItem *> m_seqlist;
	FileScaner *m_scaner;
	QMutex m_mutex;
	UploadWorker *m_uploadworker;
};

#endif