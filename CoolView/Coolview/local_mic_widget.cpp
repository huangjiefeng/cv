#include "local_mic_widget.h"

#include <cassert>
#include <string>
#include <vector>

#include <DeviceManager/DeviceManager.h>

#include "config/RunningConfig.h"

#include "display_controller_interface.h"
#include "voice_volume_setting_widget.h"

LocalMicWidget::LocalMicWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);
  volume_setting_ = nullptr;
}

LocalMicWidget::~LocalMicWidget() {

}

void LocalMicWidget::Initialize( IDisplayController *controller ) {
  connect(this, &LocalMicWidget::ChangeVoiceVolumeSignal,
          controller, &IDisplayController::HandleVoiceChangeRequestSlot);

  connect(controller, &IDisplayController::NotifyVoiceChangeSignal,
    this, &LocalMicWidget::HandleVoiceChangedNotifySlot);

  connect(ui_.micVolumeButton, &QToolButton::clicked,
          this, &LocalMicWidget::ShowVolumeSettingWidgetSlot);

  volume_setting_ = new VoiceVolumeSettingWidget(this);
  ui_.gridLayout->addWidget(volume_setting_, 0, 1, 0, 1);
  volume_setting_->setVisible(false);

  connect(volume_setting_, &VoiceVolumeSettingWidget::VoiceChangeRequestSignal,
          this, &LocalMicWidget::HandleVoiceChangedRequestSlot);
}

void LocalMicWidget::ShowVolumeSettingWidgetSlot( bool show ) {
  if (show) {
    std::vector<std::string> audio_input_devices;
    DeviceManager::GetInstatnce()->GetAudioCaptureDevices(&audio_input_devices);
    int volume = 0;
    if (!audio_input_devices.empty()) {
       volume = DeviceManager::GetInstatnce()->GetAudioInputVolumn(
        RunningConfig::Instance()->AudioCaptureDevice().toStdString());
    }
    volume_setting_->SetVolume(qBound(0, volume, 100));
  }
  volume_setting_->setVisible(show);
}

void LocalMicWidget::HandleVoiceChangedRequestSlot( int volume ) {
  emit ChangeVoiceVolumeSignal(kMicrophone, volume);
}

void LocalMicWidget::HandleVoiceChangedNotifySlot(AudioDeviceType type, int volume) {
  if (volume_setting_->isVisible() && type == kMicrophone) {
    volume_setting_->SetVolume(volume);
  }
}
