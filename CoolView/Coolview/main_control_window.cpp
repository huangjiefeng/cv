#include "main_control_window.h"

#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QShortcut>
#include <QTimer>

#include "CoolviewCommon.h"

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

#include "config/RunningConfig.h"
#include "util/ini/CVIniConfig.h"
#include "profile/RunningProfile.h"
#include "monitorConfig/MonitorConfig.h"

#include "devicelist_widget.h"
#include "web_page_widget.h"

#include "system_controller_interface.h"
#include "controller_manager_interface.h"
#include "daemon_controller_interface.h"
#include "control_panel_booter.h"

MainControlWindow::MainControlWindow(QWidget *parent)
    : QMainWindow(parent) {
  ui_.setupUi(this);
  quit_process_ = false;
  device_list_widget_ = nullptr;
  login_widget_ = nullptr;
  controller_manager_ = nullptr;
  show_console_shortcut_ = nullptr;
  control_panel_booter_ = nullptr;
  need_move_control_panel_ = true;

  SetGeometryInPrimaryScreen();
  setWindowFlags(Qt::FramelessWindowHint);
}

MainControlWindow::~MainControlWindow() {
  if (login_widget_) {
    login_widget_->deleteLater();
  }
  if (control_panel_booter_) {
    control_panel_booter_->deleteLater();
  }
}

void MainControlWindow::SetGeometryInPrimaryScreen()
{
  // 设置位置大小为主控制屏幕的右下角1/4的大小
  QRect rect = ScreenHelper::Instance()->GetAvailableGeometry(-1);
  setGeometry(
    rect.x() + rect.width() / 2,
    rect.y() + rect.height() / 2,
    rect.width() / 2,
    rect.height() / 2);
  
}

void MainControlWindow::Initialize( IControllerManager *controller_manager ) {
  assert(controller_manager);

  SetupGui();

  controller_manager_ = controller_manager;
  ISystemController *system_controller = 
    controller_manager->GetSystemController();
  assert(system_controller);
  // LoadMonitorConfig();

  InitializeActions();

  connect(this, SIGNAL(RestartSystemSignal()),
          system_controller, SLOT(HandleRestartSystemRequestSlot()));
  connect(this, SIGNAL(ShutDownSystemSignal()),
          system_controller, SLOT(HandleShutDownSystemRequestSlot()));
  connect(this, &MainControlWindow::QuitSystemSignal,
          system_controller, &ISystemController::HandleQuitSystemRequestSlot);
  connect(system_controller, &ISystemController::NotifyQuitSystemSignal,
          this, &MainControlWindow::HandleQuitSystemNotifySlot);
  connect(system_controller, &ISystemController::NotifyNewConfigSignal,
          this, &MainControlWindow::HandleNewConfigNotifySlot);

  ISessionController *session_controller =
    controller_manager->GetSessionController();
  assert(session_controller);
  connect(this, SIGNAL(LogoutSignal()),
          session_controller, SLOT(HandleLogoutRequestSlot()));
  connect(session_controller, SIGNAL(NotifyRecvSaviSignal(bool)),
          this, SLOT(HandleRecvSaviNotifySlot(bool)));
  connect(
    session_controller, 
    SIGNAL(NotifySessionStateChangedSignal(ISessionController::SessionState)),
    this,
    SLOT(HandleSessionStateChangedNotifySlot(ISessionController::SessionState)));

  IDisplayController *display_controller =
    controller_manager->GetDisplayController();
  assert(display_controller);
  connect(
    display_controller, &IDisplayController::SetPanelVisibleSignal,
    this, &MainControlWindow::HandleSetVisibleRequestSlot);
  connect(
    display_controller, &IDisplayController::CheckPanelVisibleStateSignal,
    this, &MainControlWindow::HandleVisibleStateRequestSlot);
  connect(
    this, 
    &MainControlWindow::NotifyVisibleStateChangedSignal,
    display_controller, 
    &IDisplayController::HandlePanelVisibleStateChangedNotifySlot);
  connect(
    display_controller, &IDisplayController::NotifyScreenLayoutChangedSignal,
    this, &MainControlWindow::HandleScreenLayoutChangedNotifySlot);

  IConferenceController *conference_controller =
    controller_manager->GetConferenceController();
  assert(conference_controller);
  connect(
    conference_controller, 
    &IConferenceController::NotifyConferenceStateChangedSignal,
    this, 
    &MainControlWindow::HandleConferenceStateChangedNotifySlot);

	//按下该组合键，显示后台进程控制台窗口，便于调试
  IDaemonController *daemon_controller = 
    controller_manager->GetDaemonController();
  show_console_shortcut_ = 
    new QShortcut(QKeySequence("ctrl+shift+D"), this);  
  connect(
    show_console_shortcut_, 
    &QShortcut::activated,
    daemon_controller, 
    &IDaemonController::HandleShowConsoleSlot);

  connect(ScreenHelper::Instance(), &ScreenHelper::PrimaryScreenChanged,
          this, &MainControlWindow::HandlePrimaryScreenChangedNotifySlot);
}

