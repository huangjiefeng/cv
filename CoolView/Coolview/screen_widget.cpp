#include "screen_widget.h"

#include <cassert>

#include <QMenu>
#include <QMouseEvent>

#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>

#include "util/ini/CVIniConfig.h"
#include "config/RunningConfig.h"

#include "seat_widget.h"

#include "display_controller_interface.h"
#include "controller_manager_interface.h"

ScreenWidget::ScreenWidget(QWidget *parent, int screen_index)
    : QWidget(parent), screen_index_(screen_index) {
  ui_.setupUi(this);
  has_initialized_ = false;
  is_model_tx_ = CVIniConfig::getInstance().IsModelTX();
  screen_menu_ = nullptr;
}

ScreenWidget::~ScreenWidget() {
}

void ScreenWidget::Initialize( IControllerManager *controller_manager ) {
  assert(has_initialized_ == false);
  ConnectWithControllerManager(controller_manager);
  SetupGui();
  has_initialized_ = true;
}

void ScreenWidget::ConnectWithControllerManager( IControllerManager *controller_manager ) {
  assert(controller_manager);
  IDisplayController *display_controller =
    controller_manager->GetDisplayController();
  assert(display_controller);

  // 发送视频控制请求
  connect(
    this, 
    SIGNAL(ReceiveVideoRequestSignal(const QString&, int, int, bool)),
    display_controller, 
    SLOT(HandleReceiveVideoRequestSlot(const QString&, int, int, bool)));

  connect(
    this, &ScreenWidget::StopVideoRequestSignal,
    display_controller, &IDisplayController::HandleStopVideoRequestSlot);

  connect(
    this, 
    SIGNAL(ShowRTCPMessageRequestSignal(bool, int)),
    display_controller, 
    SLOT(HandleShowRTCPMessageRequestSlot(bool, int)));

  connect(
    this,
    SIGNAL(ReceiveDocumentRequestSignal(const QString&, int)),
    display_controller, 
    SLOT(HandleReceiveDocumentRequestSlot(const QString&, int)));

  // 发送屏幕布局变更请求
  connect(
    this, 
    &ScreenWidget::ChangeScreenLayoutRequestSignal,
    display_controller,
    &IDisplayController::HandleScreenLayoutRequestSlot);
}

void ScreenWidget::SetupGui() {
  seat_list_.reserve(10);
  // 索引为0-15的seat用于显示视频
  // 索引为16的seat用于屏幕共享
  for (int i = 0; i <= kMaxVideoSeatCount; ++i) {
    SeatWidget *seat = new SeatWidget(this, i);
    ConnectWithSeatWidget(seat);
    seat_list_.push_back(seat);
  }
  CreateMenuAndActions();
  ResetScreenWidget();
  is_rtcp_shown_ = false;
}

void ScreenWidget::ConnectWithSeatWidget( SeatWidget *seat ) {
  assert(seat != nullptr);

  connect(
    seat, SIGNAL(ReceiveVideoSignal(const QString&, int, bool)),
    this, SLOT(HandleReceiveVideoRequestSlot(const QString&, int, bool)));
  connect(seat, SIGNAL(StopVideoSignal(const QString &, int)),
          this, SLOT(HandleStopVideoRequestSlot(const QString&, int)));
  connect(seat, SIGNAL(ReceiveDocumentSignal(const QString&, int)),
          this, SLOT(HandleReceiveDocumentRequestSlot(const QString&, int)));
  connect(seat, SIGNAL(StopDocumentSignal(const QString&, int)),
          this, SLOT(HandleStopDocumentRequestSlot(const QString&, int)));
}

void ScreenWidget::CreateMenuAndActions() {
  screen_menu_ = new QMenu(this);

  screen_menu_->addAction(ui_.showRTCPAction);
  connect(ui_.showRTCPAction, SIGNAL(triggered()),
    this, SLOT(ShowRTCPMessageSlot()));
  
  // 当屏幕时第0号时，不添加布局改变的actions
  //if (screen_index_ == 0) {
  //  return;
  //}
  
  screen_menu_->addSeparator();
  /*if (screen_index_ == RunningConfig::Instance()->MainScreen()) {
    screen_menu_->addAction(ui_.oneMultiTwoTopAction);
  }*/
  
  screen_menu_->addAction(ui_.oneMultiOneAction);
  screen_menu_->addAction(ui_.twoMultiTwoAction);
  screen_menu_->addAction(ui_.threeMultiThreeAction);
  screen_menu_->addAction(ui_.fourMultiFourAction);
  screen_menu_->addAction(ui_.onePlusFiveLAction);
  screen_menu_->addAction(ui_.autoLayoutAction);
  screen_menu_->addAction(ui_.documentAction);
  
  // TODO: 只有当终端是主讲人时，才有AirPlay选项？
  screen_menu_->addAction(ui_.airplayAction);
  connect(ui_.oneMultiTwoTopAction, SIGNAL(triggered()),
    this, SLOT(OneMultiTwoTopSlot()));

  connect(ui_.oneMultiOneAction, SIGNAL(triggered()),
          this, SLOT(OneMultiOneSlot()));
  connect(ui_.twoMultiTwoAction, SIGNAL(triggered()),
          this, SLOT(TwoMultiTwoSlot()));
  connect(ui_.threeMultiThreeAction, SIGNAL(triggered()),
          this, SLOT(ThreeMultiThreeSlot()));
  connect(ui_.onePlusFiveLAction, SIGNAL(triggered()),
          this, SLOT(OnePlusFiveSlot()));
  connect(ui_.documentAction, SIGNAL(triggered()),
          this, SLOT(DocumentLayoutSlot()));
  connect(ui_.autoLayoutAction, SIGNAL(triggered()),
          this, SLOT(AutoLayoutSlot()));
  connect(ui_.fourMultiFourAction, SIGNAL(triggered()),
          this, SLOT(FourMultiFourSlot()));
  connect(ui_.airplayAction, SIGNAL(triggered()),
    this, SLOT(AirPlaySlot()));
}

