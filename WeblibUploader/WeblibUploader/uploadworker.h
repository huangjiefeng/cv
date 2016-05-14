#ifndef _UPLOADWORKER_H_
#define _UPLOADWORKER_H_

#include "uploadoperator.h"
#include "commonstruts.h"
#include <QNetworkAccessManager>
#include <QList>
#include <QElapsedTimer>

class UploadOperator;

class UploadWorker :public QObject
{
	Q_OBJECT
public:
	UploadWorker();
	virtual ~UploadWorker();

	ErrorInfo addUploadFile(const QString &filepath, int parentid, int groupid);
	void stopUploadFile(const QString &filepath);
	void startAll();
	void stopAll();
	bool isFullQueue() { return m_uploadlist.size() >= UPLOAD_THREAD_LIMIT; };
signals:
	void reportStatus(UploadItem *iteminfo);
	void uploadComplete(const QString &filename);
	void uploadStop(const QString &filename);

private slots:
	void transfer();
	void scheduleTransfer();
	void refreshStatus(UploadOperator *sender);
	void uploadFinished(bool error, bool abored, UploadOperator *sender);

private:
	bool m_stop;
	bool m_uploading;
	QElapsedTimer stopWatch;
	QList<UploadOperator *> m_uploadlist;
};

#endif