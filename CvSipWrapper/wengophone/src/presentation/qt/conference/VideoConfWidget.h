#ifndef VIDEOCONFWIDGET_H
#define VIDEOCONFWIDGET_H

#include <QFrame>
#include <QAction>
#include <QGridLayout>

#include "VideoFrame.h"

///Coolview3.0 ½çÃæÔªËØ  zhenHua.sun 2010-08-10
class VideoConfWidget : public QWidget
{
    Q_OBJECT

    public:
        VideoConfWidget(QWidget *parent = 0);

        VideoFrame *memberFrame1;
        VideoFrame *memberFrame2;
        VideoFrame *memberFrame3;
        VideoFrame *memberFrame4;
        VideoFrame *memberFrame5;
        VideoFrame *memberFrame6;
        VideoFrame *memberFrame7;
        VideoFrame *memberFrame8;
        VideoFrame *memberFrame9;
        VideoFrame *memberFrame10;

        QGridLayout *mainLayout;

    private:
        QAction *closeAction;

    public slots:
        void size1and1();
        void size2and2();
        void size3and3();
        void sizeL();
        void sizeFullScreen();
        void sizeAll();
        void closeFullScreen();
   // private:
     //   QPainter *paint;

};
#endif // VIDEOCONFWIDGET_H
