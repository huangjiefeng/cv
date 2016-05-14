#include "record_db_controller.h"

#include "dbus/txMonitor/common/TxMonitorServiceCommon.h"
#include "record_sqlite_db.h"


RecordDBController::RecordDBController()
{
  logger_.reset(CreateLogger());
  logger_->SetModuleName("rec_db");

  db_.reset(new RecordSqliteDB);
}

RecordDBController::~RecordDBController()
{
}

void RecordDBController::HandleRecordComplete( 
  const RecordCompleteParam &param)
{
  LOG_PRINTF_EX(logger_, "insert record for [%s][%d]", 
    qPrintable(param.terminal.uri), param.view.camera_index);

  //检查会议是否存在
  ConferencePointer conference = FindConference(param.conference.cid);
  if (!conference) {
    conference = CreateConference(param);
    if (!conference) {
      return;
    }
  }

  //再检查终端是否存在
  TerminalPointer terminal = FindTerminal(conference, param.terminal.uri);
  if (!terminal) {
    terminal = CreateTerminal(param);
    if (!terminal) {
      return;
    }
  }

  //然后检查分段是否存在
  EpisodePointer episode = FindEpisode(terminal, param.episode.id);
  if (!episode) {
    episode = CreateEpisode(terminal, param);
    if (!episode) {
      return;
    }
  }

  //最后插入视频记录
  if (CreateView(episode, param) != 0) {
    //处理错误
  }
}

RecordDBController::ConferencePointer RecordDBController::FindConference(
  int cid)
{
  if (db_cache_.contains(cid)) {
    //缓存命中
    return db_cache_[cid];
  }

  //否则在数据库查找
  IRecordDB::ConferenceEntityPointer conference_entity = 
    db_->SelectConference(cid);
  if (!conference_entity) {
    return nullptr;
  }

  //找到则放入缓存并返回
  ConferencePointer conference = CacheConference(conference_entity);

  return conference;
}

RecordDBController::ConferencePointer RecordDBController::CreateConference( 
  const RecordCompleteParam &param)
{
  //检查重复性
  ConferencePointer conference = FindConference(param.conference.cid);
  if (conference) {
    LOG_ERROR_EX(logger_, "insert conference exist, cid=%d", 
      param.conference.cid);
    return conference;
  }

  //创建实体
  IRecordDB::ConferenceEntityPointer conference_entity(
    new IRecordDB::ConferenceEntity);
  conference_entity->cid = param.conference.cid;
  conference_entity->title = param.conference.title;
  conference_entity->start_time = param.conference.start_time;
  conference_entity->description = param.conference.description;

  IRecordDB::TerminalEntityPointer terminal_entity(
    new IRecordDB::TerminalEntity);
  terminal_entity->uri = param.terminal.uri;
  terminal_entity->name = param.terminal.name;

  conference_entity->terminal_list.insert(
    terminal_entity->uri, terminal_entity);

  //插入数据库
  if (db_->InsertConference(conference_entity) != 0) {
    LOG_ERROR_EX(logger_, "insert conference entity failed, cid=%d", 
      conference_entity->cid);
    return nullptr;
  }

  //写缓存
  conference = CacheConference(conference_entity);
  return conference;
}

RecordDBController::ConferencePointer RecordDBController::CacheConference( 
  const IRecordDB::ConferenceEntityPointer conference_entity)
{
  if (!conference_entity) return nullptr;

  ConferencePointer conference(new ConferenceNode);
  conference->cid = conference_entity->cid;
  for (auto &terminal_entity : conference_entity->terminal_list) {
    TerminalPointer terminal(new TerminalNode);
    terminal->uri = terminal_entity->uri;
    conference->terminals.insert(terminal->uri, terminal);
  }

  db_cache_.insert(conference->cid, conference); //如果原来存在，会覆盖旧的值
  return conference;
}

RecordDBController::TerminalPointer RecordDBController::FindTerminal( 
  ConferencePointer conference, const QString &uri )
{
  if (conference->terminals.contains(uri)) {
    //缓存命中
    return conference->terminals[uri];
  }

  //terminal是Conference中的字段，缓存没有就真没有了，
  //除非缓存没更新，但只有这里写数据库，故缓存与数据库是保持一致的
  return nullptr;
}

RecordDBController::TerminalPointer RecordDBController::CreateTerminal( 
  const RecordCompleteParam &param )
{
  //获取会议实体
  IRecordDB::ConferenceEntityPointer conference_entity = 
    db_->SelectConference(param.conference.cid);
  if (!conference_entity) {
    LOG_ERROR_EX(logger_, "insert terminal failed, conference not exist");
    return nullptr;
  }

  //创建终端实体
  IRecordDB::TerminalEntityPointer terminal_entity(new IRecordDB::TerminalEntity);
  terminal_entity->uri = param.terminal.uri;
  terminal_entity->name = param.terminal.name;

  conference_entity->terminal_list.insert(terminal_entity->uri, terminal_entity);

  //更新会议实体
  if (db_->UpdateConference(conference_entity) != 0) {
    LOG_ERROR_EX(logger_, "update conference failed");
    return nullptr;
  }

  //更新缓存
  //注意不能直接将会议实体转成缓存覆盖原来的，只能将终端插入字典，不然会破坏现有的缓存状态！
  ConferencePointer conference = FindConference(conference_entity->cid);
  TerminalPointer terminal = CacheTerminal(conference, terminal_entity);

  //返回
  return terminal;
}

