#ifndef WEB_PAGE_WIDGET_H
#define WEB_PAGE_WIDGET_H

#include <QWidget>
#include "ui_web_page_widget.h"

class WebPageWidget : public QWidget {
  Q_OBJECT

public:
  WebPageWidget(QWidget *parent = 0);
  ~WebPageWidget();

  void OpenPage(const QString &url);
  void ClosePage();

private:
  Ui::WebPageWidget ui_;
  bool display_page_;
};

#endif // WEB_PAGE_WIDGET_H
