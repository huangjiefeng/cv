#ifndef UPLOAD_STATUS_WIDGET_H
#define UPLOAD_STATUS_WIDGET_H

#include <QWidget>
#include <QLabel>
#include "ui_upload_status_widget.h"

#include <memory>

class ILogger;
class RecordSqliteDB;
class WorkerService;
enum ItemOperate;
struct UploadItem;

class UploadStatusWidget : public QWidget
{
  Q_OBJECT

public:
  UploadStatusWidget(QWidget *parent = 0);
  ~UploadStatusWidget();

  void Initialize();


protected slots:
  void HandleShowErrorMessageSlot(const QString &info);
  void HandleItemStateChangedSlot(
    ItemOperate oper, const UploadItem &item);

private:
  Ui::UploadStatusWidget ui;

  QWidget * weblib_upload_widget_;
  QLabel * status_msg_label_;

  WorkerService * upload_worker_service_;
  std::shared_ptr<ILogger> logger_;
  std::shared_ptr<RecordSqliteDB> db_;
  QString file_uploaded_mark_;
};

#endif // UPLOAD_STATUS_WIDGET_H