RecordDBController::TerminalPointer RecordDBController::CacheTerminal( 
  ConferencePointer conference, 
  const IRecordDB::TerminalEntityPointer terminal_entity)
{
  if (!conference) {
    return nullptr;
  }

  TerminalPointer terminal(new TerminalNode);
  terminal->uri = terminal_entity->uri;

  conference->terminals.insert(terminal->uri, terminal); //如果原来存在，会覆盖旧的值

  return terminal;
}

RecordDBController::EpisodePointer RecordDBController::FindEpisode( 
  TerminalPointer terminal, 
  long long id )
{
  if (!terminal) {
    return nullptr;
  }

  EpisodeList::iterator episode_it = std::find_if(
    terminal->episodes.begin(),
    terminal->episodes.end(), 
    [&id](const EpisodePointer &e){
      return e->id == id;
    });
  if (episode_it != terminal->episodes.end()) {
    //缓存命中
    return (*episode_it);
  }

  //否则在数据库查找
  IRecordDB::EpisodeEntityPointer episode_entity = 
    db_->SelectEpisode(id);
  if (!episode_entity) {
    return nullptr;
  }

  //找到则放入缓存并返回
  EpisodePointer episode = CacheEpisode(terminal, episode_entity);

  return episode;
}

RecordDBController::EpisodePointer RecordDBController::CreateEpisode( 
  TerminalPointer terminal, 
  const RecordCompleteParam &param )
{
  //检查重复性
  EpisodePointer episode = FindEpisode(terminal, param.episode.id);
  if (episode) {
    LOG_ERROR_EX(logger_, "insert episode exist, id=%d", 
      param.episode.id);
    return episode;
  }

  //创建实体
  IRecordDB::EpisodeEntityPointer episode_entity(
    new IRecordDB::EpisodeEntity);
  episode_entity->id = param.episode.id;
  episode_entity->cid = param.conference.cid;
  episode_entity->terminal_uri = param.terminal.uri;
  episode_entity->requester = param.episode.requester;
  episode_entity->title = param.episode.title;
  episode_entity->start_time = param.view.start_time/1000; //以view的时间为准，注意view的时间精度是ms
  episode_entity->duration = 0; //插入view时再计算
  episode_entity->participants = param.episode.participants;
  episode_entity->keywords = param.episode.keywords;
  episode_entity->description = param.episode.description;

  //插入数据库
  if (db_->InsertEpisode(episode_entity) != 0) {
    LOG_ERROR_EX(logger_, "insert episode entity failed, id=%d", 
      episode_entity->id);
    return nullptr;
  }

  //写缓存
  episode = CacheEpisode(terminal, episode_entity);
  return episode;
}

RecordDBController::EpisodePointer RecordDBController::CacheEpisode( 
  TerminalPointer terminal, 
  const IRecordDB::EpisodeEntityPointer episode_entity)
{
  if (!episode_entity) return nullptr;

  EpisodeList::iterator episode_it = std::find_if(
    terminal->episodes.begin(),
    terminal->episodes.end(), 
    [&episode_entity](const EpisodePointer &e){
      return e->id == episode_entity->id;
  });
  if (episode_it != terminal->episodes.end() ||
      terminal->episodes.size() > 3) { 
    //如果出现重复要求缓存某集记录，则清除之前的所有记录，重新插入
    //或者缓存超过3集，也清除重来啦(3集是根据底层分集时的特性估计的，不建议小于这个值)
    terminal->episodes.clear();
  }

  EpisodePointer episode(new EpisodeNode);
  episode->id = episode_entity->id;

  terminal->episodes.push_back(episode);
  return episode;
}

int RecordDBController::CreateView( 
  EpisodePointer episode, 
  const RecordCompleteParam &param )
{
  //view不验证重复性，默认每个新来的都是新纪录

  //创建实体
  IRecordDB::ViewEntityPointer view_entity(new IRecordDB::ViewEntity);
  view_entity->vid = 0; // 插入时忽略该值的
  view_entity->episode_id = episode->id;
  view_entity->camera_index = param.view.camera_index;
  view_entity->start_time = param.view.start_time;
  view_entity->duration = param.view.duration;
  view_entity->media_format = param.view.media_format;
  view_entity->file = param.view.file;

  //插入数据库
  if (db_->InsertView(view_entity) != 0) {
    LOG_ERROR_EX(logger_, "insert view entity failed, episode_id=%d, file=%s", 
      episode->id, qPrintable(view_entity->file));
    return -1;
  }

  //重新计算episode的duration
  IRecordDB::EpisodeEntityPointer episode_entity = 
    db_->SelectEpisode(episode->id);
  if (episode_entity) {
    bool need_update = false;
    if (episode_entity->start_time > view_entity->start_time/1000) {
      //用更早的view起始时间替代原时间，注意view的时间精度是ms，episode是s
      episode_entity->start_time = view_entity->start_time/1000;
      need_update = true;
    }
    //注意view的时间精度是ms，episode是s
    long long dur = 
      (view_entity->start_time + view_entity->duration)/1000 - episode_entity->start_time;
    if (episode_entity->duration < dur) {
      episode_entity->duration = dur;
      need_update = true;
    }
    if (need_update) db_->UpdateEpisode(episode_entity);
  }

  return 0;
}
