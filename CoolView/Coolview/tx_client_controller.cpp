#include "tx_client_controller.h"

#include <ctime>

#include "conference_controller_interface.h"
#include "profile/RunningProfile.h"
#include "config/RunningConfig.h"
#include "util/report/RecordStat.h"
#include "util/MediaManager.h"
#include "util/ini/CVIniConfig.h"
#include "util/ini/TxConfig.h"
#include "tx_util.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"
#include "dbus/txController/common/TxControllerServiceCommon.h"
#include "tx/WebService/cvtx/tx_soapCvTxProxy.h"
#include "udp_report_sender.h"

#include "msg_manager_interface.h"

using namespace tx_util;

TXClientController::TXClientController(QObject *parent,
                                       RunningProfile *profile,
                                       RunningConfig *config)
  : ITXClientController(parent),
    profile_(profile),
    running_config_(config)
{
  send_video_auto_cut_timer_ = nullptr;
  recording_path_ = TXGetRecordRootDir();
  tx_soap_proxy_ = nullptr; //在拿到tx信息后再创建
  get_tx_record_status_timer_ = nullptr;
  rec_stat_sender_to_confroom_ = nullptr;
  thread_ = nullptr;
}

TXClientController::~TXClientController()
{
  if (rec_stat_sender_to_confroom_) {
    rec_stat_sender_to_confroom_->Close();
    delete rec_stat_sender_to_confroom_;
    rec_stat_sender_to_confroom_ = nullptr;
  }
  if (thread_) {
    thread_->deleteLater();
  }
}

void TXClientController::Initialize( 
  IMsgManager *msg_manager, 
  IConferenceController *conference_controller,
  MediaManager *media_manager) 
{
  assert(conference_controller);
  conference_controller_ = conference_controller;
  terminal_manager_ = &(conference_controller->GetTerminalManager());
  connect(conference_controller_, &IConferenceController::NotifyConferenceStateChangedSignal,
    this, &ITXClientController::HandleConferenceStateChangedSlot);

  assert(media_manager);
  media_manager_ = media_manager;
  connect(media_manager_, &MediaManager::NotifySendMediaStateChanged,
    this, &TXClientController::HandleSendMediaStateChangedNotifySlot);

  /*IMsgReceiver *receiver = msg_manager->GetMsgReceiver();
  assert(receiver);
  connect(
    receiver, &IMsgReceiver::NotifyRecvRecordControlAckSignal,
    this, &TXClientController::HandleRecvRecordControlAckNotifySlot);*/
  
  /*IMsgSender *sender = msg_manager->GetMsgSender();
  assert(sender);
  connect(
    this, &TXClientController::SendRecordControlRequestSignal,
    sender, &IMsgSender::SendRecordControlSlot);*/

  send_video_auto_cut_timer_ = new QTimer(this);
  connect(send_video_auto_cut_timer_, SIGNAL(timeout()),
    this, SLOT(HandleSegmentLocalRecordVideoRequestSlot()));

  get_tx_record_status_timer_ = new QTimer(this);
  connect(get_tx_record_status_timer_, SIGNAL(timeout()),
    this, SLOT(HandleGetTxRecordStatusTimerTimeoutSlot()));

  rec_stat_sender_to_confroom_ = new UDPReportSender;
  rec_stat_sender_to_confroom_->Connect(
    "127.0.0.1", CVIniConfig::getInstance().getConfRoomQosServerUdpPort());

  thread_ = new QThread;
  moveToThread(thread_);
  thread_->start();

  qRegisterMetaType<RecStatItem>("RecStatItem"); //本类在单独线程中运行，要在注册一次
}

