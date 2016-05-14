#ifndef LOCAL_MIC_WIDGET_H
#define LOCAL_MIC_WIDGET_H

#include <QWidget>
#include "ui_local_mic_widget.h"

#include "media_description.h"

class IDisplayController;
class VoiceVolumeSettingWidget;

class LocalMicWidget : public QWidget {
  Q_OBJECT

public:
  LocalMicWidget(QWidget *parent = 0);
  ~LocalMicWidget();
  
  void Initialize(IDisplayController *controller);

Q_SIGNALS:
  void ChangeVoiceVolumeSignal(AudioDeviceType type, int volume);

private Q_SLOTS:
  void HandleVoiceChangedNotifySlot(AudioDeviceType type, int volume);
  void HandleVoiceChangedRequestSlot(int volume);
  void ShowVolumeSettingWidgetSlot(bool show);

private:
  Ui::LocalMicWidget ui_;
  VoiceVolumeSettingWidget *volume_setting_;
};

#endif // LOCAL_MIC_WIDGET_H
