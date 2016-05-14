#include "tx_controller.h"

#include <ctime>

#include "util/MediaManager.h"
#include "util/ini/CVIniConfig.h"
#include "util/ini/TxConfig.h"
#include "config/RunningConfig.h"
#include "profile/RunningProfile.h"
#include "udp_report_sender.h"
#include "CoolviewCommon.h"
#include "tx_util.h"

#include "dbus/txController/service/TxControllerAdaptor.h"
#include "dbus/txController/common/TxControllerServiceCommon.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"
#include "dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h"
#include "dbus/txMonitor/common/TxMonitorServiceCommon.h"
#include "dbus/txMonitor/client/TxMonitorIf.h"

#include "display_controller_interface.h"
#include "msg_manager_interface.h"

using namespace tx_util;

TXController::TXController(QObject *parent, 
                           RunningProfile *profile,
                           RunningConfig *config)
    : ITXController(parent), 
      profile_(profile), 
      running_config_(config) {
  new TxControllerAdaptor(this);

  conference_controller_ = nullptr;
  media_manager_ = nullptr;
  is_tx_terminal_ = CVIniConfig::getInstance().IsModelTX();
  recording_path_ = TXGetRecordRootDir();
  auto_record_delay_ = CTxConfig::getInstance().GetRecAutoStartDelay();
}

TXController::~TXController() {
}

void TXController::Initialize(
    ISessionController *session_controller,
    IMsgManager *msg_manager,
    IConferenceController *conference_controller,
    IDisplayController *display_controller,
    MediaManager *media_manager) 
{
  if (!is_tx_terminal_) {
    //非TX则不处理任何信息
    return;
  }

  // 注册DBus
  QDBusConnection connection = QDBusConnection::sessionBus();
  bool ret = connection.registerService(TXCONTROLLER_SERVICE_NAME);
  ret = connection.registerObject(TXCONTROLLER_SERVICE_OBJECT_PATH ,this);

  assert(conference_controller);
  conference_controller_ = conference_controller;
  terminal_manager_ = &(conference_controller->GetTerminalManager());

  // 连接其他controllers的信号
  // 处理终端状态变更，有些用QueuedConnection确保发送信号的方法调用完成后再调用这里的槽
  connect(conference_controller_, &IConferenceController::NotifyTerminalReceivedSignal,
    this, &TXController::HandleTerminalStatusChangedSlot, Qt::QueuedConnection); //首次收到终端列表触发这个信号
  connect(conference_controller_, &IConferenceController::NotifyTerminalUpdateSignal,
    this, &TXController::HandleTerminalStatusChangedSlot, Qt::QueuedConnection); //终端状态变更（包括重登陆）触发这个信号
  connect(conference_controller_, &IConferenceController::NotifyTerminalReloginSignal,
    this, &TXController::HandleTerminalReloginSlot); //此处不要使用QueuedConnection，确保在恢复媒体流操作前调用槽

  connect(conference_controller_, &IConferenceController::NotifyStreamMediaReceiveStartedSignal,
    this, &TXController::HandleStreamMediaReceiveStartedSlot);
  connect(conference_controller_, &IConferenceController::NotifyStreamMediaReceiveStoppedSignal,
    this, &TXController::HandleStreamMediaReceiveStoppedSlot);
  connect(conference_controller_, &IConferenceController::NotifyConferenceStateChangedSignal,
    this, &ITXController::HandleConferenceStateChangedSlot);
  connect(this, &TXController::NotifyAllRecordJobFinishSignal,
    conference_controller_, &IConferenceController::HandleAllMediaProcessExitSlot);
  
  assert(session_controller);
  connect(
    session_controller, 
    &ISessionController::NotifySessionStateChangedSignal,
    this,
    &TXController::HandleSessionStateChangedNotifySlot);

  assert(media_manager);
  media_manager_ = media_manager;

  assert(display_controller);
  connect(this, &TXController::StartReceiveVideoRequestSignal,
    display_controller, &IDisplayController::HandleRequestStreamMediaAutoReceiveSlot);
  connect(this, &TXController::StopReceiveVideoRequestSignal,
    display_controller, &IDisplayController::HandleStopVideoRequestSlot);

  IMsgReceiver *receiver = msg_manager->GetMsgReceiver();
  assert(receiver);
  connect(
    receiver, &IMsgReceiver::NotifyRecvRecordControlSignal,
    this, &TXController::HandleRecvRecordControlSlot);

  IMsgSender *sender = msg_manager->GetMsgSender();
  assert(sender);
  connect(
    this, &TXController::SendRecordControlRequestAckSignal,
    sender, &IMsgSender::SendRecordControlAckSlot);

  // timers
  auto_record_timer_ = new QTimer;
  connect(auto_record_timer_, SIGNAL(timeout()),
    this, SLOT(HandleAutoRecordTimerTimeoutSlot()));

  // tx monitor client
  tx_monitor_.reset(new TxMonitorIf(TX_MONITOR_SERVICE_NAME, 
    TX_MONITOR_SERVICE_OBJECT_PATH, QDBusConnection::sessionBus(), this));
  assert(tx_monitor_);

  // initialize variables
  max_file_size_ = CTxConfig::getInstance().GetRecMaxFileSize();
  if (0 > max_file_size_) max_file_size_ = 0;

  max_file_duration_ = CTxConfig::getInstance().GetRecMaxFileDuration() * 1000; // sec to ms
  if (0 > max_file_duration_) max_file_duration_ = 0;
}

