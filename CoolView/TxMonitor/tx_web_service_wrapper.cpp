#include "tx_web_service_wrapper.h"

#include <QThread>

#include "record_db_interface.h"
#include "record_sqlite_db.h"
#include "dbus/txController/client/TxControllerIf.h"
#include "dbus/txController/common/TxControllerServiceCommon.h"
#include "util/ini/TxConfig.h"
#include "Util.h"
#include "log.h"

// 重要！
// 必须将如下定义加入生成的soapCvTxService.h头部，或定义在项目配置的预处理器中
// #ifndef WITH_PURE_VIRTUAL
// #define WITH_PURE_VIRTUAL
// #endif
#include "tx/WebService/cvtx/tx_soap.nsmap"
#include "tx/WebService/cvtx/tx_soapCvTxService.h"

using namespace tx_soap;

/************************************************************************/
/* Implementation of CvTxService                                       */
/*   继承一次再修改，这避免每次用工具生成CvTxService后，都要修改       */
/*   CvTxService这个类                                                 */
/************************************************************************/

class CvTxWebServiceImpl : public CvTxService {
public:
  CvTxWebServiceImpl(ILogger *logger, QObject *parent = nullptr);
  CvTxWebServiceImpl(const struct soap&);
  virtual ~CvTxWebServiceImpl();

  //重写有关方法
  CvTxService *copy();
  int run(int port);

  //增加接口-注意线程安全性
  void stop();
  void wait_to_quit();

  //
  //web service接口实现
  //
  //数据库查询
  int QueryDBConference( int offset, int count, 
    ns__QueryDBConferenceResult *result ) override;
  int QueryDBTerminal( int cid, 
    ns__QueryDBTerminalResult *result ) override;
  int QueryDBEpisodeDates( int cid, std::string uri,  
    ns__QueryDBEpisodeDatesResult *result ) override;
  int QueryDBEpisode( int cid, std::string uri, int offset, int count, 
    ns__QueryDBEpisodeResult *result ) override;
  int QueryDBEpisodeByTime( int cid, std::string uri, LONG64 start, LONG64 end, 
    ns__QueryDBEpisodeResult *result ) override;
  int QueryDBView( LONG64 eid, 
    ns__QueryDBViewResult *result ) override;
  //录像控制
  int StartRecord(ns__StartRecordParam param, int *result) override;
  int StopRecord(ns__StopRecordParam param, int *result) override;
  int GetRecordStatus(std::string focus, std::string uri, 
    ns__GetRecordStatusResult *result) override;

protected:
  bool stop_;
  HANDLE service_stopped_event_;

  std::shared_ptr<IRecordDB> db_;
  QString rec_path_prefix_;

  TxControllerIf *tx_controller_;
  RecordFocusStatus record_focus_status_;
  time_t record_focus_status_update_time_;

  ILogger *logger_;
};

CvTxWebServiceImpl::CvTxWebServiceImpl(const struct soap& s)
  : CvTxService(s),
    stop_(true),
    service_stopped_event_(NULL),
    record_focus_status_update_time_(0),
    logger_(nullptr)
{
  service_stopped_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
  db_.reset(new RecordSqliteDB);
  rec_path_prefix_ = CTxConfig::getInstance().GetUploadPath();
}

CvTxWebServiceImpl::CvTxWebServiceImpl(ILogger *logger, QObject *parent)
  : CvTxService(SOAP_XML_INDENT | SOAP_XML_STRICT),
    stop_(true),
    service_stopped_event_(NULL),
    logger_(logger)
{
  service_stopped_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
  db_.reset(new RecordSqliteDB(parent));
  tx_controller_ = new TxControllerIf(TXCONTROLLER_SERVICE_NAME, 
    TXCONTROLLER_SERVICE_OBJECT_PATH, QDBusConnection::sessionBus(), parent);
}

CvTxWebServiceImpl::~CvTxWebServiceImpl()
{
  CloseHandle(service_stopped_event_);
}

CvTxService *CvTxWebServiceImpl::copy()
{	
  CvTxWebServiceImpl *dup = SOAP_NEW_COPY(CvTxWebServiceImpl(*(struct soap*)this));
  return dup;
}

int CvTxWebServiceImpl::run(int port)
{
  stop_ = false;
  ResetEvent(service_stopped_event_);

  if (soap_valid_socket(this->master) || soap_valid_socket(bind(NULL, port, 100)))
  {	
    while (!stop_)
    {	
      if (!soap_valid_socket(accept())) {
        Sleep(1);
        continue;
      }
      if (serve()) {
        continue;
      }
    }
    soap_destroy(this);
    soap_end(this);
  }

  SetEvent(service_stopped_event_);
  stop_ = true; //ensure flag set
  return this->error;
}

void CvTxWebServiceImpl::stop()
{
  stop_ = true;
}

