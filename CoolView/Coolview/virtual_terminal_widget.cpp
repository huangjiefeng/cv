#include "virtual_terminal_widget.h"

#include <QMouseEvent>
#include <QPoint>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QPixmap>

#include "util/ini/CVIniConfig.h"

#include "controller_manager_interface.h"
#include "display_controller_interface.h"
#include "tx_controller_interface.h"

#include "mime_text_parser_and_builder.h"

VirtualTerminalWidget::VirtualTerminalWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);
  resize(32, 32);
}

VirtualTerminalWidget::~VirtualTerminalWidget() {
}

void VirtualTerminalWidget::Initialize(
    ConstTerminalPointer terminal,
    IControllerManager *controller_manager) {
  
  IDisplayController *dispay_controller = 
    controller_manager->GetDisplayController();
  assert(dispay_controller);
  connect(
    this, SIGNAL(StopReceivedVideoSignal(const QString&)),
    dispay_controller, SLOT(HandleStopVideoRequestSlot(const QString&)));

  ITXController *tx_controller = controller_manager->GetTXController();
  if (tx_controller) {
    connect(
      this, &VirtualTerminalWidget::CutFileRequestSignal,
      tx_controller, &ITXController::HandleSegmentMediaRequestSlot);
  }

  CreateMenuAndActions();

  Update(terminal);
}

void VirtualTerminalWidget::CreateMenuAndActions() {
  menu_ = new QMenu(this);
  menu_->addAction(ui_.stopReceiveVideoAction);
  connect(ui_.stopReceiveVideoAction, SIGNAL(triggered()),
    this, SLOT(StopReceivedVideoSlot()));
  ui_.stopReceiveVideoAction->setEnabled(false);

  // 如果是TX终端，则有分段录制的action
  if (CVIniConfig::getInstance().IsModelTX()) {
    menu_->addAction(ui_.cutFileAction);
    connect(ui_.cutFileAction, SIGNAL(triggered()), this, SLOT(CutFileSlot()));
  }
}

void VirtualTerminalWidget::Update(ConstTerminalPointer terminal) {
  uri_ = terminal->uri;
  virtual_uri_ = terminal->virtual_uri;
  virtual_index_ = terminal->virtual_index;
  name_ = terminal->name;

  bool is_available = terminal->is_available;
  ui_.stopReceiveVideoAction->setEnabled(is_available);
  ui_.videoShowedLabel->setEnabled(is_available);

  image_url_ = QString(":/image/video%1.png").arg(terminal->virtual_index + 1);
  ui_.videoShowedLabel->setPixmap(QPixmap(image_url_));
  ui_.videoShowedLabel->setScaledContents(true);
  SetShowStatus(false);
  setToolTip(name_);
}

void VirtualTerminalWidget::SetShowStatus( bool show ) {
  ui_.stopReceiveVideoAction->setEnabled(show);
  ui_.videoShowedLabel->setStyleSheet(
    show ? "QLabel#videoShowedLabel { background-color: white }" : "");
}

void VirtualTerminalWidget::CutFileSlot() {
  emit CutFileRequestSignal(virtual_uri_);
}

void VirtualTerminalWidget::StopReceivedVideoSlot() {
  emit StopReceivedVideoSignal(virtual_uri_);
}

bool VirtualTerminalWidget::IsShow() const {
  return !ui_.videoShowedLabel->styleSheet().isEmpty();
}

void VirtualTerminalWidget::mousePressEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton) {
    start_pos_ = event->globalPos();
  }
  QWidget::mousePressEvent(event);
}

void VirtualTerminalWidget::mouseMoveEvent( QMouseEvent *event ) {
  if (event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - start_pos_).manhattanLength();
    if (distance >= QApplication::startDragDistance()) {
      PerformDrag();
    }
  }
  // note: 接受event信号，使得terminal widget不会执行drag操作
  event->accept();
}

void VirtualTerminalWidget::PerformDrag() {
  MimeTextBuilder text_builder(uri_);
  text_builder.AddVirtualURI(virtual_uri_);

  QMimeData *mime_data = new QMimeData();
  mime_data->setText(text_builder.GetText());

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mime_data);

  QPixmap p(image_url_);
  drag->setPixmap(p);
  drag->setHotSpot(QPoint(p.size().width() / 2, p.size().height() / 2));

  // 在windows上event loop会被阻塞
  drag->exec();

  // 在windwos需要删除drag
  drag->deleteLater();
}

void VirtualTerminalWidget::mouseReleaseEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton) {
    menu_->exec(event->globalPos());
  }
  QWidget::mouseReleaseEvent(event);
}



