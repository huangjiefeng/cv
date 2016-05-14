#ifndef SIP_MSG_BUILDER_H
#define SIP_MSG_BUILDER_H

#include <QDomDocument>
#include <QDomElement>
#include <QVariant>

// 构造的消息样例：
//<?xml version=1.0 encoding=utf-8?>
//<coolview command="command参数" type="type参数">
//  <name参数>value参数</name参数>
//   ...
//</coolview>
// 在返回字符串时，选择-1参数，去掉了换行和element之间的空格

class SipMsgBuilder {
public:
  SipMsgBuilder(const QString &command, const QString &type = QString());
  ~SipMsgBuilder();

  bool AddParameter(const QString &name, const QVariant &value);

  template <typename T>
  void AddArgument(const QString &name, const T &value) {
    AddArgument(command_element_, name, value);
  }

  QString GetMsg() const {
    return doc_.toString(-1);
  }

private:
  template <typename T>
  void AddArgument(QDomElement &parent, const QString &name, const T &value) 
  {
    QDomElement argument_element = doc_.createElement(name);
    argument_element.appendChild(doc_.createTextNode(QString::number(value)));
    parent.appendChild(argument_element);
  }

  void AddArgument(QDomElement &parent, const QString &name, const QString &value);
  void AddArgument(QDomElement &parent, const QString &name, bool value);

  QDomDocument doc_;
  QDomElement command_element_;
};


#endif // SIP_MSG_BUILDER_H
