#include "voice_volume_setting_widget.h"

#include <cassert>

#include <DeviceManager/DeviceManager.h>

#include "config/RunningConfig.h"

VoiceVolumeSettingWidget::VoiceVolumeSettingWidget( QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);

  connect(ui_.voiceSlider, &QSlider::valueChanged,
          this, &VoiceVolumeSettingWidget::HandleSliderValueChangedSlot);
}

VoiceVolumeSettingWidget::~VoiceVolumeSettingWidget() {

}

void VoiceVolumeSettingWidget::SetVolume( int volume ) {
  if (ui_.voiceSlider->value() != volume) {
    ui_.voiceSlider->setValue(volume);
    ui_.voiceLabel->setText(QString::number(volume));
  }
}

void VoiceVolumeSettingWidget::HandleSliderValueChangedSlot( int volume ) {
  ui_.voiceLabel->setText(QString::number(volume));
  emit VoiceChangeRequestSignal(volume);
}
