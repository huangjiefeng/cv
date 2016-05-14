#include "monitor_tab.h"

#include <cassert>

#include <QTimer>
#include <QDebug>

#include "monitor_client.h"
#include "config/RunningConfig.h"

#include "monitor_controller_interface.h"

MonitorTab::MonitorTab(QWidget *parent)
    : QWidget(parent), 
      refresh_info_timer_(nullptr) {
  ui_.setupUi(this);

  refresh_info_timer_ = new(std::nothrow) QTimer(this);
  if (refresh_info_timer_) {
    connect(refresh_info_timer_, &QTimer::timeout,
      this, &MonitorTab::RequestHardwareInfoSignal);
  }

  RunningConfig *config = RunningConfig::Instance();
  
  ui_.mainboardActionComboBox->setCurrentIndex(config->GetMainboardTempAction());
  ui_.mainboardTempWarningEdit->setText(
    QString::number(config->GetMainboardTempThreshold()));

  ui_.cpuActionComboBox->setCurrentIndex(config->GetCpuTempAction());
  ui_.cpuTempWarningEdit->setText(QString::number(config->GetCpuTempThreshold()));

  connect(ui_.monitorConfigSaveButton,SIGNAL(clicked()),this,SLOT(saveMonitorConfigSlot()));
}

void MonitorTab::Initiliaze( IMonitorController *controller ) {
  assert(controller);

  connect(this, &MonitorTab::RequestHardwareInfoSignal,
    controller, &IMonitorController::HandleRequestHardwareInfoSlot);

  connect(controller, &IMonitorController::NotifyHardwareInfoSignal,
    this, &MonitorTab::HandleHardwareInfoNotifySlot);

  connect(controller, &IMonitorController::NotifyNetworkInfoSignal,
    this, &MonitorTab::HandleNetworkInfoNotifySlot);
}

MonitorTab::~MonitorTab() {
}

void MonitorTab::saveMonitorConfigSlot() {
  RunningConfig *config = RunningConfig::Instance();
  config->SetMainboardTempThreshold(ui_.mainboardTempWarningEdit->text().toFloat());
  config->SetMainboardTempAction(ui_.mainboardActionComboBox->currentIndex());
  config->SetCpuTempThreshold(ui_.cpuTempWarningEdit->text().toFloat());
  config->SetCpuTempAction(ui_.cpuActionComboBox->currentIndex());
  config->saveConfig();
}

void MonitorTab::StartRequestHardwareInfo() {
  if (refresh_info_timer_ && !refresh_info_timer_->isActive()) {
    emit RequestHardwareInfoSignal();
    // TODO: 配置文件设置间隔
    refresh_info_timer_->start(1000);
  }
}

void MonitorTab::StopRequestHarewareInfo() {
  if (refresh_info_timer_ && refresh_info_timer_->isActive()) {
    refresh_info_timer_->stop();
  }
}

void MonitorTab::HandleHardwareInfoNotifySlot( const HardwareInfo &info ) {
  // 显示主板温度
  if (info.mainboard_temperature != info.mainboard_temperature) {
    ui_.mainboardTempLabel->setText("-");
  } else {
    ui_.mainboardTempLabel->setText(
      QString::fromLocal8Bit("%1 °C").arg(info.mainboard_temperature));
  }
  // 显示CPU负载
  if (info.cpu_load != info.cpu_load) {
    ui_.cpuLoadLabel->setText("-");
  } else {
    ui_.cpuLoadLabel->setText(QString("%1 %").arg(info.cpu_load));
  }
  // 显示CPU温度
  if (info.cpu_temperature != info.cpu_temperature) {
    ui_.cpuTempLabel->setText("-");
  } else {
    ui_.cpuTempLabel->setText(
      QString::fromLocal8Bit("%1 °C").arg(info.cpu_temperature));
  }

  // 显示内存可用空间
  if (info.ram_available_mem != info.ram_available_mem) {
    ui_.ramDataLabel->setText("-");
  } else {
    ui_.ramDataLabel->setText(
      QString::fromLocal8Bit("%1 GB").arg(info.ram_available_mem));
  }

  // 添加硬盘的UI
  while (hdd_lebals.size() < info.hdd.size()) {
    int hdd_index = hdd_lebals.size() + 1;
    HDDLebals hdd;
    hdd.temperature = new QLabel(this);
    ui_.formLayout->addRow(
      QString::fromLocal8Bit("硬盘%1温度:").arg(hdd_index),
      hdd.temperature);
    hdd.temperature->setAlignment(Qt::AlignRight);
    hdd.used_space = new QLabel(this);
    ui_.formLayout->addRow(
      QString::fromLocal8Bit("硬盘%1已用空间:").arg(hdd_index),
      hdd.used_space);
    hdd.used_space->setAlignment(Qt::AlignRight);
    hdd_lebals.push_back(hdd);
  }

  // 显示硬盘温度和已用空间
  for (int i = 0; i < info.hdd.size(); ++i) {
    if (info.hdd[i].temperature != info.hdd[i].temperature) {
      hdd_lebals[i].temperature->setText("-");
    } else {
      hdd_lebals[i].temperature->setText(
        QString::fromLocal8Bit("%1 °C").arg(info.hdd[i].temperature));
    }
    if (info.hdd[i].used_space != info.hdd[i].used_space) {
      hdd_lebals[i].used_space->setText("-");
    } else {
      hdd_lebals[i].used_space->setText(
        QString::fromLocal8Bit("%1 %").arg(info.hdd[i].used_space));
    }
  }
}

void MonitorTab::HandleNetworkInfoNotifySlot(
    double in_bandwidth, double out_bandwidth) {
  ui_.inBandwidthLabel->setText(
    QString("%1 KB/s").arg(in_bandwidth));
  ui_.outBandwidthLabel->setText(
    QString("%1 KB/s").arg(out_bandwidth));
}



