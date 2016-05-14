#ifndef MAIN_CONTROL_WINDOW_H
#define MAIN_CONTROL_WINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QtNetWork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QHash>

#include <dbus/performance/monitor/common/MonitorConfigCommon.h>
#include <util/desktop/screen_helper.h>

#include "ui_main_control_window.h"

#include "session_controller_interface.h"
#include "conference_controller_interface.h"
#include "display_controller_interface.h"

#include "terminal_config_type.h"


class HelpWidget;
class DeviceListWidget;
class WebPageWidget;
class IControllerManager;
class ControlPanelBooter;

class MainControlWindow : public QMainWindow {
  Q_OBJECT
    
public:
  MainControlWindow(QWidget *parent = 0);
  ~MainControlWindow();

  void Initialize(IControllerManager *controller_manager);

  //告知遥控器控制面板可视与否
  void PanelVisibleStateUpdateNotify( const int controllerIndex = -1);

  //更新系统相关的监控信息
  void UpgradeMonitorCofInfo(MonitorConfigInfo monitorConfigInfo);

  void SetLoginWidget(QWidget *login_widget);
  void SetDeviceWidget(DeviceListWidget *device_list_widget);

  void AddWidgetToConferenceArea(QWidget *widget) {
    widget->setParent(ui_.conferenceContents);
    ui_.verticalLayout->addWidget(widget);
  }

public Q_SLOTS:
  void SetWindowTitleSlot(const QString &title);
  void HandleActivateCameraSettingSlot(int index);
  void HandleActivateTerminalConfigSlot(TerminalConfigType type);

  void ConfirmAndLogoutSlot();
  void ConfirmAndQuitSystemSlot();
  void ConfirmAndShutdownSlot();
  void ConfirmAndRestartSystemSlot();

Q_SIGNALS:
  void RestartSystemSignal();
  void ShutDownSystemSignal();
  void QuitSystemSignal();
  void LogoutSignal();

  void NotifyVisibleStateChangedSignal(int telecontroller_id, bool visible);

private Q_SLOTS:
  // ui
  void ShowDeviceListSlot();

  void HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state);
  void HandleRecvSaviNotifySlot(bool res);
  void HandleConferenceStateChangedNotifySlot(
    const QString &uri, 
    IConferenceController::ConferenceState state);

  void HandleSetVisibleRequestSlot(bool visible);
  void HandleVisibleStateRequestSlot(int telecontroller_id);

  void HandleQuitSystemNotifySlot();

  void HandleScreenLayoutChangedNotifySlot(
    int screen_index, 
    IDisplayController::ScreenLayout layout);

  void HandleNewConfigNotifySlot();

  void HandlePrimaryScreenChangedNotifySlot(int primary_screen);

protected:
  //重载关闭事件
  void closeEvent( QCloseEvent * event ) override;

private:
  void SetupGui();
  void InitializeActions();

  void ActivateWindow();
  void SetWelcomeTitle();

  void QuitSystem();

  void MoveToTheCenterOfMainWindow(QWidget *widget);

  void SetGeometryInPrimaryScreen();
  void MoveControlPanelBooter();

private:
  Ui::MainControlWindow ui_;

  QWidget *login_widget_;
  DeviceListWidget *device_list_widget_;

  bool quit_process_;
  bool stay_login_;
  QString current_conference_uri_;
  
  QString savi_status_;
  QNetworkAccessManager* network_access_manager_;

  IControllerManager *controller_manager_;

  QShortcut *show_console_shortcut_;

  ControlPanelBooter *control_panel_booter_;
  bool need_move_control_panel_;
};

#endif // MAIN_CONTROL_WINDOW_H
