#include "http_msg_manager.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>
#include <QList>

#include "profile/RunningProfile.h"

HttpMsgManager::HttpMsgManager(QObject *parent)
    : QObject(parent) {
  login_reply_ = nullptr;
  get_terminals_reply_ = nullptr;
  has_login_ = false;
}

HttpMsgManager::~HttpMsgManager() {

}

void HttpMsgManager::RequestTerminalListSlot( const QString &conference_cid ) {
  if (!processing_mutex_.tryLock()) {
    return;
  }

  cid_ = conference_cid;
  if (!has_login_) {
    DoLogin();
    return;
  }

  QString url = "http://";
  url += GetHttpHost();
  url += "/ConferenceControl-war/terminal/getConferenceTerminalList.action";
  url_ = url;

  QString param = "conferenceId=" + conference_cid;
  QNetworkRequest request = GetPostRequest(url_);

  get_terminals_reply_ = network_access_manager_.post(request, param.toLocal8Bit());
  connect(get_terminals_reply_, &QNetworkReply::finished,
          this, &HttpMsgManager::HandleTerminalListReplySlot);
}

void HttpMsgManager::DoLogin() {
  QString host = GetHttpHost();
  //QString username = QString::fromStdString(profile->username());
  //QString password = QString::fromStdString(profile->password());
  // TODO: 暂时只能用此账号密码登陆
  QString username = QString::fromStdString("scut_http");
  QString password = QString::fromStdString("123456");

  QString url = "http://";
  url += host;
  url += "/ConferenceControl-war/terminal/terminalAuthenticate.action";
  QString param = 
    QString("terminalAccount=%1&password=%2").arg(username).arg(password);

  url_ = url;
  QNetworkRequest request = GetPostRequest(url_);

  login_reply_ = network_access_manager_.post(request, param.toLocal8Bit());
  connect(login_reply_, &QNetworkReply::finished,
          this, &HttpMsgManager::LoginFinishedSlot);
}

QString HttpMsgManager::GetHttpHost() {
  auto profile = RunningProfile::getInstance();
  QString host = QString::fromStdString(profile->realm());
  int port = profile->http_port();
  return port == 0 ? host : host + ":" + QString::number(port);
}

QNetworkRequest HttpMsgManager::GetPostRequest( const QUrl &url ) {
  QNetworkRequest request;
  request.setUrl(url);
  request.setHeader(
    QNetworkRequest::ContentTypeHeader, 
    "application/x-www-form-urlencoded");

  auto cookies = network_access_manager_.cookieJar()->cookiesForUrl(url);
  for (auto &cookie : cookies) {
    request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookie));
  }
  
  return request;
}

void HttpMsgManager::LoginFinishedSlot() {
  bool has_error = false;
  if (login_reply_->error()) {
    QMessageBox::information(
      nullptr, 
      tr("HTTP"),
      tr("Load failed: %1.").arg(login_reply_->errorString()));

  } else {
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(login_reply_->readAll(), &jsonError);
    if(jsonError.error != QJsonParseError::NoError) { 
      QMessageBox::information(
        nullptr, 
        tr("HTTP"),
        tr("JSON format error: %1.").arg(jsonError.errorString()));

    } else {
      QJsonObject jobj = json.object();
      if (!jobj.contains("type") || jobj["type"].toString() != "success") {
        QMessageBox::information(
          nullptr, 
          tr("HTTP"),
          tr("Login failed: %1.").arg(
            jobj["detail"].isUndefined() ? "NULL" : jobj["detail"].toString()));

      } else {
        has_login_ = true;
      }
    }
  }

  login_reply_->deleteLater();
  login_reply_ = nullptr;
  processing_mutex_.unlock();

  RequestTerminalListSlot(cid_);
}

void HttpMsgManager::HandleTerminalListReplySlot() {
  if (get_terminals_reply_->error()) {
    QMessageBox::information(
      nullptr,
      tr("HTTP"),
      tr("Load failed: %1.").arg(get_terminals_reply_->errorString()));
  } else {
    // TODO:暂不清楚会话有效期，故每次请求都登陆一次
    // TODO: 添加心跳，维持session有效
    has_login_ = false;

    QByteArray byte = get_terminals_reply_->readAll();
    ParseTerminalList(byte);
  }

  get_terminals_reply_->deleteLater();
  get_terminals_reply_ = nullptr;

  processing_mutex_.unlock();
}

void HttpMsgManager::ParseTerminalList( const QByteArray &json_data ) {
  QJsonParseError parse_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(json_data, &parse_error);
  if(parse_error.error != QJsonParseError::NoError) {  
    return;
  }
  if(!json_doc.isObject()) {  
    return;
  }

  QJsonObject obj = json_doc.object();
  if(!obj.contains("terminals") || !obj["terminals"].isArray()) {  
    return;
  }

  QJsonArray terminal_array = obj["terminals"].toArray();
  const int total_count = terminal_array.size();

  TerminalList terminals;
  // 预留total_count * 4的内存空间
  // 因为一个终端往往有4个虚拟终端
  terminals.reserve(total_count * 4);

  for (int row = 0; row < total_count; ++row) {
	if (!terminal_array[row].isArray()) {
      continue;
	}

	QJsonArray virtual_termial_array = terminal_array[row].toArray(); 
	for (int index = 0; index < virtual_termial_array.size(); ++index) {
	  if (!virtual_termial_array[index].isObject()) {
		continue;
	  }
      
      QJsonObject terminal_obj = virtual_termial_array[index].toObject();
      TerminalPointer terminal(new TerminalDescription());
      ParseSingleTerminal(terminal_obj, terminal);
	  terminals.push_back(terminal);
	}
  }
  emit NotifyTerminalListReceivedSignal(terminals);
}

void HttpMsgManager::ParseSingleTerminal(
    const QJsonObject &object, TerminalPointer terminal) {
  terminal->total_terminal_num = object["count"].toString().toInt();
  terminal->dns = object["dns"].toString();

  terminal->uri = object["uri"].toString();

  terminal->virtual_uri = object["virtualUri"].toString();
  if (terminal->virtual_uri.isEmpty()) {
    terminal->virtual_uri = 
      TerminalHelper::ConstructDefaultVirtualURI(terminal->uri);
  }

  terminal->virtual_index = 
    TerminalHelper::GetVirtualIndex(terminal->virtual_uri);

  terminal->virtual_count = object["virtualTerminalCount"].toString().toInt();
  if (terminal->virtual_count <= 0) {
    terminal->virtual_count = 1;
  }

  terminal->name = object["name"].toString();
  if (terminal->virtual_index > 0) {
    terminal->name += QString("(%1)").arg(terminal->virtual_index);
  }

  terminal->location = object["location"].toString();
  terminal->model = object["model"].toString();
  terminal->version = object["version"].toString();
  terminal->multicast_address = object["multicastAddress"].toString();
  terminal->sdp = object["sdp"].toString();
  terminal->has_video = terminal->sdp.contains("m=video");
  terminal->has_audio = terminal->sdp.contains("m=audio");
  terminal->video_port = object["videoPort"].toString().toInt();
  terminal->audio_port = object["audioPort"].toString().toInt();
  terminal->small_video_port = object["smallVideoPort"].toString().toInt();
  terminal->is_available = object["status"].toString() == "Available";
  terminal->is_speaking = object["isSpeaking"].toString() == "true";
  terminal->is_handup = object["isHandUp"].toString() == "true";
  terminal->is_main_speaker = object["isSpeakerTerminal"].toString() == "true";
  terminal->is_chairman_terminal = object["isChairManTerminal"].toString() == "true";
}






