#ifndef LOCAL_SPEAKER_WIDGET_H
#define LOCAL_SPEAKER_WIDGET_H

#include <QWidget>
#include "ui_local_speaker_widget.h"

#include "media_description.h"

class IDisplayController;
class VoiceVolumeSettingWidget;

class LocalSpeakerWidget : public QWidget {
  Q_OBJECT

public:
  LocalSpeakerWidget(QWidget *parent = 0);
  ~LocalSpeakerWidget();

  void Initialize(IDisplayController *controller);

Q_SIGNALS:
  void ChangeVoiceVolumeSignal(AudioDeviceType type, int volume);

private Q_SLOTS:
  void HandleVoiceChangedNotifySlot(AudioDeviceType type, int volume);
  void HandleVoiceChangedRequestSlot(int volume);
  void ShowVolumeSettingWidgetSlot(bool show);

private:
  Ui::LocalSpeakerWidget ui_;
  VoiceVolumeSettingWidget *volume_setting_;
};

#endif // LOCAL_SPEAKER_WIDGET_H
