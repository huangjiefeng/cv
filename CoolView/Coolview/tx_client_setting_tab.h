#ifndef TX_CLIENT_SETTING_TAB_H
#define TX_CLIENT_SETTING_TAB_H

#include <QWidget>
#include "ui_tx_client_setting_tab.h"

#include "util/report/RecordStat.h"

class IControllerManager;

class TxClientSettingTab : public QWidget
{
  Q_OBJECT

public:
  TxClientSettingTab(QWidget *parent, 
    IControllerManager *controller_manager);
  ~TxClientSettingTab();

  void Initialize();

public Q_SLOTS:
  void ReadConfig();
  void SaveConfig();

  void HandleReceiveUDPRecReportSlot(const RecStatItem &item);

private:
  unsigned long long GetLocalRecordDirFreeSpace();

private:
  Ui::TxClientSettingTab ui;

  IControllerManager *controller_manager_;

  QHash<int, RecStatItem> local_record_status_;
  QString local_record_path_;
};

#endif // TX_CLIENT_SETTING_TAB_H