void CvTxWebServiceImpl::wait_to_quit()
{
  WaitForSingleObject(service_stopped_event_, INFINITE);
}

int CvTxWebServiceImpl::QueryDBConference( int offset, int count, ns__QueryDBConferenceResult *result )
{
  result->offset = offset;

  db_->SelectConferenceCount(result->total);
  if (result->total < 0) {
    result->total = 0;
    return SOAP_OK;
  }

  IRecordDB::ConferenceEntityList conferences;
  if (db_->SelectConferences(offset, count, conferences) != 0) {
    return SOAP_OK;
  }

  for (auto conference_entity : conferences) {
    ns__DBConferenceNode conference_node;
    conference_node.cid = conference_entity->cid;
    conference_node.title = conference_entity->title.toLocal8Bit().constData();
    result->conferences.push_back(conference_node);
  }

  return SOAP_OK;
}

int CvTxWebServiceImpl::QueryDBEpisodeDates( 
  int cid, 
  std::string uri, 
  ns__QueryDBEpisodeDatesResult *result )
{
  QString quri = uri.c_str();
  IRecordDB::EpisodeDateList dates;
  if (db_->SelectEpisodeDate(cid, quri, dates) != 0) {
    return SOAP_OK;
  }

  for (auto &d : dates) {
    result->dates.push_back(d);
  }
  return SOAP_OK;
}

int CvTxWebServiceImpl::QueryDBTerminal( int cid, ns__QueryDBTerminalResult *result )
{
  IRecordDB::TerminalEntityList terminals;
  if (db_->SelectTerminal(cid, terminals) != 0) {
    return SOAP_OK;
  }

  for (auto terminal_entity : terminals) {
    ns__DBTerminalNode terminal_node;
    terminal_node.uri = terminal_entity->uri.toLocal8Bit().constData();
    terminal_node.name = terminal_entity->name.toLocal8Bit().constData();
    result->terminals.push_back(terminal_node);
  }
  return SOAP_OK;
}

int CvTxWebServiceImpl::QueryDBEpisode( 
  int cid, 
  std::string uri, 
  int offset, 
  int count, 
  ns__QueryDBEpisodeResult *result )
{
  QString quri = uri.c_str();
  result->offset = offset;

  if (db_->SelectEpisodeCount(cid, quri, result->total) != 0) {
    return SOAP_OK;
  }

  IRecordDB::EpisodeEntityList episodes;
  if (db_->SelectEpisodes(cid, quri, offset, count, episodes) != 0) {
    return SOAP_OK;
  }

  for (auto episode_entity : episodes) {
    ns__DBEpisodeNode episode_node;
    episode_node.id = episode_entity->id;
    episode_node.terminal_uri = episode_entity->terminal_uri.toLocal8Bit().constData();
    episode_node.requester = episode_entity->requester.toLocal8Bit().constData();
    episode_node.title = episode_entity->title.toLocal8Bit().constData();
    episode_node.start_time = episode_entity->start_time;
    episode_node.duration = episode_entity->duration;
    episode_node.participants = episode_entity->participants.toLocal8Bit().constData();
    episode_node.keywords = episode_entity->keywords.toLocal8Bit().constData();
    episode_node.description = episode_entity->description.toLocal8Bit().constData();
    result->episodes.push_back(episode_node);
  }

  return SOAP_OK;
}

int CvTxWebServiceImpl::QueryDBEpisodeByTime( 
  int cid, 
  std::string uri, 
  LONG64 start, 
  LONG64 end, 
  ns__QueryDBEpisodeResult *result)
{
  QString quri = uri.c_str();
  result->offset = -1; //这种查询方式下offset无效

  if (db_->SelectEpisodeCount(cid, quri, result->total) != 0) {
    return SOAP_OK;
  }

  IRecordDB::EpisodeEntityList episodes;
  if (db_->SelectEpisodesByInterval(cid, quri, start, end, episodes) != 0) {
    return SOAP_OK;
  }

  for (auto episode_entity : episodes) {
    ns__DBEpisodeNode episode_node;
    episode_node.id = episode_entity->id;
    episode_node.terminal_uri = episode_entity->terminal_uri.toLocal8Bit().constData();
    episode_node.requester = episode_entity->requester.toLocal8Bit().constData();
    episode_node.title = episode_entity->title.toLocal8Bit().constData();
    episode_node.start_time = episode_entity->start_time;
    episode_node.duration = episode_entity->duration;
    episode_node.participants = episode_entity->participants.toLocal8Bit().constData();
    episode_node.keywords = episode_entity->keywords.toLocal8Bit().constData();
    episode_node.description = episode_entity->description.toLocal8Bit().constData();
    result->episodes.push_back(episode_node);
  }

  return SOAP_OK;
}

