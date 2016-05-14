#ifndef HTTP_MSG_MANAGER_H
#define HTTP_MSG_MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMutex>

#include "terminal_description.h"

QT_BEGIN_NAMESPACE

class QNetworkRequest;
class QNetworkReply;

QT_END_NAMESPACE


class HttpMsgManager : public QObject {
  Q_OBJECT
public:
  HttpMsgManager(QObject *parent);
  ~HttpMsgManager();

Q_SIGNALS:
  void NotifyTerminalListReceivedSignal(const TerminalList &terminals);

public Q_SLOTS:
  void RequestTerminalListSlot(const QString &conference_cid);

private Q_SLOTS:
  void LoginFinishedSlot();
  void HandleTerminalListReplySlot();

private:
  void DoLogin();
  QString GetHttpHost();
  QNetworkRequest GetPostRequest(const QUrl &url);

  void ParseTerminalList(const QByteArray &json_data);
  void ParseSingleTerminal(
    const QJsonObject &json_object, 
    TerminalPointer terminal);

private:
  QUrl url_;
  QNetworkAccessManager network_access_manager_;
  QNetworkReply *login_reply_; //TODO:并发时该如何保存指针？
  QNetworkReply *get_terminals_reply_;

  QMutex processing_mutex_;
  bool has_login_;
  QString cid_;
};

#endif // HTTP_MSG_MANAGER_H