QString TXClientController::GenerateLocalRecordFilePath(int virtual_index) 
{
  //本地录像视频位置
  QString file = QString::fromLocal8Bit(
    CTxConfig::getInstance().GetRecLocalFileNameFormat().c_str());
  if (file.isEmpty()) {
    file = QString("%1\\LocalVideo\\%2\\Local_%3_%4_%5.mp4")
      .arg(recording_path_)
      .arg(TX_FNFMT_DATE)
      .arg(TX_FNFMT_DATETIME).arg(virtual_index + 1).arg(TX_FNFMT_VINFO);
  } else {
    FileNameParam param;
    param.camid = virtual_index + 1;
    QString f = GetRecordFileNamePath(file, param);
    if (f.size() > 1 && f[0] == '\\') {
      file = recording_path_ + f;
    } else {
      file = recording_path_ + "\\" + f;
    }
  }
  return file;
}

void TXClientController::HandleSendMediaStateChangedNotifySlot( 
  const QString &media_id, 
  int type )
{
  //本地录像按时分割，如果配置开启了这项功能，则开启定时器
  int segment_interval_sec = CTxConfig::getInstance().GetRecLocalDuration();
  if (segment_interval_sec > 0 && !send_video_auto_cut_timer_->isActive()) {
    send_video_auto_cut_timer_->start(segment_interval_sec * 1000);
  }

  //未开启本地录制，则不作任何事
  if (!running_config_->isEnableLocalRecord()) {
    return;
  }

  const int stream_count = running_config_->VideoCaptureDeviceCount();
  const int index = getVirtualIndexFromSendMediaID(media_id);
  if (0 <= index && stream_count > index && conference_controller_ &&
    type == StreamedMedia_Running) {
      //先查找当前有没有正在录制
      if (-1 == media_manager_->GetRecvMemberPositionFromName(
        media_id, nullptr, nullptr, nullptr)) {
          conference_controller_->HandleAddRecvStreamMediaRequestSlot(
            getLocalRecordMediaID(index), -1, -1, false, true);
      }
  }
}

void TXClientController::HandleSegmentLocalRecordVideoRequestSlot()
{
  static int job_id = 0;
  if (/*IsInConference() &&*/ CVIniConfig::getInstance().IsModelHD()) {
    const int stream_count = running_config_->VideoCaptureDeviceCount();
    for (int i = 0; i < stream_count; ++i) {
      media_manager_->SegmentMedia(getLocalRecordMediaID(i), job_id);
    }
    ++job_id;
  }
}

void TXClientController::HandleConferenceStateChangedSlot(
  const QString &conference_uri, 
  IConferenceController::ConferenceState state)
{
  if (state == IConferenceController::kIsInConference) {
    if (!ConferenceHelper::IsVirtualConference(conference_controller_->GetCurrentConference()->uri)) {
      int interval = CTxConfig::getInstance().GetSoapCRefreshStatusInterval();
      if (interval > 0) {
        get_tx_record_status_timer_->start(interval * 1000);
      }
      delete tx_soap_proxy_;
      tx_soap_proxy_ = nullptr;
    }

  } else if (state == IConferenceController::kIsLeavingConference || 
             state == IConferenceController::kIsNotInConference) {
    get_tx_record_status_timer_->stop();
    //如果本地终端允许录制，则退出会议要重置本地回显录制状态
    //注意：这时不能调用ChangeRecordState，因为terminal_manager_中可能已经没有本终端的状态了
    if (IsSelfRecordCtrlAllow()) {
      for (int i = 0; i < running_config_->VideoCaptureDeviceCount(); ++i) {
        media_manager_->ChangeMediaState(getLocalPreviewMediaID(i), UiStateTypeRecord, 
          UiMediaState_RecordStop, true); //给user_id也没问题的
      }
    }
  }
}

void TXClientController::HandleOneVideoRecordRequestSlot( 
  const QString &user_id, int type)
{
  //该方法已废弃

  //if (!conference_controller_->IsInConference()) {
  //  return;
  //}
  //TerminalPointer vterminal = terminal_manager_->FindTerminalByUserId(user_id);
  //TerminalPointer txTerminal = terminal_manager_->FindAvailableTxTerminal();
  //if (vterminal && txTerminal && !TerminalHelper::IsModelTX(vterminal->model)) {
  //  SendRecordControlRequest(vterminal->virtual_uri, txTerminal->uri, type);
  //} else {
  //  //操作失败
  //  media_manager_->ChangeMediaState(user_id, UiStateTypeRecord, 
  //    UiMediaState_RecordError, true);
  //}
}

