#ifndef CONFERENCE_CONTROLLER_H
#define CONFERENCE_CONTROLLER_H

#include <QMutex>
#include <QHash>
#include <QTimer>
#include <QRect>
#include <QLinkedList>

#include "conference_controller_interface.h"
#include "stream_receiver_info_manager.h"
#include "terminal_manager.h"

#include "session_controller_interface.h"
#include "floor_controller_interface.h"

#include "util/udp/UdpSender.h"

class RunningProfile;
class RunningConfig;

class MediaManager;
class IMsgManager;
class IMsgReceiver;
class IMsgSender;

class IControllerManager;

struct ConferenceJoinParam;
struct MediaRelayParam;
struct MediaReplyParam;
struct MediaControlParam;
struct ScreenControlParam;

class SDPInfo;
struct LoginPermissionInfo;
struct LoginRejectionInfo;
struct QoSServerInfo;
struct MediaControlMsg;
struct MediaStatusMsg;
struct MediaRelayMsg;
struct MediaReplyMsg;

enum MediaControlType;
enum MediaStreamType;

class TxMinitorIf;
class CvPerfMonitorIf;

class HttpMsgManager;

class ConferenceController : public IConferenceController {
  Q_OBJECT
 public:
  enum MyEnum {
    kSendHearBeatInfoInterval = 60000,
    kServerHeartBeatTimeout = 180000,
    kTestTimeout = 30000,
    kCheckTerminalListInterval = 20000,
  };

 public:
  ConferenceController(QObject *parent,
                       RunningProfile *profile,
                       RunningConfig *config,
                       MediaManager *media_manager,
                       IControllerManager *controller_manager);

  ~ConferenceController();

  void Initialize(
    IMsgManager *msg_manager,
    HttpMsgManager *http_msg_manager,
    ISessionController *session_controller,
    IFloorController *floor_controller);

  bool IsInConference() const;
  bool IsMulticastConference() const;

  ConferenceState GetCurrentState() const override { return state_; }
  ConferencePointer GetCurrentConference() const override { return current_conference_; };
  TerminalManager &GetTerminalManager() override { return terminals_; };

 public Q_SLOTS:
  void HandleConferenceListRequestSlot() override;
  void HandleJoinConferenceRequestSlot(const QString &uri) override;
  void HandleLeaveConferenceRequestSlot() override;
  void HandleTerminalListRequestSlot() override;
  void HandleHandUpRequestSlot( bool handup ) override;

  // 发送QoS信息给QoS服务器
  void HandleReceiveUDPQoSReportSlot(
    const RtpStatItem& rtp_stat) override;

  // 向指定用户请求媒体流
  // seat:视频窗口所在位置，1表示第一个窗口，以此类推
  void HandleAddRecvStreamMediaRequestSlot(
    const QString &remote_vuri,
    int screen_index, 
    int seat,
    bool use_small_video,
    bool enable_audio) override;

  // 向指定用户请求媒体流，处理确认超时 
  void HandleStartMediaReplyTimeoutSlot(
    const QString &remote_vuri) override;

  // 告知远程用户停止发送视频流到本地
  void HandleRemoveRecvStreamMediaRequestSlot(
    const QString &remote_vuri) override;

  // 改变视屏显示位置
  // 由底层逻辑判断
  void HandleChangeVideoSeatRequestSlot( 
    const QString &remote_vuri,
    int screen_index,
    int seat) override;

  // 设置是否允许远程终端发言
  void HanldePermitTerminalSpeakRequestSlot(
    const QString &uri, 
    bool allow) override;

  // 创建本地发送进程
  void HandleCreateSendStreamMediaRequestSlot() override;

  // 告知所有参会终端的媒体发送状态，并进行对媒体流进行控制
  // type: 媒体类型，只支持kAudioControlType和kVideoControlType
  // enable: 如果发送媒体流，则为true，否则为false
  // virtual_index: 指定为哪一路视频流，音频为所有路
  void HandleControlSendMediaRequestSlot(
    MediaControlType type, 
    bool enable,
    int virtual_index = 0) override;

