#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include "ui_StatusWidget.h"

class StatusWidget :  public QWidget, public Ui::statusWidget
{
    Q_OBJECT

    public:
        StatusWidget(QWidget *parent = 0);
 /*   private slots:
        void calling();*/
};

#endif // STATUSWIDGET_H
