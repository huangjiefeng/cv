#ifndef _DISPLAY_CONTROLLER_INTERFACE_H_
#define _DISPLAY_CONTROLLER_INTERFACE_H_

#include <QObject>

#include "media_description.h"

class IDisplayController : public QObject {
  Q_OBJECT
public:
  enum ScreenLayout {
    // TODO: 添加kLayoutOfUnknown的转换
    kLayoutOfUnknown = -1,
    kLayoutOf1x1 = 0,
    kLayoutOf2x2 = 1,
    kLayoutOf3x3 = 2,
    kLayoutOf1plus5 = 3,
    kLayoutOfAuto = 11,
    kLayoutOfDocument = 10,
    kLayoutOfTop1x2 = 12,
    kLayoutOf4x4 = 13,
    kLayoutOfAirplay = 14,
  };

  static ScreenLayout ConvertToScreenLayout(int layout) {
    // TODO: 根据ConferenceRoom::ConfRoomDisplayModel转换
    return static_cast<ScreenLayout>(layout);
  }

public:
  IDisplayController(QObject *parent) : QObject(parent) {}
  virtual ~IDisplayController() {}

  static void RegisterMetaType() {
    qRegisterMetaType<IDisplayController::ScreenLayout>(
      "IDisplayController::ScreenLayout");
  }

  virtual int GetScreenCount() const = 0;

Q_SIGNALS:
  // 告知接收了vuri视频
  void NotifyReceiveVideoSignal(
    const QString &vuri,
    int screen_index, 
    int seat_index, 
    bool use_small_video);

  // 告知停止了接收vuri视频
  void NotifyStopVideoSignal(
    const QString &vuri,
    int screen_index,
    int seat_index);

  // 告知uri音频的接收状态
  void NotifyReceiveAudioSignal(const QString &uri, bool enable);

  // 告知接收了uri的共享屏幕
  void NotifyReceiveDocumentSignal(const QString &uri, int screen_index);
  // 告知停止了uri的共享屏幕
  void NotifyStopDocumentSignal(const QString &uri, int screen_index);

  // 告知RTCP的显示状态：显示与关闭
  void NotifyShowRTCPMessageSignal(bool show, int screen_index);

  // 告知本地音频的发送状态
  void NotifySendLocalAudioSignal(bool enable);
  // 告知本地视频的发送状态
  // 当virtual_index为-1时，为所有本地视频
  void NotifySendLocalVideoSignal(int virtual_index, bool enable);

  // 告知音量发生了变化
  void NotifyVoiceChangeSignal(AudioDeviceType type, int volume);

  // 告知静音发生了变化
  void NotifyMuteChangeSignal(AudioDeviceType type, bool is_mute);

  // 告知uri终端被允许了发言
  void NotifyTerminalSpeakSignal(const QString &uri, bool allow);

  // 告知屏幕布局发生了变化
  void NotifyScreenLayoutChangedSignal(
    int screen_index, 
    IDisplayController::ScreenLayout layout);

  // 请求界面显示或隐藏控制面板
  // 界面控制面板须处理该信号
  void SetPanelVisibleSignal(bool visible);

  // 请求获取界面visible的状态
  // 界面控制面板须处理该信号
  void CheckPanelVisibleStateSignal(int telecontroller_index);

public Q_SLOTS:
  // 处理接收视频请求
  virtual void HandleReceiveVideoRequestSlot(
    const QString &vuri, 
    int screen_index, 
    int seat_index, 
    bool use_small_video) = 0;

  // 处理自动接收终端音视频
  virtual void HandleRequestStreamMediaAutoReceiveSlot(
    const QString &remote_uri) = 0;

  // 处理停止接收视频请求
  virtual void HandleStopVideoRequestSlot(const QString &vuri) = 0;

  // 处理接收和断开音频请求
  virtual void HandleReceiveAudioRequestSlot(
    const QString &uri, 
    bool enable) = 0;

  // 处理接收共享屏幕请求
  virtual void HandleReceiveDocumentRequestSlot(
    const QString &uri,
    int screen_index) = 0;

  // 处理停止接收指定uri的共享屏幕请求
  // 若uri为空，则停止接收所有的共享屏幕
  virtual void HandleStopDocumentRequestSlot(
    const QString &uri) = 0;

  // 处理加载共享文件请求
  virtual void HandleOpenDocumentRequestSlot(
    const QString &filename,
    int screen_index) = 0;

  virtual void HandlePPTControlSlot(int type) = 0;

  // 处理显示和关闭RTCP消息请求
  virtual void HandleShowRTCPMessageRequestSlot(
    bool show, 
    int screen_index) = 0;

  // 控制本地音频的启停
  virtual void HandleSendLocalAudioRequestSlot(bool enable) = 0;

  // 控制本地视频的启停
  virtual void HandleSendLocalVideoRequestSlot(int virtual_index, bool enable) = 0;

  // 改变speaker或mic的音量
  virtual void HandleVoiceChangeRequestSlot(
    AudioDeviceType type,
    int volume) = 0;

  // 改变speaker或mic的静音状态
  virtual void HandleVoiceMuteRequestSlot(
    AudioDeviceType type, 
    bool isMute) = 0;

  // 允许和禁止发言
  virtual void HandlePermitSpeakRequestSlot(
    const QString &uri,
    bool allow) = 0;

  // 处理屏幕布局请求
  virtual void HandleScreenLayoutRequestSlot(
    int screen_index, 
    IDisplayController::ScreenLayout layout) = 0;

  // 界面改变控制面板显示状态后，需要发送该信号到此槽
  virtual void HandlePanelVisibleStateChangedNotifySlot(
    int telecontroller_index, bool visible) = 0;

  // 接收所有本地回显
  virtual void HandleReceiveAllLocalVideoNotifySlot() = 0;
};


#endif // _DISPLAY_CONTROLLER_INTERFACE_H_