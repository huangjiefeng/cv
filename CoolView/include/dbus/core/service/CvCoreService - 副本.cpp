#include <dbus/core/service/CvCoreService.h>

#include <QApplication>

#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/core/service/CvCoreAdaptor.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>
#include <dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h>
#include <dbus/performance/monitor/common/MonitorConfigCommon.h>

#include <log/log.h>

#include "util/ini/CVIniConfig.h"
#include "udp_report_receiver.h"

#include "tx_controller_interface.h"
#include "tx_client_controller.h"
#include "fileplay_controller_interface.h"
#include "system_controller_interface.h"
#include "controller_manager_interface.h"
#include "conference_controller_interface.h"

#include "ptz_controller.h"

CoreService2::CoreService2() {
  new CvCoreAdaptor(this);
  QDBusConnection connection = QDBusConnection::sessionBus();
  bool res = connection.registerService( CVCORE_SERVICE_NAME );
  res = connection.registerObject( CVCORE_SERVICE_OBJECT_PATH, this );
  if( !res )
    CV_LOG_ERROR( "注册CV_CORE_SERVICE失败");
  has_initialized_ = false;
  RegisterHandlers();
}

CoreService2::~CoreService2() {
  if (udp_thread_.isRunning()) {
    emit StopUDPReceiverSignal();
    udp_thread_.terminate();
    udp_thread_.wait();
    udp_report_receiver_->deleteLater();
  }
}

void CoreService2::RegisterHandlers() {
  command_handler_dict_.insert(
    CoreAction_SendQoS, std::mem_fn(&CoreService2::SendQoS));
  
  command_handler_dict_.insert(
    CoreAction_StartRecvStreamedMedia, 
    std::mem_fn(&CoreService2::StartRecvStreamMedia));

  command_handler_dict_.insert(
    CoreAction_StopRecvStreamedMedia, 
    std::mem_fn(&CoreService2::StopRecvStreamMedia));

  command_handler_dict_.insert(
    CoreAction_SetVolume, 
    std::mem_fn(&CoreService2::SetVolume));

  command_handler_dict_.insert(
    CoreAction_ShutDown, 
    std::mem_fn(&CoreService2::ShutDown));

  command_handler_dict_.insert(
    CoreAction_Restart, 
    std::mem_fn(&CoreService2::Restart));

  command_handler_dict_.insert(
    CoreAction_StopLocalAudio, 
    std::mem_fn(&CoreService2::StopLocalAudio));

  command_handler_dict_.insert(
    CoreAction_StopLocalVideo, 
    std::mem_fn(&CoreService2::StopLocalVideo));

  command_handler_dict_.insert(
    CoreAction_StartSendScreenMedia, 
    std::mem_fn(&CoreService2::StartSendScreenMedia));

  command_handler_dict_.insert(
    CoreAction_StartRecvScreenMedia, 
    std::mem_fn(&CoreService2::StartRecvScreenMedia));

  command_handler_dict_.insert(
    CoreAction_StopRecvScreenMedia, 
    std::mem_fn(&CoreService2::StopRecvScreenMedia));

  command_handler_dict_.insert(
    CoreAction_ControlAudioRecv, 
    std::mem_fn(&CoreService2::ControlAudioRecv));

  command_handler_dict_.insert(
    CoreAction_EnterMeeting, 
    std::mem_fn(&CoreService2::JoinConference));

  command_handler_dict_.insert(
    CoreAction_ChangeLayout, 
    std::mem_fn(&CoreService2::ChangeScreenLayout));

  command_handler_dict_.insert(
    CoreAction_QuitMeeting, 
    std::mem_fn(&CoreService2::LeaveConference));

  command_handler_dict_.insert(
    CoreAction_CloseScreenRecv, 
    std::mem_fn(&CoreService2::CloseScreenRecv));

  command_handler_dict_.insert(
    CoreAction_OpenScreenShare, 
    std::mem_fn(&CoreService2::OpenScreenShare));

  command_handler_dict_.insert(
    CoreAction_CloseScreenShare, 
    std::mem_fn(&CoreService2::CloseScreenShare));

  command_handler_dict_.insert(
    CoreAction_ShareScreenControl, 
    std::mem_fn(&CoreService2::SharedScreenControl));

  command_handler_dict_.insert(
    CoreAction_PPTControlCommand, 
    std::mem_fn(&CoreService2::PPTControlCommand));
  
  command_handler_dict_.insert(
    CoreAction_SetPanelVisible, 
    std::mem_fn(&CoreService2::SetPanelVisible));

  command_handler_dict_.insert(
    CoreAction_GetPannelState, 
    std::mem_fn(&CoreService2::GetPanelState));

  command_handler_dict_.insert(
    CoreAction_RecvMonitorCofinfo, 
    std::mem_fn(&CoreService2::RecvMonitorConfinfo));

  command_handler_dict_.insert(
    CoreAction_RecoveryMeidaProcess, 
    std::mem_fn(&CoreService2::RecoverMediaProcess));

  command_handler_dict_.insert(
    CoreAction_SegmentVideo, 
    std::mem_fn(&CoreService2::SegmentVideo));

  command_handler_dict_.insert(
    CoreAction_SegmentVideo, 
    std::mem_fn(&CoreService2::SegmentVideo));

  command_handler_dict_.insert(
    CoreAction_ControlVideoRecord, 
    std::mem_fn(&CoreService2::ControlVideoRecord));
}