void MainControlWindow::SetupGui() {
  if (RunningConfig::Instance()->EnableControlPanelBooter()) {
    control_panel_booter_ = new ControlPanelBooter(nullptr, *this);
    MoveControlPanelBooter();
    control_panel_booter_->hide();
  }
}

void MainControlWindow::MoveControlPanelBooter() {
  if (control_panel_booter_) {
    // 移动到mainControlWindows所在的屏幕的右下角
    QRect rect = ScreenHelper::Instance()->GetAvailableGeometry();
    control_panel_booter_->move(
      rect.x() + rect.width() - control_panel_booter_->width(),
      rect.y() + rect.height() - control_panel_booter_->height());
  }
}

void MainControlWindow::InitializeActions() {
#ifdef _DEBUG
  connect(ui_.actionQuit, SIGNAL(triggered()),
    this, SLOT(ConfirmAndQuitSystemSlot()));
#else
  connect(ui_.actionQuit, SIGNAL(triggered()),
    this, SLOT(ConfirmAndShutdownSlot()));
#endif
  connect(ui_.actionLogOut, SIGNAL(triggered()),
    this, SLOT(ConfirmAndLogoutSlot()));
  connect(ui_.actionRestart, SIGNAL(triggered()), 
    this,SLOT(ConfirmAndRestartSystemSlot()));
  /*connect(ui_.actionWebSite, SIGNAL(triggered()),
    this, SLOT(ShowWebPageSlot()));*/
  connect(ui_.actionDevice, SIGNAL(triggered()),
    this, SLOT(ShowDeviceListSlot()));
  if (!CVIniConfig::getInstance().IsModelTX()) {
    ui_.actionDevice->setVisible(false);
  }
}

void MainControlWindow::SetLoginWidget( QWidget *login_widget ) {
  login_widget_ = login_widget;
  if (login_widget)
    MoveToTheCenterOfMainWindow(login_widget);
}

void MainControlWindow::SetDeviceWidget( DeviceListWidget *device_list_widget ) {
  device_list_widget_ = device_list_widget;
  if (device_list_widget) 
    MoveToTheCenterOfMainWindow(device_list_widget);
}

void MainControlWindow::MoveToTheCenterOfMainWindow( QWidget *widget ) {
  QRect rect = QApplication::desktop()->availableGeometry(this);
  int half_width = rect.width() / 2;
  int half_height = rect.height() / 2;
  int offset_x = rect.x() + half_width;
  int offset_y = rect.y() + half_height;
  widget->move(
    offset_x + (half_width - widget->width()) / 2, 
    offset_y + (half_height - widget->height()) / 2);
}

void MainControlWindow::ShowDeviceListSlot() {
  if (device_list_widget_) {
    if (!device_list_widget_->isVisible()) {
      device_list_widget_->ReadConfig();
    }
    device_list_widget_->show();
    device_list_widget_->raise();
    device_list_widget_->setFocus();
  }
}

void MainControlWindow::closeEvent( QCloseEvent * event ) {
  if (quit_process_) {
    event->accept();
  } else {
#ifdef _DEBUG
    ConfirmAndQuitSystemSlot();
#else
    ConfirmAndShutdownSlot();
#endif
    if (quit_process_) {
      event->accept();
    } else {
      event->ignore();
    }
  }
}

void MainControlWindow::ConfirmAndQuitSystemSlot() {
  QString infoMsg = QString::fromLocal8Bit(
    !current_conference_uri_.isEmpty() ? 
    "您正在参加会议，确定退出系统？" : "确定退出系统？");

  QMessageBox::StandardButton res = QMessageBox::warning(
    this, tr("CoolView"), infoMsg, QMessageBox::Yes | QMessageBox::No);
  if(res == QMessageBox::Yes) {
    emit QuitSystemSignal();
  }
}

void MainControlWindow::HandleQuitSystemNotifySlot() {
  quit_process_ = true;
  stay_login_ = false;
  if (control_panel_booter_)
    control_panel_booter_->close();
  close();
}

void MainControlWindow::QuitSystem() {
  quit_process_ = true;
  stay_login_ = false;
  emit LogoutSignal();
  if (login_widget_)
    login_widget_->hide();
  if (control_panel_booter_)
    control_panel_booter_->close();
  close();
}

void MainControlWindow::ConfirmAndShutdownSlot() {
  QString infoMsg = QString::fromLocal8Bit(
    !current_conference_uri_.isEmpty() ?
    "您正在参加会议，确定退出系统并关机？" : "确定退出系统并关机？");

  QMessageBox::StandardButton res = QMessageBox::warning(
    this, tr("CoolView"), infoMsg, QMessageBox::Yes | QMessageBox::No);
  if(res == QMessageBox::Yes) {
    quit_process_ = true;
    emit ShutDownSystemSignal();
  }
}

void MainControlWindow::ConfirmAndRestartSystemSlot() {
  QString msg = !current_conference_uri_.isEmpty() ? 
    QString::fromLocal8Bit("您正在参加会议，确定重启电脑？") :
    QString::fromLocal8Bit("确定重启电脑？");

  QMessageBox::StandardButton res = QMessageBox::warning(
    this, tr("CoolView"), msg, QMessageBox::Yes | QMessageBox::No);
  if(res == QMessageBox::Yes) {
    emit RestartSystemSignal();
  }
}

