#include "speaker_volume_control_button.h"
#include <DeviceManager/DeviceManager.h>

SpeakerVolumeControlButton::SpeakerVolumeControlButton(QWidget *parent)
    : QPushButton(parent) {
  setIconSize(QSize(64, 64));
  resize(70, 70);
  setMaximumSize(70, 70);
  is_setting_mode_ = false;
  setFlat(true);

  SetSpeakerStatus(DeviceManager::GetInstatnce()->GetAudioOutputMute() == 1);

  connect(
    this, &SpeakerVolumeControlButton::clicked, 
    this, &SpeakerVolumeControlButton::HandleButtonClickedSlot);
}

SpeakerVolumeControlButton::~SpeakerVolumeControlButton() {

}

void SpeakerVolumeControlButton::HandleButtonClickedSlot( bool checked ) {
  if (is_setting_mode_) {
    emit DisplaySpeakerConfigSignal();
  } else {
    emit RequestControlVoiceMuteSignal(kSpeaker, !is_mute_);
  }
}

void SpeakerVolumeControlButton::SetSpeakerStatus( bool is_mute ) {
  is_mute_ = is_mute;
  image_url_ = is_mute ? ":/image/speakerNoMute.png" : ":/image/speakerMute.png";
  setIcon(QIcon(image_url_));
}

void SpeakerVolumeControlButton::SetSettingMode( bool is_setting_mode ) {
  is_setting_mode_ = is_setting_mode;
  setIcon(QIcon(is_setting_mode ? ":/image/speakerSet.png" : image_url_));
}

void SpeakerVolumeControlButton::HandleMuteChangeNotifySlot( AudioDeviceType type, bool is_mute ) {
  if (type == kSpeaker) {
    SetSpeakerStatus(is_mute);
  }
}

