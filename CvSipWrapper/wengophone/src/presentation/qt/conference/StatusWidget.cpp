#include <QtGui>
#include "StatusWidget.h"

StatusWidget::StatusWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    //phoneButton->hide();

    setAttribute(Qt::WA_DeleteOnClose);
   // connect(phoneButton,SIGNAL(clicked()),this,SLOT(calling()));
}

/*void StatusWidget::calling()
{
    SipCallingDialog *dial = new SipCallingDialog();
    dial->show();
}*/
