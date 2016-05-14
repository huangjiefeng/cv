/**
 * @brief 用于管理视频窗口
 */

#ifndef MEDIA_WINDOW_MANAGER_H
#define MEDIA_WINDOW_MANAGER_H

#include <QtCore/QtCore>
#include <vector>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
#include <util/desktop/screen_helper.h>

class VideoFrame;
typedef std::vector<VideoFrame*> MediaWindowVector;
typedef struct _RecStatItem RecStatItem;

class QtConfrenceVideoWindow;
class QtMeetingPlaceWidget;
class RtpStatItemInfo;
class QtConferenceRoom;
class TelecontrollerIf;
class QtScreenShare;
class ChannelDispatcherIf;
class KinectIf;
class KinectInfoWidget;
class CvCoreIf;

class MediaWindowManager : public QObject
{
	Q_OBJECT
public:
	static MediaWindowManager* getInstance()
	{
		static MediaWindowManager manager;
		return &manager;
	}


public slots:
	/** 
	 * @brief 根据指定参数返回媒体窗口
	 * @return 如果成功则返回媒体窗口，否则返回NULL
	 */
	VideoFrame* AddMediaWindow( const QString& userID , const QString& displayName , const int screenIndex, const int seet
		,const int videoWidth , const int videoHeight );

	/**
	 * @brief 添加媒体流
	 * @params userID SIP账号
	 * @return NULL
	 */
	void AddScreenMediaRecv( const QString& userID , const QString&ip , const int port , const int screenIndex=-1 );

	/**
	 * @brief 添加主讲人屏幕接收流
	 * @params userID SIP账号
	 * @return NULL
	 */
	void AddMainSpeakerScreenMediaRecv( const QString& userID , const QString&ip , const int port );

	/**
	 * @brief 移除媒体流
	 * @params userID SIP账号
	 * @return NULL
	 */
	void RemoveScreenMediaRecv( const QString& userID,const int screenIndex=-1 );


	/**
	 * @brief 关闭媒体窗口
	 */
	void CloseMediaWindow( const QString& userID );

	/**
	 * @brief 释放所有的媒体窗口
	 */
	void ClearMediaWindow( );

	/// 显示媒体窗口
	void ShowVideoFrame( VideoFrame* frame );

	/// 隐藏媒体窗口
	void HideVideoFrame( VideoFrame* frame );

	/**
	 * @brief 更改接收媒体流的状态
	 * @param frame 媒体流窗口
	 * @param mediaType audio为音频，video为视频
	 * @param enable 如果接收则为True，否则为false
	 */
	void ChangeMediaState( const VideoFrame* frame , const QString& mediaType , const UiMediaState mediaState,bool isAudioSend);


	/// 更改媒体窗口位置
	void ChangeWindowPostion( const QString& userID , const int newScreenIndex, const int newSeet );

	/**
	 * @brief 显示rtcp信息
	 */
	void ShowRtcpMessage( const int screenIndex, const bool show );

	/**
	 * @biref 更新rtcp信息
	 */
	//void UpdateRtcpMessage( const RtpStatItemInfo& stateInfo );

	/**
	 * @brief 更新rtcp信息
	 */
	void UpdateRtcpMessage( const RtpStatItem& stateItem);
    void UpdateRecordMessage( const RecStatItem& stateItem );

	/**
	 * @brief 更改界面布局
	 */
	void ChangeLayout( int screenIndex, int displayModel );

	/**
	 * @brief 控制共享屏幕的界面的按钮，如果当前终端为主讲人则把“屏幕共享”按钮显示出来，否则隐藏它
	 * @param control 如果为true，则显示按钮，否则隐藏按钮
	 */
	void ControlScreenShare( const bool control );

	 /**
	 * @brief 在指定屏幕下打开指定路径下的共享屏幕文件
	 */
	void OpenScreenShareFile(const QString &filePath, int screenIndex);

	 /**
	 * @brief 移除屏幕共享发送进程
	 */
	void RemoveSendShareScreen();

	/**
	 * @brief 获取指定用户名的媒体窗口。需作多线程同步，且应该由调用者管理同步锁
	 * @return 如果失败则返回NULL
	 */
	VideoFrame* FindMediaWindow( const QString& userID );

	/**
	 * @brief 显示器数目发生改变时需要将多余的会议室UI和视频窗口
	 */
	void ScreenCountChanged( int newCount );