void MainControlWindow::ConfirmAndLogoutSlot() {
  QString message = QString::fromLocal8Bit(
    !current_conference_uri_.isEmpty() ? 
    "您正在参加会议，确定要注销吗？" : "确定要注销吗？");

  QMessageBox::StandardButton res = QMessageBox::warning(
    this, tr("CoolView"), message, QMessageBox::Yes | QMessageBox::No);
  if(res == QMessageBox::Yes) {
    stay_login_ = false;
    emit LogoutSignal();
    if (login_widget_) {
      login_widget_->raise();
      login_widget_->show();
    }
  }
}

void MainControlWindow::HandleRecvSaviNotifySlot( bool res ) {
  savi_status_ = 
    QString::fromLocal8Bit(res ? "(通过真实源地址认证)" : "(没有通过真实源地址认证)");
  if (current_conference_uri_.isEmpty()) {
    SetWelcomeTitle();
  }
}

void MainControlWindow::SetWelcomeTitle() {
  QString title = 
    QString::fromLocal8Bit(RunningProfile::getInstance()->username().c_str());
  title += QString::fromLocal8Bit("，欢迎使用CoolView视频会议系统！");
  title += savi_status_;
  setWindowTitle(title);
}

void MainControlWindow::HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state) {
  switch (state) {
    case ISessionController::kIsOnline:
      ActivateWindow(); break;
    case ISessionController::kIsOffline:
      if (control_panel_booter_) control_panel_booter_->hide();
      hide(); break;
  }
}

void MainControlWindow::ActivateWindow() {
  showNormal();
  stay_login_ = true;
  savi_status_.clear();
  SetWelcomeTitle();
  control_panel_booter_->show();
  emit NotifyVisibleStateChangedSignal(-1, true);
}

void MainControlWindow::HandleSetVisibleRequestSlot( bool visible ) {
  // TODO: 暂时去掉遥控器控制控制窗口显示和隐藏的功能
  //visible ? this->showNormal() : showMinimized();
  //emit NotifyVisibleStateChangedSignal(-1, visible);
}

void MainControlWindow::HandleConferenceStateChangedNotifySlot(
    const QString &uri, IConferenceController::ConferenceState state) {
  if (state == IConferenceController::kIsNotInConference && 
      current_conference_uri_ == uri) {
    current_conference_uri_.clear();
    SetWelcomeTitle();
  } else if (state == IConferenceController::kIsInConference) {
    current_conference_uri_ = uri;
  }
}

void MainControlWindow::SetWindowTitleSlot( const QString &title ) {
  setWindowTitle(title);
}

void MainControlWindow::HandleVisibleStateRequestSlot(int telecontroller_id) {
  emit NotifyVisibleStateChangedSignal(telecontroller_id, !isMinimized());
}

void MainControlWindow::HandleScreenLayoutChangedNotifySlot(
    int screen_index, IDisplayController::ScreenLayout layout) {
  if (layout == IDisplayController::kLayoutOfTop1x2) {
    screen_index = ScreenHelper::Instance()->PosIndexToScreenIndex(screen_index);
    QRect rect = QApplication::desktop()->availableGeometry(screen_index);
    setGeometry(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2, rect.width() / 2, rect.height() / 2);
    if (control_panel_booter_ && need_move_control_panel_) {
      // 移动到mainControlWindows所在的屏幕的右下角
      control_panel_booter_->move(
        rect.x() + rect.width() - control_panel_booter_->width(),
        rect.y() + rect.height() - control_panel_booter_->height());
      need_move_control_panel_ = false;
    }
    if (isVisible()) {
      showNormal();
    }
  }
}

void MainControlWindow::HandleActivateCameraSettingSlot( 
  const int index )
{
  ShowDeviceListSlot();
  if (device_list_widget_) {
    device_list_widget_->HandleActivateCameraSettingSlot(index);
  }
}

void MainControlWindow::HandleActivateTerminalConfigSlot( 
    TerminalConfigType type) {
  ShowDeviceListSlot();
  if (device_list_widget_) {
    device_list_widget_->HandleActivateDeviceConfigSlot(type);
  }
}

void MainControlWindow::HandleNewConfigNotifySlot()
{
#ifdef _DEBUG
  QMessageBox::information(this, QString::fromLocal8Bit("新的配置参数"),
    QString::fromLocal8Bit("接收到新的配置参数"), QMessageBox::Yes);
#else
  QMessageBox::information(this, QString::fromLocal8Bit("新的配置参数"),
    QString::fromLocal8Bit("接收到新的配置参数，终端将在3秒内重启"));
  QTimer::singleShot(3000, this, SIGNAL(RestartSystemSignal()));
#endif
}

void MainControlWindow::HandlePrimaryScreenChangedNotifySlot(int primary_screen) {
  SetGeometryInPrimaryScreen();
  MoveControlPanelBooter();
}


