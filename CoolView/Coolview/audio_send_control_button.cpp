#include "audio_send_control_button.h"

AudioSendControlButton::AudioSendControlButton(QWidget *parent)
    : QPushButton(parent) {
  setIconSize(QSize(64, 64));
  resize(70, 70);
  setMaximumSize(70, 70);
  SetSendStatus(true);
  is_setting_mode_ = false;
  setFlat(true);

  connect(
    this, &AudioSendControlButton::clicked, 
    this, &AudioSendControlButton::HandleButtonClickedSlot);
}

AudioSendControlButton::~AudioSendControlButton() {

}

void AudioSendControlButton::Reset() {
  SetSendStatus(true);
}

void AudioSendControlButton::HandleButtonClickedSlot( bool checked ) {
  if (is_setting_mode_) {
    emit DisplayMicConfigSignal();
  } else {
    emit RequestControlAudioSendSignal(!send_audio_);
  }
}

void AudioSendControlButton::HandleControlAudioSendNotifySlot( bool send ) {
  SetSendStatus(send);
}

void AudioSendControlButton::SetSendStatus( bool send ) {
  send_audio_ = send;
  image_url_ = send ? ":/image/voiceStop.png" : ":/image/voiceSend.png";
  setIcon(QIcon(image_url_));
}

void AudioSendControlButton::SetSettingMode( bool is_setting_mode ) {
  is_setting_mode_ = is_setting_mode;
  setIcon(QIcon(is_setting_mode ? ":/image/voiceSet.png" : image_url_));
}

