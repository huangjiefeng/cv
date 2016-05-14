/** Coolview 3.0 界面元素
 *  zhenHua.sun 2010-08-10
 */

#ifndef QTCONFVIDEOWIDGET_H
#define QTCONFVIDEOWIDGET_H

#include <vector>

#include "ui_confvideowidget.h"
#include <QtGui/QtGui>
#include <util/SafeDelete.h>
#include <vector>
#include <map>
using namespace std;
class QtWengoPhone;
class CWengoPhone;
class QtConfrenceVideoWindow;
class VideoFrame;

const int VIDEONUM = 9;

class QtConfVideoWidget : public QWidget 
{
 	Q_OBJECT
public:
	QtMeetingPlaceWidget(QtWengoPhone & qtWengoPhone);
	~QtMeetingPlaceWidget(void);

	QtConfrenceVideoWindow *GetUseableWin();
	void closeWindow(const std::string &userId);
	void closeAllWindow();
	//检查是否已经接收了此用户的音视频
	bool checkUserExist(std::string &userId);
	QGridLayout * getWidgetLayout();

	void teleGridLayout(const QString& layout); //Add by: hexianfu 2010-4-16 for the telecontroller

Q_SIGNALS:
	void closeVideoWinSinal(QtConfrenceVideoWindow * videoWin);

private Q_SLOTS:
	void closeVideoWinSlot(QtConfrenceVideoWindow * videoWin);
	void setSizeSlot(QtConfrenceVideoWindow *videoWin,int width,int height);
	void mouseMoveSlot(QtConfrenceVideoWindow *videoWin,const QPoint &point);
	void unReceiveMedia(const QString &userId);
	void sizeAll();
	void size3and3();
	void size2and2();
	void size1and1();
	void sizeL();
	void addVideoWindow();

private:
	Ui::Form *_ui;
	QtConfrenceVideoWindow **videoWindowArray;
	QtWengoPhone &_qtWengoPhone;
	CWengoPhone & _cWengoPhone;
	map<QtConfrenceVideoWindow *,QMdiSubWindow *> video_subwindow_map;
	map<QtConfrenceVideoWindow *,VideoFrame *> video_frame_map;
	//VideoFrame *memberFrames[VIDEONUM];
	vector<VideoFrame *>memberFrames;
	QWidget *widget;
	QGridLayout *mainLayout;
};


#endif // QTCONFVIDEOWIDGET_H
