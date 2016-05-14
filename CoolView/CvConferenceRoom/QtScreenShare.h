#pragma once
#include "ui_screenShare.h"
#include <QtGui/QtGui>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
class QtConferenceRoom;
class CvStreamedMediaIf;
class ChannelDispatcherIf;
class QtScreenWidget;
class QtScreenShare:public QWidget
{
	Q_OBJECT
public:
	static enum PPTControlType
	{
		///未知
		PPTControl_Unknow,

		///下一张PPT
		PPTControl_Next,

		///上一张PPT
		PPTControl_Pre,

		///关闭PPT
		PPTControl_Close,
	};

    enum SharedMode{
      kVideoAudioMode,
      kDocumentMode,
      kAirPlayMode,
    };

	QtScreenShare( QtConferenceRoom* room );
	~QtScreenShare(void);
	void stopScreenGraph();
	void cvHostUnShareScreenEventHandler();
	void openFile(QString filename , int screenIndex);

	QtConferenceRoom* ConfRoom() const { return _confRoom; }
	void ConfRoom(QtConferenceRoom* val) { _confRoom = val; }

	/**
	 * @brief 返回屏幕窗口的句柄值
	 * @return 如果不成功则返回0，否则返回句柄值
	 */
	int getScreenWindow( );

	QString UserID() const { return _userID; }
	void UserID(QString val) { _userID = val; }

	void setMediaState( const UiMediaState state );

	bool getScreenShareState() {return isScreenShare ;}

	/**
	 * @brief 控制PPT的播放
	 * @param type 控制类型，参照PPTControlType枚举类型
	 */
	void controlPPT( const PPTControlType type );
	
	void killPPT();

	/**
	 * @brief 控制共享屏幕的界面的按钮，如果当前终端为主讲人则把“屏幕共享”按钮显示出来，否则隐藏它
	 * @param control 如果为true，则显示按钮，否则隐藏按钮
	 */
	void ControlScreenShare( const bool control );

    void SetSharedMode(SharedMode mode);
    SharedMode GetSharedMode() const { return _sharedMode; }

Q_SIGNALS:
	void cvHostUnShareScreenSinal();
public Q_SLOTS:
	void openFile();
	void closeFile();
	/**
	 * @brief 创建屏幕流发送屏幕截图
	 * @params screenIndex 截图所在的屏幕索引
	 * @params left 截图左边界在screenIndex屏幕下的左边界
	 * @params top 截图左边界在screenIndex屏幕下的上边界
	 * @params right 截图左边界在screenIndex屏幕下的右边界
	 * @params bottom 截图左边界在screenIndex屏幕下的下边界
	 */
	void sendSpecificScreen( const int left=0,const int top=0,const int right=0, const int bottom=0 );

	void recvScreen( const bool enable);
	void cvHostUnShareScreenSlot();

	/** 
	 * @brief 清空屏幕共享的所有状态
	 */
	void release( );

	/**
	 * @brief 打开ppt等辅助文件的进程退出的时候触发该SLOT
	 */
	void pptProcessFinishSlot(int,QProcess::ExitStatus );

protected:
	virtual bool eventFilter(QObject*, QEvent*);
	virtual void resizeEvent ( QResizeEvent * event );
private:
    void changeToDocumentMode();
    void changeToAirPlayMode();
    void changeToVideoAudioMode();
    void notifyAllScreenSharedState(bool isShared);
    bool activateAirServer();

private:
	Ui::ScreenShare *_ui;
	QtScreenWidget *widget;
	bool firstOpenFile;
	bool isSendScreen;
	bool isRecvScreen;
	bool isScreenShare;

	QtConferenceRoom*	_confRoom;

	CvStreamedMediaIf*		_streamProxy;
	ChannelDispatcherIf*	_channelProxy;

	QString			_userID;

	///用于打开PPT的进程指针
	QProcess*	_pptProcess;
	///PPT的窗口指针
	HWND		_pptWnd;
	///幻灯片指针
	HWND		_slideWnd;


    // AirServer的窗口句柄
    HWND _airServerWnd;

    SharedMode _sharedMode;
};
