#include "record_sqlite_db.h"

#include "util.h"


#define CONFERENCE_TABLE "conference"
#define EPISODE_TABLE "episode"
#define VIEW_TABLE "view"

const unsigned short kConferenceTitleLen = 64;
const unsigned short kTerminalUriLen = 255; //服务器数据库是255
const unsigned short kEpisodeTitleLen = 255;

inline QString TruncateString(const QString &str, int limit)
{
  if (str.length() > limit) {
    return str.left(limit);
  }
  return str;
}


//保存线程与QSqlDatabase实例关系
QHash<Qt::HANDLE, QSqlDatabase> db_dict__;


RecordSqliteDB::RecordSqliteDB(QObject *parent)
  : IRecordDB(parent)
{
  logger_.reset(CreateLogger());
  logger_->SetModuleName("db");

  InitialDB();
}

RecordSqliteDB::~RecordSqliteDB()
{

}

void RecordSqliteDB::InitialDB()
{
  // get db file path
  char app_dir[MAX_PATH] = {0};
  GetApplicationDirPath(app_dir, nullptr);
  QString db_path = 
    QString::fromLocal8Bit(app_dir) + "\\txcache\\rec_repo.db";
  MakeParentDir(db_path.toStdString());

  // open or create db connection
  Qt::HANDLE tid = QThread::currentThreadId();
  if (db_dict__.contains(tid)) {
    db_ = db_dict__[tid];
    LOG_INFO_EX(logger_, "open connection in thread[%x]", tid);
  } else {
    db_ = QSqlDatabase::addDatabase("QSQLITE", 
      QString("TX_DB_%1").arg((int)tid));
    db_.setDatabaseName(db_path);
    if (!db_.open()){  
      HandleQueryError("open db failed"); 
      return;
    }
    db_dict__[tid] = db_;
    LOG_INFO_EX(logger_, "create connection in thread[%x]", tid);
  }

  // create query
  query_ = QSqlQuery(db_);

  // create table if needed
  QStringList table_list = db_.tables();
  if (!table_list.contains(CONFERENCE_TABLE)) {
    CreateConferenceTable();
  }
  if (!table_list.contains(EPISODE_TABLE)) {
    CreateEpisodeTable();
  }
  if (!table_list.contains(VIEW_TABLE)) {
    CreateViewTable();
  }
}

void RecordSqliteDB::CreateConferenceTable()
{
  QString sql = "create table %1("
    "cid integer,"
    "title varchar(%2),"
    "start_time varchar(32),"
    "description text,"
    "terminals text);";
  sql = sql = sql.arg(CONFERENCE_TABLE).arg(kConferenceTitleLen);

  if (!query_.exec(sql)) {
    HandleQueryError("create conference table failed");
  }
}

void RecordSqliteDB::CreateEpisodeTable()
{
  QString sql = "create table %1("
    "id bigint," // sqlite中int都是64bit
    "cid integer,"
    "terminal varchar(%2),"
    "requester varchar(%2),"
    "title varchar(%3),"
    "start_time bigint,"
    "duration bigint,"
    "participants text,"
    "keywords text,"
    "description text);";
  sql = sql.arg(EPISODE_TABLE).arg(kTerminalUriLen).arg(kEpisodeTitleLen);

  if (!query_.exec(sql)) {
    HandleQueryError("create episode table failed");
  }
}

void RecordSqliteDB::CreateViewTable()
{
  QString sql = "create table %1("
    "vid integer primary key autoincrement," //自增长的主键，SQLite3中为64bit
    "eid bigint," // 确保与episode_id一致
    "camera_index tinyint,"
    "start_time bigint,"
    "duration bigint,"
    "media_format text,"
    "file text,"
    "status integer default 0);";
  sql = sql.arg(VIEW_TABLE);

  if (!query_.exec(sql)) {
    HandleQueryError("create view table failed");
  }
}

int RecordSqliteDB::InsertConference( const ConferenceEntityPointer &c )
{
  QString sql = "INSERT INTO %1 (cid, title, start_time, description, terminals) "
    "VALUES (:id, :title, :start, :description, :terminals)";
  sql = sql.arg(CONFERENCE_TABLE);

  QString title = TruncateString(c->title, kConferenceTitleLen);
  QString terminal_json = TerminalDictToJson(c->terminal_list);

  query_.prepare(sql);
  query_.bindValue(":id", c->cid);
  query_.bindValue(":title", title);
  query_.bindValue(":start", c->start_time);
  query_.bindValue(":description", c->description);
  query_.bindValue(":terminals", terminal_json);

  if (!query_.exec()) {
    HandleQueryError("insert conference failed");
    return -1;
  }
  return 0;
}

