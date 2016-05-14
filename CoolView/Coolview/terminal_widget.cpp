#include "terminal_widget.h"

#include <cassert>

#include <QMenu>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QTimer>

#include <util/ini/CVIniConfig.h>
#include <profile/RunningProfile.h>

#include "display_controller_interface.h"
#include "controller_manager_interface.h"

#include "virtual_terminal_widget.h"

#include "mime_text_parser_and_builder.h"

TerminalWidget::TerminalWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);

  is_online_ = false;
  previous_status_is_offline_ = false;
  is_audio_received_ = true;
  is_main_speaker_terminal_ = false;
  has_initialized_ = false;
  controller_manager_ = nullptr;
  hide_timer_.setSingleShot(true);
  show_virtual_terminals_ = false;
  ui_.headFrame->setObjectName("headframe");
  current_animation_index_ = 0;
  current_loop_ = 0;
}

TerminalWidget::~TerminalWidget() {
}

void TerminalWidget::Initialize( IControllerManager *controller_manager) {
  assert(!has_initialized_);
  assert(controller_manager);

  controller_manager_ = controller_manager;

  is_online_ = false;
  is_audio_received_ = true;
  is_main_speaker_terminal_ = false;
  setted_by_first_terminal_ = false;
  received_count_ = 0;

  ui_.videoMarkButton->hide();
  ui_.audioMarkButton->hide();
  ui_.handUpMarkButton->hide();
  ui_.speakMarkButton->hide();
  ui_.frame_2->hide();

  connect(&hide_timer_, SIGNAL(timeout()), 
          this, SLOT(HideVirtualTerminalsSlot()));

  connect(&animate_online_timer_, SIGNAL(timeout()),
          this, SLOT(TerminalOnlineAnimationSlot()));

  CreateMenuAndActions();

  IDisplayController *display_controller = 
    controller_manager->GetDisplayController();
  assert(display_controller);

  connect(this, 
          SIGNAL(ControlReceiveAudioSignal(const QString&, bool)),
          display_controller, 
          SLOT(HandleReceiveAudioRequestSlot(const QString&, bool)));

  connect(display_controller,
          SIGNAL(NotifyReceiveVideoSignal(const QString&, int, int, bool)),
          this,
          SLOT(HandleReceiveVideoNotifySlot(const QString&)));

  connect(this,
          &TerminalWidget::StopReceiveVideoSignal,
          display_controller, 
          &IDisplayController::HandleStopVideoRequestSlot);

  connect(display_controller, 
          SIGNAL(NotifyStopVideoSignal(const QString&, int, int)),
          this,
          SLOT(HandleStopVideoNotifySlot(const QString&)));

  connect(this,
    &TerminalWidget::PermitSpeakSignal,
    display_controller,
    &IDisplayController::HandlePermitSpeakRequestSlot);

  has_initialized_ = true;
}

void TerminalWidget::CreateMenuAndActions() {
  menu_ = new QMenu(this);

  menu_->addAction(ui_.stopReceiveAllVideoAction);
  connect(
    ui_.stopReceiveAllVideoAction, SIGNAL(triggered()),
    this, SLOT(StopReceiveAllVideoSlot()));
  
  menu_->addSeparator();

  menu_->addAction(ui_.receiveAudioAction);
  connect(ui_.receiveAudioAction, SIGNAL(triggered()), 
    this, SLOT(ControlAudioReceivedSlot()));

  // 只有本终端是主持人时，才添加允许发言的action
  connect(ui_.permitSpeakAction, SIGNAL(triggered()),
          this, SLOT(PermitSpeakSlot()));

  ui_.stopReceiveAllVideoAction->setEnabled(false);
  ui_.receiveAudioAction->setEnabled(false);
}

void TerminalWidget::AddVirtualTerminal(ConstTerminalPointer terminal) {
  if (FindVirtualTerminal(terminal->virtual_index) == nullptr) {
    // 更新界面
    if (terminal->virtual_index == 0 || !setted_by_first_terminal_) {
      setted_by_first_terminal_ = true;
      UpdateStatus(terminal);
    }

    // 添加新终端
    VirtualTerminalWidget *widget = new VirtualTerminalWidget(this);
    widget->Initialize(terminal, controller_manager_);
    //const int row = terminal->virtual_index / kWidgetsPerRow;
    //const int col = terminal->virtual_index - row * kWidgetsPerRow;
    //ui_.virtualGridLayout->addWidget(widget, row, col);
    ui_.virtualGridLayout->addWidget(widget, 0, terminal->virtual_index);
    virtual_terminal_widgets_.push_back(widget);
  }
}

