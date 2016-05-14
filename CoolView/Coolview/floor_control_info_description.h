#ifndef FLOOR_CONTROL_INFO_DESCRIPTION_H
#define FLOOR_CONTROL_INFO_DESCRIPTION_H

#include <cstdint>
#include <memory>

#include <QObject>
#include <QString>
#include <QVector>

struct ChairDescription
{
  uint16_t user_id;
  QVector<uint16_t> floor_ids;
};

typedef QVector<ChairDescription> ChairDescriptionList;

struct FloorDescription
{
  bool isAudio() const { return media_sdp.startsWith("m=audio"); }
  bool isVideo() const { return media_sdp.startsWith("m=video"); }
  bool isApplication() const { return media_sdp.startsWith("m=application"); }

  uint16_t floor_id;
  int media_label;
  QString media_sdp; // m=audio ..., m=video ..., m=application ppt
};

typedef QVector<FloorDescription> FloorDescriptionList;

struct FloorControlInfoDescription
{
  QString cid;
  QString ip;
  uint16_t port;
  QString transport;
  ChairDescriptionList chairs;
  FloorDescriptionList floors;
};

typedef std::shared_ptr<FloorControlInfoDescription> FloorControlInfoPointer;

class FloorControlInfoRegister
{
public:
  static void RegisterMetaType()
  {
    qRegisterMetaType<FloorControlInfoDescription>("FloorControlInfoDescription");
    qRegisterMetaType<FloorControlInfoPointer>("FloorControlInfoPointer");
  }
};


#endif // !FLOOR_CONTROL_INFO_DESCRIPTION_H