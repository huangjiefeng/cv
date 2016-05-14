#ifndef SEAT_WIDGET_H
#define SEAT_WIDGET_H

#include <QWidget>
#include "ui_seat_widget.h"

class SeatWidget : public QWidget {
  Q_OBJECT
public:
  // TODO: 从display controller那里获取参数
  static const int FIRST_SEAT = 0;
  static const int SHARED_SCREEN_SEAT = 16;

public:
  SeatWidget(QWidget *parent, int seat_index);
  ~SeatWidget();

  // 将该座位设置为共享屏幕
  void Set(const QString &uri, const QString &name);
  // 将该座位设置为视频显示
  void Set(const QString &vuri, const QString &name, bool use_small_video);
  // 重置该座位
  void Reset();

  void SetSeatTitle(const QString &title);

  const QString& terminal_vuri() const { return terminal_vuri_; }

  int seat_index() const { return seat_index_; }

Q_SIGNALS:
  void ReceiveVideoSignal(QString vuri, int seat_index, bool use_small_video);
  void StopVideoSignal(QString vuri, int seat_index);
  void ReceiveDocumentSignal(QString uri, int seat_index);
  void StopDocumentSignal(QString uri, int seat_index);

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void PerformDrag();
  
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
  void resizeEvent(QResizeEvent *event);
  void AdjustUI(int width, int height);

  void AddVideoActionToMenu(bool use_small_video);

private:
  Ui::SeatWidget ui_;

  int seat_index_;

  QMenu *menu_;

  QString image_url_;
  QString terminal_vuri_;
  QString terminal_uri_;
  bool use_small_video_;
  bool is_show_;
  bool is_choosen_;

  // the start point of the drag
  QPoint start_pos_;
};

#endif // SEAT_WIDGET_H