int RecordSqliteDB::UpdateConference( const ConferenceEntityPointer &c )
{
  QString sql = "UPDATE %1 SET title=:title, start_time=:start, "
    "description=:description, terminals=:terminals "
    "WHERE cid=:id";
  sql = sql.arg(CONFERENCE_TABLE);

  QString title = TruncateString(c->title, kConferenceTitleLen);
  QString terminal_json = TerminalDictToJson(c->terminal_list);

  query_.prepare(sql);
  query_.bindValue(":id", c->cid);
  query_.bindValue(":title", title);
  query_.bindValue(":start", c->start_time);
  query_.bindValue(":description", c->description);
  query_.bindValue(":terminals", terminal_json);

  if (!query_.exec()) {
    HandleQueryError("update conference failed");
    return -1;
  }
  return 0;
}

IRecordDB::ConferenceEntityPointer RecordSqliteDB::SelectConference( int cid )
{
  //不建议使用SELECT *，其返回的字段的顺序是不确定的，取值要多做一步判断
  //SELECT中给出字段，则可用索引取值，索引从0开始
  QString sql = "SELECT cid, title, start_time, description, terminals "
    "FROM %1 WHERE cid=%2";
  sql = sql.arg(CONFERENCE_TABLE).arg(cid);

  if (!query_.exec(sql)) {
    HandleQueryError("select conference failed");
    return nullptr;
  }

  if (query_.next()) {
    ConferenceEntityPointer conference_entity(new ConferenceEntity);
    conference_entity->cid = query_.value(0).toInt();
    conference_entity->title = query_.value(1).toString();
    conference_entity->start_time = query_.value(2).toString();
    conference_entity->description = query_.value(3).toString();
    QString json = query_.value(4).toString();
    IRecordDB::TerminalDict &dict = conference_entity->terminal_list;
    TerminalContainerFromJson(json, [&dict](TerminalEntityPointer t){
        dict.insert(t->uri, t);
      });
    return conference_entity;
  }
  return nullptr;
}

int RecordSqliteDB::SelectConferenceCount(int &count)
{
  QString sql = "SELECT COUNT(cid) FROM %1";
  sql = sql.arg(CONFERENCE_TABLE);

  if (!query_.exec(sql)) {
    HandleQueryError("select conference count failed");
    return -1;
  }

  if (query_.next()) {
    count = query_.value(0).toInt();
  }
  return 0;
}

int RecordSqliteDB::SelectConferences(
  int offset, 
  int limit, 
  ConferenceEntityList &list)
{
  QString sql = "SELECT cid, title, start_time, description, terminals "
    "FROM %1";
  sql = sql.arg(CONFERENCE_TABLE);

  if (limit > 0) {
    sql += QString(" LIMIT %1 OFFSET %2").arg(limit).arg(offset);
  }

  if (!query_.exec(sql)) {
    HandleQueryError("select conferences failed");
    return -1;
  }

  while (query_.next()) {
    ConferenceEntityPointer conference_entity(new ConferenceEntity);
    conference_entity->cid = query_.value(0).toInt();
    conference_entity->title = query_.value(1).toString();
    conference_entity->start_time = query_.value(2).toString();
    conference_entity->description = query_.value(3).toString();
    //出于效率的考虑，该方法不返回终端列表
    /*QString json = query_.value(4).toString();
    TerminalDictFromJson(json, conference_entity->terminal_list);*/
    list.push_back(conference_entity);
  }
  return 0;
}

int RecordSqliteDB::SelectTerminal(int cid, TerminalEntityList &list)
{
  QString sql = "SELECT terminals FROM %1 WHERE cid=%2";
  sql = sql.arg(CONFERENCE_TABLE).arg(cid);

  if (!query_.exec(sql)) {
    HandleQueryError("select terminals failed");
    return -1;
  }

  if (query_.next()) {
    QString json = query_.value(0).toString();
    TerminalContainerFromJson(json, [&list](TerminalEntityPointer t){
        list.push_back(t);
      });
  }
  return 0;
}