QString TXController::GenerateRecordFilePath(TerminalPointer terminal) {
  FileNameParam param;
  param.camid = terminal->virtual_index + 1; // index号要从1开始，故需要加1

  ConferencePointer current_conference = conference_controller_->GetCurrentConference();
  assert(current_conference);
  int pos = current_conference->start_time.indexOf(' '); //日期时间之间有个空格，只取日期
  if (pos != -1) {
    param.conf_date = current_conference->start_time.mid(0, pos);
  }
  param.conf = current_conference->title;

  param.terminal = terminal->name;
  pos = param.terminal.lastIndexOf('(');
  if (-1 < pos) {
    param.terminal.truncate(pos);
  }

  if (terminal_record_dict_.contains(terminal->uri) && terminal_record_dict_[terminal->uri]) {
    TerminalRecordPointer terminal_record = terminal_record_dict_[terminal->uri];
    param.title = terminal_record->episode_user_info.title;
    param.participant = terminal_record->episode_user_info.participants;
    param.keywords = terminal_record->episode_user_info.keywords;
    param.description = terminal_record->episode_user_info.description;
  } else {
    param.title = param.terminal;
  }

  QString file = QString::fromLocal8Bit(
    CTxConfig::getInstance().GetRecFileNameFormat().c_str());
  if (file.isEmpty()) {
    // 路径：{录制缓存位置}/{会议日期}_{会议名}/{录像日期}/{终端名}/{文件名}
    // 文件名：{标题}-{日期时间}-{媒体流编号}-{视频格式}.mp4
    QString file = QString("%1\\%2_%3\\%4\\%5\\%6_%7_%8_%9.mp4")
      .arg(recording_path_)
      .arg(param.conf_date).arg(param.conf).arg(TX_FNFMT_DATE).arg(param.terminal)
      .arg(param.title).arg(TX_FNFMT_DATETIME).arg(param.camid).arg(TX_FNFMT_VINFO);
  } else {
    QString f = GetRecordFileNamePath(file, param);
    if (f.size() > 1 && f[0] == '\\') {
      file = recording_path_ + f;
    } else {
      file = recording_path_ + "\\" + f;
    }
  }

  return file;
}

int TXController::GetRecordInitialJobId(const QString &vuri) {
  const QString uri = TerminalHelper::GetTerminalURI(vuri);
  if (terminal_record_dict_.contains(uri)) {
    //存在记录说明录像是远程或自动启动，还可能是终端掉线重登后重新接收
    //远程和自动启动时：是先加记录才到这一步的，所以肯定有一个episode记录。
    //手动启动时：终端首个vuri到这里找不到记录，会执行if后的逻辑，后面的vuri也一定能找到记录了
    //掉线重新接收：则原来存在episode记录，新episode的job_id不从0开始，见HandleTerminalReloginSlot
    TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
    if (terminal_record_node->episodes.empty()) {
      //逻辑非法，要记录日志
      return 0;
    }
    int job_id = terminal_record_node->episodes.last()->job_id;
    ViewRecordPointer view = FindViewNode(vuri, job_id);
    if (!view || view->start) {
      qWarning() << "Start record [" << vuri << "] with old job_id=" << job_id;
    }
    return job_id;
  }

  //还没有记录，则应该是自动启动和远程控制启动以外的方式，如直接操作TX手动启动的
  //此时应该创建记录节点
  StartRecordParam param;
  param.target_uri = uri;
  param.requester_uri = QString::fromLocal8Bit("手动");
  TerminalRecordPointer terminal_record_node = StartRecord(param, true);
  if (!terminal_record_node || terminal_record_node->episodes.empty()) {
    //这绝对是异常情况，要记录日志的
    return 0;
  }
  return terminal_record_node->episodes.last()->job_id;
}

