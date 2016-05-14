#include "HttpWrapper.h"

#include <QtNetwork>
#include <QMessageBox>

#include "profile/RunningProfile.h"

//For test only

struct TerminalPorts
{
	int videoPort;
	int audioPort;
	int smallPort;

	TerminalPorts()
	{
		videoPort = 0;
		audioPort = 0;
		smallPort = 0;
	}
};

QMap<QString, TerminalPorts> terminalPortsTable;

void initTerminalPorts()
{
	const char *tName[] = {
		"scut_yss@sip.ccnl.scut.edu.cn",
		"scut_lkz@sip.ccnl.scut.edu.cn",
		"scut_tx3@sip.ccnl.scut.edu.cn",
		"scut_t2@sip.ccnl.scut.edu.cn",
		"scut_t1@sip.ccnl.scut.edu.cn",
		"scut_t5@sip.ccnl.scut.edu.cn",
		"scut_t3@sip.ccnl.scut.edu.cn",
		"test_m1@sip.ccnl.scut.edu.cn",
	};

	terminalPortsTable.clear();

	for (int i = 0; i < sizeof(tName)/sizeof(char *); ++i)
	{
		TerminalPorts ports;
		ports.videoPort = 11000 + i * 12;
		ports.audioPort = 11004 + i * 12;
		ports.smallPort = 11008 + i * 12;

		const char * uri = tName[i];
		terminalPortsTable[uri] = ports;
	}
}

TerminalPorts getTerminalPorts(const QString uri, const int vindex = 0)
{
	TerminalPorts ports = terminalPortsTable[uri];
	const int vincrement = 2000;
	ports.videoPort += vindex * vincrement;
	ports.audioPort += vindex * vincrement;
	ports.smallPort += vindex * vincrement;
	return ports;
}

//=============

HttpWrapper::HttpWrapper()
	: QObject(NULL)
{
	loginReply_ = NULL;
	getTerminalReply_ = NULL;
	login_ = false;
	initTerminalPorts(); // TODO:test
}

HttpWrapper::~HttpWrapper()
{

}



void HttpWrapper::RequestTerminalList( const QString cid )
{
	cid_ = cid;
	if (!login_)
	{
		DoLogin();
		return;
	}

	QString host = GetHttpHost();
	QString urlStr = QString("http://%1/ConferenceControl-war/terminal/getConferenceTerminalList.action").arg(host);
	QString paramStr = "conferenceId=" + cid;

	url_ = urlStr;
	QNetworkRequest request = GetPostRequest(url_);

	getTerminalReply_ = qnam_.post(request, paramStr.toLocal8Bit());
	connect(getTerminalReply_, SIGNAL(finished()),
		this, SLOT(GetTerminalListFinishedSlot()));
}

void HttpWrapper::GetTerminalListFinishedSlot()
{
	if (getTerminalReply_->error())
	{
		QMessageBox::information(NULL, tr("HTTP"),
			tr("Load failed: %1.")
			.arg(getTerminalReply_->errorString()));
	}
	else
	{
		//TODO:暂不清楚会话有效期，故每次请求都登陆一次
		login_ = false;

		QByteArray byte = getTerminalReply_->readAll();
		ParseTerminalList(byte);
	}

	getTerminalReply_->deleteLater();
	getTerminalReply_ = 0;
}

void HttpWrapper::ParseTerminalList( QByteArray data )

{
	QJsonParseError jsonError;
	QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
	if(jsonError.error != QJsonParseError::NoError)
	{  
		return;
	}
	if(!json.isObject())
	{  
		return;
	}

	QJsonObject obj = json.object();
	if(!obj.contains("terminals") || !obj["terminals"].isArray())  
	{  
		return;
	}

	QJsonArray terminalsArray = obj["terminals"].toArray();  
	const int totalCount = terminalsArray.size();
	for (int row = 0; row < terminalsArray.size(); row++)
	{
		if (!terminalsArray[row].isArray())
		{
			continue;
		}

		QJsonArray virtualArray = terminalsArray[row].toArray(); 
		for (int index = 0; index < virtualArray.size(); ++index)
		{
			if (!virtualArray[index].isObject())
			{
				continue;
			}

			QJsonObject tObj = virtualArray[index].toObject();
			TerminalInfo *info = new TerminalInfo;

			info->uri = tObj["uri"].toString().toStdString();

			std::string::size_type startIndex;
			if( ( startIndex = info->uri.find("@") )!=std::string::npos ) 
			{
				//更换当前的域名
				const std::string username = info->uri.substr( 0, startIndex );
				if (username == RunningProfile::getInstance()->username())
				{
					RunningProfile::getInstance()->set_user_uri( info->uri );
				}
			}
			info->virtualUri = tObj["virtualUri"].toString().toStdString();
			if (info->virtualUri.empty())
			{
				info->virtualUri = "v1#";
				info->virtualUri += info->uri;
			}
			info->virtualCount = tObj["virtualTerminalCount"].toString().toInt();
			if (0 >= info->virtualCount)
			{
				info->virtualCount = 1;
			}
			info->name = tObj["name"].toString().toStdString();
			info->dns = tObj["dns"].toString().toStdString();
			info->model = tObj["model"].toString().toStdString();
			info->version = tObj["version"].toString().toStdString();
			info->sdp = tObj["sdp"].toString().toStdString();
			info->multicastAddress = tObj["multicastAddress"].toString().toStdString();
			info->videoPort = tObj["videoPort"].toString().toInt();
			info->audioPort = tObj["audioPort"].toString().toInt();
			info->smallVideoPort = tObj["smallVideoPort"].toString().toInt();
			info->status = tObj["status"].toString().toStdString();
			info->location = tObj["location"].toString().toStdString();
			info->isSpeaking = tObj["isSpeaking"].toString().toStdString();
			info->isHandUp = tObj["isHandUp"].toString().toStdString();
			info->isChairmanTerminal = tObj["isChairManTerminal"].toString().toStdString();
			info->isMainSpeaker = tObj["isSpeakerTerminal"].toString().toStdString();
			info->totalTermialNum = tObj["count"].toString().toInt();

			int nSharpPos = info->virtualUri.find("#");
			if (std::string::npos != nSharpPos)
			{
				std::string indexStr = info->virtualUri.substr(1, nSharpPos - 1);
				info->virtualIndex = atoi(indexStr.c_str()) - 1; // 跳过首个v
				if (0 < info->virtualIndex)
				{
					info->name += "(" + indexStr + ")";
				}
			}
			else
			{
				info->virtualIndex = 0;
			}

			//TODO:test
			/*TerminalPorts ports = getTerminalPorts(info->uri.c_str(), info->virtualIndex);
			info->videoPort = ports.videoPort;
			info->audioPort = ports.audioPort;
			info->smallVideoPort = ports.smallPort;
			info->sdp = "m=audio  RTP/AVP "
				"101 m=video  RTP/AVP 34 "
				"a=fmtp:34 CUSTOM=1920,1080 MaxBR=1960 FPS=30"
				"a=rtpmap:34 H263/90000/1";
			info->totalTermialNum = totalCount;*/

			//TODO:暂时与SIP一样一次发一个
			TerminalVector list;
			list.push_back(info);
			emit RecvTerminalListSignal(list);
		}
	}
}


