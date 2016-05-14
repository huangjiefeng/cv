#ifndef DEVICE_WIDGET_H
#define DEVICE_WIDGET_H

#include <QWidget>
#include "ui_device_widget.h"

class DeviceWidget : public QWidget {
  Q_OBJECT

public:
  enum DeviceType {
    kVideoDevice = 0,
    kSpeakerDevice,
    kMicDevice,
    kVGADevice,
    kUnknown,
  };

public:
  DeviceWidget(QWidget *parent = 0,
               const QString &device_name = "",
               DeviceType type = kUnknown);

  ~DeviceWidget();

  void SetDeviceName(const QString &device_name);
  void SetDeviceIcon(QString url);

Q_SIGNALS:
  void ShowDeviceListWidgetRequestSignal();

private:
  void InitializeIcon();

private:
  Ui::DeviceWidget ui_;
  DeviceType type_;
};

#endif // DEVICE_WIDGET_H