void TXController::HandleSegmentMediaRequestSlot(const QString &uri_or_vuri) {
  QString uri = TerminalHelper::GetTerminalURI(uri_or_vuri);
  if (!terminal_record_dict_.contains(uri)) return;

  TerminalRecordPointer terminal = terminal_record_dict_[uri];
  EpisodeSysInfoPointer last_episode;
  if (terminal->episodes.size() == 0) {
    //旧的集都没有，谈什么分集……
    return;
  }
  last_episode = terminal->episodes.back();

  //两次分集不得小于20秒，以便底层充分消化指令，不然会出乱子的哦
  long long now = GetNonrepeatableTimestamp(); // in us
  if (now - last_episode->start_time < 20 * 1000000LL) {
    return;
  }

  //创建新的分集
  EpisodeSysInfoPointer new_episode = AddEpisodeNode(terminal);

  //任意一路要求分割，则该发送端正在录制的所有视频都分割，以便统一分集
  if (new_episode) {
    //记录已经发出过分段指令
    //例如，某终端有ABCD四路视频，假设A到达限制触发自动分段，则此时四路已经被“要求”分割一次，
    //“要求”指的是命令已下达底层，但是由于进程间通信的同步问题及视频遇到关键帧才分割等原因，
    //底层并未立刻就分割。而恰好此时B也到达限制，如果再触发分段，则可能导致有些视频被多分割一次
    //导致出现一个几秒的短视频
    last_episode->segmented = true;

    //发出分段指令
    for (ViewRecordPointer view : new_episode->views) {
      view->start = true;
      media_manager_->SegmentMedia(view->vuri, new_episode->job_id);
    }
  }
}

void TXController::HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state) {
  if (is_tx_terminal_) {
    if (state == ISessionController::kIsOnline) {
      startWeblibUploader();
    } else {
      stopWeblibUploader();
    }
  }
}

void TXController::HandleConferenceStateChangedSlot(
  const QString &conference_uri, 
  IConferenceController::ConferenceState state) {
  if (state == IConferenceController::kIsInConference) {
    current_conference_ = conference_controller_->GetCurrentConference();
    terminal_record_dict_.clear(); // 两次进入会议之间应等待前一次会议的媒体流完全释放，否则录制作业就无法跟踪了
    terminal_uri_list_.clear();
    auto_record_timer_->start(kAutoRecordCheckTimeOut);
  } else if (state == IConferenceController::kIsNotInConference) {
    auto_record_timer_->stop();
  }
}

void TXController::HandleStreamMediaReceiveStartedSlot(const QString &remote_vuri) {
  QString uri = TerminalHelper::GetTerminalURI(remote_vuri);
  if (!terminal_record_dict_.contains(uri)) {
    return;
  }
  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  ViewRecordPointer view = FindViewNode(remote_vuri, 
    terminal_record_node->next_job_id - 1);
  if (view) {
    view->start = true;
  }
}

void TXController::HandleStreamMediaReceiveStoppedSlot(const QString &remote_vuri) {
  //nothing to be done
  //不要在这里移除node，还有最后的录制完成报告要发给远端
}

void TXController::HandleReceiveUDPQoSReportSlot(const RtpStatItem& rtp_stat) {
  //TODO:这是临时方案，为了取得TerminalDescription中缺少的ip；要优雅些就得改服务器
  if (strlen(rtp_stat.rem_addr) == 0) {
    //第一个报告无地址
    return;
  }
  if (!username_to_vuri_dict_.contains(rtp_stat.member_id)) return;
  QString uri = username_to_vuri_dict_[rtp_stat.member_id];
  uri = TerminalHelper::GetTerminalURI(uri); // vuri to uri
  if (!terminal_record_dict_.contains(uri)) return;
  auto terminal_record_node = terminal_record_dict_[uri];
  //if (!terminal_record_node->reporter) {
  //  terminal_record_node->source_ip = rtp_stat.rem_addr;
  //  //TODO:端口最好也从服务器拿
  //  terminal_record_node->report_port = CVIniConfig::getInstance().getConfRoomQosServerUdpPort();
  //  terminal_record_node->reporter.reset(new UDPReportSender);
  //  terminal_record_node->reporter->Connect(terminal_record_node->source_ip, terminal_record_node->report_port);
  //}
}

