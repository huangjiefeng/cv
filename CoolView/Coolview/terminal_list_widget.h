#ifndef TERMINAL_LIST_WIDGET_H
#define TERMINAL_LIST_WIDGET_H

#include <QHash>
#include <QList>

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QMenu>

#include "ui_terminal_list_widget.h"

class TerminalWidget;
class IControllerManager;

#include "conference_controller_interface.h"

class TerminalListWidget : public QWidget {
  Q_OBJECT

public:
  TerminalListWidget(QWidget *parent = 0);
  ~TerminalListWidget();

  void Initialize(IControllerManager *controller_manager);

Q_SIGNALS:
  // 用于发送停止接收视频的信号
  void StopVideoSignal(const QString &vuri);
  // 用于发送停止接收共享屏幕的信号
  void StopDocumentSignal(const QString &uri);

  void LeaveConferenceSignal();

public Q_SLOTS:
  void SetConferenceTitleSlot(const QString &title);

private Q_SLOTS:
  // 处理从conference controller接收到的信号
  void HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state);

  void HandleTerminalReceivedNotifySlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

  void HandleTerminalCountChangedNotifySlot(
    int total_terminal_count,
    const QString &conference_uri);

  void HandleTerminalUpdateNotifySlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri);

  void HandleReceiveAudioNotifySlot(const QString &uri, bool receive);

  void HandleShowAllTerminalsSlot();
  void HandleShowOnlineTerminalsSlot();
  void HandleShowOfflineTerminalsSlot();

  void ConfirmAndLeaveConferenceSlot();

private:
  enum ShowType {
    kTypeOfShowOnline,
    kTypeOfShowOffline,
    kTypeOfShowInOrder,
  };

  typedef QHash<QString, TerminalWidget*>  WidgetDictionary;
  typedef QList<TerminalWidget*> WidgetList;

  void SetupGui();
  void ClearMenu();

  void ResetTerminalistWidget();
  void RemoveAllTerminals();
  void ShowTerminalWidgetInOrder();
  void ShowTerminalWidgetByOnlineStatus(bool online);
  int CaculateWidgetsPerRow(int widget_width, int layout_width);
  void SortTerminalWidgetToList(WidgetList &widgets);
  void ShowTerminalWidgetByOnlineStatus(ShowType show_type);
  void AddWidgetToLayout(int index, TerminalWidget *widget);
  void ClearLayout();
  void ActivateSpeakControl(bool is_chairman);

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::TerminalListWidget ui_;
  QMenu *menu_;

  IControllerManager *controller_manager_;
  
  int tx_terminal_count_;
  int hd_terminal_count_;
  
  ShowType show_type_;
  int widget_width_;
  int widgets_per_row_;
  int displayed_count_;

  bool has_initialized_;
  bool is_chairman_;

  // key: uri, value: terminal widget
  WidgetDictionary terminal_widget_dict_;
  QString current_conference_uri_;
};

#endif // TERMINAL_LIST_WIDGET_H