int RecordSqliteDB::InsertEpisode( const EpisodeEntityPointer &e )
{
  QString sql = "INSERT INTO %1 (id, cid, terminal, requester, title, "
    "start_time, duration, participants, keywords, description) "
    "VALUES (:id, :cid, :terminal, :requester, :title, "
    ":start, :duration, :participants, :keywords, :description)";
  sql = sql.arg(EPISODE_TABLE);
  QString title = TruncateString(e->title, kEpisodeTitleLen);

  query_.prepare(sql);
  query_.bindValue(":id", e->id);
  query_.bindValue(":cid", e->cid);
  query_.bindValue(":terminal", e->terminal_uri);
  query_.bindValue(":requester", e->requester);
  query_.bindValue(":title", title);
  query_.bindValue(":start", e->start_time);
  query_.bindValue(":duration", e->duration);
  query_.bindValue(":participants", e->participants);
  query_.bindValue(":keywords", e->keywords);
  query_.bindValue(":description", e->description);

  if (!query_.exec()) {
    HandleQueryError("insert episode failed");
    return -1;
  }
  return 0;
}

int RecordSqliteDB::UpdateEpisode( const EpisodeEntityPointer &e )
{
  //注意id，cid，terminal_uri，requester不可更改
  QString sql = "UPDATE %1 SET title=:title, start_time=:start, duration=:dur, "
    "participants=:participants, keywords=:keywords, description=:description "
    "WHERE id=:id";
  sql = sql.arg(EPISODE_TABLE);
  QString title = TruncateString(e->title, kEpisodeTitleLen);

  query_.prepare(sql);
  query_.bindValue(":title", title);
  query_.bindValue(":start", e->start_time);
  query_.bindValue(":dur", e->duration);
  query_.bindValue(":participants", e->participants);
  query_.bindValue(":keywords", e->keywords);
  query_.bindValue(":description", e->description);
  query_.bindValue(":id", e->id);

  if (!query_.exec()) {
    HandleQueryError("update episode failed");
    return -1;
  }
  return 0;
}

IRecordDB::EpisodeEntityPointer RecordSqliteDB::SelectEpisode( long long id )
{
  QString sql = "SELECT id, cid, terminal, requester, title, "
    "start_time, duration, participants, keywords, description "
    "FROM %1 WHERE id=%2";
  sql = sql.arg(EPISODE_TABLE).arg(id);

  if (!query_.exec(sql)) {
    HandleQueryError("select episode failed");
    return nullptr;
  }

  if (query_.next()) {
    EpisodeEntityPointer episode_entity(new EpisodeEntity);
    episode_entity->id = query_.value(0).toLongLong();
    episode_entity->cid = query_.value(1).toInt();
    episode_entity->terminal_uri = query_.value(2).toString();
    episode_entity->requester = query_.value(3).toString();
    episode_entity->title = query_.value(4).toString();
    episode_entity->start_time = query_.value(5).toLongLong();
    episode_entity->duration = query_.value(6).toLongLong();
    episode_entity->participants = query_.value(7).toString();
    episode_entity->keywords = query_.value(8).toString();
    episode_entity->description = query_.value(9).toString();
    return episode_entity;
  }
  return nullptr;
}

int RecordSqliteDB::SelectEpisodeCount(
  int cid, 
  const QString &uri, 
  int &count)
{
  QString sql = "SELECT count(id) FROM %1 WHERE cid=%2";
  sql = sql.arg(EPISODE_TABLE).arg(cid);

  if (!uri.isEmpty()) {
    sql += QString(" AND terminal='%1'").arg(uri);
  }

  if (!query_.exec(sql)) {
    HandleQueryError("select episodes count failed");
    return -1;
  }

  if (query_.next()) {
    count = query_.value(0).toInt();
  }
  return 0;
}

int RecordSqliteDB::SelectEpisodes(
  int cid, 
  const QString &uri,
  int offset, 
  int limit, 
  EpisodeEntityList &list)
{
  QString sql = "SELECT id, cid, terminal, requester, title, "
    "start_time, duration, participants, keywords, description "
    "FROM %1 WHERE cid=%2";
  sql = sql.arg(EPISODE_TABLE).arg(cid);

  if (!uri.isEmpty()) {
    sql += QString(" AND terminal='%1'").arg(uri);
  }
  sql += " ORDER BY start_time DESC";
  if (limit > 0) {
    sql += QString(" LIMIT %1 OFFSET %2").arg(limit).arg(offset);
  }

  if (!query_.exec(sql)) {
    HandleQueryError("select episodes failed");
    return -1;
  }

  while (query_.next()) {
    EpisodeEntityPointer episode_entity(new EpisodeEntity);
    episode_entity->id = query_.value(0).toLongLong();
    episode_entity->cid = query_.value(1).toInt();
    episode_entity->terminal_uri = query_.value(2).toString();
    episode_entity->requester = query_.value(3).toString();
    episode_entity->title = query_.value(4).toString();
    episode_entity->start_time = query_.value(5).toLongLong();
    episode_entity->duration = query_.value(6).toLongLong();
    episode_entity->participants = query_.value(7).toString();
    episode_entity->keywords = query_.value(8).toString();
    episode_entity->description = query_.value(9).toString();
    list.push_back(episode_entity);
  }
  return 0;
}