void CoreService2::Initialize( IControllerManager *controller_manager ) {
  assert(has_initialized_ == false);
  assert(controller_manager);

  ISystemController *system_controller =
    controller_manager->GetSystemController();
  assert(system_controller);

  connect(
    this, &CoreService2::ShutDownSignal,
    system_controller, &ISystemController::HandleShutDownSystemRequestSlot);

  connect(
    this, &CoreService2::RestartSignal,
    system_controller, &ISystemController::HandleRestartSystemRequestSlot);

  connect(
    system_controller, &ISystemController::NotifyQuitSystemSignal,
    this, &CoreService2::MoveToMainThreadSlot);

  IConferenceController *conference_controller =
    controller_manager->GetConferenceController();
  assert(conference_controller);

  connect(
    this, 
    &CoreService2::SendQoSSignal,
    conference_controller, 
    &IConferenceController::HandleReceiveUDPQoSReportSlot);

  connect(
    this, 
    &CoreService2::JoinConferenceSignal,
    conference_controller,
    &IConferenceController::HandleJoinConferenceRequestSlot);

  connect(
    this,
    &CoreService2::LeaveConferenceSignal,
    conference_controller,
    &IConferenceController::HandleLeaveConferenceRequestSlot);

  connect(
    this,
    &CoreService2::CreateSendScreenMediaSignal,
    conference_controller, 
    &IConferenceController::HandleCreateSendScreenMediaRequestSlot);

  connect(
    this, 
    &CoreService2::ControlDocumentSharedSignal,
    conference_controller,
    &IConferenceController::HandleScreenShareControlRequestSlot);

  connect(
    this,
    &CoreService2::RecoverMediaProcessSignal,
    conference_controller,
    &IConferenceController::HandleRecoveryMediaProcessRequestSlot);

  IDisplayController *display_controller = 
    controller_manager->GetDisplayController();
  assert(display_controller);

  connect(
    this, &CoreService2::ReceiveVideoSignal,
    display_controller, &IDisplayController::HandleReceiveVideoRequestSlot);

  connect(
    this, &CoreService2::StopVideoSignal,
    display_controller, &IDisplayController::HandleStopVideoRequestSlot);

  connect(
    this, &CoreService2::ChangeVoiceVolumeSignal,
    display_controller, &IDisplayController::HandleVoiceChangeRequestSlot);

  connect(
    this, &CoreService2::ControlSendLocalAudioSignal,
    display_controller, &IDisplayController::HandleSendLocalAudioRequestSlot);

  connect(
    this, &CoreService2::ControlSendLocalVideoSignal,
    display_controller, &IDisplayController::HandleSendLocalVideoRequestSlot);

  connect(
    this, &CoreService2::ReceiveDocumentSignal,
    display_controller, &IDisplayController::HandleReceiveDocumentRequestSlot);

  connect(
    this, &CoreService2::StopDocumentSignal,
    display_controller, &IDisplayController::HandleStopDocumentRequestSlot);

  connect(
    this, &CoreService2::ControlAudioReceiveSignal,
    display_controller, &IDisplayController::HandleReceiveAudioRequestSlot);

  connect(
    this, &CoreService2::OpenDocumentSignal,
    display_controller, &IDisplayController::HandleOpenDocumentRequestSlot);

  connect(
    this, &CoreService2::ControlPPTSignal,
    display_controller, &IDisplayController::HandlePPTControlSlot);

  connect(
    this, &CoreService2::ChangeSceenLayoutSignal,
    display_controller, &IDisplayController::HandleScreenLayoutRequestSlot);

  connect(
    this, &CoreService2::ShowPanelSignal,
    display_controller, &IDisplayController::SetPanelVisibleSignal);

  connect(
    this, &CoreService2::GetPanelVisibleStateSignal,
    display_controller, &IDisplayController::CheckPanelVisibleStateSignal);

  auto &ini_config = CVIniConfig::getInstance();

  if (!ini_config.IsModelTX()) {
    ITXClientController *tx_client_controller = 
      controller_manager->GetTXClientController();
    assert(tx_client_controller);

    connect(
      this, 
      &CoreService2::SegmentLocalVideoSignal,
      tx_client_controller,
      &ITXClientController::HandleSegmentLocalRecordVideoRequestSlot);

    connect(
      this,
      &CoreService2::ControlVideoRecordSignal,
      tx_client_controller,
      &ITXClientController::HandleWholeVideoRecordRequestSlot);
  }

  IFilePlayController *file_play_controller =
    controller_manager->GetFilePlayController();

  // TODO: 移到其他地方
  udp_report_receiver_ = new UDPReportReceiver(nullptr);
  bool bind = udp_report_receiver_->Bind(
    "127.0.0.1",  ini_config.getCoolviewQosServerUdpPort());
  if (bind) {
    connect(
      this, &CoreService2::StartUDPReceiverSignal,
      udp_report_receiver_, &UDPReportReceiver::StartSlot);
    connect(
      this, &CoreService2::StopUDPReceiverSignal,
      udp_report_receiver_, &UDPReportReceiver::StopSlot);
    connect(
      udp_report_receiver_, 
      &UDPReportReceiver::ReceiveUDPQoSReportSignal,
      conference_controller, 
      &IConferenceController::HandleReceiveUDPQoSReportSlot);

    if (ini_config.IsModelTX()) {
      ITXController *tx_controller = 
        controller_manager->GetTXController();
      assert(tx_controller);

      connect(
        udp_report_receiver_, 
        &UDPReportReceiver::ReceiveUDPRecReportSignal,
        tx_controller, 
        &ITXController::HandleReceiveUDPRecReportSlot);
      connect(
        udp_report_receiver_, 
        &UDPReportReceiver::ReceiveUDPQoSReportSignal,
        tx_controller, 
        &ITXController::HandleReceiveUDPQoSReportSlot);
    } else {
      ITXClientController *tx_client_controller = 
        controller_manager->GetTXClientController();
      assert(tx_client_controller);

      connect(udp_report_receiver_,
        &UDPReportReceiver::ReceiveUDPRecReportSignal,
        tx_client_controller,
        &ITXClientController::ReceiveUDPRecReportSignal);
    }

    connect(
      udp_report_receiver_,
      &UDPReportReceiver::ReceiveUDPFilePlayReportSignal,
      file_play_controller,
      &IFilePlayController::HandleReceiveUDPFilePlayReportSlot);

    udp_report_receiver_->moveToThread(&udp_thread_);
    udp_thread_.start();

    emit StartUDPReceiverSignal();
  }

  ConnectWithPTZController();

  has_initialized_ = true;
}

