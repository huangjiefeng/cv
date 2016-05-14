#ifndef AUDIO_SEND_CONTROL_BUTTON_H
#define AUDIO_SEND_CONTROL_BUTTON_H

#include <QPushButton>

class AudioSendControlButton : public QPushButton {
  Q_OBJECT

public:
  AudioSendControlButton(QWidget *parent);
  ~AudioSendControlButton();

  void SetSettingMode(bool is_setting_mode);
  void Reset();

Q_SIGNALS:
  void RequestControlAudioSendSignal(bool send);
  void DisplayMicConfigSignal();

public Q_SLOTS:
  void HandleControlAudioSendNotifySlot(bool send);

private Q_SLOTS:
  void HandleButtonClickedSlot(bool checked);

private:
  void SetSendStatus(bool send);

private:
  bool send_audio_;
  bool is_setting_mode_;
  QString image_url_;
};

#endif // AUDIO_SEND_CONTROL_BUTTON_H