void TXController::HandleReceiveUDPRecReportSlot( const RecStatItem& rec_stat )
{
  //step1: 发送信号通知界面
  emit NotifyRecStatReceivedSignal(rec_stat);

  //step2：发送报告给源端
  if (!username_to_vuri_dict_.contains(rec_stat.user_name)) {
    //这是异常，应该记录日志
    return;
  }
  QString vuri = username_to_vuri_dict_[rec_stat.user_name];
  QString uri = TerminalHelper::GetTerminalURI(vuri);
  if (!terminal_record_dict_.contains(uri)) {
    //这是异常，应该记录日志
    return;
  }

  /*TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  if (terminal_record_node->reporter)
    (terminal_record_node->reporter)->SendUDPRecReport(rec_stat);*/

  //step3：根据状态处理额外工作
  switch (rec_stat.statType) {
  case REC_STAT_FILE_WRITING:
    {
      ViewRecordPointer view_node = FindViewNode(vuri, rec_stat.rec.job_id);
      if (!view_node) return; //这是异常啊，得记录日志的
      //记下报告 - 下一步扩展功能用
      view_node->record_status = rec_stat;
      if (ShouldNewEpisodeStart(view_node)) {
        //如果要自动分段
        HandleSegmentMediaRequestSlot(uri);
      }
    }
    break;
  case REC_STAT_FILE_CLOSE:
    ViewRecordComplete(vuri, rec_stat);
    break;
  case REC_STAT_FILE_ABANDON:
    ViewRecordAbandon(vuri, rec_stat);
    break;
  case REC_STAT_MUX_STOP:
    RecordProcessExit(vuri);
    break;
  }
}

bool TXController::ShouldNewEpisodeStart(ViewRecordPointer view)
{
  //检查分段指令是否已经发出，避免二次发出分段指令
  EpisodeSysInfoPointer episode = 
    FindEpisodeNode(view->vuri, view->record_status.rec.job_id);
  if (episode->segmented)
    return false;
  //按文件大小分割，检查文件大小是否超过限制
  if (0 != max_file_size_ && 
      view->record_status.rec.size >= max_file_size_)
    return true;
  //检查文件时长是否超过最大限制
  if (0 != max_file_duration_ && 
      view->record_status.rec.duration >= max_file_duration_)
    return true;
  return false;
}

void TXController::HandleTerminalStatusChangedSlot(
  ConstTerminalPointer terminal,
  const QString &conference_uri)
{
  //不是普通终端，或是TX自己，都不处理
  if (TerminalHelper::IsModelTX(terminal->model) ||
      terminal->uri == profile_->user_uri().c_str()) {
    return;
  }

  QString uri = terminal->uri;

  //Step1：记录终端uri
  if (!terminal_uri_list_.contains(uri)) {
    terminal_uri_list_.push_back(uri);
  }
  
  //Step2：处理自动录像
  if (!CTxConfig::getInstance().GetRecAutoStart()) {
    //没有启用自动录像
    return;
  }

  //录像已存在，则可能是重复获得终端列表导致的，啥也不干
  if (terminal_record_dict_.contains(uri)) {
    return;
  }

  //我们要同时启动所有路的录制，如果该终端的虚拟终端未收全，则先不启动
  TerminalList virtual_terminals = (*terminal_manager_)[uri];
  if (virtual_terminals.size() == 0 || 
      virtual_terminals.size() < terminal->virtual_count) {
    return;
  }

  //对方不在线，则不自动接收
  if (!terminal->is_available) {
    return; //最后判断不等于自己是为了防止服务器把TX型号设置成普通终端时，导致接收自己出错
  }

  //延迟启动自动录像
  //过几秒再请求，因为请求太快时，发送端可能没有完全接收到终端列表，还不能给我们发媒体流
  AutoStartPendingInfo pending;
  pending.uri = uri;
  pending.online_time = time(nullptr);
  auto_record_pending_list_.push_back(pending);
}

void TXController::HandleTerminalReloginSlot(
  ConstTerminalPointer terminal,
  const QString &conference_uri)
{
  //仅处理virtual index为0的虚拟终端，避免重复执行
  if (!terminal || terminal->virtual_index != 0) {
    return;
  }

  QString uri = terminal->uri;
  //之前没有在录，重登陆也不录
  if (!terminal_record_dict_.contains(uri)) return;

  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  //创建新的分集，以便媒体流进程启动前获取job_id
  EpisodeSysInfoPointer new_episode = AddEpisodeNode(terminal_record_node);
}

//该方法已废弃，代码作废
void TXController::HandleRecvRecordControlSlot(const RecordControlMsg &info)
{
  if (!(conference_controller_->IsInConference() && 
      CVIniConfig::getInstance().IsModelTX())) {
    return;
  }

  qDebug() << "Record ctrl type:" << info.type << "vuri:" << info.target_vuri;
  switch (info.type)
  {
  case RecCtrl_Start:
    //RemoteStartRecord(info);
    break;
  case RecCtrl_Stop:
    //RemoteStopRecord(info);
    break;
  default:
    break;
  }
}

