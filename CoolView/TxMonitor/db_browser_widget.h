#ifndef LOCAL_BROWSER_WIDGET_H
#define LOCAL_BROWSER_WIDGET_H

#include <QWidget>
#include "ui_db_browser_widget.h"

#include <memory>

class RecordSqliteDB;

class DBBrowserWidget : public QWidget
{
  Q_OBJECT

public:
  DBBrowserWidget(QWidget *parent = 0);
  ~DBBrowserWidget();

  void Initialize();

Q_SIGNALS:
  void NotifyViewSelectedSignal(int vid, const QString &file);

public slots:
  void HandleConferenceRefreshButtonClickedSlot();
  void HandleViewClickedSlot(const QModelIndex &index);

private:
  void RefreshConference();
  void GetEpisodeDates(const QModelIndex &index);
  void GetEpisodesByDate(const QModelIndex &index);
  void GetViewByEpisode(const QModelIndex &index);
  void SendFileItemDoubleClickedSignal(const QModelIndex &index);
  void ClearConferenceView();
  void ClearEpisodeView();
  void ClearFileView();

private:
  Ui::DBBrowserWidget ui;

  std::shared_ptr<RecordSqliteDB> db_;
  QString rec_path_prefix_;
};

#endif // LOCAL_BROWSER_WIDGET_H
