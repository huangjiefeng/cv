#ifndef LOCAL_TERMINAL_WIDGET_H
#define LOCAL_TERMINAL_WIDGET_H

#include <QWidget>
#include <QList>

#include "ui_local_terminal_widget.h"

#include "conference_controller_interface.h"

#include "terminal_config_type.h"

class QPushButton;
class VideoSendControlButton;
class AudioSendControlButton;
class IControllerManager;
class IDisplayController;
class HandUpButton;
class SpeakerVolumeControlButton;
class FilePlayCtrlWidget;

//class LocalSpeakerWidget;
//class LocalMicWidget;

class LocalTerminalWidget : public QWidget {
  Q_OBJECT

public:
  LocalTerminalWidget(QWidget *parent = 0);
  ~LocalTerminalWidget();

  void Initialize(IControllerManager *controller_manager);

Q_SIGNALS:
  void StopVideoSignal(const QString &vuri);
  void StopDocumentSignal(const QString &uri);
  void ConfigCameraSignal(int index);
  void DisplayTerminalConfigSignal(TerminalConfigType type);

  void RequestLogoutSignal();
  void RequestShutdownSignal();
  void RequestRestartSignal();
  void RequestQuitSystemSignal();

private Q_SLOTS:
  void HandleConferenceStateChangedNotifySlot(
    const QString &confernece_uri,
    IConferenceController::ConferenceState state);

  void HandleStartVideoNotifySlot(const QString &vuri);
  void HandleStopVideoNotifySlot(const QString &vuri);

  void DisplayDeviceConfigSlot();
  void DisplayMicConfigSlot();
  void DisplaySpeakerConfiSlot();
  void DisplayDeviceTestConfigSlot();
  void DisplayMonitorConfigSlot();

  void ChangeSettingModeSlot(bool setting);

private:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  void resizeEvent(QResizeEvent *event);

  void InitializeExtraSettingButtons();
  void InitializeExitButton();
  void AddButtonsToLayout();
  QPushButton* CreateButton(const QString &icon, const QString &tooltip);

  int CaculateWidgetsPerRow(int widget_width, int layout_width);
  void AddWidgetToLayout(QWidget *widget, int row, int col);

  void SetLocalTerminalDisplayStatus(const QString &vuri, bool show);
  void CleatLayout();

  void ShowFilePlayCtrlWidget(bool show);

private:
  Ui::LocalTerminalWidget ui_;
  int widget_width_;
  int widgets_per_row_;
  IControllerManager *controller_manager_;
  
  //LocalSpeakerWidget *speaker_;
  //LocalMicWidget *mic_;
  QPushButton *device_setting_button_;
  QList<VideoSendControlButton*> virtual_terminals_;
  AudioSendControlButton *send_audio_button_;
  HandUpButton *hand_up_button_;
  SpeakerVolumeControlButton *speaker_volume_control_button_;

  // extra setting button
  //QPushButton *volume_button_;
  //QPushButton *device_test_button_;
  QPushButton *monitor_button_;
  bool is_setting_mode_;

  // exit button and actions
  QPushButton *exit_button_;
  QMenu *exit_menu_;

  // virtual conf control
  FilePlayCtrlWidget *play_ctrl_widget_;
};

#endif // LOCAL_TERMINAL_WIDGET_H
