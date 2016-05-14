#include "sip_msg_parser.h"

#include <QDomNodeList>
#include <QRegExp>

#include "log/Log.h"
#include "profile/RunningProfile.h"
#include "sdp_parser.h"

namespace details
{

QString GetNextLine(const QString &str, int start_pos, int &end_pos)
{
  end_pos = str.indexOf("\n", start_pos);
  int len = end_pos == -1 ? end_pos : end_pos - start_pos; // not include the '\n'
  return str.mid(start_pos, len);
}
   
} // namespace details

SipMsgParser::SipMsgParser() {

}

SipMsgParser::~SipMsgParser()
{

}

bool SipMsgParser::Init( const QString &xml_content ) {
  if (!doc_.setContent(xml_content)) {
    return false;
  }

  QDomNodeList node_list = doc_.elementsByTagName("coolview");
  if (node_list.isEmpty())
    return false;

  QDomNode first_node = node_list.at(0);
  coolview_element_ = first_node.toElement();
  return coolview_element_.isNull() ? false : true;
}

QString SipMsgParser::GetCommandType() const {
  if (coolview_element_.isNull()) {
    return QString();
  }
  return coolview_element_.attribute("command");
}

void SipMsgParser::GetConferenceList( ConferenceList &conferences ) const {  
  if (coolview_element_.isNull()) {
    return;
  }

  QDomElement conference_element = coolview_element_.firstChildElement();
  ConferenceDictionary conference_dict;
  for (; !conference_element.isNull(); 
       conference_element = conference_element.nextSiblingElement()) {
    QString uri = GetFirstChildElementValue(conference_element, "uri");
    ConferenceDictionary::iterator it = conference_dict.find(uri);
    if (it == conference_dict.end()) {
      ConferencePointer conference(new ConferenceDescription());
      GetConferenceDescription(conference_element, conference);
      conference_dict.insert(uri, conference);
    }
  }

  conferences.append(conference_dict.values());
}

void SipMsgParser::GetConferenceDescription(const QDomElement &conference_element, 
                                            ConferencePointer conference ) const {
  conference->uri = GetFirstChildElementValue(conference_element, "uri");
  conference->cid = GetFirstChildElementValue(conference_element, "cid");
  conference->title = GetFirstChildElementValue(conference_element, "title");
  conference->description = GetFirstChildElementValue(conference_element, "description");
  conference->host_uri = GetFirstChildElementValue(conference_element, "hostUri");
  conference->control_mode = GetFirstChildElementValue(conference_element, "controlMode");
  conference->join_mode = GetFirstChildElementValue(conference_element, "joinMode");
  conference->join_password = GetFirstChildElementValue(conference_element, "joinPassword");
  conference->start_time = GetFirstChildElementValue(conference_element, "startTime");
  conference->duration = GetFirstChildElementValue(conference_element, "duration");
  conference->multicast_address = GetFirstChildElementValue(conference_element, "multicastAddress");
  conference->ppt_port = GetFirstChildElementValue(conference_element, "pptPort").toInt();
  conference->chairman= GetFirstChildElementValue(conference_element, "chairman");
  conference->level = GetFirstChildElementValue(conference_element, "level");
  QString use_FCS = GetFirstChildElementValue(coolview_element_, "useFCS");
  conference->use_FCS = use_FCS == "true";
}

void SipMsgParser::GetTerminalList( TerminalList &terminals ) const {
  if (coolview_element_.isNull()) {
    return;
  }
  
  QDomElement terminals_element = coolview_element_.firstChildElement("terminals");
  if (terminals_element.isNull()) {
    return;
  }

  TerminalDict terminal_dict;
  QDomElement terminal_element = terminals_element.firstChildElement("terminal");
  for (; !terminal_element.isNull();
       terminal_element = terminal_element.nextSiblingElement()) {   
    QString vuri = GetFirstChildElementValue(terminal_element, "virtualuri");
    if (vuri.isEmpty()) {
      QString uri = GetFirstChildElementValue(terminal_element, "uri");
      vuri = TerminalHelper::ConstructDefaultVirtualURI(uri);
    }

    TerminalDict::iterator it = terminal_dict.find(vuri);
    if (it == terminal_dict.end()) {
      TerminalPointer terminal(new TerminalDescription());
      GetTerminalDesciption(terminal_element, terminal);
      terminal_dict.insert(vuri, terminal);
    }
  }
  terminals.append(terminal_dict.values());
}