void CoreService2::ConnectWithPTZController() {
  // TODO: 连接this和PTZController
}


void CoreService2::TeleCommand(
    int command_index, const QByteArray &intput_array) {

  auto it = command_handler_dict_.find(command_index);
  if (it == command_handler_dict_.end()) {
    qDebug() << "CoreService2: unknown command index: " << command_index;
  } else {
    qDebug() << "CoreService2: receive telecommand index: " << command_index;
    it.value()(*this, intput_array);
  }
}

void CoreService2::ShutDown( const QByteArray &input_array ) {
  emit ShutDownSignal();
}

void CoreService2::Restart(const QByteArray &input_array) {
  emit RestartSignal();
}

void CoreService2::SendQoS( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  RtpStatItemInfo info;
  in >> info;
  RtpStatItem item;
  RtpStatItemInfo::RtpStatItemInfo2RtpStatItem(info, item);
  emit SendQoSSignal(item);
}

void CoreService2::StartRecvStreamMedia( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString remote_vuri;
  int screen_index;
  int seat_index;
  int use_small_video;
  in >> remote_vuri >> screen_index >> seat_index >> use_small_video;
  emit ReceiveVideoSignal(remote_vuri, screen_index, seat_index, use_small_video);
}

void CoreService2::StopRecvStreamMedia( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString vuri;
  in >> vuri;
  emit StopVideoSignal(vuri);
}

