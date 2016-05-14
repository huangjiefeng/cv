#include "control_panel_booter.h"

#include <QBitmap>
#include <QMouseEvent>
#include <QPainter>

ControlPanelBooter::ControlPanelBooter(QWidget *parent, QWidget &control_panel)
    : QWidget(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint),
      control_panel_(control_panel),
      move_action_(false) {
  ui_.setupUi(this);
  pixmap_.load(":/image/Control-panel.png");
  //pixmap_ = pixmap_.scaled(64, 64);
  resize(pixmap_.size());
  setMask(pixmap_.mask());
}

ControlPanelBooter::~ControlPanelBooter() {

}

void ControlPanelBooter::mousePressEvent( QMouseEvent *event )
{
  if (event->button() == Qt::LeftButton) {
    offset_pos_ = event->globalPos() - pos();
    move_action_ = false;
    event->accept();
  }
}

void ControlPanelBooter::mouseMoveEvent( QMouseEvent *event )
{
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - offset_pos_);
    move_action_ = true;
    event->accept();
  }
}

void ControlPanelBooter::mouseReleaseEvent( QMouseEvent *event)
{
  if (!move_action_ && event->button() == Qt::LeftButton) {
    if (control_panel_.isVisible()) {
      control_panel_.hide();
    } else {
      control_panel_.showNormal();
    }
  }
}

void ControlPanelBooter::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, pixmap_);
}

void ControlPanelBooter::enterEvent(QEvent *)
{
  setCursor(Qt::PointingHandCursor);
}

void ControlPanelBooter::leaveEvent(QEvent *)
{
  setCursor(Qt::ArrowCursor);
}