void SipMsgParser::GetLoginTerminals( TerminalList &terminals ) const {
  TerminalDict terminal_dict;
  QDomElement terminal_element = coolview_element_.firstChildElement("terminal");
  for (; !terminal_element.isNull();
    terminal_element = terminal_element.nextSiblingElement()) {
      TerminalPointer terminal(new TerminalDescription());
      GetTerminalDesciption(terminal_element, terminal);
      terminal_dict.insert(terminal->virtual_uri, terminal);
  }
  terminals.append(terminal_dict.values());
}

void SipMsgParser::GetTerminalDesciption(const QDomElement &terminal_element,
                                         TerminalPointer terminal ) const {
  terminal->total_terminal_num = 
    GetFirstChildElementValue(terminal_element, "count").toInt();
  terminal->dns = GetFirstChildElementValue(terminal_element, "dns");  
                                           
  terminal->uri = GetFirstChildElementValue(terminal_element, "uri");

  terminal->virtual_uri = 
    GetFirstChildElementValue(terminal_element, "virtualuri");

  if (terminal->virtual_uri.isEmpty()) {
    terminal->virtual_uri = 
      TerminalHelper::ConstructDefaultVirtualURI(terminal->virtual_uri);
  }
  
  terminal->virtual_index = 
    TerminalHelper::GetVirtualIndex(terminal->virtual_uri);

  terminal->virtual_count = 
    GetFirstChildElementValue(terminal_element, "virtualTerminalCount").toInt();
  if (terminal->virtual_count <= 0) {
    terminal->virtual_count = 1;
  }

  terminal->id = GetFirstChildElementValue(terminal_element, "id").toInt();
  terminal->name = GetFirstChildElementValue(terminal_element , "name");
  if (terminal->virtual_count > 0) {
    terminal->name += QString("(%1)").arg(terminal->virtual_index + 1);
  }

  terminal->location = GetFirstChildElementValue(terminal_element , "location");
  terminal->model = GetFirstChildElementValue(terminal_element, "model");
  terminal->version = GetFirstChildElementValue(terminal_element , "version");

  terminal->multicast_address = 
    GetFirstChildElementValue( terminal_element , "multicastAddress" );
  
  terminal->sdp = GetFirstChildElementValue(terminal_element, "sdp");
  terminal->has_video = terminal->sdp.contains("m=video");
  terminal->has_audio = terminal->sdp.contains("m=audio");

  terminal->video_port = 
    GetFirstChildElementValue(terminal_element, "videoPort").toInt();
  terminal->audio_port = 
    GetFirstChildElementValue(terminal_element, "audioPort").toInt();
  terminal->small_video_port = 
    GetFirstChildElementValue(terminal_element,"smallVideoPort").toInt();

  terminal->is_available = 
    GetFirstChildElementValue(terminal_element , "status") == "Available";
  terminal->is_speaking = 
    GetFirstChildElementValue(terminal_element, "isSpeaking") == "true";
  terminal->is_handup = 
    GetFirstChildElementValue(terminal_element,"isHandUp") == "true";
  terminal->is_main_speaker = 
    GetFirstChildElementValue(terminal_element,"isSpeakerTerminal") == "true";
  terminal->is_chairman_terminal = 
    GetFirstChildElementValue(terminal_element,"isChairmanTerminal") == "true";
}

