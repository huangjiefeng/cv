#include "ui_manager.h"

#include <QApplication>
#include <QDesktopWidget>

#include "controller_manager.h"

#include "util/ini/CVIniConfig.h"

#include "login_form.h"
#include "main_control_window.h"
#include "conference_widget.h"
#include "terminal_list_widget.h"
#include "display_widget.h"
#include "local_terminal_widget.h"
#include "local_terminal_widget.h"
#include "tx_control_window.h"
#include "control_panel_booter.h"
#include "devicelist_widget.h"

UIManager::UIManager( QObject *parent )
    : QObject(parent) {
  controller_manager_ = nullptr;
  main_control_window_ = nullptr;
  login_form_ = nullptr;
  tx_control_window_ = nullptr;
  device_list_widget_ = nullptr;
}

UIManager::~UIManager() {
  if (main_control_window_) {
    main_control_window_->deleteLater();
  }
  if (tx_control_window_) {
    tx_control_window_->deleteLater();
  }
  if (device_list_widget_) {
    device_list_widget_->deleteLater();
  }
}

void UIManager::Initialize( IControllerManager *controller_manager ) {
  assert(controller_manager != nullptr);

  controller_manager_ = controller_manager;

  device_list_widget_ = new DeviceListWidget(nullptr, controller_manager);
  device_list_widget_->setWindowFlags( Qt::WindowStaysOnTopHint );
  device_list_widget_->hide();

  login_form_ = new LoginForm();
  login_form_->Initialize(controller_manager);
  login_form_->SetDeviceListWidget(device_list_widget_);
  
  main_control_window_ = new MainControlWindow();
  main_control_window_->Initialize(controller_manager);
  main_control_window_->SetLoginWidget(login_form_);
  main_control_window_->SetDeviceWidget(device_list_widget_);
  
  DisplayWidget *display_widget = new DisplayWidget();
  display_widget->Initialize(controller_manager);
  main_control_window_->AddWidgetToConferenceArea(display_widget);

  ConferenceWidget *conference_widget = new ConferenceWidget();
  conference_widget->Initialize(controller_manager);
  main_control_window_->AddWidgetToConferenceArea(conference_widget);

  TerminalListWidget *terminal_list_widget = new TerminalListWidget();
  terminal_list_widget->Initialize(controller_manager);
  main_control_window_->AddWidgetToConferenceArea(terminal_list_widget);
  terminal_list_widget->setVisible(false);

  connect(conference_widget, SIGNAL(SetWindowTitleSignal(const QString&)),
    terminal_list_widget, SLOT(SetConferenceTitleSlot(const QString&)));
  
  if (CVIniConfig::getInstance().IsModelTX()) {
    tx_control_window_ = new TxControlWindow();
    tx_control_window_->Initialize(controller_manager);
    connect(
      main_control_window_, 
      &MainControlWindow::NotifyVisibleStateChangedSignal,
      tx_control_window_, 
      &TxControlWindow::HandleMainControlWindowVisibleStateChangedNotifySlot);
#ifdef _DEBUG
    connect(main_control_window_, &MainControlWindow::QuitSystemSignal,
      tx_control_window_, &TxControlWindow::close);
#else
    connect(main_control_window_, &MainControlWindow::ShutDownSystemSignal,
      tx_control_window_, &TxControlWindow::close);
#endif
  }
  
  LocalTerminalWidget *local_terminal_widget = new LocalTerminalWidget();
  local_terminal_widget->Initialize(controller_manager);
  main_control_window_->AddWidgetToConferenceArea(local_terminal_widget);

  connect(
    local_terminal_widget, 
    &LocalTerminalWidget::ConfigCameraSignal,
    main_control_window_, 
    &MainControlWindow::HandleActivateCameraSettingSlot);

  connect(
    local_terminal_widget,
    &LocalTerminalWidget::DisplayTerminalConfigSignal,
    main_control_window_,
    &MainControlWindow::HandleActivateTerminalConfigSlot);

  connect(
    local_terminal_widget,
    &LocalTerminalWidget::RequestLogoutSignal,
    main_control_window_,
    &MainControlWindow::ConfirmAndLogoutSlot);

  connect(
    local_terminal_widget,
    &LocalTerminalWidget::RequestShutdownSignal,
    main_control_window_,
    &MainControlWindow::ConfirmAndShutdownSlot);

  connect(
    local_terminal_widget,
    &LocalTerminalWidget::RequestRestartSignal,
    main_control_window_,
    &MainControlWindow::ConfirmAndRestartSystemSlot);

  connect(
    local_terminal_widget,
    &LocalTerminalWidget::RequestQuitSystemSignal,
    main_control_window_,
    &MainControlWindow::ConfirmAndQuitSystemSlot);
}

void UIManager::Show() {
  if (login_form_) {
    login_form_->show();
    login_form_->TryAutoLogin();
  }
}