int TXController::ControlRecord(int op, const QByteArray &bytes)
{
  if (conference_controller_ && !conference_controller_->IsInConference()) {
    //不在会议中，不处理
    return TXRecord_NotInFocus;
  }

  switch (op)
  {
  case TXRecordOp_Start:
    {
      StartRecordParam param = DBusParamFromByteArray<StartRecordParam>(bytes);
      return RemoteStartRecord(param);
    }
    break;
  case TXRecordOp_Stop:
    {
      StopRecordParam param = DBusParamFromByteArray<StopRecordParam>(bytes);
      return RemoteStopRecord(param);
    }
    break;
  default:
    break;
  }
  return TXRecord_Error;
}

QByteArray TXController::GetRecordStatus(const QString &focus, const QString &uri)
{
  RecordFocusStatus focus_status;

  if (!terminal_manager_ || !current_conference_ ||
      focus != current_conference_->uri) {
    return DBusParamToByteArray(focus_status);
  }

  //只返回正在录制的终端信息
  for (const QString &uri_in_focus : terminal_uri_list_) {
    if (!uri.isEmpty() && uri != uri_in_focus) continue; //如果指定了uri，只返回特定uri的录制信息

    if (terminal_record_dict_.contains(uri_in_focus)) {
      //有信息，表示正在被录制
      TerminalRecordPointer terminal = terminal_record_dict_[uri_in_focus];
      if (terminal->episodes.empty()) continue; //这应该是异常情况，以后处理
      EpisodeSysInfoPointer episode = terminal->episodes.last();
      if (episode->views.empty()) continue; //这应该是异常情况，以后处理

      for (ViewRecordPointer view : episode->views) {
        TerminalPointer terminal_pointer = terminal_manager_->FindTerminal(view->vuri);
        RecordTerminalStatus terminal_status;
        terminal_status.vuri = view->vuri;
        terminal_status.name = (terminal_pointer ? terminal_pointer->name : 
          QString("%1(%2)").arg(terminal->name).arg(TerminalHelper::GetVirtualIndex(view->vuri)));
        terminal_status.status = TxRecordStatus_Recording;
        terminal_status.virtual_count = (terminal_pointer ? terminal_pointer->virtual_count : 0);
        terminal_status.requester_uri = terminal->requester_uri;
        if (view->record_status.statType == REC_STAT_FILE_WRITING) {
          terminal_status.duration = view->record_status.rec.duration;
          terminal_status.recorded_bytes = view->record_status.rec.size;
        }
        focus_status.terminals_status.push_back(terminal_status);
      }
    } else {
      //终端没有被录制
      TerminalPointer terminal_pointer = terminal_manager_->FindTerminal(uri_in_focus);
      if (!terminal_pointer) continue; //这应该是异常情况，以后处理
      RecordTerminalStatus terminal_status;
      terminal_status.vuri = terminal_pointer->virtual_uri;
      terminal_status.name = terminal_pointer->name;
      terminal_status.status = (terminal_pointer->is_available ? TxRecordStatus_Online : TxRecordStatus_Offline);
      focus_status.terminals_status.push_back(terminal_status);
    }
  }

  return DBusParamToByteArray(focus_status);
}

int TXController::RemoteStartRecord( const StartRecordParam &param )
{
  //准备返回数据
  assert(current_conference_);
  QString current_user_uri = QString::fromStdString(profile_->user_uri());
  const QString target_uri = TerminalHelper::GetTerminalURI(param.target_uri);

  //是否同一会议
  if (param.focus != current_conference_->uri) {
    return TXRecord_NotInFocus;
  }

  //检查终端是否有效
  TerminalList virtual_terminals = (*terminal_manager_)[target_uri];
  if (virtual_terminals.empty()) {
    return TXRecord_UriNotExist; // 启动失败
  }

  //只要该终端任何一路已经在录制了，就不允许再启动录制，不然逻辑好复杂，容易出错的
  //TODO:TX手动录制时，还可以每路随便拖啊啊啊
  if (terminal_record_dict_.contains(target_uri)) {
    return TXRecord_IsRecording; // 已经启动了
  }

  //启动录像
  TerminalRecordPointer terminal_record_node = StartRecord(param, false);
  if (!terminal_record_node) {
    return TXRecord_Error; // 启动失败
  }

  return TXRecord_Success;
}

