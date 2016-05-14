#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QtGui/QtGui>
#include "QtConfrenceVideoWindow.h"//Add By LZY 2010-10-09


class VideoFrame : public QFrame
{
	Q_OBJECT
public:
    VideoFrame(QWidget *parent );
	~VideoFrame();

	/**
	 * @brief 释放当前视屏窗口
	 */
	void Release();

protected:
	void resizeEvent( QResizeEvent *event );

public:
  QPixmap *pixmap;
	bool isUsed;
	QGridLayout layout;
	QtConfrenceVideoWindow *LinkWin;//Add By LZY 2010-10-09 添加关联的窗口指针

	int		_screenIndex;		//所在屏幕
	int		_seet;				//所在位置

	int		_videoWidth;		//视频流的实际高度
	int		_videoHeight;		//视频流的实际高度
	bool	_showRealResolution;	//显示真实的视频流大小

  QFrame* _streamIdleFrame; //没有接受东西时显示的图片
};

#endif // VIDEOFRAME_H