void SipMsgParser::GetLoginPermissionInfo( LoginPermissionInfo &info ) const {
  info.user_uri = GetFirstChildElementValue(coolview_element_, "uri");
  info.dscp = GetFirstChildElementValue(coolview_element_, "dscp").toInt();
  info.rate = GetFirstChildElementValue(coolview_element_, "rate").toInt();
}

void SipMsgParser::GetLoginRejectionInfo( LoginRejectionInfo &info ) const {
  info.user_uri = GetFirstChildElementValue(coolview_element_, "uri");
  info.description = GetFirstChildElementValue(coolview_element_, "description");
  info.suggested_rate = 
    GetFirstChildElementValue(coolview_element_, "suggestedRate").toInt();
}

void SipMsgParser::GetQoSServerInfo( QoSServerInfo &info ) const {
  info.sip_uri = GetFirstChildElementValue(coolview_element_, "sipUri");
  info.ip = GetFirstChildElementValue(coolview_element_, "ip");
  info.tcp_port = 
    GetFirstChildElementValue(coolview_element_, "simulatetestTCPPort").toInt();
  info.simulate_test_udp_port = 
    GetFirstChildElementValue(coolview_element_, "simulatetestUDPPort").toInt();
  info.operation_udp_port = 
    GetFirstChildElementValue(coolview_element_, "operationUDPPort").toInt();
}

ConfigDict SipMsgParser::GetConfigDict() const
{
  ConfigDict config;
  if (coolview_element_.isNull()) {
    return config;
  }
  
  QDomNode node = coolview_element_.firstChild();
  while (!node.isNull()) {
    if (node.isElement()) {
      auto element = node.toElement();
      if (element.tagName() == "parameter") {
        QString name_str = GetFirstChildElementValue(element, "name");
        QString type_str = GetFirstChildElementValue(element, "type").trimmed();
        QString value_str = GetFirstChildElementValue(element, "value");
        if (type_str == "text") {
          config.insert(name_str, value_str);
        } else if (type_str == "int") {
          config.insert(name_str, value_str.toInt());
        } else if (type_str == "float") {
          config.insert(name_str, value_str.toFloat());
        } else {
          // unknown type
          QString msg = QString("unsupported value type: ") + type_str;
          CV_LOG_ERROR(msg.toLocal8Bit().constData());
        }
      }
    }
    node = node.nextSibling();
  }
  return config;
}

void SipMsgParser::GetFloorControlInfo( FloorControlInfoPointer &info ) const
{
  assert(info);
  info->ip = GetFirstChildElementValue(coolview_element_, "ip");

  // parse sdp
  QString sdp = GetFirstChildElementValue(coolview_element_, "sdp");
  SdpParser parser(sdp);
  parser.GetFloorControlInfo(info);

  // parse chair list
  auto chair_list_element = coolview_element_.firstChildElement("chairList");
  if (!chair_list_element.isNull()) {
    auto chair_element = chair_list_element.firstChildElement();
    for (; !chair_element.isNull(); 
         chair_element = chair_element.nextSiblingElement()) {
      ChairDescription chairInfo;
      GetChairInfo(chair_element, chairInfo);
      info->chairs.push_back(chairInfo);
    }
  }
}

void SipMsgParser::GetChairInfo(const QDomElement &chair_element, 
                                ChairDescription &chair_info) const
{
  chair_info.user_id = static_cast<uint16_t>(
    GetFirstChildElementValue(chair_element, "userId").toInt());
  auto floor_list_element = chair_element.firstChildElement("floorList");
  if (!floor_list_element.isNull()) {
    auto floor_element = floor_list_element.firstChildElement();
    for (; !floor_element.isNull(); 
         floor_element = floor_element.nextSiblingElement()) {
      if (floor_element.tagName() == "floorId") {
        uint16_t floor_id = floor_element.firstChild().nodeValue().toInt();
        chair_info.floor_ids.push_back(floor_id);
      }
      //uint16_t floor_id = static_cast<uint16_t>(
      //  GetFirstChildElementValue(floor_element, "floorId").toInt());
    }
  }
}

