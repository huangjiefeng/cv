#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <QObject>

class IControllerManager;
class MainControlWindow;
class LoginForm;
class TxControlWindow;
class ControlPanelBooter;
class DeviceListWidget;

class UIManager : public QObject {
  Q_OBJECT

public:
  UIManager(QObject *parent);
  ~UIManager();

  void Initialize(IControllerManager *controller_manager);
  void Show();

private:
  IControllerManager *controller_manager_;
  MainControlWindow *main_control_window_;
  LoginForm *login_form_;
  TxControlWindow *tx_control_window_;
  DeviceListWidget *device_list_widget_;
};

#endif // UI_MANAGER_H
