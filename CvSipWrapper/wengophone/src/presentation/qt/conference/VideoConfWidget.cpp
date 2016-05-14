#include <QtGui>

#include "VideoConfWidget.h"

VideoConfWidget::VideoConfWidget(QWidget *parent) : QWidget(parent)
{
    QString name = "member";
    //delete mainLayout;
    memberFrame1 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame2 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame3 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame4 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame5 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame6 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame7 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame8 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame9 = new VideoFrame(this,":/image/member.jpg",name);

    memberFrame10 = new VideoFrame(this,":/image/member.jpg",name);


    mainLayout = new QGridLayout();
    size3and3();

    closeAction = new QAction(tr("ÍË³öÈ«ÆÁ"),this);
    connect(closeAction,SIGNAL(triggered()),this,SLOT(closeFullScreen()));

    memberFrame1->addAction(closeAction);
    memberFrame1->setContextMenuPolicy(Qt::ActionsContextMenu);

}




void VideoConfWidget::size1and1()
{
    memberFrame1->setVisible(true);
    memberFrame2->setHidden(true);
    memberFrame3->setHidden(true);
    memberFrame4->setHidden(true);
    memberFrame5->setHidden(true);
    memberFrame6->setHidden(true);
    memberFrame7->setHidden(true);
    memberFrame8->setHidden(true);
    memberFrame9->setHidden(true);
    memberFrame10->setHidden(true);
    setLayout(mainLayout);
}

void VideoConfWidget::size2and2()
{
   // QGridLayout *mainLayout = new QGridLayout(scrollArea);
    memberFrame1->setVisible(true);
    memberFrame2->setVisible(true);
    memberFrame3->setVisible(true);
    memberFrame4->setVisible(true);
    memberFrame5->setHidden(true);
    memberFrame6->setHidden(true);
    memberFrame7->setHidden(true);
    memberFrame8->setHidden(true);
    memberFrame9->setHidden(true);
    memberFrame10->setHidden(true);

    mainLayout->addWidget(memberFrame1,0,0);
    mainLayout->addWidget(memberFrame2,0,1);
    mainLayout->addWidget(memberFrame3,1,0);
    mainLayout->addWidget(memberFrame4,1,1);

    setLayout(mainLayout);
}

void VideoConfWidget::size3and3()
{
    memberFrame1->setVisible(true);
    memberFrame2->setVisible(true);
    memberFrame3->setVisible(true);
    memberFrame4->setVisible(true);
    memberFrame5->setVisible(true);
    memberFrame6->setVisible(true);
    memberFrame7->setVisible(true);
    memberFrame8->setVisible(true);
    memberFrame9->setVisible(true);
    memberFrame10->setHidden(true);
    mainLayout->addWidget(memberFrame9,2,2);
    mainLayout->addWidget(memberFrame1,0,0);
    mainLayout->addWidget(memberFrame2,0,1);
    mainLayout->addWidget(memberFrame3,0,2);
    mainLayout->addWidget(memberFrame4,1,0);
    mainLayout->addWidget(memberFrame5,1,1);
    mainLayout->addWidget(memberFrame6,1,2);
    mainLayout->addWidget(memberFrame7,2,0);
    mainLayout->addWidget(memberFrame8,2,1);


    setLayout(mainLayout);
}

void VideoConfWidget::sizeL()
{
    memberFrame1->setVisible(true);
    memberFrame2->setVisible(true);
    memberFrame3->setVisible(true);
    memberFrame4->setVisible(true);
    memberFrame5->setVisible(true);
    memberFrame6->setVisible(true);
    memberFrame7->setHidden(true);
    memberFrame8->setHidden(true);
    memberFrame9->setHidden(true);
    memberFrame10->setHidden(true);

    mainLayout->addWidget(memberFrame1,0,0,2,2);
    mainLayout->addWidget(memberFrame2,0,2);
    mainLayout->addWidget(memberFrame3,1,2);
    mainLayout->addWidget(memberFrame4,2,0);
    mainLayout->addWidget(memberFrame5,2,1);
    mainLayout->addWidget(memberFrame6,2,2);

    setLayout(mainLayout);
}

void VideoConfWidget::sizeAll()
{
    memberFrame1->setVisible(true);
    memberFrame2->setVisible(true);
    memberFrame3->setVisible(true);
    memberFrame4->setVisible(true);
    memberFrame5->setVisible(true);
    memberFrame6->setVisible(true);
    memberFrame7->setVisible(true);
    memberFrame8->setVisible(true);
    memberFrame9->setVisible(true);
    memberFrame10->setVisible(true);
    mainLayout->addWidget(memberFrame10,3,0);
    mainLayout->addWidget(memberFrame1,0,0);
    mainLayout->addWidget(memberFrame2,0,1);
    mainLayout->addWidget(memberFrame3,0,2);
    mainLayout->addWidget(memberFrame4,1,0);
    mainLayout->addWidget(memberFrame5,1,1);
    mainLayout->addWidget(memberFrame6,1,2);
    mainLayout->addWidget(memberFrame7,2,0);
    mainLayout->addWidget(memberFrame8,2,1);
    mainLayout->addWidget(memberFrame9,2,2);


    setLayout(mainLayout);

}
void VideoConfWidget::sizeFullScreen()
{
    memberFrame1->setWindowFlags(Qt::Dialog);
    memberFrame1->showFullScreen();
  //  memberFrame1->raise();
  //  memberFrame1->activateWindow();
    //QWidget *fullscreen = new QWidget();
   // QFrame *frame = new QFrame(memberFrame1);
   // frame->setParent(fullscreen);
   // fullscreen->raise();
   // fullscreen->activateWindow();
   // fullscreen->showFullScreen();
}
void VideoConfWidget::closeFullScreen()
{
    memberFrame1->setWindowFlags(Qt::Widget);
    memberFrame1->showNormal();

}
