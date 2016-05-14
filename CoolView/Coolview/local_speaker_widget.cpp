#include "local_speaker_widget.h"

#include <cassert>

#include "display_controller_interface.h"
#include "voice_volume_setting_widget.h"

#include "DeviceManager/DeviceManager.h"

LocalSpeakerWidget::LocalSpeakerWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);
  volume_setting_ = nullptr;
}

LocalSpeakerWidget::~LocalSpeakerWidget() {
}

void LocalSpeakerWidget::Initialize( IDisplayController *controller ) {
  assert(controller);
  connect(controller, &IDisplayController::NotifyVoiceChangeSignal,
    this, &LocalSpeakerWidget::HandleVoiceChangedNotifySlot);

  connect(this, &LocalSpeakerWidget::ChangeVoiceVolumeSignal,
          controller, &IDisplayController::HandleVoiceChangeRequestSlot);

  connect(ui_.speakerVolumeButton, &QToolButton::clicked,
          this, &LocalSpeakerWidget::ShowVolumeSettingWidgetSlot);

  volume_setting_ = new VoiceVolumeSettingWidget(this);
  ui_.gridLayout->addWidget(volume_setting_, 0, 1, 0, 1);
  volume_setting_->setVisible(false);

  connect(volume_setting_, &VoiceVolumeSettingWidget::VoiceChangeRequestSignal,
          this, &LocalSpeakerWidget::HandleVoiceChangedRequestSlot);
}

void LocalSpeakerWidget::ShowVolumeSettingWidgetSlot( bool show ) {
  if (show) {
    int volume = DeviceManager::GetInstatnce()->GetAudioOutputVolumn();
    volume_setting_->SetVolume(qBound(0, volume, 100));
  }
  volume_setting_->setVisible(show);
}

void LocalSpeakerWidget::HandleVoiceChangedRequestSlot( int volume ) {
  emit ChangeVoiceVolumeSignal(kSpeaker, volume);
}

void LocalSpeakerWidget::HandleVoiceChangedNotifySlot( 
    AudioDeviceType type, int volume) {
  if (volume_setting_->isVisible() && type == kSpeaker) {
    volume_setting_->SetVolume(volume);
  }
}




