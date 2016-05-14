#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H

#include <QWidget>

#include "ui_login_form.h"
#include "CoolviewCommon.h"

class MainControlWindow;
class IPSettingDialog;
class DeviceListWidget;

class IControllerManager;
class ISystemController;

#include "session_controller_interface.h"

class LoginForm : public QWidget {
  Q_OBJECT
public:
  LoginForm(QWidget *parent = nullptr);
  ~LoginForm();

  void Initialize(IControllerManager *controller_manager);
  void TryAutoLogin();
  void SetDeviceListWidget(DeviceListWidget *device_list_widget) {
    device_list_widget_ = device_list_widget;
  }

Q_SIGNALS:
  void LoginRequestSignal();
  void ShutDownSystemSignal();
  void RestartSystemSignal();
  void QuitSytemSignal();

private Q_SLOTS:
  // 处理从session controller获得的提示信号
  void HandleLoginErrorNotifySlot(ISessionController::LoginError error);
  void HandleSessionStateChangedNotifySlot(ISessionController::SessionState state);

  void TryToLoginSlot();

  // 显示和隐藏设置界面
  void ShowOrHideExtraSettingSlot();

  // 用户对自动登录勾选框进行了操作
  void AutoLoginCheckedSlot(int state);

  // 弹出网页配置窗口
  void ShowIpConfigDialogSlot();

  // 弹出设备设置界面
  void ShowDeviceSettingWidgetSlot();

  //刷新IP列表
  void LoadIPListSlot();

  void LoadConfigSlot();

  void SaveConfigSlot();

  void UpdateScreenSlot();

  void HandlePrimaryScreenChangedNotifySlot();

private:
  void SetupGui();
  void HandleLoginNofity();
  void HandleLogoutNofity();
  void SetGeometryInPrimaryScreen();

protected:
  // 重载关闭事件
  void closeEvent(QCloseEvent * event) override;

  //重写鼠标事件gmlan20150714
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

  //设置主窗口背景图、边框、圆角//gmlan20150714
  void paintEvent(QPaintEvent *event);

private:
  // 设定了固定IP并自动登录时，可能会出现启动时IP地址获取失败的情况，此时通过
  // 睡眠重试来解决固定IP临时不可用的问题		by Ruan 2013/1/14
  bool CheckIPState();

private:
  Ui::LoginForm ui_;

  IControllerManager *controller_manager_;

  // IpConfig
  IPSettingDialog *ip_config_dialog_;

  // 设备配置
  DeviceListWidget *device_list_widget_;

  QTimer update_screen_timer_;

  bool has_initialized_;

  //重写鼠标事件gmlan20150714
  QPoint dragPosition;
};

#endif // LOGIN_FORM_H