void TerminalWidget::UpdateVirtualTerminal(ConstTerminalPointer terminal) {
  auto widget = FindVirtualTerminal(terminal->virtual_index);
  if (widget) {
    widget->Update(terminal);
    if (terminal->virtual_index == 0) {
      UpdateStatus(terminal);
    }
  }
}

VirtualTerminalWidget* TerminalWidget::FindVirtualTerminal( int virtual_index ) {
  for (auto widget : virtual_terminal_widgets_) {
    if (widget->virtual_index() == virtual_index) {
      return widget;
    }
  }
  return nullptr;
}

void TerminalWidget::UpdateStatus(ConstTerminalPointer terminal ) {
  terminal_uri_ = terminal->uri;

  ui_.memberNameLabel->setText(
    terminal->name.left(terminal->name.indexOf('(')));
  //setToolTip(ui_.memberNameLabel);

  if (terminal->has_video) {
    ui_.videoMarkButton->show();
  } else {
    ui_.videoMarkButton->hide();
  }
  if (terminal->is_main_speaker) {
    ui_.audioMarkButton->show();
  } else {
    ui_.audioMarkButton->hide();
  }
  if (terminal->is_handup) {
    ui_.handUpMarkButton->show();
  } else {
    ui_.handUpMarkButton->hide();
  }
  if (terminal->is_speaking) {
    ui_.speakMarkButton->show();
  } else {
    ui_.speakMarkButton->hide();
  } 

  is_main_speaker_terminal_ = terminal->is_chairman_terminal;

  image_url_ = terminal->is_chairman_terminal ? 
    ":/image/Chairman.png" : ":/image/users_group.png";
  ui_.headFrame->setStyleSheet("QFrame#headframe{border-image:url("+image_url_+")}");


  if (previous_status_is_offline_ && terminal->is_available) {
    StartOnlineAnimation();
  }
  set_online(terminal->is_available);
  
}

void TerminalWidget::set_online( bool is_online ) {
  if (is_online && received_count_ > 0) {
    return;
  }
  previous_status_is_offline_ = !is_online;
  setEnabled(is_online);
  ui_.frame->setStyleSheet(
    QString("QFrame{border-width:2px;border-color:") + 
    (is_online ? "lightGreen" : "darkGray") + 
    ";border-style:solid;border-radius: 5px;}");
  ui_.frame_2->setStyleSheet(ui_.frame->styleSheet());
  is_online_ = is_online;
  if (!is_online) {
    StopOnlineAnimation();
  }
}

void TerminalWidget::StartOnlineAnimation() {
  if (animate_online_timer_.isActive()) {
    return;
  }
  animate_online_timer_.start(animate_interval_);
}

void TerminalWidget::StopOnlineAnimation() {
  if (animate_online_timer_.isActive()) {
    animate_online_timer_.stop();
    current_animation_index_ = 0;
    current_loop_ = 0;
  }
  ui_.memberNameLabel->setStyleSheet("QLabel { border: 0px; }");
}

void TerminalWidget::TerminalOnlineAnimationSlot() {
  static const char *label_color[] = {"yellow", "lightblue", "green", "blue", "black"};
  if (current_animation_index_ == sizeof(label_color)/sizeof(label_color[0])) {
    current_animation_index_ = 0;
    if (++current_loop_ == animate_loops_) {
      current_loop_ = 0;
      if (animate_online_timer_.isActive()) {
        animate_online_timer_.stop();
        return;
      }
    }
  }
  ui_.memberNameLabel->setStyleSheet(
    QString("QLabel { border: 0px; color: %1; }").arg(label_color[current_animation_index_++]));
}

void TerminalWidget::ControlAudioReceivedSlot() {
  ui_.receiveAudioAction->setChecked(!is_audio_received_);
  for (auto widget : virtual_terminal_widgets_) {
    emit ControlReceiveAudioSignal(widget->virtual_uri(), !is_audio_received_);
  }
}

