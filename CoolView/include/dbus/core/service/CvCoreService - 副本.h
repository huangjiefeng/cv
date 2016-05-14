#ifndef CORE_SERVICE_2_H
#define CORE_SERVICE_2_H

#include <functional>

#include <QtCore/QtCore>
#include <QObject>

#include <QThread>

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

#include "display_controller_interface.h"

class UDPReportReceiver;

class IControllerManager;
enum AudioDeviceType;
enum MediaStreamType;

class CoreService2 : public QObject {
  Q_OBJECT
public:
  static CoreService2* getInstance() {
    static CoreService2 service;
    return &service;
  }

  void Initialize(
    IControllerManager *controller_manager);

public Q_SLOTS:
  // DBUS调用槽
  void TeleCommand(int command_index, const QByteArray &intput_array);

  // 将core service移到主线程中
  void MoveToMainThreadSlot();

Q_SIGNALS:
  void ShutDownSignal();
  void RestartSignal();

  void SendQoSSignal(const RtpStatItem &item);
  void JoinConferenceSignal(const QString &conference_uri);
  void LeaveConferenceSignal();
  void CreateSendScreenMediaSignal(const QRect &wnd);
  void ControlDocumentSharedSignal(bool enable);
  void SegmentLocalVideoSignal();
  void RecoverMediaProcessSignal(
    MediaStreamType media_type, 
    bool is_send,
    const QString &remote_user_id);

  void ReceiveVideoSignal(
    const QString &vuri, 
    int screen_index, 
    int seat_index, 
    bool use_small_video);
  void StopVideoSignal(const QString &vuri);

  void ControlAudioReceiveSignal(const QString &uri, bool receive);
  void ChangeVoiceVolumeSignal(AudioDeviceType type, int volume);
  
  void ControlSendLocalAudioSignal(bool enable);
  void ControlSendLocalVideoSignal(int virtual_index, bool enable);
 
  void OpenDocumentSignal(const QString &filename, int screen_index);
  void ReceiveDocumentSignal(const QString &uri, int screen_index);
  void StopDocumentSignal(const QString &uri);
  void ControlPPTSignal(int type);
  
  void ChangeSceenLayoutSignal(
    int screen_index, 
    IDisplayController::ScreenLayout layout);

  void ShowPanelSignal(bool visible);
  void GetPanelVisibleStateSignal(int telecontroller_id);

  void StartUDPReceiverSignal();
  void StopUDPReceiverSignal();

  void NetworkBandwidthSignal(double in_bandwidth, double out_bandwidth);

  void ControlVideoRecordSignal(const QString &user_id, int type, QByteArray param);

private:
  CoreService2();
  ~CoreService2();

  void RegisterHandlers();

  void ShutDown(const QByteArray &input_array);
  void Restart(const QByteArray &input_array);

  void SendQoS(const QByteArray &input_array);
  void JoinConference(const QByteArray &input_array);
  void LeaveConference(const QByteArray &input_array);
  void StartSendScreenMedia(const QByteArray &input_array);
  void SharedScreenControl(const QByteArray &input_array);

  void StartRecvStreamMedia(const QByteArray &input_array);
  void StopRecvStreamMedia(const QByteArray &input_array);
  void SetVolume(const QByteArray &input_array);
  void StopLocalAudio(const QByteArray &input_array);
  void StopLocalVideo(const QByteArray &input_array);
  void StartRecvScreenMedia(const QByteArray &input_array);
  void StopRecvScreenMedia(const QByteArray &input_array);
  void ControlAudioRecv(const QByteArray &input_array);

  void ChangeScreenLayout(const QByteArray &input_array);
  
  void CloseScreenRecv(const QByteArray &input_array);
  void OpenScreenShare(const QByteArray &input_array);
  void CloseScreenShare(const QByteArray &input_array);
  void PPTControlCommand(const QByteArray &input_array);
  
  void SegmentVideo(const QByteArray &input_array);
  
  void SetPanelVisible(const QByteArray &input_array);
  void GetPanelState(const QByteArray &input_array);
  
  void RecvMonitorConfinfo(const QByteArray &input_array);
  void RecoverMediaProcess(const QByteArray &input_array);

  void ControlVideoRecord(const QByteArray &input_array);

  void ConnectWithPTZController();

private:
  typedef std::function<
    void(CoreService2&, const QByteArray&)
  > CommandHandler;

  typedef QHash<int, CommandHandler> HandlerDict;

  HandlerDict command_handler_dict_;

  // TODO: 移到其他地方
  UDPReportReceiver *udp_report_receiver_;
  QThread udp_thread_;

  bool has_initialized_;
};

#endif // CORE_SERVICE_2_H
