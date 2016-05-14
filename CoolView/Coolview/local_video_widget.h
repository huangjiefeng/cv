#ifndef LOCAL_VIDEO_WIDGET_H
#define LOCAL_VIDEO_WIDGET_H

#include <QWidget>
#include "ui_local_video_widget.h"

#include "terminal_description.h"

class IControllerManager;

class LocalVideoWidget : public QWidget {
  Q_OBJECT

public:
  LocalVideoWidget(QWidget *parent = 0);
  ~LocalVideoWidget();

  void Initilize(IControllerManager *controller_manager);
  void Set(const QString &media_id, const int index);
  void SetDisplay(bool show);

  const QString& media_id() const { return media_id_; }

Q_SIGNALS:
  void StopVideoSignal(const QString &vuri);
  void ConfigCameraSignal(const int index);

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void PerformDrag();

private slots:
  void HandleVideoButtonClickedSlot(bool checked);

private:
  Ui::LocalVideoWidget ui_;
  QString image_url_;
  QString media_id_;
  int camera_index_;

  // the start point of the drag
  QPoint start_pos_;
};

#endif // LOCAL_VIDEO_WIDGET_H