void ScreenWidget::ResetScreenWidget() {
  TwoMultiTwoSlot();
  // ResetSeats();
  is_rtcp_shown_ = false;
}

void ScreenWidget::ResetSeats() {
  for (int i = 0; i < seat_list_.size(); ++i) {
    seat_list_[i]->Reset();
  }
}

void ScreenWidget::ShowRTCPMessageSlot() {
  ui_.showRTCPAction->setChecked(is_rtcp_shown_);
  emit ShowRTCPMessageRequestSignal(!is_rtcp_shown_, screen_index_);
}

void ScreenWidget::SetShowReceiveRTCPMessage( bool show ) {
  is_rtcp_shown_ = show;
  ui_.showRTCPAction->setChecked(show);
}

void ScreenWidget::OneMultiOneSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOf1x1);
}

void ScreenWidget::TwoMultiTwoSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOf2x2);
}

void ScreenWidget::ThreeMultiThreeSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOf3x3);
}

void ScreenWidget::OnePlusFiveSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOf1plus5);
}

void ScreenWidget::AutoLayoutSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOfAuto);
}

void ScreenWidget::DocumentLayoutSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOfDocument);
}

void ScreenWidget::OneMultiTwoTopSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOfTop1x2);
}

void ScreenWidget::FourMultiFourSlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOf4x4);
}

void ScreenWidget::AirPlaySlot() {
  emit ChangeScreenLayoutRequestSignal(
    screen_index_, IDisplayController::kLayoutOfAirplay);
}

void ScreenWidget::ChangeScreenLayout(IDisplayController::ScreenLayout layout) {
  switch (layout) {
    case IDisplayController::kLayoutOf1x1:
      HandleOneMultiOneNotify();
      break;
    case IDisplayController::kLayoutOf2x2:
      HandleTwoMultiTwoNotify();
      break;
    case IDisplayController::kLayoutOf3x3:
      HandleThreeMultiThreeNotify();
      break;
    case IDisplayController::kLayoutOf1plus5:
      HandleOnePlusFiveNotify();
      break;
    case IDisplayController::kLayoutOfAuto:
      HandleAutoLayoutNotify();
      break;
    case IDisplayController::kLayoutOfDocument:
      HandleDocumentLayoutNotify();
      break;
    case IDisplayController::kLayoutOfTop1x2:
      HandleTopOneMultiTwoNotify();
      break;
    case IDisplayController::kLayoutOf4x4:
      HandleFourMultiFourNotify();
      break;
    case IDisplayController::kLayoutOfAirplay:
      HandleAirPlayNotify();
      break;
  }
}

void ScreenWidget::HandleOneMultiOneNotify() {
  ClearMenuMarks();
  ui_.oneMultiOneAction->setChecked(true);
  ClearScreen();
  ui_.gridLayout->addWidget(seat_list_[0]);
  seat_list_[0]->show();
}

void ScreenWidget::HandleTwoMultiTwoNotify() {
  ClearMenuMarks();
  ui_.twoMultiTwoAction->setChecked(true);

  ClearScreen();
  for (int i = 0; i < 4; ++i) {
    int row = i / 2;
    int col = i - row * 2;
    ui_.gridLayout->addWidget(seat_list_[i], row, col);
    seat_list_[i]->show();
  }
}

void ScreenWidget::HandleThreeMultiThreeNotify() {
  ClearMenuMarks();
  ui_.threeMultiThreeAction->setChecked(true);

  ClearScreen();
  for (int i = 0; i < 9; ++i) {
    int row = i / 3;
    int col = i - row * 3;
    ui_.gridLayout->addWidget(seat_list_[i], row, col);
    seat_list_[i]->show();
  }
}

void ScreenWidget::HandleOnePlusFiveNotify() {
  ClearMenuMarks();
  ui_.onePlusFiveLAction->setChecked(true);

  ClearScreen();
  ui_.gridLayout->addWidget(seat_list_[0], 0, 0, 2, 2);
  ui_.gridLayout->addWidget(seat_list_[1], 0, 2, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[2], 1, 2, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[3], 2, 0, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[4], 2, 1, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[5], 2, 2, 1, 1);
  for (int i = 0; i < 6; ++i) {
    seat_list_[i]->show();
  }
}