void TerminalWidget::SetAudioReceivedMark( bool receive ) {
  is_audio_received_ = receive;
  ui_.receiveAudioAction->setChecked(receive);
}

void TerminalWidget::HandleReceiveVideoNotifySlot( const QString &vuri ) {
  if (terminal_uri_ != TerminalHelper::GetTerminalURI(vuri)) {
    return;
  }
  for (auto widget : virtual_terminal_widgets_) {
    if (widget->virtual_uri() == vuri) {
      widget->SetShowStatus(true);
      break;
    }
  }

  ++received_count_;
  if (received_count_ == 1) {
    ui_.frame->setStyleSheet("QFrame{border-width:2px;border-color:blue;border-style:solid;border-radius: 5px;}");
    ui_.frame_2->setStyleSheet(ui_.frame->styleSheet());
    ui_.stopReceiveAllVideoAction->setEnabled(true);
    ui_.receiveAudioAction->setEnabled(true);
    ui_.receiveAudioAction->setChecked(true);
    is_audio_received_ = true;
  }
}

void TerminalWidget::StopReceiveAllVideoSlot() {
  for (auto widget : virtual_terminal_widgets_) {
    if (widget->IsShow()) {
      emit StopReceiveVideoSignal(widget->virtual_uri());
    }
  }
}

void TerminalWidget::HandleStopVideoNotifySlot( const QString &vuri ) {
  if (terminal_uri_ != TerminalHelper::GetTerminalURI(vuri)) {
    return;
  }
  
  for (auto widget : virtual_terminal_widgets_) {
    if (widget->virtual_uri() == vuri) {
      widget->SetShowStatus(false);
      break;
    }
  }

  --received_count_;
  if (received_count_ == 0) {
    set_online(is_online_);
    ui_.stopReceiveAllVideoAction->setEnabled(false);
    ui_.receiveAudioAction->setEnabled(false);
  }
}

void TerminalWidget::ActivateSpeakControl( bool activate ) {
  menu_->removeAction(ui_.permitSpeakAction);
  if (activate) {
    menu_->addAction(ui_.permitSpeakAction);
  }
}

void TerminalWidget::PermitSpeakSlot() {
  emit PermitSpeakSignal(terminal_uri_, ui_.permitSpeakAction->isChecked());
}

void TerminalWidget::mousePressEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton) {
    start_pos_ = event->globalPos();
  }
  QWidget::mousePressEvent(event);
}

void TerminalWidget::mouseMoveEvent( QMouseEvent *event ) {
  if (event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - start_pos_).manhattanLength();
    if (!show_virtual_terminals_ && 
        distance >= QApplication::startDragDistance() * 10) {
      PerformDrag();
    }
  }
  QWidget::mouseMoveEvent(event);
}

void TerminalWidget::PerformDrag() {
  MimeTextBuilder text_builder(terminal_uri_);
  for (auto widget : virtual_terminal_widgets_) {
    if (widget) {
      text_builder.AddVirtualURI(widget->virtual_uri());
    }
  }
  QMimeData *mime_data = new QMimeData();
  mime_data->setText(text_builder.GetText());
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mime_data);

  QPixmap p(image_url_);
  drag->setPixmap(p);
  drag->setHotSpot(QPoint(p.size().width() / 2, p.size().height() / 2));
  
  // 在windows上exec会阻塞event loop
  drag->exec();
  
  // 在windows上需要删除drag
  drag->deleteLater();
}

void TerminalWidget::mouseReleaseEvent( QMouseEvent *event ) {
  if (event->button() == Qt::RightButton) {
    menu_->exec(event->globalPos());
  } else if (event->button() == Qt::LeftButton) {
    ShowVirtualTerminals();
  }
  event->accept();
}

void TerminalWidget::ShowVirtualTerminals() {
  ui_.frame->hide();
  ui_.frame_2->show();
  hide_timer_.start(5000);
  show_virtual_terminals_ = true;
}

void TerminalWidget::HideVirtualTerminalsSlot() {
  ui_.frame_2->hide();
  ui_.frame->show();
  show_virtual_terminals_ = false;
}