int TXController::RemoteStopRecord( const StopRecordParam &param )
{
  //准备返回数据
  assert(current_conference_);
  QString target_uri = TerminalHelper::GetTerminalURI(param.target_uri);

  //是否同一会议
  if (param.focus != current_conference_->uri) {
    return TXRecord_NotInFocus;
  }

  if (!terminal_record_dict_.contains(target_uri)) {
    //没有在录
    return TXRecord_IsNotRecording;
  }

  //多所有终端做处理，以确保每路都停止了！
  TerminalList terminals = (*terminal_manager_)[target_uri];
  for (auto &t : terminals) {
    //停止接收和录制
    emit StopReceiveVideoRequestSignal(t->virtual_uri);
    //报告操作人信息给UI
    emit NotifyRecordRequesterSignal(t->virtual_uri, "");
  }

  //不要删除UDP报告发送器，底层的文件结束报告会延迟到达，删除reporter
  //会导致报告无法送出。下次发起录制时会重设IP和端口的
  //record_terminal_iter->reporter = nullptr;

  return TXRecord_Success;
}

TXController::TerminalRecordPointer TXController::StartRecord(
  const StartRecordParam &param, 
  bool manual)
{
  QString uri = param.target_uri;
  TerminalList virtual_terminals = (*terminal_manager_)[uri];
  if (terminal_record_dict_.contains(uri) || 
      virtual_terminals.empty() || 
      !virtual_terminals[0]->is_available) {
    //已经在录制中，或终端不存在，不在线，均返回失败
    return nullptr;
  }

  //添加终端的录制记录
  TerminalRecordPointer terminal_record_node = AddTerminalNode(uri);
  if (!terminal_record_node) {
    //添加失败
    return nullptr;
  }

  //从控制参数读取视频集信息
  if (param.title.isEmpty()) {
    //没有输入标题，默认以终端名做标题
    terminal_record_node->episode_user_info.title = terminal_record_node->name;
  } else {
    terminal_record_node->episode_user_info.title = param.title;
  }
  terminal_record_node->episode_user_info.participants = param.participants;
  terminal_record_node->episode_user_info.keywords = param.keywords;
  terminal_record_node->episode_user_info.description = param.description;
  terminal_record_node->requester_uri = param.requester_uri;

  //TODO:本来该在这里初始化reporter，但是TerminalDescription里没有ip  ||-_-
  //     所以初始化reporter在收到底层的QoS报告后，获得了ip再创建

  assert(terminal_record_node->episodes.size() == 1);
  for (auto &t : virtual_terminals) {
    //保存user_name与vuri映射关系
    QString user_name = TerminalHelper::GetUsername(t->virtual_uri);
    username_to_vuri_dict_[user_name] = t->virtual_uri;
    //创建第一集的view_node
    AddViewNode(terminal_record_node->episodes.at(0), t->virtual_uri);
    if (!manual) {
      //非手动启动时，要启动接收和录像，具体实现见连接的槽
      //手动启动意味着接收进程已经启动，不需要再次启动了
      emit StartReceiveVideoRequestSignal(t->virtual_uri); 
    }
    //更新操作人信息给UI
    emit NotifyRecordRequesterSignal(t->virtual_uri, param.requester_uri);
  }

  return terminal_record_node;
}

