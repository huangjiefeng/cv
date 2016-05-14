#include "stdafx.h"


#include "QtStatusWidget.h"

QtStatusWidget::QtStatusWidget(QWidget *parent) : QWidget(parent)
{
    //phoneButton->hide();

    setAttribute(Qt::WA_DeleteOnClose);
   // connect(phoneButton,SIGNAL(clicked()),this,SLOT(calling()));
}

