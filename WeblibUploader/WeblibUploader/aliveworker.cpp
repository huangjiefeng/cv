#include "aliveworker.h"
#include "connectionmanager.h"
#include <QDebug>

AliveWorker::AliveWorker(bool start, int interval) : m_timer(NULL), m_interval(interval)
{
	QUrl aliveurl(WEBLIB_BASEURL + "/user/alive.action");
	m_request = new QNetworkRequest(aliveurl);
	m_request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_manager = ConnectionManager::getInstance().getAvailableManager();

	m_timer = new QTimer();
	m_timer->setInterval(m_interval);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotSendAliveRequest()));
	if(start)
		m_timer->start();
}

AliveWorker::~AliveWorker()
{
	ConnectionManager::getInstance().releaseManager(m_manager);
	if(m_request != NULL)
	{
		delete m_request;
		m_request = NULL;
	}

	if(m_timer != NULL)
	{
		disconnect(m_timer, SIGNAL(timeout()), this, SLOT(slotSendAliveRequest()));
		m_timer->stop();
		delete m_timer;
		m_timer = NULL;
	}
}

bool AliveWorker::getStatus()
{
	return m_timer->isActive();
}

void AliveWorker::start()
{
	m_timer->start();
}

void AliveWorker::stop()
{
	m_timer->stop();
}

void AliveWorker::slotSendAliveRequest()
{
	if(m_request == NULL)
	{
		qDebug() << "aliveworker: NULL manager";
		emit aliveError(QString::fromLocal8Bit("aliveworker未获取到可用网络连接"));
		return;
	}

	QNetworkReply *reply = m_manager->get(*m_request);
	connect(reply, SIGNAL(finished()), this, SLOT(slotAliveReply()));
	qDebug() << "send alive request";
}

void AliveWorker::slotAliveReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	if(reply->error())
	{
		qCritical() << "服务器断开连接: " << reply->readAll();
		emit aliveError(QString::fromLocal8Bit("服务器断开连接"));
		reply->deleteLater();
		return;
	}
	disconnect(reply, SIGNAL(finished()), this, SLOT(slotAliveReply()));
	reply->deleteLater();
	qDebug() << "alive connected";
}

void AliveWorker::slotAliveError(QNetworkReply::NetworkError errorCode)
{
	qCritical() << "alive error: " << errorCode;
	emit aliveError(QString::fromLocal8Bit("服务器断开连接"));
}