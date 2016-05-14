#pragma once

#include <QtCore/QtCore>

#define TX_MONITOR_SERVICE_NAME "com.dcampus.coolview.txMonitor"
#define TX_MONITOR_SERVICE_OBJECT_PATH "/com/dcampus/coolview/txMonitor"


//位开关表示的文件状态
enum RecordedFileStatus
{
  RecFileStat_Deleted = 1,
  RecFileStat_Uploaded = 1 << 1,
};

enum RecStatType
{
  RecStat_FileClose,
};

class RecordCompleteParam
{
public:
  struct {
    int cid;
    QString title;
    QString start_time; //TODO:以后与服务器协商改成longlong或标准时间格式
    QString description;
  } conference;

  struct {
    QString uri;
    QString name;
  } terminal;

  struct {
    long long id; // the episode id
    QString requester; // user who request the recording
    QString title;
    long long start_time; // seconds since 1970-1-1 00:00:00 UTC
    QString participants;
    QString keywords;
    QString description;
  } episode;

  struct {
    int camera_index;
    long long start_time; // ms since 1970-1-1 00:00:00.000 UTC
    long long duration; // ms
    QString media_format;
    QString file;
  } view;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator>>(QDataStream &in, RecordCompleteParam &d)
  {
    in >> d.conference.cid >> d.conference.title >> d.conference.start_time >> d.conference.description
      >> d.terminal.uri >> d.terminal.name
      >> d.episode.id >> d.episode.requester >> d.episode.title >> d.episode.start_time 
      >> d.episode.participants >> d.episode.keywords >> d.episode.description
      >> d.view.camera_index >> d.view.start_time >> d.view.duration >> d.view.media_format >> d.view.file;
    return in;
  }
  friend QDataStream &operator<<(QDataStream &out, RecordCompleteParam &d)
  {
    out << d.conference.cid << d.conference.title << d.conference.start_time << d.conference.description
      << d.terminal.uri << d.terminal.name
      << d.episode.id << d.episode.requester << d.episode.title << d.episode.start_time 
      << d.episode.participants << d.episode.keywords << d.episode.description
      << d.view.camera_index << d.view.start_time << d.view.duration << d.view.media_format << d.view.file;
    return out;
  }
#endif

};
