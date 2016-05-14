#ifndef TX_CONTROL_WINDOW_H
#define TX_CONTROL_WINDOW_H

#include <QWidget>

#include "ui_tx_control_window.h"

#include "conference_controller_interface.h"


class IControllerManager;

class TxControlWindow : public QWidget
{
  Q_OBJECT

public:
  TxControlWindow(QWidget *parent = 0);
  ~TxControlWindow();

  void Initialize(IControllerManager *controller_manager);

public Q_SLOTS:
  void HandleMainControlWindowVisibleStateChangedNotifySlot(
    int telecontroller_id, bool visible);

private:
  Ui::TxControlWindow ui;
};

#endif // TX_CONTROL_WINDOW_H
