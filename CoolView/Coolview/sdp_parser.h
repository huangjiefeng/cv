#ifndef SDP_PARSER_H
#define SDP_PARSER_H

#include <QString>

#include "floor_control_info_description.h"

class SdpParser
{
public:
  SdpParser(const QString &sdp)
    : sdp_(sdp), start_pos_(0), end_pos_(0)
  {
  }

  int GetFloorControlInfo(FloorControlInfoPointer &info);


private:
  QString GetNextLine();
  
  int GetBfcpPortAndTransport(
    const QString &line,
    uint16_t &port, 
    QString &transport) const;

  int GetFloorIdAndLabel(
    const QString &line,
    uint16_t &floor_id, 
    int &media_label) const;

  int GetConferenceId(
    const QString &line,
    QString &conference_id) const;

  int GetLabel(
    const QString &line,
    int &label) const;
  

private:
  const QString &sdp_;
  int start_pos_;
  int end_pos_;
};


#endif //SDP_PARSER_H