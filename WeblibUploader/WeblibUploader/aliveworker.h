#ifndef _ALIVEWORKDER_H_
#define _ALIVEWORKDER_H_

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QTimer>
#include "weblibsetting.h"

class AliveWorker : public QObject
{
	Q_OBJECT
public:
	AliveWorker(bool start, int interval);
	virtual ~AliveWorker();
	void start();
	void stop();
	bool getStatus();

signals:
	void aliveError(const QString &info);

private slots:
	void slotSendAliveRequest();
	void slotAliveReply();
	void slotAliveError(QNetworkReply::NetworkError errorCode);

private:
	QNetworkAccessManager *m_manager;
	QNetworkRequest *m_request;
	int m_interval;
	QTimer *m_timer;
};

#endif