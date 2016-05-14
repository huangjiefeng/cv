#ifndef RECORD_DB_INTERFACE_H
#define RECORD_DB_INTERFACE_H

#include <memory>
#include <QtCore>

class IRecordDB : public QObject
{
  Q_OBJECT

public:
  IRecordDB(QObject *parent) : QObject(parent) {}
  virtual ~IRecordDB() {}

public:
  //实体数据结构，成员与数据库存储的字段对应
  //终端实体没有对应的表，其被放在会议表中
  struct TerminalEntity { 
    QString uri;
    QString name;
  };
  typedef std::shared_ptr<TerminalEntity> TerminalEntityPointer;
  typedef QHash<QString, TerminalEntityPointer> TerminalDict;
  typedef QVector<TerminalEntityPointer> TerminalEntityList;

  struct ConferenceEntity {
    int cid; // primary key
    QString title;
    QString start_time;
    QString description;
    TerminalDict terminal_list;
  };
  typedef std::shared_ptr<ConferenceEntity> ConferenceEntityPointer;
  typedef QVector<ConferenceEntityPointer> ConferenceEntityList;

  struct EpisodeEntity {
    long long id; // primary key
    int cid;
    QString terminal_uri;
    QString requester; // the uri initiate the recording
    QString title;
    long long start_time; // seconds since 1970-1-1 00:00:00 UTC
    long long duration;
    QString participants;
    QString keywords;
    QString description;
  };
  typedef std::shared_ptr<EpisodeEntity> EpisodeEntityPointer;
  typedef QVector<EpisodeEntityPointer> EpisodeEntityList;
  typedef QVector<short> EpisodeDateList; //有数据的日期，以1970-1-1以来的天数表示

  struct ViewEntity {
    int vid;
    long long episode_id;
    short camera_index;
    long long start_time; // ms since 1970-1-1 00:00:00.000 UTC
    long long duration; // ms
    QString media_format;
    QString file;
    int status;
  };
  typedef std::shared_ptr<ViewEntity> ViewEntityPointer;
  typedef QVector<ViewEntityPointer> ViewEntityList;
  
public:
  virtual int InsertConference(const ConferenceEntityPointer &c) = 0;
  virtual int UpdateConference(const ConferenceEntityPointer &c) = 0;
  virtual ConferenceEntityPointer SelectConference(int cid) = 0;
  virtual int SelectConferenceCount(int &count) = 0;
  virtual int SelectConferences(int offset, int limit, 
    ConferenceEntityList &list) = 0;

  virtual int SelectTerminal(int cid, TerminalEntityList &list) = 0;

  virtual int InsertEpisode(const EpisodeEntityPointer &e) = 0;
  virtual int UpdateEpisode(const EpisodeEntityPointer &e) = 0;
  virtual EpisodeEntityPointer SelectEpisode(long long id) = 0;
  virtual int SelectEpisodeCount(int cid, const QString &uri, 
    int &count) = 0;
  virtual int SelectEpisodes(int cid, const QString &uri,
    int offset, int limit, EpisodeEntityList &list) = 0;
  virtual int SelectEpisodeDate(int cid, const QString &uri,
    EpisodeDateList &dates) = 0;
  //arg start & end should be second since 1970-1-1 00:00
  virtual int SelectEpisodesByInterval(int cid, const QString &uri,
    long long start, long long end, EpisodeEntityList &list) = 0;

  virtual int InsertView(const ViewEntityPointer &v) = 0;
  virtual ViewEntityPointer SelectViewByFilePath(const QString &path) = 0;
  virtual int SelectViewsByEpisode(long long eid, ViewEntityList &list) = 0;
  virtual int UpdateView(const ViewEntityPointer &v) = 0;
  virtual int UpdateViewSetFlagsByPath(const QString &path, int flag) = 0;
  virtual int UpdateViewResetFlagsByPath(const QString &path, int flag) = 0;
};

#endif
