#ifndef VOICE_VOLUME_SETTING_WIDGET_H
#define VOICE_VOLUME_SETTING_WIDGET_H

#include <QWidget>
#include "ui_voice_volume_setting_widget.h"

class VoiceVolumeSettingWidget : public QWidget {
  Q_OBJECT

public:
  VoiceVolumeSettingWidget(QWidget *parent);
  ~VoiceVolumeSettingWidget();

  void SetVolume(int volume);

Q_SIGNALS:
  void VoiceChangeRequestSignal(int volume);

private Q_SLOTS:
  void HandleSliderValueChangedSlot(int volume);

private:
  Ui::VoiceVolumeSettingWidget ui_;
};

#endif // VOICE_VOLUME_SETTING_WIDGET_H
