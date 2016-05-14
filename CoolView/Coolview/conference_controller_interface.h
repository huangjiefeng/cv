#ifndef _CONFERENCE_CONTROLLER_INTERFACE_H_
#define _CONFERENCE_CONTROLLER_INTERFACE_H_

#include <QObject>

class QRect;

#include "terminal_description.h"
#include "conference_description.h"
#include "terminal_manager.h"

#include "util/report/RtpStat.h"
#include "msg_receiver_interface.h"

enum MediaStreamType;
enum MediaControlType;

class IConferenceController : public QObject {
  Q_OBJECT
 public:
  enum ConferenceState {
    kIsJoiningConference = 0,
    kIsInConference,
    kIsLeavingConference,
    kIsNotInConference,
  };

  enum JoinConferenceError {
    kOk = 0,
    kUnknownError,
    kErrorOfIsAlreadyInOneConference,
    kErrorOfIsJoiningOneConference,
    kErrorOfConferenceNotExist,
    kErrorOfJoinRejected,
    kErrorOfIsLeavingConference,
    kErrorOfPasswordNeeded,
  };

 public:
  IConferenceController(QObject *parent) : QObject(parent) {}
  virtual ~IConferenceController() {}

  static void RegisterMetaType() {
    qRegisterMetaType<IConferenceController::ConferenceState>(
      "IConferenceController::ConferenceState");
    qRegisterMetaType<IConferenceController::JoinConferenceError>(
      "IConferenceController::JoinConferenceError");
    qRegisterMetaType<RtpStatItem>("RtpStatItem");
  }

  // 为了线程安全，一下public成员函数，只能供controller调用
  virtual bool IsInConference() const = 0;
  virtual bool IsMulticastConference() const = 0;
  virtual ConferenceState GetCurrentState() const = 0;
  virtual ConferencePointer GetCurrentConference() const = 0;
  virtual TerminalManager &GetTerminalManager() /*const*/ = 0;

 Q_SIGNALS:
  void NotifyConferenceStateChangedSignal(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state);

  void NotifyJoinConferenceErrorSignal(
    const QString &conference_uri, 
    IConferenceController::JoinConferenceError error);

  void NotifyConferenceListReceivedSignal(
    const ConstConferenceList &conference_list);

  void NotifyTerminalReceivedSignal(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

  void NotifyTerminalRemovedSignal(
    const QString &terminal_uri,
    const QString &conference_uri);

  void NotifyTerminalUpdateSignal(
    ConstTerminalPointer terminal,
    const QString &conference_uri);
  
  void NotifyTerminalReloginSignal(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

  void NotifyTerminalCountChangedSignal(
    int count, 
    const QString &conference_uri);

  // 通知接收remote_vuri的音视频
  void NotifyStreamMediaReceiveStartedSignal(const QString &remote_vuri);

  // 通知接收remote_vuri的回复结果
  void NotifyStreamMediaReceiveReplySignal(const QString &remote_vuri, bool permission);
  
  // 通知停止接收remote_vuri的音视频
  void NotifyStreamMediaReceiveStoppedSignal(const QString &remote_vuri);

  //清空DisplayController中保存的音频列表   //  [12/10/2015 slt begin]
  void NotifyEraseAudioDisplayDictSignal();

  // 通知接收remote_uri的共享屏幕
  void NotifyScreenReceiveStartedSignal(
    const QString &remote_uri,
    int screen_index);

  // 通知停止接收remote_uri的共享屏幕
  void NotifyScreenReceiveStoppedSignal(
    const QString &remote_uri, 
    int screen_index);
    
  // 通知允许terminal发言
  void NotifyTerminalSpeakSinal(const QString &remote_uri, bool allow);

  void NotifyRTPStatReceivedSignal(const RtpStatItem &rtp_state);
  
  void NotifyTerminalHandUpSignal(bool handup);



  // 请求自动接收remote_vuri的音视频
  void RequestStreamMediaAutoReceiveSignal(const QString &remote_vuri);



 public Q_SLOTS:
  virtual void HandleJoinConferenceRequestSlot(const QString &uri) = 0;
  virtual void HandleLeaveConferenceRequestSlot() = 0;
  virtual void HandleConferenceListRequestSlot() = 0;
  virtual void HandleTerminalListRequestSlot() = 0;
  virtual void HandleHandUpRequestSlot(bool hand_up) = 0;

  // 发送QoS信息给QoS服务器
  virtual void HandleReceiveUDPQoSReportSlot(
    const RtpStatItem& rtp_stat) = 0;

  // 向指定用户请求媒体流
  // seat: 视频窗口所在位置，1表示第一个窗口，以此类推
  virtual void HandleAddRecvStreamMediaRequestSlot(
    const QString &remote_vuri,
    int screen_index, 
    int seat,
    bool use_small_video,
    bool enable_audio) = 0;

  // 向指定用户请求媒体流后，处理确认超时
  virtual void HandleStartMediaReplyTimeoutSlot(
    const QString &remote_vuri) = 0;

  // 告知远程用户停止发送视频流到本地
  virtual void HandleRemoveRecvStreamMediaRequestSlot(
    const QString &remote_vuri) = 0;

  // 设置是否允许远程终端发言
  virtual void HanldePermitTerminalSpeakRequestSlot(
    const QString &uri, 
    bool allow) = 0;

  // 改变视屏显示位置
  virtual void HandleChangeVideoSeatRequestSlot( 
    const QString &remote_vuri,
    int screen_index,
    int seat) = 0;

  // 创建本地发送进程
  virtual void HandleCreateSendStreamMediaRequestSlot() = 0;

  // 告知所有参会终端的媒体发送状态，并进行对媒体流进行控制
  // type: 媒体类型，只支持kAudioControlType和kVideoControlType
  // enable: 如果发送媒体流，则为true，否则为false
  // virtual_index: 指定为哪一路视频流，音频为所有路
  virtual void HandleControlSendMediaRequestSlot(
    MediaControlType type,
    bool enable,
    int virtual_index = 0) = 0;

  // 创建发送共享屏幕流
  virtual void HandleCreateSendScreenMediaRequestSlot(const QRect &wnd) = 0;

  // 创建接收共享屏幕流
  virtual void HandleCreateRecvScreenMediaRequestSlot(
    const QString &remote_uri, 
    int screen_index) = 0;

  // 告知远程用户停止发送屏幕流
  virtual void HandleRemoveRecvScreenMediaRequestSlot(
    const QString &remote_uri,
    int screen_index) = 0;

  // 控制本地屏幕共享发送/停止
  // 同时告知其他终端创建/销毁当前终端的屏幕流接收进程
  virtual void HandleScreenShareControlRequestSlot(bool enable) = 0;

  // 恢复媒体进程
  virtual void HandleRecoveryMediaProcessRequestSlot(
    MediaStreamType type, 
    bool is_send, 
    const QString &remove_user_id) = 0;

  virtual void HandleAllMediaProcessExitSlot() = 0;
};

#endif // _CONFERENCE_CONTROLLER_INTERFACE_H_