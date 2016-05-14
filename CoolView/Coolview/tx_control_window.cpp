#include "tx_control_window.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>

#include "controller_manager_interface.h"
#include "record_status_widget.h"


TxControlWindow::TxControlWindow(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);
}

TxControlWindow::~TxControlWindow()
{

}

void TxControlWindow::Initialize( IControllerManager *controller_manager )
{
  setWindowFlags(Qt::FramelessWindowHint);

  QRect rect = QApplication::desktop()->availableGeometry(0);
  setGeometry(rect.x(), rect.y(), rect.width() / 2, rect.height() / 2);

  QLayout *layout = new QVBoxLayout();
  setLayout(layout);

  RecordStatusWidget *record_status_widget = new RecordStatusWidget();
  record_status_widget->Initialize(controller_manager);
  layout->addWidget(record_status_widget);
}

void TxControlWindow::HandleMainControlWindowVisibleStateChangedNotifySlot( 
  int telecontroller_id, bool visible )
{
  if (visible) {
    show();
  } else {
    hide();
  }
}
