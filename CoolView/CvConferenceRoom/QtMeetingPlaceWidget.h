#pragma once

#include "ui_MeetingPlaceWidget.h"
#include <QtGui/QtGui>
#include <vector>
#include <map>
#include <util/report/RtpStat.h>
using namespace std;


#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
#include "qtconferenceroom.h"

class VideoFrame;
class QtMeetingPlaceWidget:public QWidget
{
	friend class ConferenceRoomService;
	friend class MediaWindowManager;

	Q_OBJECT
public:
	QtMeetingPlaceWidget( QtConferenceRoom* confRoom);
	~QtMeetingPlaceWidget(void);

public slots:
	///显示媒体窗口
	void showVideoWindow( VideoFrame* frame );

	///隐藏媒体窗口
	void hideVideoWindow( VideoFrame* frame );

	



//
//	QtConfrenceVideoWindow *GetUseableWin();
//	void closeWindow(const std::string &userId);
//	void closeAllWindow();
//	//检查是否已经接收了此用户的音视频
//	bool checkUserExist(std::string &userId);
//	QGridLayout * getWidgetLayout();
//
//	/**
//	 * @brief 是否显示RTCP信息
//	 */
//	bool isShowingRtcpMsg(){ return showRtcpMessageEnbaled; }
//
//	/**
//	 * @brief 设置视频会议的回显窗口
//	 * @return 返回分配的回显窗口句柄
//	 */
//	HWND setPreviewWnd();
//
//	/**
//	 * @brief 获取回显窗口
//	 */
//	HWND getPreviewWnd();
//
//	/**
//	 * @brief 获取userID在视频窗口中的位置
//	 * @userID	用户名
//	 * @return 不存在则返回-1
//	 */
//	int	findUserSeet( QString usreID );
//
//	/**
//	 * @brief 往座位上加入指定用户
//	 * @param seet 座位编号
//	 * @param userID 用户名
//	 * @return 如果失败则返回NULL
//	 */
//	QtConfrenceVideoWindow* assignUserSeet( const int& seet , const QString userID );
//
//
//	void teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器视频布局变更
//
//	ConferenceRoomService*	getConferenceRoomService( ) { return _qtConferenceRoom->getConferenceRoomService(); }
//
//Q_SIGNALS:
//	void closeVideoWinSinal(QtConfrenceVideoWindow * videoWin);
//
//	void windowResizeSignal( QString userID , int wnd );
//
//	/**
//	 * @brief 提示rtcpInfoWidget更新传输统计信息
//	 */
//	void updateTranStateSignal( const TranStatInfo& tranStateInfo );
//
//private Q_SLOTS:
//	void closeVideoWinSlot(QtConfrenceVideoWindow * videoWin);
//
//	void setSizeSlot(QtConfrenceVideoWindow *videoWin,int width,int height);
//	void mouseMoveSlot(QtConfrenceVideoWindow *videoWin,const QPoint &point);
//	void unReceiveMedia(QString userId);//Remove Param Const By LZY 2010-09-28
//
	void sizeAll();
	void size3And3();
	void size2And2();
	void size1And1();
	void size1And1Real();
	void sizeL3And3();
	void sizeL4And4();
	void sizeL5And5();
	void sizeAUTO();
    void size4And4();

	void sizeL1_20();
    void sizeTop1And2();
//
//	void addVideoWindow();
//
//	/**
//	 * @brief 更新RTCP信息
//	 */
//	void updateRtcpEventHandler( const RtpStatItemInfo &rtpstatInfo );
//
//
//	/**
//	 * @brief 更新传输统计信息
//	 */
//	void updateTranStateEventHandler( const TranStatInfo &tranStatInfo );
//
//
//	/**
//	 * @brief RTCP信息显示开关
//	 * @param visiable true则显示，否则不显示
//	 */
//	void showRtcpMessage( bool visiable );
//
	/**
	 * @brief 更新显示模式
	 * @param displayModel 显示模式
	 */
	void updateDisplay( ConfRoomDisplayModel displayModel );
//
//	void teleSetLayoutTypeCommand(QString LayoutType); //Modify By LZY 2010-09-27 处理SetLayoutType动作Slot
//
//	///@brief 将某个用户的视频窗口全屏
//	void FullScreenShow( QString userID );
//
//
//	///窗口调整
//	void windowResizeSlot( QString userID , int wnd );

private:

	Ui::MeetingPlaceWidget *_ui;

	//受管理的窗口列表
	std::vector<VideoFrame*>	memberFrames;

	QWidget *widget;
	QGridLayout *mainLayout;

	///@brief 是否在视频窗口显示rtcp信息
	bool showRtcpMessageEnbaled;

	///记录当前显示模式
	ConfRoomDisplayModel _displayModel;

	///父组件
	QtConferenceRoom*		_qtConferenceRoom;


};

