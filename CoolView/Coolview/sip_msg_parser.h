#ifndef SIP_MSG_PARSER_H
#define SIP_MSG_PARSER_H

#include <QList>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "terminal_description.h"
#include "conference_description.h"
#include "qos_description.h"
#include "config_description.h"
#include "floor_control_info_description.h"

class SipMsgParser {
public:
  SipMsgParser();
  ~SipMsgParser();

  bool Init(const QString &xml_content);

  QString GetCommandType() const;
  void GetConferenceList(ConferenceList &conferences) const;
  void GetTerminalList(TerminalList &terminals) const;
  void GetLoginTerminals(TerminalList &terminals) const;

  void GetLoginPermissionInfo(LoginPermissionInfo &info) const;
  void GetLoginRejectionInfo(LoginRejectionInfo &info) const;
  void GetQoSServerInfo(QoSServerInfo &info) const;

  void GetFloorControlInfo(FloorControlInfoPointer &info) const;

  QString GetURI() const;
  QString GetTerminalURI() const;
  QString GetVirtualURI() const;
  QString GetPermission() const;
  QString GetType() const;
  QString GetReceiverIP() const;
  QString GetSenderIP() const;
  QString GetResult() const;
  QString GetTerminalCount() const;
  QString GetEnable() const;
  QString GetQosType() const;
  QString GetTextMessage() const;
  QString GetTxURI() const;
  int GetErrorCode() const;
  int GetTransactionId() const;
  int GetPort() const;
  ConfigDict GetConfigDict() const;

private:
  QString GetFirstChildElementValue(const QDomElement &parent,
                                    const QString &child_name) const;

  void GetConferenceDescription(
    const QDomElement &conference_element,
    ConferencePointer description) const;

  void GetTerminalDesciption( const QDomElement &terminal_element, TerminalPointer terminal) const;
  
  void GetChairInfo(const QDomElement &chair_element, ChairDescription &chair_info) const;

private:
  QDomDocument doc_;
  QDomElement coolview_element_;
};

inline QString SipMsgParser::GetURI() const {
  return GetFirstChildElementValue(coolview_element_, "uri");
}

inline QString SipMsgParser::GetTerminalURI() const {
  return GetFirstChildElementValue(coolview_element_, "terminalUri");
}

inline QString SipMsgParser::GetVirtualURI() const {
  return GetFirstChildElementValue(coolview_element_, "virtualUri");
}

inline QString SipMsgParser::GetPermission() const {
  return GetFirstChildElementValue(coolview_element_, "permission");
}

inline QString SipMsgParser::GetType() const {
  return GetFirstChildElementValue(coolview_element_, "type");
}

inline QString SipMsgParser::GetReceiverIP() const {
  return GetFirstChildElementValue(coolview_element_, "ip");
}


inline QString SipMsgParser::GetSenderIP() const {
  return GetFirstChildElementValue(coolview_element_, "ip");
}


inline QString SipMsgParser::GetResult() const {
  return GetFirstChildElementValue(coolview_element_, "result");
}

inline QString SipMsgParser::GetTerminalCount() const {
  return GetFirstChildElementValue(coolview_element_, "count");
}

inline QString SipMsgParser::GetEnable() const {
  return GetFirstChildElementValue(coolview_element_, "enable");
}

inline QString SipMsgParser::GetTextMessage() const {
  return GetFirstChildElementValue(coolview_element_, "message");
}

inline QString SipMsgParser::GetQosType() const {
  QString command_type = GetCommandType();
  if (command_type == "qos") {
    return coolview_element_.attribute("type");
  }
  return QString();
}

inline QString SipMsgParser::GetTxURI() const {
  return GetFirstChildElementValue(coolview_element_, "txUri");
}

inline int SipMsgParser::GetTransactionId() const {
  return GetFirstChildElementValue(coolview_element_, "transactionId").toInt();
}

inline int SipMsgParser::GetErrorCode() const {
  return GetFirstChildElementValue(coolview_element_, "errorCode").toInt();
}

inline int SipMsgParser::GetPort() const {
  return GetFirstChildElementValue(coolview_element_, "port").toInt();
}

inline QString SipMsgParser::GetFirstChildElementValue(
    const QDomElement &parent, const QString &child_name) const {
  QDomElement element = parent.firstChildElement(child_name);
  if (!element.isNull()) {
    QDomNode node = element.firstChild();
    return node.nodeValue();
  }
  return QString();
}

#endif // SIP_MSG_PARSER_H