  // 创建发送共享屏幕流
  void HandleCreateSendScreenMediaRequestSlot(const QRect &wnd) override;

  // 创建接收共享屏幕流
  void HandleCreateRecvScreenMediaRequestSlot(
    const QString &remote_uri, 
    int screen_index) override;

  // 告知远程用户停止发送屏幕流
  void HandleRemoveRecvScreenMediaRequestSlot(
    const QString &remote_uri,
    int screen_index) override;

  // 控制本地屏幕共享发送/停止
  // 同时告知其他终端创建/销毁当前终端的屏幕流接收进程
  void HandleScreenShareControlRequestSlot(bool enable) override;

  // 恢复媒体进程
  void HandleRecoveryMediaProcessRequestSlot(
    MediaStreamType type, 
    bool is_send, 
    const QString &remove_user_id) override;

  void HandleAllMediaProcessExitSlot() override;

Q_SIGNALS:
  void SendConferenceListRequestMsgSignal();
  void SendJoinConferenceRequestMsgSignal(
    const ConferenceJoinParam &conference_param,
    const SDPInfo &sdp_info);
  void SendServerOnlineTestSignal();
  void SendHeartBeatToKeepInConferenceMsgSignal(const QString &conference_uri);
  void SendLeaveConferenceRequestMsgSignal(const QString &conference_uri);
  void SendTerminalListRequestMsgSignal(const QString &conference_uri);
  void SendMainSpeakerInfoRequestSignal(const QString &conference_uri);
  void SendChairmanInfoRequestSignal(const QString &conference_uri);
  void SendSharedScreenInfoRequestSignal(const QString &conference_uri);
  void SendHandUpMsgSignal(
    const QString &conference_uri,
    const QString &local_uri,
    bool handup);
  void SendAllowSpeakMsgSignal(
    const QString &conference_uri,
    const QString &terminal_uri,
    bool allow);
  void SendRequestStartMediaMsgSignal(const MediaRelayParam &param);
  void SendReplyStartMediaMsgSignal(const MediaReplyParam &param);
  void SendRequestStopMediaMsgSignal(const MediaRelayParam &param);
  void SendReplyStopMediaMsgSignal(const MediaReplyParam &param);
  void SendScreenControlMsgSignal(const ScreenControlParam &param);
  void SendMediaControlMsgSignal(const MediaControlParam &param);

  void SendTerminalListRequestByCIDSignal(const QString &cid);
  void SendFloorControlInfoRequest(const QString &cid);

  // 发言权控制
  void CreateFloorControlClientSignal(
    uint16_t userid, const FloorControlInfoPointer &info);
  void ApplyFloorRequestSignal();
  void AcceptFloorRequestSignal(uint16_t user_id);
  void ReleaseFloorRequestSignal(uint16_t user_id);
  void DestoryFloorControlClientSignal();

