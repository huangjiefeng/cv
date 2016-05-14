#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QtGui/QtGui>

#include "ui_HelpWidget.h"

class HelpWidget : public QWidget, public Ui::HelpForm
{
public:
    HelpWidget(QWidget *parent);
    ~HelpWidget();
};

#endif // HELPWIDGET_H
