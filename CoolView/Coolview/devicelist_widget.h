#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include "ui_device_setting_widget.h"
#include <QtGui/QtGui>
//#include "ptz_tab.h"               //2014.4.2.gmlan-增加云台控制功能
#include "terminal_config_type.h"

//class DeviceSettingTab;
class DeviceTestTab;
class MonitorTab;
class TxSettingTab;
class TxClientSettingTab;
class IControllerManager;
class AudioSettingTab;
class VideoSettingTab;
class PtzTab;

class DeviceListWidget : public QWidget, public Ui::DeviceSettingForm
{
  Q_OBJECT

public:
  DeviceListWidget(
    QWidget *parent = nullptr,
    IControllerManager *controller_manager = nullptr);

  ~DeviceListWidget();

public slots:
  void HandleTabChangedSlot(int index);
  void ReadConfig();

  // 激活某路摄像头设置
  void HandleActivateCameraSettingSlot(const int index);
  void HandleActivateDeviceConfigSlot(TerminalConfigType type);

  void HandlePrimaryScreenChangedSlot(int primary_screen);

private:
  void SetGeometryInPrimaryScreen();
    
private:
  //DeviceSettingTab * device_setting_tab_;
  VideoSettingTab *video_setting_tab_;
  DeviceTestTab * device_test_tab_;
  AudioSettingTab *audio_setting_tab_;
  MonitorTab * monitor_tab_;
  TxSettingTab * tx_setting_tab_;	
  TxClientSettingTab * tx_client_setting_tab_;
  PtzTab *ptz_tab_;               //2014.4.2.gmlan-增加云台控制功能
};

#endif // DEVICELISTWIDGET_H
