#include "web_page_widget.h"

#include "cwebpage/cwebpage.h"

WebPageWidget::WebPageWidget(QWidget *parent)
    : QWidget(parent), display_page_(false) {
  ui_.setupUi(this);
}

WebPageWidget::~WebPageWidget() {
  ClosePage();
}

void WebPageWidget::OpenPage( const QString &url ) {
  display_page_ = true;
  EmbedBrowserObject((HWND)winId());
  DisplayHTMLPage((HWND)winId(), (TCHAR*)url.toLocal8Bit().data());
}

void WebPageWidget::ClosePage() {
  if (display_page_) {
    display_page_ = false;
    UnEmbedBrowserObject((HWND)winId());
  }
}


