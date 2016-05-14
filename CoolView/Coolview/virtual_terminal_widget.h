#ifndef VIRTUAL_TERMINAL_WIDGET_H
#define VIRTUAL_TERMINAL_WIDGET_H

#include <QWidget>

class QPoint;
class QMouseEvent;

#include "ui_virtual_terminal_widget.h"
#include "terminal_description.h"

class IControllerManager;

class VirtualTerminalWidget : public QWidget {
  Q_OBJECT

public:
  VirtualTerminalWidget(QWidget *parent = 0);
  ~VirtualTerminalWidget();

  void Initialize(
    ConstTerminalPointer terminal,
    IControllerManager *controller_manager);

  void Update(ConstTerminalPointer terminal);
  
  void SetShowStatus(bool show);
  bool IsShow()const;

  const QString& uri() const { return uri_; }
  int virtual_index() const { return virtual_index_; }
  const QString& virtual_uri() const { return virtual_uri_; }
  const QString& name() const { return name_; }

Q_SIGNALS:
  void StopReceivedVideoSignal(const QString &vuri);
  void CutFileRequestSignal(const QString &vuri);

private Q_SLOTS:
  void StopReceivedVideoSlot();
  void CutFileSlot();

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void PerformDrag();

  void CreateMenuAndActions();

private:
  Ui::VirtualTerminalWidget ui_;

  QMenu *menu_;
  QString image_url_;
  QString uri_;
  int virtual_index_;
  QString virtual_uri_;
  QString name_;

  // the start point of the drag
  QPoint start_pos_;
};

#endif // VIRTUAL_TERMINAL_WIDGET_H