	/**
	 * @brief 将界面上的用户通知给遥控器，以便同步
	 * @param controllerIndex，通知事件发回给遥控器的索引值，-1将是所有遥控器都会收到通告
	 */
	void MemberLocationUpdateNotify( const int controllerIndex=-1);

	/**
	 * @brief 将屏幕共享的状态更新信息通知给遥控器，以便同步
	 * @param controllerIndex，通知事件发回给遥控器的索引值，-1将是所有遥控器都会收到通告
	 */
	void ScreenShareStateUpdateNotify( const int controllerIndex=-1);

	/**
	 * @brief 将添加新用户的信息通告给所有终端
	 * @param memberName 远程用户的名称，如果是本地回显则是LOCAL_PLAY_USER_ID
	 * @param screen 屏幕的索引
	 * @param seet 接收远程用户的窗口位置，从1开始算
	 * @param audioEnable 如果在接收音频则设置true
	 * @param videoEnable 如果在接收视频则设置true
	 */
	void MemberLocationAddNotify( const QString& memberName , const int screen , const int seet , const bool audioEnable , const bool videoEnable );

	/**
	 * @brief 将移除用户的信息通告给所有终端
	 */
	void MemberLocationRemoveNotify( const QString& memberName, const int screen , const int seet  );

	/**
	 * @brief 将移除用户的信息通告给所有终端
	 * @memberName 用户的会员名
	 */
	void MemberLocationChangeNotify( const QString& memberName , const int newScreenIndex, const int newSeet );

	/**
	 * @brief 界面布局更新时通知遥控器
	 * @param controllerIndex，通知事件发回给遥控器的索引值，-1将是所有遥控器都会收到通告
	 */
	void LayoutChangedNofity( const int controllerIndex=-1 );

	/**
	 * @brief 将媒体的状态更新信息通知给遥控器，以便同步
	 * @param controllerIndex，通知事件发回给遥控器的索引值，-1将是所有遥控器都会收到通告
	 */
	void MediaStateChangedNotify( const QString&memberName , const QString&mediaType, const QString&mediaState, const int controllerIndex=-1 );

	

	/**
	 * @brief 设置屏幕共享的端口
	 */
	void SetScreenPort( const int screenPort ){ _screenPort = screenPort;}
	int GetScreenPort( ){ return _screenPort; }
	/**
	 * @brief 设置屏幕共享的初始发送IP
	 */
	void SetScreenIP( const QString& screenIP ){ _screenIP = screenIP; }
	QString GetScreenIP() { return _screenIP; }

	void KinectGestureSlot(const QString &to, int type);

  void ControlVideoRecording(const QString userId, int type, QByteArray param);

public:	
	bool	isSendingScreen(){ return _isSendingScreen; }
	void	isSendingScreen(bool sendScreen ) { _isSendingScreen = sendScreen; }


private slots:
	void KinectMessageSlot(int type, const QString &message);
    void updateScreenSlot();

private:
	MediaWindowManager();
	~MediaWindowManager();
	
	//memberFrames会在多线程环境中引用，需要同步锁。调用的地方实在太多，目前只在RTCP相关方法中加了锁
  //多线程之间交互已改用信号槽连接，不再需要锁
	//QMutex _memberFramesLock;
	MediaWindowVector	_memberFrames;

	//QtMeetingPlaceWidget*	_meetingPlaceWidgets[2];

	//存放会议室UI的列表，大小与显示器保持一致
	QVector<QtConferenceRoom*>	_confRoomVector;

	QtScreenShare*				_screenShareWidget;

	///Kinect消息窗口
	KinectInfoWidget*			_kinectInfoWidget;

	//Telecontroller dbus proxy
	TelecontrollerIf*			_teleControlProxy;

	//channeldispatcher dbus proxy
	ChannelDispatcherIf*		_channelDispatcherProxy;

	//kinect dbus proxy
	KinectIf*				_kinectProxy;

  CvCoreIf *_coreProxy;

	//记录屏幕共享的端口，在创建本地回显的时候设置
	//在退出会议的时候清零
	int							_screenPort;
	QString						_screenIP;

	///是否正在发送屏幕共享
	bool						_isSendingScreen;

  //是否正在显示rtcp信息
  bool				_isShowingRtcp;

  QTimer _updateScreenTimer;
};

#endif