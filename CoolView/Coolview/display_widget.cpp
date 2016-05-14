#include "display_widget.h"

#include <cassert>

#include "screen_widget.h"
#include "seat_widget.h"

#include "conference_controller_interface.h"
#include "display_controller_interface.h"
#include "controller_manager_interface.h"

DisplayWidget::DisplayWidget(QWidget *parent /*= nullptr*/)
    : QWidget(parent) {
  ui_.setupUi(this);
  screen_count_ = 0;
}

DisplayWidget::~DisplayWidget() {
}

void DisplayWidget::Initialize(IControllerManager *controller_manager) {
  assert(controller_manager);
  
  IDisplayController *display_controller = 
    controller_manager->GetDisplayController();
  assert(display_controller);

  screen_count_ = display_controller->GetScreenCount();

  connect(
    display_controller, 
    SIGNAL(NotifyReceiveVideoSignal(const QString&, int, int, bool)),
    this,
    SLOT(HandleReceiveVideoNotifySlot(const QString&, int, int, bool)));
  connect(
    display_controller, 
    SIGNAL(NotifyStopVideoSignal(const QString&, int, int)),
    this,
    SLOT(HandleStopVideoNotifySlot(const QString&, int, int)));
  connect(
    display_controller,
    SIGNAL(NotifyStopDocumentSignal(const QString&, int)),
    this,
    SLOT(HandleStopDocumentNotifySlot(const QString&, int)));
  connect(
    display_controller,
    SIGNAL(NotifyScreenLayoutChangedSignal(int, IDisplayController::ScreenLayout)),
    this, 
    SLOT(HandleScreenLayoutChangedSlot(int, IDisplayController::ScreenLayout)));
  connect(
    display_controller,
    &IDisplayController::NotifyShowRTCPMessageSignal,
    this,
    &DisplayWidget::HandleShowRTCPMessageNotifySlot);

  IConferenceController *conference_controller = 
    controller_manager->GetConferenceController();
  assert(conference_controller);
  connect(
    conference_controller, 
    &IConferenceController::NotifyTerminalReceivedSignal,
    this, 
    &DisplayWidget::HandleTerminalReceiveNotifySlot);

  for (int i = 0; i < screen_count_; ++i) {
    ScreenWidget *screen_widget = new ScreenWidget(this, i);
    screens_.push_back(screen_widget);
    screen_widget->Initialize(controller_manager);
    ui_.horizontalLayout->addWidget(screen_widget);
  }
}

void DisplayWidget::ResetDisplayWidget() {
  terminal_name_dict_.clear();
  for (ScreenWidget *screen : screens_) {
    screen->ResetScreenWidget();
  }
}

void DisplayWidget::HandleTerminalReceiveNotifySlot( 
    ConstTerminalPointer terminal, const QString &conference_uri) {
  terminal_name_dict_.insert(terminal->virtual_uri, terminal->name);
}

void DisplayWidget::HandleReceiveVideoNotifySlot(const QString &vuri,
                                                 int screen_index, 
                                                 int seat_index,
                                                 bool use_small_video) {
  assert(0 <= screen_index && screen_index < screen_count_);
  screens_[screen_index]->AddVideoSeat(
    vuri, terminal_name_dict_[vuri], seat_index, use_small_video);
}

void DisplayWidget::HandleStopVideoNotifySlot( 
    const QString &vuri, int screen_index, int seat_index) {
  assert(0 <= screen_index && screen_index < screen_count_);
  screens_[screen_index]->RemoveVideoSeat(vuri, seat_index);
}

void DisplayWidget::HandleReceiveDocumentNotifySlot(
    const QString &uri, int screen_index) {
  assert(0 <= screen_index && screen_index < screen_count_);
  screens_[screen_index]->AddDocument(
    uri, terminal_name_dict_[TerminalHelper::ConstructDefaultVirtualURI(uri)]);
}

void DisplayWidget::HandleStopDocumentNotifySlot(
    const QString &uri, int screen_index) {
  assert(0 <= screen_index && screen_index < screen_count_);
  screens_[screen_index]->RemoveDocument(uri);
}

void DisplayWidget::HandleScreenLayoutChangedSlot(
    int screen_index, IDisplayController::ScreenLayout layout) {
  assert(0 <= screen_index && screen_index < screen_count_);
  screens_[screen_index]->ChangeScreenLayout(layout);
}

void DisplayWidget::HandleShowRTCPMessageNotifySlot( 
    bool show, int screen_index ) {
  assert(0 <= screen_index && screen_index < screen_count_);
  for (int i = 0; i < screen_count_; ++i) {
    screens_[i]->SetShowReceiveRTCPMessage(show);
  }
}
