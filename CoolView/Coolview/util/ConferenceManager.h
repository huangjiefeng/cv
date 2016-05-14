#ifndef CONFERENCE_MANAGER_H
#define	CONFERENCE_MANAGER_H

#include <QtCore/QtCore>
#include <QTimer>
#include <vector>
using namespace std;
#include "CVMsgParser.h"
#include "util/ini/CVIniConfig.h"

#include <QtCore/QVariantMap>
#include <QtCore/QList>
#include "../profile/RunningProfile.h"
#include <util/report/RtpStat.h>
#include <util/udp/UdpSender.h>

TerminalInfo* FindTerminal( const TerminalVector& terminalList , const string& uri );
void TransformConferenceForQt( const ConfInfo& conf , QVariantMap* confMap );
void TransformConfListForQt( const ConferenceVector& confList , QList<QVariantMap>* confMapList );
void TransformMemberForQt( const MemberInfo& member , QVariantMap* memberMap );
void TransformMemberListForQt( const MemberVector& memberList , QList<QVariantMap>* memberMapList );
void TransformTerminalForQt( const TerminalInfo&terminal , QVariantMap* terminalMap );
void TransformTerminalListForQt( const TerminalVector& terminalList , QList<QVariantMap>* terminalMapList );

typedef struct _RecStatItem RecStatItem;
typedef std::map<const std::string, TerminalVector> TerminalMap;

//记录当前会议状态...zhenHua.sun 2010-09-08
enum ConferenceState
{
	NotInConference,    //没有加入任何会议
	IsInConference,     //加入了一个会议，正在会话中
	ExitingConference,  //用户正在退出会议
	WaitingPermission,  //正在等待控制服务器的许可，未加入会议
	JoinRejected,               //加入会议被拒绝
};


class CvSipWrapperIf;
class CvPerfMonitorIf;
class TxMonitorIf;

class HttpWrapper;

class ConferenceManager : public QObject
{
	Q_OBJECT
public:
    enum JoinConferenceError {
      kOk = 0,
      kNeedPassword = -1,
      kInAConference = -2,
      kIsLeavingConference = -3,
      kConferenceNotExisted = -4,
      kJoinRejected = -5,
      kUnknown = -10,
    };

public:
	ConferenceManager( );
	~ConferenceManager();

	///检查地址是否支持组播，如果为空或者0.0.0.0则不支持组播，否则默认支持组播
	bool checkSupportMulticast( const std::string& ipaddress );

	///获取当前会议的组播地址
	bool getCurrentMulticastAddress( string* ipaddress );

	bool checkIsInConference();

	//判断当前会议是否组播会议
	bool checkCurrentIsMulticastConference();

	//验证一个地址是否支持组播方法，若为空或0.0.0.0则不支持组播，其他地址则默认看成是支持组播
	static bool check_support_multicast(const string& ipaddress);

	bool hostOrNot();

	/************************************************************************/
	/* 会议控制
	*/
	/************************************************************************/

	/**
	 * @brief 开始使用会议终端，同时获得会议列表
	 */
	void startConference( const std::string& username , const std::string& sipServer );

	 /**
     * @brief 生成加入指定会议的消息并发送给会议服务器
     * @param index 会议列表对应的行索引
     * @param 会议URI
     * @return 成功则返回0，返回其他值则表示操作失败：
							-1  需要密码
							-2	 用户已经进入了一个会议，正在开会中（不能自动切换会议）
							-3  用户正在退出会议
							-4  参数错误，比如uri
							-5	 控制服务器不允许加入，可能是带宽原因
							其他	 未知原因
     */
    int makeJoinConfInvite(const std::string& focus_URI);


	/**
	 * @brief 加入指定会议，通常用于QoS控制中
	 */
	int joinConference(const string& curFocusURI);

	/**
	 * @brief 拒绝加入指定会议，通常用于QoS控制
	 */
	int rejectToJoinConference( const string& curFocusURI, const string& reason );

	/**
	 * @brief 完成部分会议功能
	 */
	void makeInfoCall( const std::string& fousURI , const std::string& szContent );

