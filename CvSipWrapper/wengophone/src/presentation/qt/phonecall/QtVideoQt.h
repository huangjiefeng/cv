/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef OWQTVIDEOQT_H
#define OWQTVIDEOQT_H

#include "QtVideo.h"

#include <QtGui/QImage>
/************************************************************************///Add by zhuang 09-01-07
#include "QtWin.h"

class QtWin;
/************************************************************************/

class QtVideoQtEventManager;

class QSize;
class QImage;

/************************************************************************///Add by zhuang 08-12-29
/*                                                                      */
struct MediaCallbk
{
	HWND preview_window;
	HWND video_hwnd;
	int preview_width;
	int preview_height;
	int width;
	int height;
	int recvgraphid;
	void (*ResizeCbk)(HWND hwnd,int width,int height,int recvgraphid,int fullscreenflag);

};
typedef struct MediaCallbk media_cbk_s;
/************************************************************************/

class QtVideoQt : public QtVideo {
	Q_OBJECT
public:

	QtVideoQt(QWidget * parent);

	~QtVideoQt();

	void showImage(piximage * remoteVideoFrame, piximage * localVideoFrame);
	/************************************************************************///Add by zhuang 08-12-26
	/* 获得窗口句柄                                                         *///modify by zhuang 09-01-06
	void getWinId(void *video_hwnd,HWND preview_win);
	/************************************************************************/

	QSize getFrameSize() const;

	bool isInitialized() const {
		return true;
	}

	bool isFullScreen() const {
		return _fullScreen;
	}

	void fullScreen();

	void unFullScreen();
	
	static void convertPixImageToQImage(piximage* pixImage, const QSize& size, QImage* image);

Q_SIGNALS:

	void toggleFlipVideoImageSignal();

protected:
	virtual bool eventFilter(QObject*, QEvent*);

private Q_SLOTS:
	void toggleAttachDetach();

	void paintEvent();

	//Add by zhuang 08-12-29
	//加入Resize事件
	void resizeEvent();

	void fullScreenButtonClicked();

	void flipWebcamButtonClicked();

private:
	/** Current frame to show inside the video window. */
	QImage _image;

	bool _fullScreen;

	bool _detached;

	bool _showLocalVideo;

	/** Store desktop resolution before switching to fullscreen */
	QSize _normalDesktopResolution;
	QSize _fullScreenDesktopResolution;

	/** 
	 * Recompute size to compensate for the ratio differences between normal
	 * and fullscreen resolutions
	 */
	QSize compensateForFullScreenRatio(QSize);
	/************************************************************************///Add by zhuang 08-12-29
	/*                                                                      */
	media_cbk_s qt_media_callbak;
	QtWin *atachDetachWin;
	QString _userName;
public:
	void closeWin();
	void detach_resize();
	void detach_fullscreen();
	//设置通话的用户名
	void setUserName(QString userName);
	QString& getUserName();
	/************************************************************************/
};


#endif	//OWQTVIDEOQT_H
