#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <QHash>
#include <QTime>
#include <QTimer>

#include "display_controller_interface.h"
#include "conference_controller_interface.h"

class MediaManager;
class TelecontrollerIf;

class DisplayController : public IDisplayController {
  Q_OBJECT

 public:
   DisplayController(QObject *parent, MediaManager *media_manager);

  virtual ~DisplayController();

  void Initialize(IConferenceController *conference_controller);

  int GetScreenCount() const override { return screen_count_; }

 public Q_SLOTS:
  // 处理接收视频请求
  void HandleReceiveVideoRequestSlot(
    const QString &vuri, 
    int screen_index, 
    int seat_index, 
    bool use_small_video) override;

  // 处理自动接收终端音视频
  void HandleRequestStreamMediaAutoReceiveSlot(
    const QString &remote_uri) override;

  // 处理停止接收视频请求
  void HandleStopVideoRequestSlot(const QString &vuri) override;

  // 处理接收和断开音频请求
  void HandleReceiveAudioRequestSlot(
    const QString &uri, 
    bool enable) override;

  // 处理接收共享屏幕请求
  void HandleReceiveDocumentRequestSlot(
    const QString &uri,
    int screen_index) override;

  // 处理停止接收指定uri的共享屏幕请求
  // 若uri为空，则停止接收所有的共享屏幕
  void HandleStopDocumentRequestSlot(
    const QString &uri) override;

  // 处理加载共享文件的请求
  void HandleOpenDocumentRequestSlot(
    const QString &filename, 
    int screen_index) override;

  void HandlePPTControlSlot(int type) override;

  // 处理显示和关闭RTCP消息请求
  void HandleShowRTCPMessageRequestSlot(
    bool show, 
    int screen_index) override;
 
  // 控制本地音频的启停
  void HandleSendLocalAudioRequestSlot(bool enable) override;

  // 控制本地视频的启停
  void HandleSendLocalVideoRequestSlot(int virtual_index, bool enable) override;

  // 改变speaker和mic的音量
  void HandleVoiceChangeRequestSlot(
    AudioDeviceType type,
    int volume) override;

  void HandleVoiceMuteRequestSlot(
    AudioDeviceType type,
    bool isMute) override;

  // 允许和禁止发言
  void HandlePermitSpeakRequestSlot(
    const QString &uri,
    bool allow) override;

  // 改变布局
  void HandleScreenLayoutRequestSlot( 
    int screen_index, 
    IDisplayController::ScreenLayout layout) override;

  // 处理控制面版状态改变的通知
  void HandlePanelVisibleStateChangedNotifySlot(
    int telecontroller_index, 
    bool visible) override;

  // 接收所有本地回显
  void HandleReceiveAllLocalVideoNotifySlot() override;

  //清空会议接受音频列表
  void HandleClearAudioDisplayDictNotifySlot();

 protected Q_SLOTS:
   // TODO: 移除该项，改为接收底层的音视频相关的通知
   // 在远程终端退出的时候停止相关的视频的UI显示
  void HandleTerminalUpdateNotifySlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

  // TODO: 移除该项，改为接收底层的音视频相关的通知
  // 在退出会议时，停止相关视频的UI显示
  void HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state);

  // 处理成功接收共享屏幕的信号
  void HandleScreenReceiveStartedNotifySlot(
    const QString &remote_uri, 
    int screen_index);

  // 处理成功停止接收共享屏幕的信号
  void HandleScreenReceiveStoppedNotifySlot(
    const QString &remote_uri,
    int screen_index);

  // 检查是否存在请求对方发送媒体流的请求超时
  void HandleCheckReceiveVideoAckSlot();

  // 收到发送端的确认，更改ack状态
  void HandleStreamMediaReceiveReplyNotifySlot(
    const QString &remote_vuri, bool permission);


 protected:
  struct VideoSeatInfo {
    QString terminal_vuri;
    int screen_index;
    int seat_index;
    bool use_small_video;
    bool is_ack; // 是否收到发送端确认信息，用于超时检查
    QTime ack_time_counter; // 从开始请求到当前的时间计数器，用于超时检查
  };

  struct SharedScreenSeatInfo {
    QString terminal_uri;
    int screen_index;
  };

  struct AudioSeatInfo {
    QString terminal_vuri;
    bool is_audio_enabled;
    bool is_audio_received;
  };

  typedef QHash<QString, VideoSeatInfo> DisplayDict;
  typedef QHash<QString, QList<AudioSeatInfo>> DisplayAudioDict;
  typedef QList<SharedScreenSeatInfo> SharedScreenList;
  typedef QList<IDisplayController::ScreenLayout> ScreenLayoutVector;

  void AddVideoSeat(
    const QString &vuri, 
    int screen_index, 
    int seat_index, 
    bool use_small_video);

  VideoSeatInfo* FindVideoSeat(const QString &vuri);
  VideoSeatInfo* FindVideoSeat(int screen_index, int seat_index);
  int FindFreeVideoSeat(int screen_index);
  int GetVideoSeatCount(int screen_index);

  void RearrangeAudioRender(const QString &id);
  
  SharedScreenSeatInfo* FindSharedScreenSeat(const QString &uri);
  void RemoveSharedScreenSeat(const QString &uri);

  bool IsLocalTerminalURI(const QString &vuri) const;

 protected:
  MediaManager *media_manager_;
  // key: id(vuri or localpreviewid), value: video seat
  DisplayDict video_display_dict_;
  // key: uri, value: QList<AudioSeatInfo>
  DisplayAudioDict audio_display_dict_;
  SharedScreenList shared_screen_list_;
  IConferenceController *conference_controller_;
  TelecontrollerIf *telecontroller_proxy_;
  int screen_count_;
  ScreenLayoutVector screen_layout_;

  QString current_conference_uri_;
  bool has_initialized_;
  bool is_model_hd_;

  // 用于检查是否存在请求对方发送媒体流的请求超时的计时器
  QTimer *check_receive_video_ack_timer_;
};

#endif // DISPLAY_CONTROLLER_H
