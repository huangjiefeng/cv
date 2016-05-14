#ifndef _UPLOADOPERATOR_H_
#define _UPLOADOPERATOR_H_

#include <QTcpSocket>
#include <QDateTime>
#include <QList>
#include <QNetworkCookieJar>
#include "weblibsetting.h"
#include "commonstruts.h"
#include "uploadfile.h"

class UploadFile;

class UploadOperator : public QObject
{
	Q_OBJECT
public:
	UploadOperator(QString filename, int parentid, int groupid);
	virtual ~UploadOperator();

	qint64 sendBuffer(qint64 size, bool &bufferfull);
	bool initializeConnect(QNetworkCookieJar *cookie);
	bool canTransferMore();
	void updateStatus(qint64 inc);
	void closeUploader();

	const QString &getFileName() { return m_filename; }
	qint64 getBytesUploaded() { return m_bytesuploaded; }
	qint64 bytesToWrite();
	int getRetrys() { return m_retry; }

	QString getSpeed() 
	{ 
		if(!m_error)	return m_speed;
		else			return m_errorinfo;	
	}

	qint64 getSize() { return m_file->filesize(); }
	const QString &getLeftTime() { return m_lefttime; }
	int getPecent() { return m_pecent; }
	int getGroupId() { return m_groupid; }
	int getParentId() { return m_parentid; }
	QString getErrorInfo() { return m_errorinfo; }
	UploadStatus getStatus() { return m_status; }

signals:
	void readyToTransfer();
	void refreshStatus(UploadOperator *sender);
	void uploadFinished(bool error, bool abored, UploadOperator *sender);

public slots:
	void restartUpload();

private:
	bool resetParams();
	void setError(const QString &info);

private slots:
	void finished();
	void socketError(QAbstractSocket::SocketError info);

private:
	int m_parentid;
	int m_groupid;
	char *m_buffer;
	bool m_aborded;
	int m_retry;
	QString m_speed;
	QString m_lefttime;
	QString m_filename;
	bool m_error;
	QString m_errorinfo;
	int m_pecent;
	qint64 m_bytesuploaded;
	qint64 m_speedcounter;
	UploadStatus m_status;
	QDateTime m_pretime;
	QNetworkCookieJar *m_cookie;

	QTcpSocket *m_socket;
	UploadFile *m_file;
};

#endif