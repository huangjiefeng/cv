#include "stdafx.h"

#include "VideoFrame.h"

VideoFrame::VideoFrame(QWidget *parent,QString s)
{
	int y = QApplication::desktop()->height();
	this->setMinimumSize(230,(y-70)/3);
	
	this->setFrameStyle(QFrame::Box);
    this->setParent(parent);

	//add by huixin.du 2010-09-07
	QPalette p;
	p.setBrush(QPalette::Background,QBrush(Qt::white));
	this->setPalette(p);
	this->setAutoFillBackground(true);

    //this->setWindowFlags( Qt::X11BypassWindowManagerHint);
	isUsed = false;
	layout.setSpacing(0);
	layout.setContentsMargins(2, 2, 2, 2);   //modified by huixin.du 2010-09-07
	
	setLayout(&layout);

	//应该对图片指针进行初始化...zhenHua.sun 2010-09-08
	pixmap = NULL;
}

VideoFrame::~VideoFrame()
{
	if (pixmap!=NULL)
	{
		delete pixmap;
	}
}
