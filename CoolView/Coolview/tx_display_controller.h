#ifndef TX_DISPLAY_CONTROLLER_H
#define TX_DISPLAY_CONTROLLER_H

#include "display_controller.h"

class TXDisplayController : public DisplayController
{
  Q_OBJECT

public:
  TXDisplayController(QObject *parent, MediaManager *media_manager);
  ~TXDisplayController();

 public Q_SLOTS:
  // 处理接收视频请求
  void HandleReceiveVideoRequestSlot(
    const QString &vuri, 
    int screen_index, 
    int seat_index, 
    bool use_small_video) override;

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

  // 处理加载共享文件的请求
  void HandleOpenDocumentRequestSlot(
    const QString &filename, 
    int screen_index) override;

  // 改变布局
  void HandleScreenLayoutRequestSlot( 
    int screen_index, 
    IDisplayController::ScreenLayout layout) override;

private:
  ScreenLayout ValidateScreenLayout(int layout);

private:
  ScreenLayout screen_type_;
};

#endif // TX_DISPLAY_CONTROLLER_H
