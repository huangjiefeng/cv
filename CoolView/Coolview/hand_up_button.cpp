#include "hand_up_button.h"

#include <cassert>

#include "conference_controller_interface.h"

HandUpButton::HandUpButton(QWidget *parent)
    : QPushButton(parent) {
  SetHandUpStatus(false);
  setMinimumSize(70, 70);
  setMaximumSize(70, 70);
  setFlat(true);
  setToolTip(QString::fromLocal8Bit("æŸ ÷"));
  setIconSize(QSize(64, 64));
}

HandUpButton::~HandUpButton() {

}

void HandUpButton::Initialize( IConferenceController *controller ) {
  assert(controller);
  connect(
    this, &HandUpButton::HandUpSignal,
    controller, &IConferenceController::HandleHandUpRequestSlot);

  connect(
    controller, &IConferenceController::NotifyTerminalHandUpSignal,
    this, &HandUpButton::HandleHandUpNotifySlot);

  connect(this, &HandUpButton::clicked,
          this, &HandUpButton::HandUpSlot);
}

void HandUpButton::Reset() {
  SetHandUpStatus(false);
}

void HandUpButton::HandUpSlot() {
  emit HandUpSignal(!is_handup_);
}

void HandUpButton::HandleHandUpNotifySlot( bool handup ) {
  SetHandUpStatus(handup);
}

void HandUpButton::SetHandUpStatus( bool handup ) {
  is_handup_ = handup;
  setChecked(handup);
  setIcon(QIcon(handup ? ":/image/SpeechSend.png" : ":/image/Speech.png"));
}