int RecordSqliteDB::SelectEpisodeDate(
  int cid, 
  const QString &uri,
  EpisodeDateList &dates)
{
  //以降序方式返回1970-1-1以来的天数列表
  QString sql = "SELECT DISTINCT(start_time/86400) date "
    "FROM %1 WHERE cid=%2";
  sql = sql.arg(EPISODE_TABLE).arg(cid);
  if (!uri.isEmpty()) {
    sql += QString(" AND terminal='%1'").arg(uri);
  }
  sql += " ORDER BY date DESC";

  if (!query_.exec(sql)) {
    HandleQueryError("select episode interval failed");
    return -1;
  }

  while (query_.next()) {
    short date = (short)(query_.value(0).toLongLong());
    dates.push_back(date);
  }
  return 0;
}

int RecordSqliteDB::SelectEpisodesByInterval(
  int cid, 
  const QString &uri,
  long long start, 
  long long end,
  EpisodeEntityList &list)
{
  QString sql = "SELECT id, cid, terminal, requester, title, "
    "start_time, duration, participants, keywords, description "
    "FROM %1 WHERE cid=%2";
  sql = sql.arg(EPISODE_TABLE).arg(cid);

  if (!uri.isEmpty()) {
    sql += QString(" AND terminal='%1'").arg(uri);
  }
  if (start > 0) {
    sql += QString(" AND start_time>=%1").arg(start);
  }
  if (end > start) {
    sql += QString(" AND start_time<=%1").arg(end);
  }
  sql += " ORDER BY start_time DESC";

  if (!query_.exec(sql)) {
    HandleQueryError("select episodes by interval failed");
    return -1;
  }

  while (query_.next()) {
    EpisodeEntityPointer episode_entity(new EpisodeEntity);
    episode_entity->id = query_.value(0).toLongLong();
    episode_entity->cid = query_.value(1).toInt();
    episode_entity->terminal_uri = query_.value(2).toString();
    episode_entity->requester = query_.value(3).toString();
    episode_entity->title = query_.value(4).toString();
    episode_entity->start_time = query_.value(5).toLongLong();
    episode_entity->duration = query_.value(6).toLongLong();
    episode_entity->participants = query_.value(7).toString();
    episode_entity->keywords = query_.value(8).toString();
    episode_entity->description = query_.value(9).toString();
    list.push_back(episode_entity);
  }
  return 0;
}

int RecordSqliteDB::InsertView( const ViewEntityPointer &v )
{
  QString sql = "INSERT INTO %1 (eid, camera_index, "
    "start_time, duration, media_format, file) "
    "VALUES (:eid, :cam, :start, :dur, :format, :file)";
  sql = sql.arg(VIEW_TABLE);

  query_.prepare(sql);
  //query_.bindValue(":vid", v->vid); //不设置主键，自增长属性自动设置其值
  query_.bindValue(":eid", v->episode_id);
  query_.bindValue(":cam", v->camera_index);
  query_.bindValue(":start", v->start_time);
  query_.bindValue(":dur", v->duration);
  query_.bindValue(":format", v->media_format);
  query_.bindValue(":file", v->file);

  if (!query_.exec()) {
    HandleQueryError("insert view failed");
    return -1;
  }
  return 0;
}

IRecordDB::ViewEntityPointer RecordSqliteDB::SelectViewByFilePath(
  const QString &path)
{
  QString sql = "SELECT vid, eid, camera_index, "
    "start_time, duration, media_format, file, status "
    "FROM %1 WHERE file='%2'";
  sql = sql.arg(VIEW_TABLE).arg(GetDBFilePath(path));

  if (!query_.exec(sql)) {
    HandleQueryError("select view by path failed");
    return nullptr;
  }

  if (query_.next()) {
    ViewEntityPointer view_entity(new ViewEntity);
    view_entity->vid = query_.value(0).toInt();
    view_entity->episode_id = query_.value(1).toLongLong();
    view_entity->camera_index = (short)query_.value(2).toInt();
    view_entity->start_time = query_.value(3).toLongLong();
    view_entity->duration = query_.value(4).toLongLong();
    view_entity->media_format = query_.value(5).toString();
    view_entity->file = query_.value(6).toString();
    view_entity->status = query_.value(7).toInt();

    return view_entity;
  }
  return nullptr;
}

