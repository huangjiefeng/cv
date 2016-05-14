#ifndef CV_SIP_WRAPPER_H
#define CV_SIP_WRAPPER_H

#include <QtCore/QtCore>
#include <dbus/sipwrapper/common/SipWrapperCommonService.h>

#include <sipwrapper/EnumPhoneCallState.h>
#include <sipwrapper/EnumPhoneLineState.h>


class PhApiWrapper;
class SipWrapper;
class CvPerfMonitorIf;
typedef std::map<QString , int>		ConferenceIDMap;
typedef std::map<QString , int>		UserIDMap;

enum SipWrapperState
{
	SIP_IDLE,		//无任何用户登录
	SIP_LOGIN,		//用户登录
	SIP_MEETING		//用户正在开会
};

class CvSipWrapperService : public QObject
{
	Q_OBJECT

public:
	static CvSipWrapperService* getInstance()
	{
		static CvSipWrapperService  service;
		return &service;
	}

public Q_SLOTS: // METHODS
	void acceptSubscription(int sid);

	/**
	 * @brief 添加sip信息
	 * @param displayName 类似：username@servername
	 * @param username	不包含域名信息的部分
	 * @param identity	与username一致
	 * @param password
	 * @param realm			域名，如ser.scut.edu.cn
	 * @param proxyServer	sip服务器，当前为ser.scut.edu.cn
	 * @param registerServer 注册服务器，当前为ser.scut.edu.cn
	 * @param address_family	IP类型
	 */
	void addVirtualLine(const QString &displayName, const QString &username, const QString &identity, const QString &password, const QString &realm, const QString &proxyServer, const QString &registerServer, int address_family);
	void exitConference(const QString &confURI);
	void init(int ipv6_enable);
	bool isInitialized();
	void makeJoinConfInvite(const QString &username, const QString &focusURI, const QString &szContent, const QByteArray &sdpMessage);

	/**
	 * @brief 主要向指定的会议发送命令，如举手等
	 * @sipURI 对应了会议的focusURI
	 * @szContent 内容
	 */
	void makeInfoCall(const QString &sipURI, const QString &szContent);
	void registerVirtualLine(const QString &username, const QString &content);
	void rejectSubscription(int sid);
	void removeVirtualLine(const QString &username, bool force);
	void sendQosPara(const QString &currentUserID, const QString &focusURI, const QString &szContent);
	void sendSipMsg(const QString &currentUserID, const QString &remoteURI, const QString &msgContent);
	void setNatType(int natType);
	void setProxy(const QString &address, int port, const QString &login, const QString &password);
	void setSIP(const QString &server, int serverPort, int localPort);
	void setSipOptions(const QString &optname, const QString &optval);
	void setTunnel(const QString &address, int port, bool ssl);
	void setUaName(const QString &name);
	void startConference(const QString &username, const QString &confURI, const QString &szContent);
	void terminate();


	/************************************************************************/
	/* SIP call相关的服务接口
	 * @param username SIP账号名称
	 * @param memberURI	远程SIP账号
	 * @sdpMessage 建立会话所需的媒体参数
	 */
	/************************************************************************/
	void acceptCall(const QString &username, const QString &memberURI, bool enableVideo, const QByteArray &sdpMessage);
	void closeCall(const QString &username, const QString &memberURI);
	void holdCall(const QString &username, const QString &memberURI);
	void makeCall(const QString &username, const QString &memberURI, const QByteArray &sdpMessage);
	void rejectCall(const QString &username, const QString &memberURI);
	void resumeCall(const QString &username, const QString &memberURI);

Q_SIGNALS: // SIGNALS
	void cvMessageReceivedEvent(const QString &message, const QString &from);
	void incomingSubscribeSignal(int sid, const QString &from, const QString &evtType);
	void phoneCallStateChangedSignal(const QString &callId, int state, const QString &from);
	void phoneLineStateChangedSignal(const QString &lineId, int state);

public Q_SLOTS: // METHODS
	void ExitProcess();
	void Recover(const QByteArray &processImage);

private Q_SLOTS:
	//-----------------------------------进程监控相关----------------------------------
	//注册监控信息
	void registerProcess( );
	//注销
	void unregisterProcess();
	//发送Hello报文
	void sendHelloInfoToMonitor( );

	//---------------------------------会议相关-----------------------------------------
	void autoJoinConference();

private:
	CvSipWrapperService();
	~CvSipWrapperService();

	void initialSipWrapper();

	void phoneLineStateChangedEventHandler(SipWrapper & sender, int lineId,
		EnumPhoneLineState::PhoneLineState state);

	void phoneCallStateChangedEventHandler(SipWrapper & sender, int callId,
		EnumPhoneCallState::PhoneCallState state, const std::string & from);

	//处理会议URI发送过来的信息
	void cvMessageReceivedEventHandler(const std::string & message,const std::string & from);

	PhApiWrapper*		_phapiWrapper;

	///记录conf focus UI和callID的对应关系
	ConferenceIDMap		_confMap;

	///记录登录用户名和lineID的对应关系
	UserIDMap			_userMap;

	//保留进程的状态信息，以便恢复
	CvPerfMonitorIf*	_perfMonitorProxy;

	//登录会议计时器，在邀请加入会议之后开始计时，如果超时没有收到终端列表，那么需要重新发起邀请
	QTimer*				_confInviteTimer;

	SipWrapperState	_state;			//SipWrapper的状态
	QString			_username;		//用户名
	QString			_registContent;	//注册信息中包含的内容
	QString			_realm;			//域名
	QString			_userSipURI;
	QString			_password;
	QString			_confURI;		//会议URI
	QString			_joinConfContent;	//加入_confURI所需要发送给会议服务器的SIp消息体
	QByteArray		_sdpMsg;		//返回给会议服务器的SDP消息
	int				_ipFamily;		//网络类型，IPv4或IPv6
	QTimer*			_helloTimer;		//定时向监控进程发送hello报文
};


#endif