#include "ip_setting_dialog.h"

#include <windows.h>
#include <QMessageBox>
#include <DeviceManager/DeviceManager.h>

#include "CoolviewCommon.h"

IPSettingDialog::IPSettingDialog(QWidget *parent)
    : QDialog(parent) {
  ui_.setupUi(this);

  connect(ui_.adaptorListCombo, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(AdaptorChangedSlot(const QString&)));
  connect(ui_.cancelButton, SIGNAL(clicked()), 
          this, SLOT(CancelButtonClickedSlot()));
  connect(ui_.saveButton, SIGNAL(clicked()), this, SLOT(SaveButtonClickedSlot()));
  connect(ui_.enableDHCPCheckbox, SIGNAL(stateChanged(int)),
          this, SLOT(DHCPChangedSlot(int)));

  ReadConfig();
}

IPSettingDialog::~IPSettingDialog() {

}

void IPSettingDialog::ReadConfig()
{
  // 读取适配器
  std::vector<std::string> adaptor_list;
  DeviceManager::GetInstatnce()->GetNetworkAdaptorList(&adaptor_list);
  QStringList adaptors;
  TransformStringVectorToQStringList(&adaptor_list , &adaptors);

  ui_.adaptorListCombo->clear();
  ui_.adaptorListCombo->addItems(adaptors);
}

void IPSettingDialog::SaveButtonClickedSlot() {
  if(!ui_.enableDHCPCheckbox->isChecked()) {
    QString ip = ui_.ipAddressLineEdit->text().trimmed();
    QString mask = ui_.maskLineEdit->text().trimmed();
    QString gateway = ui_.gatewayLineEdit->text().trimmed();
    QStringList dnsServers = ui_.dnsLineEdit->text().split(" ");

    if(ip.isEmpty() || mask.isEmpty() || 
       gateway.isEmpty() || dnsServers.isEmpty()) {
      QMessageBox::information( this ,
        QString::fromLocal8Bit("错误"),
        QString::fromLocal8Bit("请正确填写所有参数"),
        QMessageBox::Ok);
      return;
    }
    DeviceManager::GetInstatnce()->SetNetworkAdaptorInfo(
      ui_.adaptorListCombo->currentText().toStdString(),
      ip.toStdString(),
      mask.toStdString(),
      gateway.toStdString(),
      dnsServers[0].toStdString(),
      false);

  } else {
    std::string ip = "";
    std::string mask = "";
    std::string gateway = "";
    std::string dns = "";
    DeviceManager::GetInstatnce()->SetNetworkAdaptorInfo(
      ui_.adaptorListCombo->currentText().toStdString(),
      ip, mask, gateway,dns, true);
  }

  QMessageBox::StandardButton res = 
    QMessageBox::information(
       this,
       QString::fromLocal8Bit("提示"), 
       QString::fromLocal8Bit("IP配置已经修改，需要重启才能生效，是否现在重启？"),
       QMessageBox::Ok | QMessageBox::Cancel);
  if(res == QMessageBox::Ok) {
    emit RestartSystemSignal();
  }

  hide();
  emit ReloadConfigSignal();
}

void IPSettingDialog::AdaptorChangedSlot( const QString&name ) {
  std::string ip;
  std::string mask;
  std::string dnsServer;
  std::string gateway;
  bool enableDHCP;
  DeviceManager::GetInstatnce()->GetNetworkAdaptorInfo(
    name.toStdString(), &ip, &mask, &gateway, &dnsServer, &enableDHCP);

  ui_.ipAddressLineEdit->setText(ip.c_str());
  ui_.maskLineEdit->setText(mask.c_str());
  ui_.gatewayLineEdit->setText(gateway.c_str());
  ui_.dnsLineEdit->setText(dnsServer.c_str());
  if(enableDHCP) {
    ui_.ipAddressLineEdit->setDisabled(true );
    ui_.maskLineEdit->setDisabled(true );
    ui_.gatewayLineEdit->setDisabled(true);
    ui_.dnsLineEdit->setDisabled(true);
    ui_.enableDHCPCheckbox->setCheckState( Qt::Checked );
  }
}

void IPSettingDialog::DHCPChangedSlot( int state ) {
  bool disable = state == Qt::Checked;
  ui_.ipAddressLineEdit->setDisabled(disable);
  ui_.maskLineEdit->setDisabled(disable);
  ui_.gatewayLineEdit->setDisabled(disable);
  ui_.dnsLineEdit->setDisabled(disable);
}


