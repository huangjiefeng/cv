#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QtGui/QtGui>

class VideoFrame : public QFrame
{
    public:
        VideoFrame(QWidget *parent,QString s);
		~VideoFrame();
        QPixmap *pixmap;
		bool isUsed;
		QGridLayout layout;
};

#endif // VIDEOFRAME_H