void TXClientController::HandleWholeVideoRecordRequestSlot( 
  const QString &user_id, int type, QByteArray param)
{
  if (!conference_controller_->IsInConference() ||
      ConferenceHelper::IsVirtualConference(conference_controller_->GetCurrentConference()->uri)) {
    media_manager_->ChangeMediaState(user_id, UiStateTypeRecord, 
      UiMediaState_RecordError, true); //给user_id也没问题的
    return;
  }

  TerminalPointer vterminal;
  if (isLocalPreviewMediaID(user_id)) {
    vterminal = terminal_manager_->FindTerminal(QString::fromStdString(profile_->user_uri()));
  } else {
    vterminal = terminal_manager_->FindTerminalByUserId(user_id);
  }

  if (vterminal && !TerminalHelper::IsModelTX(vterminal->model)) {
    SendRecordControlRequest(vterminal->uri, type, param);
  } else {
    media_manager_->ChangeMediaState(user_id, UiStateTypeRecord, 
      UiMediaState_RecordError, true); //给user_id也没问题的
  }
}

void TXClientController::HandleRecvRecordControlAckNotifySlot( 
  const RecordControlAckMsg &ack)
{
  //该方法已废弃
}

void TXClientController::SendRecordControlRequest( 
  const QString &uri,
  int type, 
  QByteArray param_bytes )
{
  int tx_result = 0;
  int soap_result = SOAP_OK;

  if (!tx_soap_proxy_) {
    //尝试初始化SOAP proxy
    CreateTxSoapProxy();
    if (!tx_soap_proxy_) ChangeRecordState(uri, UiMediaState_RecordError);
    return;
  }

  if (RecCtrl_Start == type) {
    QDataStream in(param_bytes);
    in.setVersion(QDataStream::Qt_4_4);
    StartRecordParam param;
    in >> param;

    tx_soap::ns__StartRecordParam start_param;
    start_param.focus = conference_controller_->GetCurrentConference()->uri.toLocal8Bit().constData();
    start_param.target_uri = uri.toLocal8Bit().constData();
    start_param.requester_uri = profile_->user_uri();
    start_param.title = param.title.toLocal8Bit().constData();
    start_param.participants = param.participants.toLocal8Bit().constData();
    start_param.keywords = param.keywords.toLocal8Bit().constData();
    start_param.description = param.description.toLocal8Bit().constData();

    soap_result = tx_soap_proxy_->StartRecord(start_param, &tx_result);
  } 
  else if (RecCtrl_Stop == type) {
    tx_soap::ns__StopRecordParam stop_param;
    stop_param.focus = conference_controller_->GetCurrentConference()->uri.toLocal8Bit().constData();
    stop_param.target_uri = uri.toLocal8Bit().constData();

    soap_result = tx_soap_proxy_->StopRecord(stop_param, &tx_result);
  }
  else {
    //无效的操作
    ChangeRecordState(uri, UiMediaState_RecordError);
    return;
  }

  //处理错误
  if (soap_result != SOAP_OK) {
    //调用远程方法出错
    ChangeRecordState(uri, UiMediaState_RecordSoapError);
  }
  else if (tx_result != TXRecord_Success) {
    //TX返回错误
    int state = UiMediaState_RecordTxError | (tx_result << 16); //复用这个变量传送TX错误
    ChangeRecordState(uri, (UiMediaState)state);
  }
  else {
    //成功
    if (type == RecCtrl_Start) {
      ChangeRecordState(uri, UiMediaState_RecordRun);
    }
    //RecCtrl_Stop时，不立刻设置按钮状态，等下次获取录制状态后根据实际情况再设置
  }
}

void TXClientController::CreateTxSoapProxy()
{
  if (tx_soap_proxy_) {
    delete tx_soap_proxy_;
  }

  static std::string server;
  server = CTxConfig::getInstance().GetSoapCServer();
  if (!server.empty()) {
    tx_soap_proxy_ = new tx_soap::CvTxProxy(server.c_str());
  }
}

