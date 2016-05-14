#ifndef HTTPWRAPPER_H
#define HTTPWRAPPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkCookie>

#include "util/CVMsgParser.h"

QT_BEGIN_NAMESPACE
class QNetworkReply;
class QNetworkRequest;

QT_END_NAMESPACE

#include "terminal_description.h"

class HttpWrapper : public QObject
{
	Q_OBJECT

public:
	HttpWrapper();
	~HttpWrapper();

	void RequestTerminalList(const QString cid);

Q_SIGNALS:
	void RecvTerminalListSignal(TerminalVector);
    void NotifyTerminalListReceivedSignal(const TerminalList &terminals);

public Q_SLOTS:
    void RequestTerminalListSlot(const QString &cid);

private:
	void DoLogin();
	QNetworkRequest GetPostRequest(QUrl url);

	QString GetHttpHost();
	void ParseTerminalList(QByteArray data);
    void ParseTerminalListFromJsonArray(QByteArray data);
    void ParseSingleTerminalFromJson(const QJsonObject &object);

private slots:
	void LoginFinishedSlot();
	void GetTerminalListFinishedSlot();
    void HandleTerminalListReplySlot();

private:
	QUrl url_;
	QNetworkAccessManager qnam_;
	QNetworkReply *loginReply_; //TODO:并发时该如何保存指针？
	QNetworkReply *getTerminalReply_;

	bool login_;
	QString cid_;
	
};

#endif // HTTPWRAPPER_H
