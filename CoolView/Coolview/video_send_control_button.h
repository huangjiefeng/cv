#ifndef LOCAL_VIDEO_WIDGET_H
#define LOCAL_VIDEO_WIDGET_H

#include <QWidget>
#include <QPushButton>

#include "terminal_description.h"

class IDisplayController;

class VideoSendControlButton : public QPushButton {
  Q_OBJECT

public:
  VideoSendControlButton(QWidget *parent = 0);
  ~VideoSendControlButton();

  void Initilize(IDisplayController *controller);
  void Set(const QString &media_id, const int index);
  void SetDisplay(bool show);
  void SetSettingMode(bool is_setting_mode);

  const QString& media_id() const { return media_id_; }

Q_SIGNALS:
  void RequestControlVideoSendSignal(int virtual_index, bool send);
  void ConfigCameraSignal(int virtual_index);

private Q_SLOTS:
  void HandleVideoButtonClickedSlot(bool checked);
  void HandleControlVideoSendNotifySlot(int virtual_index, bool send);

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void PerformDrag();

  void SetSendStatus(bool send);

private:
  //Ui::LocalVideoWidget ui_;
  QString image_url_;
  QString media_id_;
  int camera_index_;
  bool send_video_;
  bool is_setting_mode_;

  // the start point of the drag
  QPoint start_pos_;
};

#endif // LOCAL_VIDEO_WIDGET_H
