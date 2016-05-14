#ifndef CONTROL_PANEL_BOOTER_H
#define CONTROL_PANEL_BOOTER_H

#include <QWidget>
#include <QPixmap>

#include "ui_control_panel_booter.h"

class ControlPanelBooter : public QWidget
{
  Q_OBJECT

public:
  ControlPanelBooter(QWidget *parent, QWidget &control_panel);
  ~ControlPanelBooter();


private:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent( QMouseEvent * event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void paintEvent(QPaintEvent *event);
  virtual void enterEvent(QEvent *event);
  virtual void leaveEvent(QEvent *event);

private:
  Ui::ControlPanelBooter ui_;
  QWidget &control_panel_;
  QPixmap pixmap_;
  QPoint offset_pos_;
  bool move_action_;
};

#endif // CONTROL_PANEL_BOOTER_H
