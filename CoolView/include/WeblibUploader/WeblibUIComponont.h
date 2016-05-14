#ifndef WEBLIB_UI_COMP_H
#define WEBLIB_UI_COMP_H

#include "dllexports.h"

#include <QtWidgets/QWidget>


class WEBLIB_DLLSPEC WeblibUIComponont {
public:
  static QWidget * getSettingWidget();
  static QWidget * getUploadStatusWidget();
  static QWidget * getUploadHistoryWidget();
};

#endif
