#ifndef TX_SETTING_TAB_H
#define TX_SETTING_TAB_H

#include <QWidget>
#include "ui_tx_setting_tab.h"

class TxSettingTab : public QWidget
{
  Q_OBJECT

public:
  TxSettingTab(QWidget *parent = 0);
  ~TxSettingTab();

  void Initialize();

private:
  Ui::TxSettingTab ui;

};

#endif // TX_SETTING_TAB_H
