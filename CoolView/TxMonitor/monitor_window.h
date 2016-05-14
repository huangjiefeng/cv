#ifndef MONITOR_WINDOW_H
#define MONITOR_WINDOW_H

#include <QWidget>
#include "ui_monitor_window.h"

class QTextBrowser;
class ComponentManager;
class UploadStatusWidget;
struct NetworkInfo;
class DBBrowserWidget;
class FileBrowserWidget;

class MonitorWindow : public QWidget
{
  Q_OBJECT

public:
  MonitorWindow(QWidget *parent = 0);
  ~MonitorWindow();

  void Initialize(ComponentManager *component_manager);

public slots:
  void HandleWeblibErrorMessageSlot(const QString &msg);
  void HandleNetworkScheduleNotifySlot(
    const NetworkInfo &info,
    quint64 weblib_speed_limit);
  void HandleFreeDiskSpaceChangeNotifySlot(quint64 space);
  void HandleViewSelectedSlot(int vid, const QString &file);

private:
  void InitialUploadTab();
  void InitialLocalStorageTab();

  void UpdateGlobalStatusMsg();

private:
  Ui::MonitorWindow ui;

  QTabWidget *upload_tab_widget_;
  UploadStatusWidget *upload_status_widget_;
  QWidget *upload_history_widget_;
  QTextBrowser *upload_log_widget_;

  QTabWidget *local_storage_tab_widget_;
  DBBrowserWidget *db_browser_widget_;
  FileBrowserWidget *file_browser_widget_;

  // global msg
  QString network_schedule_msg_;
  QString disk_space_msg_;

  QString library_path_;
};

#endif // MONITOR_WINDOW_H