int RecordSqliteDB::SelectViewsByEpisode(
  long long eid, 
  ViewEntityList &list)
{
  QString sql = "SELECT vid, eid, camera_index, "
    "start_time, duration, media_format, file, status "
    "FROM %1 WHERE eid=%2 ORDER BY camera_index ASC";
  sql = sql.arg(VIEW_TABLE).arg(eid);

  if (!query_.exec(sql)) {
    HandleQueryError("select views failed");
    return -1;
  }

  while (query_.next()) {
    ViewEntityPointer view_entity(new ViewEntity);
    view_entity->vid = query_.value(0).toInt();
    view_entity->episode_id = query_.value(1).toLongLong();
    view_entity->camera_index = (short)query_.value(2).toInt();
    view_entity->start_time = query_.value(3).toLongLong();
    view_entity->duration = query_.value(4).toLongLong();
    view_entity->media_format = query_.value(5).toString();
    view_entity->file = query_.value(6).toString();
    view_entity->status = query_.value(7).toInt();

    list.push_back(view_entity);
  }
  return 0;
}

int RecordSqliteDB::UpdateView(const ViewEntityPointer &v)
{
  QString sql = "UPDATE %1 SET status=:status, file=:file "
    "WHERE vid=:vid";
  sql = sql.arg(VIEW_TABLE);

  query_.prepare(sql);
  query_.bindValue(":status", v->status);
  query_.bindValue(":file", v->file);
  query_.bindValue(":vid", v->vid);

  if (!query_.exec()) {
    HandleQueryError("update view failed");
    return -1;
  }
  return 0;
}

int RecordSqliteDB::UpdateViewSetFlagsByPath(const QString &path, int flags)
{
  QString sql = "UPDATE %1 SET status=status|%2 "
    "WHERE file='%3'";
  sql = sql.arg(VIEW_TABLE).arg(flags).arg(GetDBFilePath(path));

  if (!query_.exec(sql)) {
    HandleQueryError("set view flags failed");
    return -1;
  }
  return 0;
}

int RecordSqliteDB::UpdateViewResetFlagsByPath(const QString &path, int flags)
{
  QString sql = "UPDATE %1 SET status=status&(~%2) "
    "WHERE file='%3'";
  sql = sql.arg(VIEW_TABLE).arg(flags).arg(GetDBFilePath(path));

  if (!query_.exec(sql)) {
    HandleQueryError("reset view flags failed");
    return -1;
  }
  return 0;
}

QString RecordSqliteDB::TerminalDictToJson( const TerminalDict &dict )
{
  QJsonArray terminal_jarray;
  for (TerminalEntityPointer terminal : dict) {
    QJsonObject terminal_jobj;
    terminal_jobj.insert("uri", terminal->uri);
    terminal_jobj.insert("name", terminal->name);
    terminal_jarray.push_back(terminal_jobj);
  }
  
  QJsonDocument document;
  document.setArray(terminal_jarray);
  QByteArray byte_array = document.toJson(QJsonDocument::Compact);

  return QString(byte_array);
}

void RecordSqliteDB::TerminalContainerFromJson( 
  const QString &json, 
  std::function<void (TerminalEntityPointer)> func)
{
  QJsonParseError json_error;
  QJsonDocument parse_doucment = QJsonDocument::fromJson(
    json.toUtf8(), &json_error); 
  if(json_error.error == QJsonParseError::NoError) {
    if (parse_doucment.isArray()) {
      QJsonArray terminal_jarray = parse_doucment.array();
      for (auto value : terminal_jarray) {
        if (!value.isObject()) continue;
        QJsonObject terminal_jobj = value.toObject();
        TerminalEntityPointer t(new TerminalEntity);
        if (terminal_jobj.contains("uri")) 
          t->uri = terminal_jobj["uri"].toString();
        if (terminal_jobj.contains("name")) 
          t->name = terminal_jobj["name"].toString();
        func(t);
      }
    }
  }
}

void RecordSqliteDB::HandleQueryError( const QString &msg )
{
  //连查询语句也打出来似乎不太安全，不过现在不考虑，以方便排障为主
  LOG_ERROR_EX(logger_, "msg: %s\n\treason: %s\n\tsql: %s",
    qPrintable(msg), 
    qPrintable(query_.lastError().text()),
    qPrintable(query_.lastQuery()));
}
