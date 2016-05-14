/** Coolview 3.0 ½çÃæÔªËØ
 *  zhenHua.sun 2010-08-10
 */
#include "stdafx.h"
#include "qtconfvideowidget.h"
#include <QtGui\QWidget>
//#include "confvideowindow.h"


QtConfVideoWidget::QtConfVideoWidget(QWidget *parent)
{
    setupUi(this);

    member1 = new QFrame(this);
    member2 = new QFrame(this);
    member3 = new QFrame(this);
    member4 = new QFrame(this);
    member5 = new QFrame(this);
    member6 = new QFrame(this);
    member7 = new QFrame(this);
    member8 = new QFrame(this);
    member9 = new QFrame(this);

    size1and1();
}

void QtConfVideoWidget::size1and1()
{
    QGridLayout *mainLayout = new QGridLayout;
    member2.setHidden(true);
    member3.setHidden(true);
    member4.setHidden(true);
    member5.setHidden(true);
    member6.setHidden(true);
    member7.setHidden(true);
    member8.setHidden(true);
    member9.setHidden(true);
    mainLayout->addWidget(member1,0,0,Qt::AlignCenter);
}

void QtConfVideoWidget::size2and2()
{
    QGridLayout *mainLayout = new QGridLayout;
    member2.setVisible(true);
    member3.setVisible(true);
    member4.setVisible(true);
    member5.setHidden(true);
    member6.setHidden(true);
    member7.setHidden(true);
    member8.setHidden(true);
    member9.setHidden(true);
    mainLayout->addWidget(member1,0,0,Qt::AlignCenter);
    mainLayout->addWidget(member2,0,1,Qt::AlignCenter);
    mainLayout->addWidget(member3,1,0,Qt::AlignCenter);
    mainLayout->addWidget(member4,1,1,Qt::AlignCenter);
}

void QtConfVideoWidget::size3and3()
{
    QGridLayout *mainLayout = new QGridLayout;
    member2.setVisible(true);
    member3.setVisible(true);
    member4.setVisible(true);
    member5.setVisible(true);
    member6.setVisible(true);
    member7.setVisible(true);
    member8.setVisible(true);
    member9.setVisible(true);
    mainLayout->addWidget(member1,0,0,Qt::AlignCenter);
    mainLayout->addWidget(member2,0,1,Qt::AlignCenter);
    mainLayout->addWidget(member3,0,2,Qt::AlignCenter);
    mainLayout->addWidget(member4,1,0,Qt::AlignCenter);
    mainLayout->addWidget(member5,1,1,Qt::AlignCenter);
    mainLayout->addWidget(member6,1,2,Qt::AlignCenter);
    mainLayout->addWidget(member7,2,0,Qt::AlignCenter);
    mainLayout->addWidget(member8,2,1,Qt::AlignCenter);
    mainLayout->addWidget(member9,2,2,Qt::AlignCenter);
}

void QtConfVideoWidget::sizeL()
{
    QGridLayout *mainLayout = new QGridLayout;
    member2.setVisible(true);
    member3.setVisible(true);
    member4.setVisible(true);
    member5.setVisible(true);
    member6.setVisible(true);
    member7.setHidden(true);
    member8.setHidden(true);
    member9.setHidden(true);
    mainLayout->addWidget((member1,0,0,2,2,Qt::AlignCenter);
    mainLayout->addWidget(member2,0,2,Qt::AlignCenter);
    mainLayout->addWidget(member3,1,2,Qt::AlignCenter);
    mainLayout->addWidget(member4,2,0,Qt::AlignCenter);
    mainLayout->addWidget(member5,2,1,Qt::AlignCenter);
    mainLayout->addWidget(member6,2,2,Qt::AlignCenter);
}