void TXController::ViewRecordComplete(
  const QString &vuri, 
  const RecStatItem &rec_stat)
{
  QString uri = TerminalHelper::GetTerminalURI(vuri);
  //能到这里下一行肯定没问题
  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  assert(terminal_record_node);
  EpisodeSysInfoPointer episode = FindEpisodeNode(uri, rec_stat.close.job_id);

  //重要：不要从terminal_manager_读取数据，因为该方法可能在退出会议后调用
  //（退出会议——录像完成——调用此处），此时terminal_manager_已经清空。
  //所有要用到的终端信息要在terminal_record_node存副本
  //TerminalPointer terminal = terminal_manager_->FindTerminal(vuri);

  RecordCompleteParam param;
  if (current_conference_) {
    param.conference.cid = current_conference_->cid.toInt();
    param.conference.title = current_conference_->title;
    param.conference.start_time = current_conference_->start_time;
    param.conference.description = current_conference_->description;
  }
  if (terminal_record_node && episode) {
    param.terminal.uri = terminal_record_node->uri;
    param.terminal.name = terminal_record_node->name;

    param.episode.id = episode->start_time; //以没有重复的时间戳作为运行时id
    param.episode.requester = terminal_record_node->requester_uri;
    param.episode.title = terminal_record_node->episode_user_info.title;
    param.episode.participants = terminal_record_node->episode_user_info.participants;
    param.episode.keywords = terminal_record_node->episode_user_info.keywords;
    param.episode.description = terminal_record_node->episode_user_info.description;
    param.episode.start_time = episode->start_time / 1000000LL; // 微秒转换成秒
  }
  param.view.camera_index = TerminalHelper::GetVirtualIndex(vuri);
  param.view.start_time = rec_stat.close.start_time; // in ms
  param.view.duration = rec_stat.close.duration; // in ms
  param.view.file = QString::fromLocal8Bit(rec_stat.close.file);
  {
    // JSON
    QJsonObject video;
    video.insert("width", rec_stat.close.video_width);
    video.insert("height", rec_stat.close.video_height);
    video.insert("fps", rec_stat.close.video_fps);
    QJsonObject audio;
    audio.insert("rate", rec_stat.close.audio_sample_rate);
    audio.insert("channel", rec_stat.close.audio_channel);
    audio.insert("bit", rec_stat.close.audio_bits_count);
    QJsonObject media;
    media.insert("video", video);
    media.insert("audio", audio);
    QJsonDocument document;
    document.setObject(media);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    param.view.media_format = QString(byte_array);
  }

  //tell tx monitor
  if (tx_monitor_) {
    tx_monitor_->ReportRecStat(RecStat_FileClose, DBusParamToByteArray(param));
  }

  ViewRecordPointer view = FindViewNode(vuri, rec_stat.close.job_id);
  if (view) {
    view->complete = true;
  }
  //清理录制记录
  RemoveFinishedEpisodeNode(vuri);
}

void TXController::ViewRecordAbandon(
  const QString &vuri, 
  const RecStatItem &rec_stat)
{
  ViewRecordPointer view = FindViewNode(vuri, rec_stat.close.job_id);
  if (view) {
    view->complete = true;
    qDebug() << "Record [" << view->vuri << "] file abandon";
  }
  //清理录制记录
  RemoveFinishedEpisodeNode(vuri);
}

void TXController::RecordProcessExit(const QString &vuri)
{
  //录像进程退出，则所有分集对应的view都应该处理完了。
  //这里检查一下有没有因逻辑漏洞没有关闭的分集，有的话记录日志，并删除终端记录，以免影响下次录制
  const QString uri = TerminalHelper::GetTerminalURI(vuri);
  if (!terminal_record_dict_.contains(uri)) return;

  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];

  for (auto episode : terminal_record_node->episodes) {
    for (auto view : episode->views) {
      if (view->vuri == vuri && !view->complete) {
        qWarning() << "Record [" << vuri << "] not closed properly. Episode id=" 
          << episode->start_time;
        view->complete = true;
      }
    }
  }
  RemoveFinishedEpisodeNode(vuri);

  if (terminal_record_node->episodes.empty()) {
    terminal_record_dict_.remove(uri);
  }

  if (terminal_record_dict_.empty()) {
    emit NotifyAllRecordJobFinishSignal();
  }
}

TXController::TerminalRecordPointer TXController::AddTerminalNode( 
  const QString uri_or_vuri )
{
  QString uri = TerminalHelper::GetTerminalURI(uri_or_vuri);
  TerminalPointer terminal = terminal_manager_->FindTerminal(uri);
  if (!terminal) return nullptr;

  //已创建则返nullptr
  if (terminal_record_dict_.contains(uri)) 
    return nullptr;

  TerminalRecordPointer terminal_record_node(new TerminalRecordNode);
  terminal_record_node->uri = uri;
  terminal_record_node->name = 
    terminal->name.left(terminal->name.lastIndexOf("(")); //去掉末尾的编号
  terminal_record_node->next_job_id = 0;
  //terminal_record_node->report_port = 0;

  AddEpisodeNode(terminal_record_node); //添加第一个分集记录

  //不要立刻添加view记录！
  //因为如果是自动接收调用的这个，此时终端列表可能没接收完，创建的view_node也就不完整。
  //所以还是要在实际收到录制报告时再添加view记录，见HandleReceiveUDPRecReportSlot

  terminal_record_dict_.insert(uri, terminal_record_node);
  return terminal_record_node;
}

TXController::EpisodeSysInfoPointer TXController::AddEpisodeNode(
  TerminalRecordPointer terminal)
{
  if (!terminal) {
    return nullptr;
  }
  EpisodeSysInfoPointer episode(new EpisodeSysInfo);
  episode->job_id = terminal->next_job_id++;
  episode->start_time = GetNonrepeatableTimestamp(); //微秒级不重复的时间戳
  episode->segmented = false;

  //如果不是第一集，则按照之前的记录创建view_node
  if (terminal->episodes.size()) {
    for (ViewRecordPointer view : terminal->episodes[0]->views) {
      AddViewNode(episode, view->vuri);
    }
  }

  terminal->episodes.push_back(episode);
  return episode;
}

