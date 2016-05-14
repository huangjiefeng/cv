#include "uploadoperator.h"
#include <QDebug>
#include <QDateTime>
#include <QHostInfo>
#include <QTextCodec>
#include <QNetworkCookie>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

UploadOperator::UploadOperator(QString filename, int parentid, int groupid)
{
	m_parentid = parentid;
	m_groupid = groupid;
	m_retry = 0;
	m_filename = filename;
	m_file = NULL;
	m_socket = NULL;
	m_error = false;
	m_buffer = new char[UPLOAD_BLOCK_SIZE];
	m_status = Pending;
}

UploadOperator::~UploadOperator()
{
	delete m_buffer;
	m_buffer = NULL;
	closeUploader();
}

void UploadOperator::setError(const QString &info)
{
	m_status = Error;
	m_error = true;
	m_errorinfo = info;
}

//不要在初始化函数里发射信号
bool UploadOperator::resetParams()
{
	closeUploader();

	m_file = new UploadFile(m_filename);
	m_aborded = false;
	m_pecent = 0;
	m_error = false;
	m_bytesuploaded = 0;
	m_speedcounter = 0;
	m_speed = QString::fromLocal8Bit("正在请求上传");
	m_status = Uploading;
	m_pretime = QDateTime::currentDateTime();

	QByteArray boundaryRegular(QString("---------------------------%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()).toLatin1());
	QByteArray boundary("\r\n--"+boundaryRegular+"\r\n");
	QByteArray boundaryLast("\r\n--"+boundaryRegular+"--\r\n\r\n");

	QByteArray mimedata("--" + boundaryRegular + "\r\n");
	mimedata.append("Content-Disposition: form-data; name=\"groupId\"\r\n\r\n");
	mimedata.append(QByteArray::number(m_groupid));
	mimedata.append(boundary);
	mimedata.append("Content-Disposition: form-data; name=\"parentId\"\r\n\r\n");
	mimedata.append(QByteArray::number(m_parentid));
	mimedata.append(boundary);
	mimedata.append("Content-Disposition: form-data; name=\"filedata\"; filename=\"" + m_file->getFileName().toUtf8() + "\"\r\n");
	mimedata.append("Content-Type: application/octet-stream\r\n\r\n");
	m_file->setHead(mimedata);
	m_file->setTail(boundaryLast);

	QUrl webliburl(WEBLIB_BASEURL);

	QByteArray httpheader("POST /group/uploadResource.action HTTP/1.1\r\n");
	httpheader.append("Host: " + webliburl.host() + "\r\n");
	httpheader.append("User-Agent: Weblib Upload Client\r\n");
	httpheader.append("Accept: */*\r\n");
	httpheader.append("Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n");
	httpheader.append("Accept-Encoding: gzip,deflate\r\n");
	httpheader.append("Content-Type: " + QByteArray("multipart/form-data; boundary=").append(boundaryRegular) + "\r\n");
	httpheader.append("Content-Length: " + QByteArray::number(m_file->sizepart()) + "\r\n");

	QList<QNetworkCookie> list = m_cookie->cookiesForUrl(WEBLIB_BASEURL + "/");
	for(int i=0; i<list.size(); i++)
	{
		if(list.at(i).name() == "JSESSIONID")
		{
			httpheader.append("Cookie: " + list.at(i).name() + "=" + list.at(i).value() + "\r\n");
			break;
		}
	}
	httpheader.append("Connection: keep-alive\r\n");
	httpheader.append("Cache-Control: no-cache\r\n\r\n");
	m_file->setHttpHeader(httpheader);

	m_socket = new QTcpSocket(this);
	m_socket->connectToHost(webliburl.host(), webliburl.port() == -1 ? 80 : webliburl.port());
	connect(m_socket, SIGNAL(connected()), this, SIGNAL(readyToTransfer()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(finished()));
	if(!m_file->openFile())
	{
		setError(QString::fromLocal8Bit("打开文件失败"));
		return false;
	}

	return true;
}

bool UploadOperator::canTransferMore()
{
	return (m_file != NULL) 
		&& (m_error == false)
		&& (m_socket != NULL)
		&& (m_file->isOpen())
		&& !(m_file->isFinished())
		&& (m_socket->state() == QAbstractSocket::ConnectedState);
}

qint64 UploadOperator::bytesToWrite()
{
	return m_socket->bytesToWrite();
}

bool UploadOperator::initializeConnect(QNetworkCookieJar *cookie)
{
	m_cookie = cookie;
	return resetParams();
}

void UploadOperator::restartUpload()
{
	m_retry++;
	if(!resetParams())
		emit uploadFinished(true, false, this);
}

qint64 UploadOperator::sendBuffer(qint64 size, bool &bufferfull)
{
	qint64 maxlen, len, sended, inc;
	sended = 0;
	while(size > 0 && (m_socket->bytesToWrite() <= UPLOAD_BLOCK_SIZE * 10))
	{
		maxlen = qMin(UPLOAD_BLOCK_SIZE, size);
		len = m_file->readData(m_buffer, maxlen);
		if(len <= 0)
			break;

		size -= len;
		inc = m_socket->write(m_buffer, len);
		sended += inc;
		m_bytesuploaded += inc;
	}

	if(size > 0)
		bufferfull = true;

	return sended;
}

void UploadOperator::updateStatus(qint64 inc)
{
	QDateTime curtime = QDateTime::currentDateTime();
	if(m_pretime.msecsTo(curtime) > 1000)
	{
		qint64 inc = m_bytesuploaded - m_speedcounter;
		double speed = (double)inc / m_pretime.msecsTo(curtime) * 1000;
		m_speed = SpeedConverter(speed);
		m_speedcounter = m_bytesuploaded;
		m_pecent = (double)m_bytesuploaded / m_file->sizefull() * 100;
		qint64 lefttime = (m_file->sizefull() - m_bytesuploaded) / speed;
		m_lefttime = TimeConverter(lefttime);
		m_pretime = curtime;
		m_status = Uploading;
		emit refreshStatus(this);
	}
}

void UploadOperator::finished()
{
	disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(finished()));

	m_status = Done;
	QByteArray httpresponse =m_socket->readAll();
	int startindex = httpresponse.indexOf("Content-Length:");
	if(startindex == -1)
	{
		setError("Parsing http header length failed");
		emit uploadFinished(true, false, this);
		return;
	}
	int endindex = httpresponse.indexOf("\r\n", startindex);

	if(endindex >= httpresponse.length())
	{
		setError("Parsing http header length failed");
		emit uploadFinished(true, false, this);
		return;
	}

	QString rsplenstr = httpresponse.mid(startindex + 15, endindex - startindex - 15);
	bool ok = false;
	int contentlen = rsplenstr.toInt(&ok);
	if(!ok)
	{
		setError("Wrong http header length");
		emit uploadFinished(true, false, this);
		return;
	}

	QJsonParseError jsonok;
	QJsonObject result = QJsonDocument::fromJson(httpresponse.right(contentlen), &jsonok).object();
	if(jsonok.error != QJsonParseError::NoError)
	{
		qCritical() << "Upload Json Error:" << httpresponse;
		setError("Error parsing http response to json: " + httpresponse);
		emit uploadFinished(true, false, this);
		return;
	}

	if(result["success"].toBool() == true)
	{
		emit uploadFinished(false, false, this);
	}
	else
	{
		setError(result["detail"].toString());
		emit uploadFinished(true, false, this);
	}
}

void UploadOperator::socketError(QAbstractSocket::SocketError info)
{
	if(m_error == false)
		setError(m_socket->errorString());

	emit uploadFinished(true, false, this);
}

void UploadOperator::closeUploader()
{
	if(m_socket != NULL)
	{
		disconnect(m_socket, SIGNAL(connected()), this, SIGNAL(readyToTransfer()));
		disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
		m_socket->disconnectFromHost();
		m_socket->deleteLater();
		m_socket = NULL;
	}

	if(m_file != NULL)
	{
		m_file->closeFile();
		delete m_file;
		m_file = NULL;
	}
}