void CoreService2::SetVolume( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString direction;
  int volume;
  in >> direction;
  in >> volume;
  AudioDeviceType device_type = direction == "in" ? 
    AudioDeviceType::kMicrophone : AudioDeviceType::kSpeaker;
  emit ChangeVoiceVolumeSignal(device_type, volume);
}

void CoreService2::StopLocalAudio( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  int send;
  in >> send;
  emit ControlSendLocalAudioSignal(send);
}

void CoreService2::StopLocalVideo( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  int virtual_index = -1, send;
  in >> send >> virtual_index;
  emit ControlSendLocalVideoSignal(virtual_index, send);
}

void CoreService2::StartSendScreenMedia( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QRect wnd;
  in >> wnd;
  emit CreateSendScreenMediaSignal(wnd);
}

void CoreService2::StartRecvScreenMedia( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString uri;
  int screen_index;
  in >> uri >> screen_index;
  emit ReceiveDocumentSignal(uri, screen_index);
}

void CoreService2::StopRecvScreenMedia( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString uri;
  in >> uri;
  emit StopDocumentSignal(uri);
}

void CoreService2::ControlAudioRecv( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString uri;
  bool receive;
  in >> uri >> receive;
  emit ControlAudioReceiveSignal(uri, receive);
}

void CoreService2::JoinConference( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString conference_uri;
  in >> conference_uri;
  emit JoinConferenceSignal(conference_uri);
}

void CoreService2::LeaveConference( const QByteArray &input_array ) {
  emit LeaveConferenceSignal();
}

void CoreService2::ChangeScreenLayout( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  int screen_index;
  int layout_index;
  in >> screen_index >> layout_index;
  emit ChangeSceenLayoutSignal(
    screen_index, IDisplayController::ConvertToScreenLayout(layout_index));
}

void CoreService2::CloseScreenRecv( const QByteArray &input_array ) {
  emit StopDocumentSignal(QString());
}

void CoreService2::OpenScreenShare( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString filename;
  int screen_index;
  in >> filename >> screen_index;
  emit OpenDocumentSignal(filename, screen_index);
}

void CoreService2::SharedScreenControl( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  bool enable;
  in >> enable;
  emit ControlDocumentSharedSignal(enable);
}

void CoreService2::CloseScreenShare( const QByteArray &input_array ) {
  emit ControlDocumentSharedSignal(false);
}

void CoreService2::PPTControlCommand( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  int type;
  in >> type;
  emit ControlPPTSignal(type);
}

void CoreService2::SetPanelVisible( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  bool visible;
  in >> visible;
  emit ShowPanelSignal(visible);
}

void CoreService2::GetPanelState( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  int telecontroll_index;
  in >> telecontroll_index;
  emit GetPanelVisibleStateSignal(telecontroll_index);
}

void CoreService2::SegmentVideo( const QByteArray &input_array ) {
  emit SegmentLocalVideoSignal();
}

void CoreService2::RecvMonitorConfinfo( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  MonitorConfigInfo monitorConfigInfo;
  in >> monitorConfigInfo;
  emit NetworkBandwidthSignal(
    monitorConfigInfo.netWorkInBandWidth, 
    monitorConfigInfo.netWorkOutBandWidth);
}

void CoreService2::RecoverMediaProcess( const QByteArray &input_array ) {
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  MediaMonitoredProcess media_process ;
  in >> media_process;
  if (media_process.mediaType == STREAM_MEDIA_RECV) {
    emit RecoverMediaProcessSignal(kStreamType, false, media_process.userID);
  }
}

void CoreService2::MoveToMainThreadSlot() {
  moveToThread(QApplication::instance()->thread());
}

void CoreService2::ControlVideoRecord( const QByteArray &input_array )
{
  QDataStream in(input_array);
  in.setVersion(QDataStream::Qt_4_4);
  QString user_id;
  int type;
  QByteArray param;
  in >> user_id >> type >> param;
  emit ControlVideoRecordSignal(user_id, type, param);
}