	/**
	 * @brief 发送QoS信息给QoS服务器
	 */
	void sendRTPStatToQoSServer(const RtpStatItem& rtpstat);

	/**
	 * @brief 发送录制信息给服务器 - 暂未实现发送
	 */
	void sendRecStatToServer(const RecStatItem& recstat);

	/**
	 * @brief 完成用户的举手与放手功能
	 * @param handup 如果为true则通知服务器用户举手了，否则为false
	 * @return NULL
	 */
	void MemberHandup( const std::string memberURI , const bool handup );

	///**
	// * @brief 向指定用户请求媒体流
	// * @param localAddr 媒体流的目标地址
	// * @param remoteMemberUniformURI 远程成员的URI，格式为sip:name@realm
	// */
	//void askForStreamMedia( const std::string localAddr , const std::string remoteMemberUniURI  );

	///**
	// * @brief 告知远程用户停止发送视频流到本地
	// * @param localAddr 媒体流的目标地址
	// * @param remoteMemberUniformURI 远程成员的URI，格式为sip:name@realm
	// */
	//void stopRemoteStreamMedia( const std::string localAddr , const std::string remoteMemberUniURI );

	/**
	 * @brief 向指定用户请求媒体流
	 * @param userName 用户名
	 * @param remoteMemberURI 远程成员的URI，格式为name@realm;或本地进程的mediaID
	 * @param screenIndex 屏幕选择
	 * @param seet 视频窗口所在位置，1表示第一个窗口，以此类推
	 */
	void addRecvStreamMedia( const std::string username , const std::string remoteMemberURI
		, const int screenIndex, const int seet);

	/**
	 * @brief 创建本地回显和本地录制进程. 注意:本地流控制以mediaID作为与底层交互的标识符,而非virtualURI
	 */
	void addRecvLocalMedia( const std::string localMediaID, const int screenIndex, const int seet, bool smallvideo = false);


	/**
	 * @brief 告知远程用户停止发送视频流到本地
	 * @param userName 用户名
	 * @param remoteMemberURI 远程成员的URI，格式为name@realm
	 */
	void removeRecvStreamMedia( const std::string username, const std::string remoteMemberURI );

	/**
	 * @brief 停止本地流的接收. 注意:本地流控制以mediaID作为与底层交互的标识符,而非virtualURI
	 */
	void removeRecvLocalMedia( const std::string localMediaID );

	/**
	 * @brief 改变视屏显示位置
	 */
	void changeMemberSeat( const std::string remoteMemberURI , const int screenIndex , const int seat );

	/**
	 * @brief 完成用户的消息发送
	 * @param username 成员名称
	 * @param message 消息
	 */
	void sendTextMessage( const std::string& membername , const std::string& message );

	void exitConference( );

	std::string createJoinConferenceMsg(const string& confUri);

	//设置并保存QoS服务器信息
	void saveQoSServerInfo(const MsgQoSServerInfo & serverInfo);

	/**
	 * @brief 创建本地发送进程
	 */
	void createSendStreamMedia();

	/**
	 * @brief 进入会议后的初始化动作，如果是组播会议，则发送到组播地址
	 */
	void initSendStreamMedia();

	/**
	 * @brief 退出会议后，重置发送进程
	 */
	void resetSendStreamMedia();

	/**
	 * @brief 告知所有参会终端的媒体发送状态，并进行对媒体流进行控制
	 * @param type 媒体类型，0为音频，1为视频
	 * @param enable 如果发送媒体流，则为true，否则为false
	 */
	void controlSendMedia( const int&type , const bool enable );

	/**
	 * @brief 创建发送媒体流
	 */
	void createSendScreenMedia( const QRect wnd );

	/**
	 * @brief 创建接收媒体流
	 */
	void createRecvScreenMedia( const QString& memberURI, const int screenIndex );

	/**
	 * @brief 告知远程用户停止发送屏幕流
	 */
	void removeRecvScreenMedia( const QString& memberURI , const int screenIndex);

	/**
	 * @brief 获取当前的会议模式
	 * @return 发布会返回report,讨论会返回discuss，如果没有进入会议返回空字符串
	 */
	string GetCurrentConferenceMode() const { return _confMode; }

