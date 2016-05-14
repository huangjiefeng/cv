#include "sip_msg_builder.h"

#include <cassert>
#include "log/Log.h"

SipMsgBuilder::SipMsgBuilder( const QString &command, const QString &type ) {
  doc_.appendChild(
    doc_.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

  command_element_ = doc_.createElement("coolview");
  command_element_.setAttribute("command", command);
  if (!type.isEmpty()) {
    command_element_.setAttribute("type", type);
  }
  doc_.appendChild(command_element_);
}

SipMsgBuilder::~SipMsgBuilder() {
}

bool SipMsgBuilder::AddParameter( const QString &name, const QVariant &value )
{
  QDomElement parameter_element = doc_.createElement("parameter");
  QMetaType::Type var_type = static_cast<QMetaType::Type>(value.type());
  AddArgument(parameter_element, "name", name);
  if (var_type == QMetaType::Int) {
    AddArgument(parameter_element, "type", QString("int"));
    AddArgument(parameter_element, "value", value.toInt());
  } else if (var_type == QMetaType::Float) {
    AddArgument(parameter_element, "type", QString("float"));
    AddArgument(parameter_element, "value", value.toFloat());
  } else if (var_type == QMetaType::QString) {
    AddArgument(parameter_element, "type", QString("text"));
    AddArgument(parameter_element, "value", value.toString());
  } else {
    // Unsupported config value type
    auto msg = QString("Unsupported config value type: ") + value.typeName();
    CV_LOG_ERROR(msg.toLocal8Bit().constData());
    return false;
  }
  command_element_.appendChild(parameter_element);
  return true;
}

void SipMsgBuilder::AddArgument( QDomElement &parent, const QString &name, const QString &value )
{
  assert(!command_element_.isNull());
  QDomElement argument_element = doc_.createElement(name);
  argument_element.appendChild(doc_.createTextNode(value));
  parent.appendChild(argument_element);
}

void SipMsgBuilder::AddArgument( QDomElement &parent, const QString &name, bool value )
{
  assert(!command_element_.isNull());
  QDomElement argument_element = doc_.createElement(name);
  argument_element.appendChild(doc_.createTextNode(value ? "true" : "false"));
  parent.appendChild(argument_element);
}

