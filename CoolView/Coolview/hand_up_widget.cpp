#include "hand_up_widget.h"

#include <cassert>

#include "conference_controller_interface.h"

HandUpWidget::HandUpWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);
  is_handup_ = false;
}

HandUpWidget::~HandUpWidget() {

}

void HandUpWidget::Initialize( IConferenceController *controller ) {
  assert(controller);
  connect(
    this, &HandUpWidget::HandUpSignal,
    controller, &IConferenceController::HandleHandUpRequestSlot);

  connect(
    controller, &IConferenceController::NotifyTerminalHandUpSignal,
    this, &HandUpWidget::HandleHandUpNotifySlot);

  connect(ui_.handUpButton, &QPushButton::clicked,
          this, &HandUpWidget::HandUpSlot);
}

void HandUpWidget::Reset() {
  is_handup_ = false;
  ui_.handUpButton->setChecked(false);
}

void HandUpWidget::HandUpSlot() {
  emit HandUpSignal(!is_handup_);
}

void HandUpWidget::HandleHandUpNotifySlot( bool handup ) {
  is_handup_ = handup;
  ui_.handUpButton->setChecked(handup);
}