int CvTxWebServiceImpl::QueryDBView( LONG64 eid, ns__QueryDBViewResult *result )
{
  IRecordDB::ViewEntityList views;
  if (0 != db_->SelectViewsByEpisode(eid, views)) {
    return SOAP_OK;
  }
  for (auto view_entity : views) {
    ns__DBViewNode view_node;
    view_node.id = view_entity->vid;
    view_node.camera_index = view_entity->camera_index;
    view_node.start_time = view_entity->start_time;
    view_node.duration = view_entity->duration;
    view_node.format = view_entity->media_format.toLocal8Bit().constData();
    QString relative_path = view_entity->file;
    if (relative_path.indexOf(":") != -1) {
      //找到盘符，是绝对路径，转为相对路径
      relative_path = GetRelativePath(rec_path_prefix_, view_entity->file);
    }
    if (relative_path.indexOf(":") == 1) {
      //路径转换失败将保留绝对路径，避免将包含盘符的路径返回给客户端，暴漏服务器文件系统
      relative_path = "/file_not_within_legal_path.tmp";
    } else {
      relative_path.replace("\\", "/");
    }
    view_node.file_path = relative_path.toLocal8Bit().constData();
    view_node.status = view_entity->status;
    result->views.push_back(view_node);
  }
  return SOAP_OK;
}

int CvTxWebServiceImpl::StartRecord( ns__StartRecordParam param, int *result )
{
  StartRecordParam start_param;
  start_param.focus = QString::fromLocal8Bit(param.focus.c_str());
  start_param.target_uri = QString::fromLocal8Bit(param.target_uri.c_str());
  start_param.requester_uri = QString::fromLocal8Bit(param.requester_uri.c_str());
  start_param.title = QString::fromLocal8Bit(param.title.c_str());
  start_param.participants = QString::fromLocal8Bit(param.participants.c_str());
  start_param.keywords = QString::fromLocal8Bit(param.keywords.c_str());
  start_param.description = QString::fromLocal8Bit(param.description.c_str());
  QDBusPendingReply<int> reply = tx_controller_->ControlRecord(TXRecordOp_Start, DBusParamToByteArray(start_param));

  reply.waitForFinished();
  *result = reply.value();
  return SOAP_OK;
}

int CvTxWebServiceImpl::StopRecord( ns__StopRecordParam param, int *result )
{
  StopRecordParam stop_param;
  stop_param.focus = QString::fromLocal8Bit(param.focus.c_str());
  stop_param.target_uri = QString::fromLocal8Bit(param.target_uri.c_str());
  QDBusPendingReply<int> reply = tx_controller_->ControlRecord(TXRecordOp_Stop, DBusParamToByteArray(stop_param));

  reply.waitForFinished();
  *result = reply.value();
  return SOAP_OK;
}

int CvTxWebServiceImpl::GetRecordStatus( std::string focus, std::string uri, 
                                      ns__GetRecordStatusResult *result )
{
  time_t now = time(NULL);
  if (now - record_focus_status_update_time_ >= 1) {
    //如果上次更新间隔大于1s，则更新状态
    QString qfocus = QString::fromLocal8Bit(focus.c_str());
    QString quri = QString::fromLocal8Bit(uri.c_str());
    QDBusPendingReply<QByteArray> reply = tx_controller_->GetRecordStatus(qfocus, quri);
    reply.waitForFinished();
    if (reply.isError()) {
      LOG_ERROR_EX(logger_, "GetRecordStatus error");
      return SOAP_OK;
    }

    record_focus_status_update_time_ = now;
    record_focus_status_ = DBusParamFromByteArray<RecordFocusStatus>(reply.value());
  }

  for (auto &t : record_focus_status_.terminals_status) {
    ns__RecordTerminalStatus status;
    status.vuri = t.vuri.toLocal8Bit().constData();
    status.name = t.name.toLocal8Bit().constData();
    status.status = t.status.toLocal8Bit().constData();
    status.requester_uri = t.requester_uri.toLocal8Bit().constData();
    status.virtual_count = t.virtual_count;
    status.duration = t.duration;
    status.recorded_bytes = t.recorded_bytes;
    result->terminals.push_back(status);
  }
  return SOAP_OK;
}


/************************************************************************/
/* VodServiceWrapper class                                              */
/************************************************************************/

TxWebServiceWrapper::TxWebServiceWrapper(ILogger *logger, QObject *parent)
  : QObject(parent),
    logger_(logger)
{

}

TxWebServiceWrapper::~TxWebServiceWrapper()
{

}

void TxWebServiceWrapper::StopService()
{
  LOG_INFO_EX(logger_, "stopping service...");
  if (service_) {
    service_->stop();
    service_->wait_to_quit();
  }
  LOG_INFO_EX(logger_, "service stopped");
}

void TxWebServiceWrapper::HandleStartServiceSlot(int port)
{
  LOG_INFO_EX(logger_, "running service in thread[%x], listening port=%d\n", 
    QThread::currentThreadId(), port);
  service_.reset(new CvTxWebServiceImpl(logger_, this));
  service_->run(port);
}
