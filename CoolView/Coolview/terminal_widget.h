#ifndef TERMINAL_WIDGET_H
#define TERMINAL_WIDGET_H

#include <QWidget>
#include <QList>
#include <QTimer>

#include "ui_terminal_widget.h"
#include "terminal_description.h"

class QMouseEvent;
class IControllerManager;
class VirtualTerminalWidget;

class TerminalWidget : public QWidget {
  Q_OBJECT
public:
  enum {
    kWidgetsPerRow = 2,
  };

public:
  TerminalWidget(QWidget *parent = 0);
  ~TerminalWidget();

  void Initialize(IControllerManager *controller_manager);

  void AddVirtualTerminal(ConstTerminalPointer terminal);
  void UpdateVirtualTerminal(ConstTerminalPointer terminal);

  bool is_online() const { return is_online_; }
  void SetAudioReceivedMark(bool receive);
  void ActivateSpeakControl(bool activate);

  bool is_main_speaker_terminal() const {
    return is_main_speaker_terminal_;
  }

Q_SIGNALS:
  void PermitSpeakSignal(const QString &uri, bool allow);
  void ControlReceiveAudioSignal(const QString &uri, bool receive);
  void StopReceiveVideoSignal(const QString &vuri);

private Q_SLOTS:
  void HandleReceiveVideoNotifySlot(
    const QString &vuri);

  void HandleStopVideoNotifySlot(
    const QString &vuri);
 
  void PermitSpeakSlot();
  void ControlAudioReceivedSlot();
  void StopReceiveAllVideoSlot();

  void HideVirtualTerminalsSlot();
  void TerminalOnlineAnimationSlot();

private:
  typedef QList<VirtualTerminalWidget*> VirtualTerminalList;

  VirtualTerminalWidget* FindVirtualTerminal(int virtual_index);

  void CreateMenuAndActions();
  void UpdateStatus(ConstTerminalPointer terminal);
  void ShowVirtualTerminals();

  void set_online(bool is_online);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void PerformDrag();

  void StartOnlineAnimation();
  void StopOnlineAnimation();

private:
  Ui::TerminalWidget ui_;
  
  IControllerManager *controller_manager_;

  QMenu *menu_;

  bool has_initialized_;
  bool setted_by_first_terminal_;
  bool show_virtual_terminals_;

  bool is_online_;
  bool is_audio_received_;
  bool is_main_speaker_terminal_;

  QString terminal_uri_;
  QString image_url_;

  // 记录播放的虚拟终端数
  int received_count_;
  VirtualTerminalList virtual_terminal_widgets_;
  
  // the start point of the drag
  QPoint start_pos_;
  
  QTimer hide_timer_;

  // 用于用户上线时，显示字体颜色的变化动作
  bool previous_status_is_offline_;
  QTimer animate_online_timer_;
  int current_animation_index_;
  int current_loop_;
  static const int animate_interval_ = 400;
  static const int animate_loops_ = 4;
};

#endif // TERMINAL_WIDGET_H
