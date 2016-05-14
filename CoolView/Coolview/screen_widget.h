#ifndef SCREEN_WIDGET_H
#define SCREEN_WIDGET_H

#include <QWidget>

#include "ui_screen_widget.h"
#include "display_controller_interface.h"

class SeatWidget;
class IControllerManager;

class ScreenWidget : public QWidget {
  Q_OBJECT

public:
  ScreenWidget(QWidget *parent, int screen_index);
  ~ScreenWidget();

  void Initialize(IControllerManager *controller_manager);

  void AddVideoSeat(
    const QString &vuri,
    const QString &name,
    int seat_index, 
    bool use_small_video);
  void RemoveVideoSeat(const QString &vuri, int seat_index);
  void AddDocument(const QString &uri, const QString &name);
  void RemoveDocument(const QString &uri);
  void SetShowReceiveRTCPMessage(bool show);
  void ChangeScreenLayout(IDisplayController::ScreenLayout layout);

  void ResetScreenWidget();

Q_SIGNALS:
  // 请求改变显示器布局
  void ChangeScreenLayoutRequestSignal(
    int screen_index, 
    IDisplayController::ScreenLayout layout);
  void ReceiveVideoRequestSignal(
    const QString &vuri,
    int screen_index, 
    int seat_index, 
    bool use_small_video);
  void StopVideoRequestSignal(const QString &vuri);
  void ReceiveDocumentRequestSignal(const QString &uri, int screen_index);
  void StopDocumentRequestSignal(const QString &uri, int screen_index);
  void ShowRTCPMessageRequestSignal(bool show, int screen_index);

private Q_SLOTS:
  // 发送显示器布局请求的slot
  void OneMultiOneSlot();
  void TwoMultiTwoSlot();
  void ThreeMultiThreeSlot();
  void OnePlusFiveSlot();
  void AutoLayoutSlot();
  void DocumentLayoutSlot();
  void ShowRTCPMessageSlot();
  void OneMultiTwoTopSlot();
  void FourMultiFourSlot();
  void AirPlaySlot();

  // 发送视频控制请求的slot
  void HandleReceiveVideoRequestSlot(
    const QString &vuri, 
    int seat_index, 
    bool use_small_video);
  void HandleStopVideoRequestSlot(const QString &vuri, int seat_index);
  void HandleReceiveDocumentRequestSlot(const QString &uri, int seat_index);
  void HandleStopDocumentRequestSlot(const QString &uri, int seat_index);

private:
  void mouseReleaseEvent(QMouseEvent *event);

  void ConnectWithControllerManager(IControllerManager *controller_manager);
  void SetupGui();
  void ConnectWithSeatWidget(SeatWidget *seat);
  void CreateMenuAndActions();
  void ClearMenuMarks();
  void ClearLayout();
  void ClearScreen();
  void ResetSeats();

  // 处理显示器布局变更
  void HandleOneMultiOneNotify();
  void HandleTwoMultiTwoNotify();
  void HandleThreeMultiThreeNotify();
  void HandleOnePlusFiveNotify();
  void HandleAutoLayoutNotify();
  void HandleDocumentLayoutNotify();
  void HandleTopOneMultiTwoNotify();
  void HandleFourMultiFourNotify();
  void HandleAirPlayNotify();
private:
  static const int kMaxVideoSeatCount = 16;

  Ui::ScreenWidget ui_;
  QMenu *screen_menu_;

  QList<SeatWidget*> seat_list_;

  int screen_index_;
  bool is_rtcp_shown_;
  bool is_model_tx_;
  bool has_initialized_;
};

#endif // SCREEN_WIDGET_H
