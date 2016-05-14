#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_

#include <QNetworkAccessManager>
#include <QList>
#include <QNetworkCookieJar>
#include <QMutex>
#include "aliveworker.h"

class AliveWorker;

struct ConnectRequester
{
	bool available;
	int requestsnum;
	QNetworkAccessManager *manager;

	ConnectRequester()
	{
		available = true;
		requestsnum = 0;
		manager = new QNetworkAccessManager();
	}

	~ConnectRequester()
	{
		available = false;
		requestsnum = 0;
		manager->deleteLater();
	}
};

class ConnectionManager : public QObject
{
	Q_OBJECT
public:
	static ConnectionManager &getInstance()
	{
		static ConnectionManager instance;
		return instance;
	}

	~ConnectionManager();

	void setManagerNumbyThread(int theadnum);
	QNetworkAccessManager *getAvailableManager();
	void releaseManager(QNetworkAccessManager *manager);
	QNetworkCookieJar *getCookies() { return m_cookies; }

	bool getLoginStatus() { return m_loginsuccess; }

public slots:
	void weblibLogin();

signals:
	void loginSuccess();
	void loginFailed(const QString & info);
	void refreshMainWindowStatus();

private slots:
	void aliveError(const QString &info);
	void replycookies();
	void replyselectmember();
	void replymemberstatus();

private:
	ConnectionManager();
	ConnectionManager(ConnectionManager const &param);
	ConnectionManager &operator = (ConnectionManager const &param);
	void startAliveThread();

private:
	bool m_loginsuccess;
	QNetworkCookieJar *m_cookies;
	QList<ConnectRequester *> m_list;
	QMutex m_mutex;

	AliveWorker *m_aliveworker;	
	QNetworkAccessManager *m_manager;
	QNetworkReply *m_reply;
};

#endif