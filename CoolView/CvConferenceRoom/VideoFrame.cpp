#include "VideoFrame.h"


VideoFrame::VideoFrame( QWidget *parent )
{
	VideoFrame::LinkWin=NULL;//Add By LZY 2010-10-09 初始化关联的窗口指针
	//pixmap = new QPixmap(s);     //delete by huixin.du 2010-09-07
	//this->setMinimumSize(230,160);
	//this->setFrameStyle(QFrame::Box);
	this->setParent(parent);

	//add by huixin.du 2010-09-07
  /*QPalette p;
  p.setBrush(QPalette::Background, QBrush(Qt::black));
  this->setPalette(p);
  this->setAutoFillBackground(true);*/
  setStyleSheet("background-color: black; border-style: outset; border-width: 1px; border-color: grey;");

	//this->setWindowFlags( Qt::X11BypassWindowManagerHint);
	isUsed = false;
	layout.setSpacing(0);
	layout.setContentsMargins(0, 0, 0, 0);   //modified by huixin.du 2010-09-07

	setLayout(&layout);

	//应该对图片指针进行初始化...zhenHua.sun 2010-09-08
	pixmap = NULL;

	_screenIndex = -1;
	_seet = -1;

	_videoWidth = -1;
	_videoHeight = -1;
	_showRealResolution = false;

  _streamIdleFrame = nullptr;
  if (QFile::exists(QApplication::applicationDirPath() + "/resources/StreamIdle.png")) 
  {
    _streamIdleFrame = new QFrame(this);
    _streamIdleFrame->setObjectName("streamIdleFrame");
    _streamIdleFrame->setStyleSheet(
      "QFrame#streamIdleFrame{border-image:url(resources/StreamIdle.png)}");
    _streamIdleFrame->setHidden(false);
  }
}

VideoFrame::~VideoFrame()
{
	if (pixmap!=NULL)
	{
		delete pixmap;
	}
}

void VideoFrame::resizeEvent( QResizeEvent *event )
{
	if( _showRealResolution &&
		LinkWin && _videoWidth>0 && _videoHeight>0 )
	{
    // 显示真实视频大小，仅当窗口大于视频大小时可用。
    // 已废弃
		int frameWidth = event->size().width();
		int frameHeight = event->size().height();
		if( frameWidth>_videoWidth && frameHeight>_videoHeight )
		{
			int xpos = (frameWidth-_videoWidth)/2;
			int ypos = (frameHeight-_videoHeight)/2;
      LinkWin->setGeometry( xpos, ypos , LinkWin->width(),LinkWin->height() );
      LinkWin->getVideoFrame()->setGeometry(0 ,0 , _videoWidth,_videoHeight);
      QRect rect( 0, _videoHeight , _videoWidth , 15 );
		}
	}
  else
  {
      if (_streamIdleFrame) {
          const int margin = 2;
          _streamIdleFrame->setGeometry(margin, margin, 
            event->size().width() - margin, event->size().height() - margin);
      }
  }
}

/**
 * @brief 释放当前视屏窗口
 */
void VideoFrame::Release()
{
	this->isUsed = false;
	if( LinkWin )
	{
		LinkWin->release();
	}
}