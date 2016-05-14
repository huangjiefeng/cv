#ifndef DISPLAY_WIDGET_H
#define DISPLAY_WIDGET_H

#include <QWidget>
#include <QList>

#include "ui_display_widget.h"

#include "display_controller_interface.h"
#include "conference_controller_interface.h"

class ScreenWidget;
class SeatWidget;
class IControllerManager;

class DisplayWidget : public QWidget {
  Q_OBJECT

public:
  DisplayWidget(QWidget *parent = nullptr);
  ~DisplayWidget();

  void Initialize(IControllerManager *controller_manager);

public Q_SLOTS:
  void HandleReceiveVideoNotifySlot(
    const QString &vuri,
    int screen_index, 
    int seat_index,
    bool use_small_video);

  void HandleStopVideoNotifySlot(
    const QString &vuri,
    int screen_index,
    int seat_index);

  void HandleReceiveDocumentNotifySlot(
    const QString &uri,
    int screen_index);

  void HandleStopDocumentNotifySlot(
    const QString &uri,
    int screen_index);

  void HandleShowRTCPMessageNotifySlot(
    bool show, 
    int screen_index);

  void HandleScreenLayoutChangedSlot(
    int screen_index,
    IDisplayController::ScreenLayout layout);

  void HandleTerminalReceiveNotifySlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

private:
  typedef QList<ScreenWidget*> ScreenList;
  // key: vuri, value: name
  typedef QHash<QString, QString> TerminalNameDict;

  void ConnectWithScreenWidget(ScreenWidget *screen_widget);
  void ResetDisplayWidget();

private:
  Ui::DisplayWidget ui_;
  int screen_count_;
  ScreenList screens_;
  TerminalNameDict terminal_name_dict_;
};

#endif // DISPLAY_WIDGET_H
