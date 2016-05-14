#include "tx_client_setting_tab.h"

#include "windows.h"

#include "controller_manager.h"
#include "tx_client_controller_interface.h"
#include "config/RunningConfig.h"
#include "util/ini/TxConfig.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"

TxClientSettingTab::TxClientSettingTab(QWidget *parent, 
                                       IControllerManager *controller_manager)
  : QWidget(parent),
    controller_manager_(controller_manager)
{
  ui.setupUi(this);
}

TxClientSettingTab::~TxClientSettingTab()
{

}

void TxClientSettingTab::Initialize()
{
  connect(ui.saveButton, &QPushButton::clicked,
    this, &TxClientSettingTab::SaveConfig);

  ui.localRecStatusLabel->setText(QString::fromLocal8Bit("暂无数据"));

  if (controller_manager_) {
    ITXClientController *tx_client_controller = controller_manager_->GetTXClientController();
    if (tx_client_controller) {
      connect(tx_client_controller, &ITXClientController::ReceiveUDPRecReportSignal,
        this, &TxClientSettingTab::HandleReceiveUDPRecReportSlot);
    }
  }

  CTxConfig *tx_config = &CTxConfig::getInstance();
  local_record_path_ = QString::fromLocal8Bit(tx_config->GetRecPath());
  if (local_record_path_.indexOf(":") == -1) {
    local_record_path_ = QApplication::applicationDirPath() + "/" + local_record_path_;
  }
  local_record_path_.replace("/", "\\");
}

void TxClientSettingTab::ReadConfig()
{
  RunningConfig* running_config = RunningConfig::Instance();
  CTxConfig *tx_config = &CTxConfig::getInstance();

  ui.ckbLocalRecord->setChecked( running_config->isEnableLocalRecord() );
  ui.spinBoxRecAutoSeg->setValue(tx_config->GetRecLocalDuration() / 60);

  QString tx_addr = QString::fromLocal8Bit(tx_config->GetSoapCServer().c_str());
  ui.txAddressLineEdit->setText(tx_addr);
}

void TxClientSettingTab::SaveConfig()
{
  RunningConfig* running_config = RunningConfig::Instance();
  CTxConfig *tx_config = &CTxConfig::getInstance();

  //本地录制
  running_config->EnableLocalRecord(ui.ckbLocalRecord->isChecked());
  tx_config->SetRecLocalDuration(ui.spinBoxRecAutoSeg->value() * 60);

  tx_config->SetSoapCServer(qPrintable(ui.txAddressLineEdit->text().toLocal8Bit()));

  running_config->saveConfig();
}

void TxClientSettingTab::HandleReceiveUDPRecReportSlot( const RecStatItem &item )
{
  if (!isLocalRecordMediaID(item.user_name)) {
    return;
  }
  int index = getVirtualIndexFromLocalRecordMediaID(item.user_name);
  local_record_status_[index] = item;

  //更新界面
  QString msg;
  RunningConfig* running_config = RunningConfig::Instance();
  for (int i = 0; i < running_config->VideoCaptureDeviceCount(); ++i) {
    if (!local_record_status_.contains(i) || 
        local_record_status_[i].statType != REC_STAT_FILE_WRITING) {
      msg += QString::fromLocal8Bit("视频输入%1：未录制\n").arg(i+1);
    }
    else {
      RecStatItem &item2 = local_record_status_[i];
      int hour = item2.rec.duration / 1000 / 3600;
      int min = item2.rec.duration / 1000 / 60 % 60;
      int sec = item2.rec.duration / 1000 % 60;
      msg += QString::fromLocal8Bit("视频输入%1：时长%2:%3:%4，文件大小%5MB\n")
        .arg(i+1)
        .arg(hour)
        .arg(min, 2, 10, QChar('0'))
        .arg(sec, 2, 10, QChar('0'))
        .arg(item2.rec.size/1000000.0, 0, 'f', 1);
    }
  }

  unsigned long long free_space = GetLocalRecordDirFreeSpace();
  msg += QString::fromLocal8Bit("磁盘可用空间：%1GB").arg((double)free_space/(1LL << 30), 0, 'f', 2);

  ui.localRecStatusLabel->setText(msg);
}

unsigned long long TxClientSettingTab::GetLocalRecordDirFreeSpace()
{
  ULARGE_INTEGER nFreeSpaceAvailable = {0};
  GetDiskFreeSpaceExA(qPrintable(local_record_path_), NULL, NULL, &nFreeSpaceAvailable);
  return nFreeSpaceAvailable.QuadPart;
}
