#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <dbus/msdx/common/MsdxServiceCommon.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

//rtcp report reader
#include <util/report/RTCPReportReader.h>

class ConferenceRoomIf;
class ChannelDispatcherIf;
class ScreenMediaInfo;
class ConferenceManager;
class CvStreamedMediaIf; 

class IConferenceController;

#include <map>
#include <vector>
using namespace std;

typedef std::map<QString , CvStreamedMediaIf*>   StreamedMediaProxyMap;
#define PORTINCREMENT 200;

class RecvMemberPosition
{
public:
	RecvMemberPosition()
	{
		_memberURI = "";
		_screenIndex = -1;
		_seat = -1;
		_smallVideo = false;
		_rtcpVideoReportReader = new RTCPReportReader(RTCPReportType::ReportType_video);
		_rtcpAudioReportReader = new RTCPReportReader(RTCPReportType::ReportType_audio);
	}

	~RecvMemberPosition()
	{
		_memberURI = "";
		_screenIndex = -1;
		_seat = -1;
		delete _rtcpVideoReportReader;
		_rtcpVideoReportReader = NULL;
		delete _rtcpAudioReportReader;
		_rtcpAudioReportReader = NULL;
	}

	QString				_memberURI;
	int					_screenIndex;
	int					_seat;
	bool                _smallVideo;
	RTCPReportReader*	_rtcpVideoReportReader;
	RTCPReportReader*	_rtcpAudioReportReader;
};

typedef vector<RecvMemberPosition*>		RecvMemberVector;

class MediaManager: public QObject
{
	Q_OBJECT
public:
	static MediaManager* getInstance()
	{
		static MediaManager manager;
		return &manager;
	}

	void setConferenceManager( ConferenceManager* confManager )
	{
		this->_conferenceManager = confManager;
	}

    void setConferenceController(IConferenceController *controller) {
      conference_controller_ = controller;
    }

	//添加接收用户的位置信息
	void AddRecvMemberPosition( const QString& memberURI , const int screenIndex , const int seet, const bool smallVideo );
	//删除接收用户信息的位置信息
	void RemoveRecvMemberPosition( const QString& memberURI );
	//根据用户URI获取位置信息
	int GetRecvMemberPositionFromURI( const QString&memberURI , int *screenIndex , int *seet, bool *smallVideo );

	/**
	 * @param memberName URI中@左边的字符，且将#替换为_
	 */
	int GetRecvMemberPositionFromName( const QString &memberName , int *screenIndex , int *seet, bool *smallVideo );
	/**
	 * @brief 清空所有的用户位置信息
	 */
	void clearRecvMemberPosition( );

	//创建接收媒体流
	void AddRecvMedia( RecvGraphInfo& info );

	//移除接收媒体流
	void RemoveRecvMedia( const QString& memberURI );

	//更改用户的显示位置
	void ChangeMemberSeat( const QString&memberURI , const int newScreenIndex , const int newSeet );

	/** 
	 * @brief 创建发送音视频流
	 * @param ip 媒体流的目标地址
	 * @param audioPort 媒体流的音频目标端口
	 * @param videoPort 媒体流的视频目标端口
	 * @param smallVideo 是否使用小流
	 * @param screenPort 屏幕端口，无用
	 * @param audioEnable 是否发送音频
	 * @param virtualNo  虚拟终端编号
	 */
	void CreateSendMedia( int virtualIndex, const QString& ip , const int&audioPort , const int&videoPort , const bool smallVideo, const int&screenPort,const bool audioEnable=true);


	//Tx分段控制
	void SegmentMedia( const QString& userId, int job_id );

	/**
	 * @brief 添加发送目标
	 * @param ip 媒体流的目标地址
	 * @param audioPort 媒体流的音频目标端口
	 * @param videoPort 媒体流的视频目标端口
	 * @param smallVideo 是否采用小流
	 * @param screenPort 屏幕端口，无用
	 * @param audioEnable 是否发送音频
	 */
	void AddSendMedia( int virtualIndex, const QString& ip , const int&audioPort , const int&videoPort , const bool smallVideo, const int&screenPort,const bool audioEnable=true );

	//移除发送媒体流
	void RemoveSendMedia( int virtualIndex, const QString&ip , const int&audioPort , const int&videoPort, const bool isSmallVideo = false );

	//设置dscp
	void SetDSCP(const int vdieoDscp , const int audioDscp );

	//显示RTCP
	void showRtcpMsg( bool visiable , int screenIndex=-1 );

	//退出会议，停止所有媒体流
	void ExitConference( );

	//销毁所有媒体流
	void ClearStreamedMedia();

	//改变界面布局
	void ChangeLayout(  const ConfRoomDisplayModel displayModel , int screenIndex=-1 );

    //控制视频流的发送
    void ControlVideoSend( bool videoSend );

	//控制视频流的发送
	void ControlVideoSend(int virtualIndex, bool videoSend );

	//控制音频流的发送
	void ControlAudioSend( bool audioSend );

	//控制视频流的接收
	void ControlVideoRecv( const QString memberURI, const bool videoRecv );