void HttpWrapper::RequestTerminalListSlot( const QString &cid ) {
  cid_ = cid;
  if (!login_) {
    DoLogin();
    return;
  }

  QString host = GetHttpHost();
  QString urlStr = QString("http://%1/ConferenceControl-war/terminal/getConferenceTerminalList.action").arg(host);
  QString paramStr = "conferenceId=" + cid;

  url_ = urlStr;
  QNetworkRequest request = GetPostRequest(url_);

  getTerminalReply_ = qnam_.post(request, paramStr.toLocal8Bit());
  connect(getTerminalReply_, SIGNAL(finished()),
          this, SLOT(HandleTerminalListReplySlot()));
}

void HttpWrapper::DoLogin() {
  QString host = GetHttpHost();
  /*QString username = QString::fromStdString(profile->username());
  QString password = QString::fromStdString(profile->password());*/
  QString username = QString::fromStdString("scut_http");
  QString password = QString::fromStdString("123456");

  QString urlStr = QString("http://%1/ConferenceControl-war/terminal/terminalAuthenticate.action").arg(host);
  QString paramStr = QString("terminalAccount=%1&password=%2").arg(username).arg(password);

  url_ = urlStr;
  QNetworkRequest request = GetPostRequest(url_);

  loginReply_ = qnam_.post(request, paramStr.toLocal8Bit());
  connect(loginReply_, SIGNAL(finished()),
          this, SLOT(LoginFinishedSlot()));
}

QString HttpWrapper::GetHttpHost()
{
  RunningProfile * profile = RunningProfile::getInstance();
  QString host = QString::fromStdString(profile->realm());
  int port = profile->http_port();

  if (0 == port) {
    return host;
  }
  return host + ":" + QString::number(port);
}

QNetworkRequest HttpWrapper::GetPostRequest( QUrl url ) {
  QNetworkRequest request;
  request.setUrl(QUrl(url));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QVariant var;
  var.setValue(qnam_.cookieJar()->cookiesForUrl(url));
  request.setHeader(QNetworkRequest::CookieHeader, var);

  return request;
}

void HttpWrapper::LoginFinishedSlot()
{
  if (loginReply_->error())
  {
    QMessageBox::information(NULL, tr("HTTP"),
      tr("Load failed: %1.")
      .arg(loginReply_->errorString()));
  }
  else
  {
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(loginReply_->readAll(), &jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {  
      QMessageBox::information(NULL, tr("HTTP"),
        tr("JSON format error: %1.")
        .arg(jsonError.errorString()));
    }
    else
    {
      QJsonObject jobj = json.object();
      if (!jobj.contains("type") || jobj["type"].toString() != "success")
      {
        QMessageBox::information(NULL, tr("HTTP"),
          tr("Login failed: %1.")
          .arg(jobj["detail"].isUndefined() ? "NULL" : jobj["detail"].toString()));
      }
      else
      {
        //TODO
        login_ = true;
        RequestTerminalList(cid_);
      }
    }
  }

  loginReply_->deleteLater();
  loginReply_ = 0;
}


void HttpWrapper::HandleTerminalListReplySlot() {
  if (getTerminalReply_->error()) {
    QMessageBox::information(NULL, tr("HTTP"),
      tr("Load failed: %1.")
      .arg(getTerminalReply_->errorString()));
  } else {
    //TODO:暂不清楚会话有效期，故每次请求都登陆一次
    login_ = false;

    QByteArray byte = getTerminalReply_->readAll();
    ParseTerminalList(byte);
  }

  getTerminalReply_->deleteLater();
  getTerminalReply_ = 0;
}