TXController::ViewRecordPointer TXController::AddViewNode( 
  EpisodeSysInfoPointer episode, 
  const QString &vuri )
{
  if (!TerminalHelper::IsVirtualURI(vuri)) return nullptr;

  //对应的view_node已存在，则失败
  for (ViewRecordPointer view : episode->views) {
    if (view->vuri == vuri) {
      return view;
    }
  }

  ViewRecordPointer view(new ViewRecordNode);
  view->vuri = vuri;
  view->start = false;
  view->complete = false;
  memset(&view->record_status, 0, sizeof(RecStatItem));
  episode->views.push_back(view);

  return view;
}

TXController::EpisodeSysInfoPointer TXController::FindEpisodeNode( 
  const QString &uri_or_vuri, int job_id)
{
  QString uri = TerminalHelper::GetTerminalURI(uri_or_vuri);
  if (!terminal_record_dict_.contains(uri)) return nullptr;

  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  auto it = std::find_if(
    terminal_record_node->episodes.begin(),
    terminal_record_node->episodes.end(),
    [&job_id](const EpisodeSysInfoPointer &node) {
      return node->job_id == job_id;
    });

  if (it == terminal_record_node->episodes.end()) return nullptr;
  return (*it);
}

TXController::ViewRecordPointer TXController::FindViewNode( 
  const QString &vuri, int job_id)
{
  EpisodeSysInfoPointer episode = FindEpisodeNode(vuri, job_id);
  if (!episode) return nullptr;

  auto it = std::find_if(
    episode->views.begin(),
    episode->views.end(),
    [&vuri](const ViewRecordPointer &node) {
      return node->vuri == vuri;
  });

  if (it == episode->views.end()) return nullptr;
  return (*it);
}

void TXController::HandleAutoRecordTimerTimeoutSlot()
{
  while (!auto_record_pending_list_.empty()) {
    AutoStartPendingInfo pending = auto_record_pending_list_.front();
    if (time(nullptr) - pending.online_time < auto_record_delay_) {
      //第一个待启动的等待时间还未到，其他肯定也没到
      return;
    }

    auto_record_pending_list_.pop_front();

    //尝试启动自动录像
    StartRecordParam param;
    param.target_uri = pending.uri;
    param.requester_uri = QString::fromLocal8Bit("自动");
    TerminalRecordPointer terminal_record_node = StartRecord(param, false);
    if (!terminal_record_node) {
      //启动失败啦，可能是自动启动前被手动启动了，或是终端又下了
      continue;
    }
  }
}

void TXController::RemoveFinishedEpisodeNode( const QString &uri_or_vuri )
{
  const QString uri = TerminalHelper::GetTerminalURI(uri_or_vuri);
  if (!terminal_record_dict_.contains(uri)) return;

  TerminalRecordPointer terminal_record_node = terminal_record_dict_[uri];
  int max_deleted_job_id = 0;

  for (auto epi_it = terminal_record_node->episodes.begin();
       epi_it != terminal_record_node->episodes.end();) {
    auto cur_epi_it = epi_it;
    bool should_delete = true;
    //查找未完成录制的视角
    for (auto view : (*cur_epi_it)->views) {
      if (!view->complete) {
        should_delete = false;
        break;
      }
    }
    if (should_delete) {
      //所有视角都完成了，则该分集信息可以删除了
      max_deleted_job_id = (*cur_epi_it)->job_id;
      epi_it = terminal_record_node->episodes.erase(cur_epi_it);
    } else {
      ++epi_it;
    }
  }

  //看看有没有之前由于某种未知原因没能正常删除的episode记录
  //底层MP4Mux保证了关闭顺序从旧到新，所以某job_id关闭了，则更小的job_id肯定无效了。
  //注意哦，虽然强制删除了释放了内存，但没有触发更新数据库操作
  for (auto epi_it = terminal_record_node->episodes.begin();
       epi_it != terminal_record_node->episodes.end();) {
    auto cur_epi_it = epi_it;
    if ((*cur_epi_it)->job_id < max_deleted_job_id) {
      epi_it = terminal_record_node->episodes.erase(cur_epi_it);
      //如果以后有了日志模块，记个日志
    } else {
      ++epi_it;
    }
  }
}

bool TXController::HasRecording()
{
  return !terminal_record_dict_.empty();
}
