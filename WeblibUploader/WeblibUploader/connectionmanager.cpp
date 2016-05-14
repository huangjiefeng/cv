#include "connectionmanager.h"
#include "weblibsetting.h"
#include <qmath.h>
#include <QTextCodec>
#include <QMutexLocker>
#include <qdebug.h>

ConnectionManager::ConnectionManager()
{
	m_reply = NULL;
	m_aliveworker = NULL;
	m_manager = NULL;
	m_loginsuccess = false;

	m_cookies = new QNetworkCookieJar();
	int num = qCeil((float)(UPLOAD_THREAD_LIMIT + 1) / REQUEST_PER_MNANAGER);
	num = num > MAX_CONNECTION_MANAGER ? MAX_CONNECTION_MANAGER : num;

	for(int i=0; i<num; i++)
	{
		ConnectRequester *request = new ConnectRequester();
		request->manager->setCookieJar(m_cookies);
		m_cookies->setParent(NULL);
		m_list.append(request);
	}
}

ConnectionManager::~ConnectionManager()
{
	for(int i=0; i<m_list.size(); i++)
		delete m_list[i];

	if(m_aliveworker != NULL)
	{
		m_aliveworker->stop();
		delete m_aliveworker;
		m_aliveworker = NULL;
	}

	delete m_cookies;
}

void ConnectionManager::setManagerNumbyThread(int limit)
{
	QMutexLocker locker(&m_mutex);

	int num = qCeil((float)(limit + 1) / REQUEST_PER_MNANAGER);
	num = num > MAX_CONNECTION_MANAGER ? MAX_CONNECTION_MANAGER : num;
	int curnum = m_list.size();
	if(num == curnum)
		return;

	if(num < curnum)
		for(int i = num - 1; i < curnum; i++)
			m_list.at(i)->available = false;

	if(num > curnum)
		for(int i = curnum - 1; i < num; i++)
		{
			ConnectRequester *request = new ConnectRequester();
			request->manager->setCookieJar(m_cookies);
			m_cookies->setParent(NULL);
			m_list.append(request);
		}
}

void ConnectionManager::releaseManager(QNetworkAccessManager *manager)
{
	QMutexLocker locker(&m_mutex);

	if(manager == NULL)
		return;

	ConnectRequester *p;
	for (int i=0; i<m_list.size(); i++)
	{
		p = m_list.at(i);
		//当连接上无活动请求时，将其删除
		if(p->manager == manager)
		{
			p->requestsnum--;
			if(p->requestsnum == 0)
			{
				m_list.removeAll(p);
				delete p;
			}
			break;
		}
	}
}

QNetworkAccessManager *ConnectionManager::getAvailableManager()
{
	QMutexLocker locker(&m_mutex);

	for(int i=0; i<m_list.size(); i++)
	{
		if(m_list.at(i)->available == true && m_list.at(i)->requestsnum < REQUEST_PER_MNANAGER)
		{
			m_list.at(i)->requestsnum++;
			return m_list.at(i)->manager;
		}
	}

	if(m_list.size() < MAX_CONNECTION_MANAGER)
	{
		ConnectRequester *request = new ConnectRequester();
		request->requestsnum++;
		request->manager->setCookieJar(m_cookies);
		m_cookies->setParent(NULL);
		m_list.append(request);
		return request->manager;
	}

	return NULL;
}


void ConnectionManager::weblibLogin()
{
	if(m_aliveworker != NULL)
	{
		m_aliveworker->stop();
		delete m_aliveworker;
		m_aliveworker = NULL;
	}
	if(!IS_SETTING_READY)
		return;

	emit refreshMainWindowStatus();

	QUrl url(WEBLIB_BASEURL + "/login/authenticate.action");
	QByteArray data;
	data.append("account=" + WEBLIB_ACCOUNT + "&password=" + WEBLIB_PASSWORD);
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_manager = ConnectionManager::getInstance().getAvailableManager();
	m_reply = m_manager->post(request, data);
	connect(m_reply, SIGNAL(finished()), this, SLOT(replycookies()));
}

void ConnectionManager::replycookies()
{
	disconnect(m_reply, SIGNAL(finished()), this, SLOT(replycookies()));
	if(m_reply->error())
	{
		m_loginsuccess = false;
		ConnectionManager::getInstance().releaseManager(m_manager);
		emit loginFailed(QString::fromLocal8Bit("weblib登录失败，请检查上传参数或网络连接是否异常"));
		return;
	}

	QString restring = m_reply->readAll().trimmed();
	m_reply->deleteLater();
	int startindex = restring.indexOf("\"id\":");
	int endindex = restring.indexOf(",", startindex);
	QUrl url(WEBLIB_BASEURL + "/login/selectMember.action");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QByteArray data;
	data.append("memberId=" + restring.mid(startindex + 5, endindex - startindex - 5));

	m_reply = m_manager->post(request, data);
	connect(m_reply, SIGNAL(finished()), this, SLOT(replyselectmember()));
}

void ConnectionManager::replyselectmember()
{
	disconnect(m_reply, SIGNAL(finished()), this, SLOT(replyselectmember()));
	if(m_reply->error())
	{
		m_loginsuccess = false;
		ConnectionManager::getInstance().releaseManager(m_manager);
		emit loginFailed(QString::fromLocal8Bit("weblib登录失败，请检查上传参数或网络连接是否异常"));
		return;
	}

	QString restring = m_reply->readAll().trimmed();

	m_reply->deleteLater();

	QUrl url(WEBLIB_BASEURL + "/user/status.action");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_reply = m_manager->get(request);
	connect(m_reply, SIGNAL(finished()), this, SLOT(replymemberstatus()));
}

void ConnectionManager::replymemberstatus()
{
	disconnect(m_reply, SIGNAL(finished()), this, SLOT(replymemberstatus()));
	if(m_reply->error())
	{
		m_loginsuccess = false;
		ConnectionManager::getInstance().releaseManager(m_manager);
		emit loginFailed(QString::fromLocal8Bit("weblib登录失败，请检查上传参数或网络连接是否异常"));
		return;
	}

	//QString restring = QTextCodec::codecForName("UTF-8")->toUnicode(m_reply->readAll().trimmed());

	m_reply->deleteLater();
	ConnectionManager::getInstance().releaseManager(m_manager);

	m_loginsuccess = true;
	startAliveThread();
	emit loginSuccess();
}

void ConnectionManager::startAliveThread()
{
	if(m_aliveworker != NULL)
	{
		disconnect(m_aliveworker, SIGNAL(aliveError(const QString &)), this, SIGNAL(aliveError(const QString &)));
		m_aliveworker->stop();
		delete m_aliveworker;
		m_aliveworker = NULL;
	}
	m_aliveworker = new AliveWorker(true, 60000);
	connect(m_aliveworker, SIGNAL(aliveError(const QString &)), this, SLOT(aliveError(const QString &)));
}

void ConnectionManager::aliveError(const QString &info)
{
	m_loginsuccess = false;
	emit loginFailed(QString::fromLocal8Bit("Weblib连接已断开，请检查网络或Welib服务器状态"));
}