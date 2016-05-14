/**
 * Coolview 3.0 界面更新  
 * zhenHua.sun 2010-08-10
 */

#ifndef QTSTATUSWIDGET_H
#define QTSTATUSWIDGET_H

#include "ui_StatusWidget.h"

class QtStatusWidget :  public QWidget, public Ui::statusWidget
{
    Q_OBJECT

public:
     QtStatusWidget(QWidget *parent = 0);

 /*   private slots:
        void calling();*/
};

#endif // QTSTATUSWIDGET_H
