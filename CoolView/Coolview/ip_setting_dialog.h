#ifndef IP_SETTING_DIALOG_H
#define IP_SETTING_DIALOG_H

#include <QDialog>
#include "ui_ip_setting_dialog.h"

class IPSettingDialog : public QDialog {
  Q_OBJECT

public:
  IPSettingDialog(QWidget *parent);
  ~IPSettingDialog();

Q_SIGNALS:
  void RestartSystemSignal();
  void ReloadConfigSignal();

private Q_SLOTS:
  void SaveButtonClickedSlot();
  void CancelButtonClickedSlot() { hide(); }
  void AdaptorChangedSlot(const QString&name);
  void DHCPChangedSlot(int state);

private:
  void ReadConfig();

private:
  Ui::IPSettingDialog ui_;
  
};

#endif // IP_SETTING_DIALOG_H