	//控制音频流的接收
	void ControlAudioRecv( const QString memberURI, const bool audioRecv );

	

	/**
	 * @brief 根据远程终端的SIP消息改变对应接收媒体流的音视频状态。
	 * @param memberURI 远程终端的SipURI
	 * @param mediaType	媒体类型
	 * @param state 媒体状态，true为正常，false为禁止
	 * @param isAudioSend 音频状态，true为本地发送音频，false为其他终端音频
	 */
	void ChangeMediaState( const QString&memberURI , const QString&mediaType , const UiMediaState state ,bool isAudioSend);

	/**
	 * @brief 控制共享屏幕的界面的按钮，如果当前终端为主讲人则把“屏幕共享”按钮显示出来，否则隐藏它
	 * @param control 如果为true，则显示按钮，否则隐藏按钮
	 */
	void ControlScreenShare( const bool control );

	/**
	 * @brief 根据指定参数创建发送屏幕流
	 * @param port 屏幕流占用的端口
	 * @dst 目标地址，如果不是组播则设置为"0.0.0.0"
	 * @wnd 共享的屏幕窗口
	 */
	void CreateSendScreenMedia( const int port ,const QString&dst, const QRect wnd );

	/**
	 * @brief 根据指定参数创建接收屏幕流
	 * @param port 屏幕流占用的端口
	 * @dst 目标地址，如果不是组播则设置为"0.0.0.0"
	 * @wnd 共享的屏幕窗口
	 */
	void CreateRecvScreenMedia( const QString&memberURI , const int port ,const QString&src, const int wnd, const int screenIndex );

	/**
	 * @brief 添加屏幕流的发送地址
	 * @param port 屏幕占用的端口
	 * @param dst 目标地址,
	 */
	void AddSendScreenMediaDst( const int port , const QString&dst );

	/**
	 * @brief 删除屏幕流的发送地址
	 * @param port 屏幕占用的端口
	 * @param dst 目标地址,
	 */
	void RemoveSendScreenMediaDst( const int port , const QString&dst );

	/**
	 * @brief 关闭接收屏幕流
	 * @param info 屏幕流信息
	 */
	void RemoveRecvScreenMedia( const QString&remoteURI="");


	/**
	 * @brief 是否启用音视频流的自动同步偏差纠错, 该设置将对所有接收流起作用。
	 *			  实现原理是检测接收缓冲已满，则自动报警，清空缓冲，重新设置音视频流的同步
	 * @param enable true=启用 false=禁用
	 */
	void EnableRecvAutoResync(bool enable);

    //是否启用自适应码率控制 -- 发送端
    // 该方法的实现只在媒体流发送进程CvStreamedMedia_send已经启动了才有效
    void EnableAdaptiveCodeRate(bool enable);


	//获取本地回显的屏幕
	int GetLocalPreviewScreen() const { return _localPreviewScreen; }

	/**
	 * @brief 根据指定参数创建主讲人的接收屏幕流
	 * @param port 屏幕流占用的端口
	 * @dst 目标地址，如果不是组播则设置为"0.0.0.0"
	 * @wnd 共享的屏幕窗口
	 */
	void CreateMainSpeakerRecvScreenMedia( const QString&memberURI , const int port ,const QString&src, const int wnd);

	 /**
	 * @brief 根据移除屏幕共享发送进程
	 */
	void RemoveScreenShareSend();

	 /**
	 * @brief 执行PPT页面更改操作
	 * @param  type 页面操作类型：是向前翻页还是向后翻页
	 */
	void PPTControlCommand(int type);

	//恢复接收媒体流
	void RecoveryRecvMedia( RecvGraphInfo& info );

Q_SIGNALS:
    void NotifySendMediaStateChanged(const QString &media_id, int type);
    void NotifyRecvMediaStateChanged(const QString &user_id, int type);

private slots:
	void rtcpReportTimerTimeout();
    void HandleNofityChannelStateChanged(const QString &channel_id, 
        const QString &user_id, const QString &channel_type, int channel_state);

private:
	MediaManager();
	~MediaManager();
	//保存多路发送streammedia的代理对象
	StreamedMediaProxyMap _sendMediaProxyMap;

	ConferenceRoomIf*		_confRoomProxy;
	ChannelDispatcherIf*	_channelDispatcherProxy;

	RecvMemberVector	_recvMemberVector;

	//是否在发送音视频
	bool			_isSendingMedia;

	//记录当前接收的屏幕流的远程用户URI
	QString			_remoteScreenMember;

	//记录DSCP值
	int _videoDscp;   //视频流dscp
	int _audioDscp;  //音频流dscp


	///读取RTCP报告的计时器
	QTimer*			_rtcpReportTimer;

	///会议管理对象
	ConferenceManager*	_conferenceManager;

    // 会议控制对象
    IConferenceController *conference_controller_;

	//本地回显窗口所在的位置
	int				_localPreviewScreen;
	//int				_localPreviewSeet;

	// Tx_temp - 防止误按频繁分割
	QDateTime       _lastFileCutTime;
  bool _isModelTx;
};
#endif