void ScreenWidget::HandleAutoLayoutNotify() {
  ClearMenuMarks();
  ui_.autoLayoutAction->setChecked(true);

  ClearScreen();
  ui_.gridLayout->addWidget(seat_list_[0], 0, 1, 2, 3);
  ui_.gridLayout->addWidget(seat_list_[1], 2, 0, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[2], 2, 1, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[3], 2, 2, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[4], 2, 3, 1, 1);
  ui_.gridLayout->addWidget(seat_list_[5], 2, 4, 1, 1);
  for(int i = 0; i < 6; ++i)  {
    seat_list_[i]->show();
  }
}

void ScreenWidget::HandleTopOneMultiTwoNotify() {
  ClearMenuMarks();
  ui_.oneMultiTwoTopAction->setChecked(true);
  ClearScreen();
  ui_.gridLayout->addWidget(seat_list_[0], 0, 0);
  ui_.gridLayout->addWidget(seat_list_[1], 0, 1);

  seat_list_[0]->show();
  seat_list_[1]->show();
}

void ScreenWidget::HandleFourMultiFourNotify() {
  ClearMenuMarks();
  ui_.fourMultiFourAction->setChecked(true);

  ClearScreen();
  for (int i = 0; i < 16; ++i) {
    int row = i / 4;
    int col = i - row * 4;
    ui_.gridLayout->addWidget(seat_list_[i], row, col);
    seat_list_[i]->show();
  }
}


void ScreenWidget::HandleDocumentLayoutNotify() {
  ClearMenuMarks();
  ClearScreen();
  ui_.documentAction->setChecked(true);
  ui_.gridLayout->addWidget(seat_list_.back());
  seat_list_.back()->SetSeatTitle(QString::fromLocal8Bit("本地文件"));
  seat_list_.back()->show();
}

void ScreenWidget::HandleAirPlayNotify() {
  ClearMenuMarks();
  ClearScreen();
  ui_.airplayAction->setChecked(true);
  ui_.gridLayout->addWidget(seat_list_.back());
  seat_list_.back()->SetSeatTitle(QString::fromLocal8Bit("AirPlay"));
  seat_list_.back()->show();
}

void ScreenWidget::ClearMenuMarks() {
  ui_.oneMultiOneAction->setChecked(false);
  ui_.twoMultiTwoAction->setChecked(false);
  ui_.threeMultiThreeAction->setChecked(false);
  ui_.onePlusFiveLAction->setChecked(false);
  ui_.documentAction->setChecked(false);
  ui_.autoLayoutAction->setChecked(false);
  ui_.oneMultiTwoTopAction->setChecked(false);
  ui_.fourMultiFourAction->setChecked(false);
  ui_.airplayAction->setChecked(false);
}

void ScreenWidget::ClearScreen() {
  ClearLayout();
  for (int i = 0; i < seat_list_.size(); ++i) {
    seat_list_[i]->hide();
  }
}

void ScreenWidget::ClearLayout() {
  QLayoutItem *item;
  while ((item = ui_.gridLayout->takeAt(0)) != 0) {
    ui_.gridLayout->removeItem(item);
  }
}

void ScreenWidget::HandleReceiveVideoRequestSlot(const QString &vuri,
                                                 int seat_index, 
                                                 bool use_small_video) {
  // 由于ConferenceManager的座位序号是从1开始
  // 而SeatWidget的座位序号是从0开始，故需要加1
  emit ReceiveVideoRequestSignal(
    vuri, screen_index_, seat_index + 1, use_small_video);
}

void ScreenWidget::AddVideoSeat(const QString &vuri,
                                const QString &name, 
                                int seat_index, 
                                bool use_small_video) {
  // 由于ConferenceManager的座位序号是从1开始
  // 而SeatWidget的座位序号是从0开始，故需要减1
  seat_list_[seat_index-1]->Set(vuri, name, use_small_video);
}

void ScreenWidget::HandleStopVideoRequestSlot(
    const QString &vuri, int seat_index) {
  emit StopVideoRequestSignal(vuri);
}

void ScreenWidget::RemoveVideoSeat( const QString &vuri, int seat_index ) {
  // 由于ConferenceManager的座位序号是从1开始
  // 而SeatWidget的座位序号是从0开始，故需要减1
  seat_list_[seat_index-1]->Reset();
}

void ScreenWidget::HandleReceiveDocumentRequestSlot(
    const QString &uri, int seat_index) {
  emit ReceiveDocumentRequestSignal(uri, screen_index_);
}

void ScreenWidget::AddDocument( const QString &uri, const QString &name ) {
  seat_list_.back()->Set(uri, name);
}

void ScreenWidget::HandleStopDocumentRequestSlot( 
    const QString &uri, int seat_index) {
  emit StopDocumentRequestSignal(uri, screen_index_);
}

void ScreenWidget::RemoveDocument( const QString &uri ) {
  // 默认最后的座位用于显示共享屏幕
  seat_list_.back()->Reset();
}

void ScreenWidget::mouseReleaseEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton && !is_model_tx_ && screen_menu_) {
    screen_menu_->exec(event->globalPos());
  }
  event->accept();
}









