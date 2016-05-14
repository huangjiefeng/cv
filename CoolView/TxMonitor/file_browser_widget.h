#ifndef FILE_BROWSER_WIDGET_H
#define FILE_BROWSER_WIDGET_H

#include <QWidget>
#include "ui_file_browser_widget.h"

class FileBrowserWidget : public QWidget
{
  Q_OBJECT

public:
  FileBrowserWidget(QWidget *parent = 0);
  ~FileBrowserWidget();

  void Initialize();

  void ExpandToPath(const QString &path);

public slots:
  void HandleDirectoryExpandedSlot(const QModelIndex & index);

private:
  Ui::FileBrowserWidget ui;

  QString library_path_;
  QModelIndex index_to_select_;
};

#endif // FILE_BROWSER_WIDGET_H