	/**
	 * @brief 当前终端是否主讲人
	 * @return 如果是则返回true，否则返回false
	 */
	bool isSpeaker() const
	{
		if( _speakerURI==RunningProfile::getInstance()->user_uri() )
		{
			return true;
		}else
		{
			return false;
		}
	}

	//从前台接收到handUp/handDown消息
	void HandUpHandler(const bool);
	
	string getConfMode();

	//从前台接收到允许发言消息
	void AllowSpeakHandler(QString uri);

	//从前台接收到禁止发言消息
	void ForbidSpeakHandler(QString uri);

	//从前台接收到向服务器请求真实源地址认证消息
	void TerminalLogin(const std::string& username , const std::string& sipServer,QString ipv4,QString ipv6,QString myVersion,int screenNum);  

	//从前台接收到需要向服务器发送ShareScreenControl命令的消息
	void ScreenShareControlHandler(const bool enable);
	
	//通知遥控器更新主席
	void ChairmanChangeNotify(bool enable);

	//进入会议时，向服务器获取终端列表
	void GetTermialList();
	
	//当终端获取完所有终端列表后，向服务器查询其他相关的会议信息（不如主讲人，正在屏幕共享者）
	void QueryCoferenceInfoBeforeFinishTerminal();

	void RecoveryMediaProcess(QString mediaType,bool isSend,QString removeUserID);

public slots:
	//向服务器发送测试消息
	void StartServerTsetSlot();

	//视频分段，从此时开始一个新的视频片段，主要用于向tx发送分集指令
	void SegmentLocalMedia();

private:
	int computeRequiredBandwidth();

	void sendUdpMessageToQosServer( char * msg, int msgLenth );

	// fot tx get record path
	QString getRecordPath(QString terminalName, const int virtualIndex);

public slots:
	/************************************************************************/
	/* Sip消息处理
	*/
	/************************************************************************/
	void cvMessageReceivedSlot(const QString &message, const QString &from);
	