 private Q_SLOTS:
  void HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state);

  void HandleOnlineMessageNotifySlot(const QString &type, const QString &from);
  void HandleQoSLoginPermissionNotifySlot(const LoginPermissionInfo &info);
  void HandleQoSLoginRejectionNotifySlot(const LoginRejectionInfo &info);
  void HandleQoSServerInfoNotifySlot(const QoSServerInfo &info);
  void HandleConferenceListReceivedNotifySlot(const ConferenceList &conferences);
  void HandleTerminalListReceivedNotifySlot(const TerminalList &terminals);
  void HandleTerminalLoginNotifySlot(const TerminalList &terminals);
  void HandleTerminalLogoutNotifySlot(const QString &terminal_uri);
  void HandleTerminalHandUpNotifySlot(const QString &terminal_uri);
  void HandleTerminalHandDownNotifySlot(const QString &terminal_uri);
  void HandleTerminalPermitSpeakNotifySlot(const QString &terminal_uri);
  void HandleTerminalForbidSpeakNotifySlot(const QString &terminal_uri);
  void HandleSpeakerTerminalNotifySlot(const QString &terminal_uri);
  void HandleChairmanTerminalNotifySlot(const QString &terminal_uri);
  void HandleChairmanUpdateNotifySlot(const QString &terminal_uri);
  void HandleSharedScreenControlNotifySlot(const QString &sender_ip, bool enable);
  void HandleSharedScreenTerminalNotifySlot(const QString &terminal_uri);
  void HandleStartMediaRelayNotifySlot(const MediaRelayMsg &info);
  void HandleStartMediaReplyNotifySlot(const MediaReplyMsg &info);
  void HandleStopMediaRelayNotifySlot(const MediaRelayMsg &info);
  void HandleStopMediaReplyNotifySlot(const MediaReplyMsg &info);
  void HandleRemoteMediaStatusNotifySlot(const MediaStatusMsg &info);
  void HandleMediaControlNotifySlot(const MediaControlMsg &info);
  void HandleFloorControlInfoNotifySlot(const FloorControlInfoPointer &info);

  // 发送心跳到会议服务器
  void SendOnlineHeartBeatSlot();
  // 向会议服务器发送在线测试心跳
  void SendServerOnlineTestSlot();
  // 检查会议服务器的心跳是否超时
  void ServerHeartBeatTimeoutSlot();
  // 检查终端列表是否到齐，若未到齐则发送终端列表请求
  void CheckTerminalListSlot();

  // 处理发言权状态
  void HandleFloorRequestStatusNotifySlot(
    uint16_t user_id, bfcpc::RequestStatus status);
  
 private:
  void ConnectWithMsgManager(IMsgManager *msg_manager);
  void ConnectWithMsgReceiver(IMsgReceiver *receiver);
  void ConnectWithMsgSender(IMsgSender *sender);

  // 申请进入会议，发送登陆消息到QoS服务器
  void ApplyForJoiningConference(const QString &uri);
  
  // 构造加入会议所需的参数param
  void ConstructJoinConferenceParam(
    ConstConferencePointer conference, 
    ConferenceJoinParam &param) const;
  
  // 构造一路有效的SDP消息
  void ConstructValidSDPInfo(SDPInfo &info) const;

  // 计算带宽需求
  int CalculateRequireBandwith() const;
  
  // 执行加入会议的动作
  void JoinConference(const QString &focus_uri);
  
  // 处理当被QoS拒绝登陆的事情
  void HandleConferenceJoinRejection(
    const QString &focus_uri, 
    const QString &reason);

  ConferencePointer FindConferenceByURI(const QString &uri);

  // 停止所有计时器
  void StopAllTimer();

  // 清空会议相关信息
  void ClearConferenceInfo();

  // 设置当前终端状态，并发送信号
  void SetState(const QString &conference_uri, ConferenceState state);
  
  // 处理当终端总数变化的事情
  void HandleTotalTerminalCountChanged(int new_total_terminal_count);
  
  bool HasReceivedAllTerminals() const;

  // TODO: 移到其他地方
  int SendMessageToNetworkMeasurer(const QString &msg) const;
  void SendUDPMessageToQoSServer(const QString &msg);

  // 正常终端初始化发送媒体流
  void InitSendStreamMedia();

  // 退出会议后，重置发送进程
  void ResetSendStreamMedia();

  // 创建本地回显和本地录制进程.
  // 注意:本地流控制以mediaID作为与底层交互的标识符,而非virtualURI
  void AddRecvLocalMedia(
    const QString &local_media_id, 
    int screen_index, 
    int seat, 
    bool is_small_video = false);

  // 停止本地流的接收. 
  // 注意:本地流控制以mediaID作为与底层交互的标识符,而非virtualURI
  void RemoveRecvLocalMedia( const QString &local_media_id);

  // 请求远程终端发送媒体流
  void RequestStartMediaRelay(
    TerminalPointer &remote_terminal, 
    bool use_small_video);

  // 根据请求信息发送一路音视频或共享屏幕流到远程终端
  void StartSendStreamMediaToTerminal(const MediaRelayMsg &info);
  // 根据请求信息停止一路音视频或共享屏幕流到远程终端
  void StopSendStreamMediaToTerminal(const MediaRelayMsg &info);

  // 添加发送一路音视频到远程终端
  void AddRemoteVideoAndAudioStreamReceiver(const StreamReceiverInfo &info);
  
  // 停止发送一路音视频到远程终端
  void RemoveRemoteVideoAndAudioStreamReceiver(
    const StreamReceiverInfo &info,
    bool search_by_recv_ip = true);

  // 停止与远程终端之间的媒体流传输，包括音视频流和共享屏幕流
  void StopStreamBetweenLocalAndRemoteTerminal(const QString &remote_uri);
  
  // 停止发送媒体流到远程终端，包括音视频流和共享屏幕流
  void StopSendStreamMediaToTerminal(const QString &recver_uri);
  
  // 停止从远程终端接收媒体流，包括音视频流和共享屏幕流
  void StopRecvStreamMediaFromTerminal(const QString &sender_uri);

  // 构造本地终端的默认虚拟终端uri，序号为0
  QString ConstructDefaultLocalVirtualURI() const;

  // 该uri对应的终端是否为本地终端
  bool IsLocalTerminal(const QString &uri) const;
  
  // 在report会议模式下，禁止发言意味着禁止发送音频和共享屏幕
  // 在disscus会议模式下，禁止发言意味着禁止发送共享屏幕
  void ForbidTerminalSpeak(const QString &terminal_uri);

  // 请求远程终端发送共享屏幕到本终端
  void StartRecvSharedScreen(const QString &remote_uri);

  // 通知遥控器更新主席
  void SendChairmanChangedNotify(bool enable);

  // 根据对终端信息执行的动作，发出终端消息信号
  void SendTeriminalNotifySignal(
    TerminalManager::ActionType action,
    TerminalPointer terminal);

 private:
   RunningProfile *profile_;
   RunningConfig *running_config_;
   MediaManager *media_manager_;
   IControllerManager *controller_manager_;

   // 是否初始化了
   bool has_initialized_;

   // 当前终端当前状态
   ConferenceState state_;
   // 是否在等待QoS服务器的允许
   bool is_waitting_permission_;

   // 当前会议URI
   // 包括正在进入，正在开会中、被拒绝加入、正在退出等状态
   QString focus_uri_;

   // 指向会议初始信息
   // 只有正在开会中，该指针才有效
   ConferencePointer current_conference_;
   QString speaker_uri_; // 在会议过程中可能会改变
   QString chairman_uri_;  // 在会议过程中可能会改变
   QString screen_shared_uri_; // 在会议过程中可能会改变
   FloorControlInfoPointer fcs_info_; //记录当前会议的发言权控制信息

   // 存放会议列表，保存的是初始的会议信息
   ConferenceDictionary conferences_;
   
   // 存放终端列表
   TerminalManager terminals_;
   
   // 当前会议的终端总数，不包括虚拟终端
   int total_terminal_count_;

   // 用于定时向会议服务器发送在线信息的计时器
   QTimer *send_heart_beat_timer_;
   
   // 用于服务器心跳超时的计时器
   QTimer *server_heart_beat_timer_;
   
   // 记录接收本地终端媒体流的远程终端信息
   StreamReceiverInfoManager receivers_;

   // 用于定时检查终端列表是否到齐的计时器
   QTimer *check_terminal_list_timer_;

   //  QoS报告发送器
   UdpSender qos_report_sender_;

   // 进入新会议后，终端列表是否第一次完整接收
   bool need_to_query_extra_info_;

   // TODO: 有问题？
   // 当本终端域名设置为ip时，在进入会议后，需要更新域名
   // 通过获取其他终端的域名，来更新自己的域名
   bool need_to_set_local_uri_;

   bool join_first_conference_;
};

inline
bool ConferenceController::IsInConference() const {
  return state_ == kIsInConference && current_conference_;
}

inline 
void ConferenceController::StopStreamBetweenLocalAndRemoteTerminal(
    const QString &remote_uri) {
  // 停止发送媒体流到离线终端
  StopSendStreamMediaToTerminal(remote_uri);
  // 关闭接收的音视频流
  StopRecvStreamMediaFromTerminal(remote_uri);
}

#endif // CONFERENCE_CONTROLLER_H
