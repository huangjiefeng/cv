#ifndef CUPLOAD_H
#define CUPLOAD_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class CUploader : public QObject
{

	Q_OBJECT;

public:

	enum UploadState
	{
		LINKING,
		SENDING,
		SEND_DONE,
		SEND_ERROR,
	};

	CUploader(QObject *parent = NULL);
	~CUploader();

	void Upload(const QString &filename, const QString &clientid, const QString &url);

signals:

	void UploadStateChanged(CUploader::UploadState state, QString msg);

private slots:

	void OnUploadFinished();  
	void OnUpLoadError(QNetworkReply::NetworkError errorCode);
	void OnUploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:

	void Reset();

	//Upload
	QNetworkAccessManager    *_uploadManager;
	QNetworkReply            *_reply;
	char*   _uploadBuf;

	
};

#endif // CUPLOAD_H