	//收到TerminalConf命令SIP消息，获得终端配置 by Patrick
	void recvTerminalConfModeMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//收到ConfList命令SIP消息， 获取会议列表
	void recvConfListMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到LoginResult命令SIP消息，登入会议
	void recvLoginResultCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到MemberList命令SIP消息，更新成员列表
	void recvMemberListCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到LoginNotify命令SIP消息，有人加入会议
	void recvLoginNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到LogoutNotify命令SIP消息，有人退出会议
	void recvLogoutNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到Invite命令SIP消息，受到邀请
	void recvInviteMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到ApplyConfResult命令SIP消息，申请即时会议成功
	void recvApplyConfResultMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到PasswordIncorrect命令SIP消息，密码不正确
	void recvPasswordIncorrectMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到Text命令SIP消息，收到文本消息
	void recvTextMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到LoginResult命令SIP消息，被踢出会议
	void recvKickedMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到WhiteBoard命令SIP消息，收到白板消息
	void recvWhiteBoardMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到HandUp命令SIP消息，举手发言
	void recvHandUpMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到HandDown命令SIP消息，取消举手
	void recvHandDownMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到Receiver命令SIP消息，发送媒体流
	//void recvReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到ScreenReceiver命令SIP消息，结束媒体流
	void recvScreenReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到LoginResult命令SIP消息，发送一路屏幕共享流
	void recvUnShareScreenMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到Cancel2Receiver命令SIP消息，结束发送一路屏幕共享流
	//void recvCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到ScreenCancel2Receiver命令SIP消息，取消屏幕共享
	void recvScreenCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到QoS命令SIP消息，qos相关命令
	void recvQoSMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到终端列表
	void recvTerminalListCommandMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri );
	void RecvTerminalListSlot(TerminalVector);

	//------------------新增的媒体流控制信令----------------zhenHua.sun 2010-12-23
	//接收到FocusURI对接收媒体流的回复
	void recvStartMediaReply(CVMsgParser& cvMessageParser , const std::string& fromUri );

	//接收到FocusURI建立媒体流的要求
	void recvStartSendMediaCommand(CVMsgParser& cvMessageParser , const std::string& fromUri );

	//接收到FocusURI对停止媒体流的回复
	void recvStopMediaReply(CVMsgParser& cvMessageParser , const std::string& fromUri );


	//接收到FocusURI停止媒体流的要求
	void recvStopSendMediaCommand(CVMsgParser& cvMessageParser , const std::string& fromUri );

	//接收到FocusURI对特定终端媒体流发送状态的通告
	void recvSendMediaControlCommand( CVMsgParser& cvMessageParser , const std::string& fromUri );

	//-------------------主讲人控制信令----------------------zhenHua.sun 2011-08-17
	///查询主讲人的URI
	void queryMainSpeaker( );

	//查询主席终端的URI
	void queryChairmanTermianl();

	//查询文档共享终端的URI
	void queryCurrentScreenShareTermianl();

	///接收到选择主讲人的信息
	void recvPermitSpeakInfo( CVMsgParser& cvMessageParser , const std::string& fromUri );

	///接收到取消主讲人的信息
	void recvForbidSpeakInfo( CVMsgParser& cvMessageParser, const std::string& fromUri );

	//通过URI取消主讲人 add by zhongbao.liang
	void forbidSpeakByURI(const std::string& Uri);
	
	///接收到真实源地址认证信息
	void recvSaviInfo( CVMsgParser& cvMessageParser, const std::string& fromUri );

	//接收到主席终端消息
	void recvChairmanTerminalHandler( CVMsgParser& cvMessageParser, const std::string& fromUri);
	
	//接收到媒体控制消息
	void recvMediaControlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到主讲人信息
	void recvMainSpeakerTermianlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到主席终端信息
	void recvChairmanTermianlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到当前进行文档共享终端信息
	void recvCurrentScreenShareTerminalInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到打开或关闭主讲人屏幕共享信息
	void recvShareScreenControlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收主讲人的屏幕共享
	void recvMainSpeakerShareScreen(QString & Uri);

	//接收到服务器的Online消息
	void recvOnlineMessageInfo( CVMsgParser& cvMessageParser, const std::string& fromUri);
	
	
	/**
	 * @brief 根据uri更新成员列表或者终端列表的状态信息
	 */
	void _SipAccountLogout( const std::string& sipURI );

	///@brief 向会议服务器发送在线信息
	void onlineInfoSlot();
	
	void TerminalLogOutSlot();
	
	//发送到服务器的测试信息，在超时前没有收到回复
	void severTestTimeOutSlot();

	//在规定超时（三分钟）内，没有收到服务器的心跳回复，初步判断终端与服务器的连接出现问题
	void serverHeartbeatMessageTimeOutSlot();
	

