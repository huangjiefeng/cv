#ifndef SPEAKER_VOLUME_CONTROL_BUTTON_H
#define SPEAKER_VOLUME_CONTROL_BUTTON_H

#include <QPushButton>

#include "media_description.h"

class SpeakerVolumeControlButton : public QPushButton {
  Q_OBJECT

public:
  SpeakerVolumeControlButton(QWidget *parent);
  ~SpeakerVolumeControlButton();

  void SetSettingMode(bool is_setting_mode);

Q_SIGNALS:
  void RequestControlVoiceMuteSignal(AudioDeviceType type, bool is_mute);
  void DisplaySpeakerConfigSignal();

public Q_SLOTS:
  void HandleMuteChangeNotifySlot(AudioDeviceType type, bool is_mute);

private Q_SLOTS:
  void HandleButtonClickedSlot(bool checked);

private:
  void SetSpeakerStatus(bool is_mute);

private:
  bool is_mute_;
  bool is_setting_mode_;
  QString image_url_;
};

#endif // SPEAKER_VOLUME_CONTROL_BUTTON_H
