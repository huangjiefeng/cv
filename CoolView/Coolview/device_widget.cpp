#include "device_widget.h"

#include <QMouseEvent>

DeviceWidget::DeviceWidget(QWidget *parent /*= 0*/,
                           const QString &device_name /*= ""*/, 
                           DeviceType type /*= kUnknown*/ )
    : QWidget(parent), type_(type) {
  ui_.setupUi(this);
  SetDeviceName(device_name);
  //setToolTip(device_name);
  InitializeIcon();

  connect(ui_.deviceButton, SIGNAL(clicked()),
          this, SIGNAL(ShowDeviceListWidgetRequestSignal()));
}

DeviceWidget::~DeviceWidget() {

}

void DeviceWidget::SetDeviceName( const QString &device_name ) {
  int index = device_name.indexOf(QLatin1Char('('));
  QString name = index != -1 ? device_name.left(index) : device_name;
  ui_.deviceNameLabel->setText(name);
}

void DeviceWidget::InitializeIcon() {
  QString icon_url;
  switch (type_) {
  case DeviceWidget::kVideoDevice:
    icon_url = ":/image/camera-web.png";
    break;
  case DeviceWidget::kSpeakerDevice:
    icon_url = ":/image/speaking.png";
    break;
  case DeviceWidget::kMicDevice:
    icon_url = ":/image/mic.png";
    break;
  case DeviceWidget::kVGADevice:
    icon_url = ":/image/camera-web.png";
    break;
  case DeviceWidget::kUnknown:
    icon_url = ":/image/computer_help.png";
    break;
  default:
    break;
  }
  SetDeviceIcon(icon_url);
}

void DeviceWidget::SetDeviceIcon(QString url) {
  //QPixmap image(url);
  //QPixmap scale_image = image.scaled(ui_.iconLabel->size());
  //ui_.iconLabel->setPixmap(scale_image);
  ui_.deviceButton->setIcon(QIcon(url));
}