Q_SIGNALS:
	void cvConfListReceivedSignal(  const ConferenceVector& confVector );
	void cvMemberListReceivedSignal( const MemberVector& memberList, const std::string& confTitle);
	void cvTerminalListReceivedSignal( const TerminalVector& terminalList , const std::string& confURI);

	void cvMemberHandUpSignal(QString memberURI );
	void cvMemberHandDownSignal(QString memberURI );

	//告知前台会议的终端总数发生了变化
	void cvTermianlCountChangeNotify(int TotalTermianlNum);

    void cvJoinConferenceSuccessSignal(const std::string &confURI);

	///用户加入会议
	void cvMemberLoginSignal( const MemberInfo& member );
	void cvTerminalLoginSignal( const TerminalVector & member );
	///认证服务器拒绝用户加入会议
	void cvJoinRejectionSignal( const std::string& confURI , const std::string& reason );
	///用户退出会议
    void cvTerminalLogoutSignal(QString sip);

	///会议服务器终止会议
	void cvEndConferenceSignal( );

	void cvConfrenceInviteSignal( const ConfInfo* conference , const std::string&from );
	void cvPasswordIncorrectSignal();
	void cvKickedSignal(); 
	void cvTextMsgReceivedSignal( const std::string& message , const std::string& from );
	void cvWhiteBoardMsgReceivedSignal( const std::string& message , const std::string& from );
	void cvPermitSpeakSignal();
	//
	void cvHostUnShareScreenSignal();

	///远程用户的媒体流发送状态发生变更，RT标识remote terminal
	void cvRTSendMediaStateChanged( const std::string&memberURI , const std::string&type , const bool enable );
	
	///远程用户接收本终端
	void cvRemoteTerminalAskForMediaSignal( const std::string&terminalURI , const std::string&terminalName);

	void RecvSaviSignal(bool res);

	void cvSetMemberWindowToFirstPositionSignal(QString);

	void cvUpgradeTerminalInfoSignal(const TerminalInfo& terminal);

	void cvModifyStopMyAudioState(bool);

	void cvServerTestResultSignal(bool isTimeOut,int interval);


	/////远程用户请求发送视频
	//void cvAskForStreamMediaSignal( const std::string& remoteIP , const int&audioPort , const int&videoPort );

	/////远程用户终止接收视频
	//void cvStopStreamMediaSignal( const std::string& remoteIP , const int&audioPort , const int&videoPort  );

	//void cvSetSendDSCPSignal( const int& dscp );
	
	//Event <void (UserProfile&sender, int callId, HWND&_previewHwnd)> setCallIdEvent;

	//void confRoomListReceivedSignal( const QList<QVariantMap>& confList );


	void receiveRtpStatSignal(const RtpStatItem& rtpstat);
	void receiveRecStatSignal(const RecStatItem &recstat);
	void streamMediaReceiveStartedSignal(QString uri);
	void streamMediaReceiveStoppedSignal(QString uri);

private:
	///用户信息
	CvSipWrapperIf*		_sipWrapperProxy;

	//TOFO:HTTP
	HttpWrapper * _httpWrapper;

	// for Tx
	TxMonitorIf * _txMonitor;

	// for Tx
	QString			_recordingPath;

	//会议列表
	ConferenceVector _confList;

	//当前会议信息
	ConfInfo* _currentConfInfo;     //当前会议信息指针，只有正在开会中(_confState= IsInConference)该指针才有效，其他时候为空

	//当前会议URI，包括正在进入，正在开会中、被拒绝加入、正在退出等状态
	string _focusURI;

	//会议状态
	ConferenceState _confState;

	//与会人员列表
	MemberVector	_memberList;

	//终端列表
	TerminalMap	_terminalList;

	//请求发言队列
	list<MemberInfo*>_requestForSpeakList;
	//发言队列
	list<MemberInfo*>_speakingList;

	//记录发送的地址
	ReceiverInfoManager _receiverInfo;

	//主持人URI
	string _hostURI;

	//主讲人URI
	string _speakerURI;
	
	//当前文档共享终端URI
	string _currentScreenShareUri;

	//主席终端URI
	string _TerminalChairmanURI;
	
	//会议控制模式
	string _confMode;

	//会议title
	string _confTitle;

	//qos服务器uri
	string _qosServerUri;

	//在线信息计时器，一段时间内向会议服务器发送在线信息
	QTimer*	_onlineInfoTimer;

	//qos报告发送器
	UdpSender _qosReportSender;

	//记录当前会议所含终端的数量
	int _totalTermianlNum;
	
	///会议服务器测试计时器
	QTimer*	_serverTestTimer;
	
	//服务器心跳超时计时器
	QTimer*	_serverHeartbeatTimer;
	

	QTime _serverTestCount;


	bool _isServerTest;

	// Tx本地录制自动分割计时器
	QTimer  _localVideoAutoCutTimer;

};

inline bool ConferenceManager::checkSupportMulticast( const std::string& ipaddress )
{
	return ipaddress.size()>0 && ipaddress!="0.0.0.0";
}

inline bool ConferenceManager::checkIsInConference()
{
	if( _confState == IsInConference )
	{
		return true;
	}else
	{
		return false;
	}
}

inline bool ConferenceManager::check_support_multicast( const string& ipaddress )
{
	return ipaddress.length() > 0 && ipaddress != "0.0.0.0";
}

inline bool ConferenceManager::hostOrNot()
{
	if(_confMode == "host" && RunningProfile::getInstance()->username() == _hostURI)
	{
		return true;
	}
	return false;
}



#endif