void TXClientController::ChangeRecordState( const QString &uri, int state )
{
  if (!terminal_manager_->ContainTerminal(uri)) {
    return;
  }
  TerminalList vterminals = (*terminal_manager_)[uri];
  for (auto t : vterminals) {
    if (uri == QString::fromStdString(profile_->user_uri())) {
      //自己终端的要替换成本地回显id
      QString local_preview_id = getLocalPreviewMediaID(t->virtual_index);
      media_manager_->ChangeMediaState(local_preview_id, UiStateTypeRecord, 
        (UiMediaState)state, true);
    } else {
      media_manager_->ChangeMediaState(t->virtual_uri, UiStateTypeRecord, 
        (UiMediaState)state, true);
    }
  }
}

void TXClientController::HandleGetTxRecordStatusTimerTimeoutSlot()
{
  if (!tx_soap_proxy_){
    CreateTxSoapProxy();
    if (!tx_soap_proxy_) return;
  }
  if (!IsRecordCtrlAllow()) {
    return;
  }

  std::string uri;
  if (IsOnlySelfRecordCtrlAllow()) {
    //只要自己的而不要全局的，则设置uri为自己
    uri = profile_->user_uri();
  }

  std::string focus;
  if (conference_controller_->GetCurrentConference()) {
    focus = conference_controller_->GetCurrentConference()->uri.toLocal8Bit().constData();
  }

  tx_soap::ns__GetRecordStatusResult rec_status_result;
  int soap_result = SOAP_OK;
  soap_result = tx_soap_proxy_->GetRecordStatus(focus, uri, &rec_status_result);
  if (soap_result == SOAP_OK) {
    for (auto &status : rec_status_result.terminals) {
      RecStatItem recstat = {0};

      if (status.status == TxRecordStatus_Recording) {
        //暂时先用底层的录制报告数据结构一个一个发给CvConf，以后有时间再专门定义一个数据结构吧
        GetRecUDPReportUserNameFromVirtualUri(recstat.user_name, sizeof(recstat.user_name), 
          QString::fromStdString(status.vuri));
        recstat.statType = REC_STAT_FILE_WRITING;
        recstat.rec.duration = status.duration;
        recstat.rec.size = status.recorded_bytes;
        rec_stat_sender_to_confroom_->SendUDPRecReport(recstat);
      } 
      else if (status.status == TxRecordStatus_Online){
        //其他设置成停止录像状态
        ChangeRecordState(TerminalHelper::GetTerminalURI(QString::fromStdString(status.vuri)), 
          UiMediaState_RecordStop);
      }
    }
  }
}

void TXClientController::GetRecUDPReportUserNameFromVirtualUri( 
  char *buf, int len, const QString &vuri )
{
  QString local_uri = QString::fromStdString(profile_->user_uri());
  QByteArray bytes;
  if (TerminalHelper::GetTerminalURI(vuri) == local_uri) {
    //是自己终端的录制状态，要把报告设成本地预览的id才行，这是历史问题了，将就下
    int index = TerminalHelper::GetVirtualIndex(vuri);
    bytes = getLocalPreviewMediaID(index).toLocal8Bit();
  } else {
    bytes = TerminalHelper::GetUsername(vuri).toLocal8Bit();
  }
  strcpy_s(buf, len, bytes.constData());
}

bool TXClientController::IsRecordCtrlAllow()
{
  return (CTxConfig::getInstance().GetSoapCAllowSelfCtrl() || 
    CTxConfig::getInstance().GetSoapCAllowGlobalCtrl());
}

bool TXClientController::IsSelfRecordCtrlAllow()
{
  return (CTxConfig::getInstance().GetSoapCAllowSelfCtrl() || 
    CTxConfig::getInstance().GetSoapCAllowGlobalCtrl());
}

bool TXClientController::IsOnlySelfRecordCtrlAllow()
{
  return CTxConfig::getInstance().GetSoapCAllowSelfCtrl() && 
    !CTxConfig::getInstance().GetSoapCAllowGlobalCtrl();
}
