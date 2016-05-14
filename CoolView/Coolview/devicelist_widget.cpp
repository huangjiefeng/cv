
#include "devicelist_widget.h"
//#include "device_setting_tab.h"

#include <util/desktop/screen_helper.h>

#include "video_setting_tab.h"
#include "device_test_tab.h"
#include "audio_setting_tab.h"
#include "monitor_tab.h"
#include "tx_setting_tab.h"
#include "tx_client_setting_tab.h"
#include "util/ini/CVIniConfig.h"
#include "controller_manager_interface.h"
#include "ptz_tab.h"

DeviceListWidget::DeviceListWidget(
    QWidget *parent, IControllerManager *controller_manager)
    : QWidget(parent),
      //device_setting_tab_(nullptr),
      device_test_tab_(nullptr),
	  //ptz_tab_(nullptr),	//2014.4.2.gmlan-增加云台控制功能
      monitor_tab_(nullptr),
      tx_setting_tab_(nullptr),
      tx_client_setting_tab_(nullptr),
      audio_setting_tab_(nullptr),
      video_setting_tab_(nullptr),
      ptz_tab_(nullptr) {
  setupUi(this);

  if (CVIniConfig::getInstance().IsModelHD()) {
    //device_setting_tab_ = new DeviceSettingTab(nullptr, controller_manager);
    //settingTabWidget->addTab(device_setting_tab_, QString(QString::fromLocal8Bit("设备设置")));

    video_setting_tab_ = new VideoSettingTab(nullptr);
    settingTabWidget->addTab(video_setting_tab_, video_setting_tab_->windowTitle());

	//2014.4.2.gmlan-增加云台控制功能
	//ptz_tab_ = new PtzTab(nullptr);
	//settingTabWidget->addTab(ptz_tab_, ptz_tab_->windowTitle());

    audio_setting_tab_ = new AudioSettingTab(nullptr, controller_manager);
    settingTabWidget->addTab(audio_setting_tab_, audio_setting_tab_->windowTitle());

    device_test_tab_ = new DeviceTestTab();
    settingTabWidget->addTab(device_test_tab_, QString(QString::fromLocal8Bit("设备调试")));
  }

  if (CVIniConfig::getInstance().IsModelTX()) {
    tx_setting_tab_ = new TxSettingTab();
    tx_setting_tab_->Initialize();
    settingTabWidget->addTab(tx_setting_tab_, QString(QString::fromLocal8Bit("上传设置")));
  } else {
    tx_client_setting_tab_ = new TxClientSettingTab(nullptr, controller_manager);
    tx_client_setting_tab_->Initialize();
    settingTabWidget->addTab(tx_client_setting_tab_, QString(QString::fromLocal8Bit("录制设置")));
  }
  
  if (controller_manager) {
    monitor_tab_ = new MonitorTab();
    monitor_tab_->Initiliaze(controller_manager->GetMonitorController());
    settingTabWidget->addTab(monitor_tab_, QString(QString::fromLocal8Bit("系统监控")));
  }

  connect(settingTabWidget, &QTabWidget::currentChanged,
          this, &DeviceListWidget::HandleTabChangedSlot);

  connect(ScreenHelper::Instance(), &ScreenHelper::PrimaryScreenChanged,
          this, &DeviceListWidget::HandlePrimaryScreenChangedSlot);

  SetGeometryInPrimaryScreen();

}

DeviceListWidget::~DeviceListWidget()
{
  //Tab的Delete交由QTabWedget去完成
}

void DeviceListWidget::SetGeometryInPrimaryScreen()
{
  QRect rect = ScreenHelper::Instance()->GetAvailableGeometry(-1);
  int half_width = rect.width() / 2;
  int half_height = rect.height() / 2;
  int offset_x = rect.x() + half_width;
  int offset_y = rect.y() + half_height;
  move(offset_x + (half_width - width()) / 2, 
    offset_y + (half_height - height()) / 2);
}

void DeviceListWidget::ReadConfig()
{
  if (audio_setting_tab_) {
    audio_setting_tab_->ReadConfig();
  }
  if (video_setting_tab_) {
    video_setting_tab_->ReadConfig();
  }
  //if (device_setting_tab_) {
  //  device_setting_tab_->ReadConfig();
  //}
  if (device_test_tab_) {
    device_test_tab_->ReadConfig();
  }
  if (tx_client_setting_tab_) {
    tx_client_setting_tab_->ReadConfig();
  }
}

void DeviceListWidget::HandleTabChangedSlot( int index ) {
  if (monitor_tab_) {
    if (index == settingTabWidget->indexOf(monitor_tab_)) {
      monitor_tab_->StartRequestHardwareInfo();
    } else {
      monitor_tab_->StopRequestHarewareInfo();
    }
  }
}

void DeviceListWidget::HandleActivateCameraSettingSlot( 
  const int index )
{
  //if (!device_setting_tab_ || 
  //  -1 == settingTabWidget->indexOf(device_setting_tab_)) {
  //  return;
  //}
  //settingTabWidget->setCurrentWidget(device_setting_tab_);
  //device_setting_tab_->ChangeCameraIndex(index);

  if (!video_setting_tab_ || 
      -1 == settingTabWidget->indexOf(video_setting_tab_)) {
    return;
  }
  settingTabWidget->setCurrentWidget(video_setting_tab_);
  video_setting_tab_->ChangeCameraIndex(index);
}

void DeviceListWidget::HandleActivateDeviceConfigSlot(TerminalConfigType type) {
  switch (type) {
    case kConfigTypeOfVideo:
      settingTabWidget->setCurrentWidget(video_setting_tab_);
      break;
    case kConfigTypeOfMic:
      settingTabWidget->setCurrentWidget(audio_setting_tab_);
      break;
    case kConfigTypeOfSpeaker:
      settingTabWidget->setCurrentWidget(audio_setting_tab_);
      break;
    case kConfigTypeOfDeviceTest:
      settingTabWidget->setCurrentWidget(device_test_tab_);
      break;
    case kConfigTypeOfMonitor:
      settingTabWidget->setCurrentWidget(monitor_tab_);
      break;
    default:
      break;
  }
}

void DeviceListWidget::HandlePrimaryScreenChangedSlot( int primary_screen )
{
  SetGeometryInPrimaryScreen();
}


