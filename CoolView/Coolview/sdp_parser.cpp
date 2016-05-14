#include "sdp_parser.h"

#include <cassert>

#include <QMap>
#include <QRegExp>

#include <log/Log.h>

int SdpParser::GetFloorControlInfo( FloorControlInfoPointer &info )
{
  QMap<int, FloorDescription> floors;
  int res = 0;
  while (end_pos_ != -1) {
    auto line = GetNextLine().trimmed();
    if (line.isEmpty()) {
      continue;
    }
    if (line.startsWith("m=application ") && line.endsWith("/BFCP *")) {
      res = GetBfcpPortAndTransport(line, info->port, info->transport);
      if (res == -1) {
        CV_LOG_ERROR("Cannot parser Bfcp application from SDP");
        break;
      }

    } else if (line.startsWith("a=confid:")) {
      res = GetConferenceId(line, info->cid);
      if (res == -1) {
        CV_LOG_ERROR("Cannot parser Bfcp confid from SDP");
        break;
      }

    } else if (line.startsWith("a=floorid:")) {
      FloorDescription floor;
      int res = GetFloorIdAndLabel(line, floor.floor_id, floor.media_label);
      if (res == -1) {
        CV_LOG_ERROR("Cannot parser Bfcp floorid from SDP");
        break;

      } else {
        floors.insert(floor.media_label, floor);
      }

    } else if (line.startsWith("m=audio") || 
               line.startsWith("m=video") || 
               line.startsWith("m=application")) {
      if (end_pos_ == -1) break;
      auto next_line = GetNextLine().trimmed();
      int label = 0;
      res = GetLabel(next_line, label);
      if (res == -1) {
        CV_LOG_ERROR("Cannot parser Bfcp label from SDP");
        break;

      } else {
        auto it = floors.find(label);
        if (it == floors.end()) continue;
        (*it).media_sdp = line;
      }
    }
  }
  if (res != -1) {
    for (auto it = floors.begin(); it != floors.end(); ++it) {
      info->floors.push_back(*it);
    }
  }

  return res;
}

QString SdpParser::GetNextLine()
{
  assert(end_pos_ != -1);
  end_pos_ = sdp_.indexOf("\n", start_pos_);
  int len = end_pos_ == -1 ? end_pos_ : end_pos_ - start_pos_; // not include the '\n'
  auto line = sdp_.mid(start_pos_, len);
  start_pos_ = end_pos_ + 1;
  return line;
}

int SdpParser::GetBfcpPortAndTransport(const QString &line, 
                                       uint16_t &port,
                                       QString &transport) const
{
  QRegExp rx("\\s*(\\d+)\\s+([\\w/]+)/BFCP \\*$");
  rx.indexIn(line, strlen("m=application "));
  if (rx.captureCount() == 2) {
    port = static_cast<uint16_t>(rx.cap(1).toInt());
    transport = rx.cap(2);
    return 0;
  }
  return -1;
}

int SdpParser::GetFloorIdAndLabel(const QString &line,
                                   uint16_t &floor_id, 
                                   int &media_label) const
{
  QRegExp rx("\\s*(\\d+)\\s+mstrm:(\\d+)\\s*");
  rx.indexIn(line, strlen("a=floorid:"));
  if (rx.captureCount() == 2) {
    floor_id = static_cast<uint16_t>(rx.cap(1).toInt());
    media_label = rx.cap(2).toInt();
    return 0;
  }
  return -1;
}

int SdpParser::GetConferenceId(const QString &line, 
                               QString &conference_id) const
{
  QRegExp rx("\\s*(\\d+)\\s*");
  rx.indexIn(line, strlen("a=confid:"));
  if (rx.captureCount() == 1) {
    conference_id = rx.cap(1);
    return 0;
  }
  return -1;
}

int SdpParser::GetLabel( const QString &line, int &label ) const
{
  QRegExp label_rx("^a=label:\\s*(\\d+)\\s*");
  label_rx.indexIn(line);
  if (label_rx.captureCount() == 1) {
    label = label_rx.cap(1).toInt();
    return 0;
  }
  return -1;
}


