#pragma  once
#include <QtCore/QtCore>
#include <QtCore/QDataStream>

#define SIP_WRAPPER_SERVICE_NAME		"com.dcampus.coolview.sipwrapper"
#define SIP_WRAPPER_OBJECT_PATH		"/com/dcampus/coolview/sipwrapper"


#define SIP_REGISTER_PORT			5060
#define SIP_PROXY_PORT				5060
#define SIP_REALM					"ser.scut.edu.cn"
#define SIP_SIMPLE_SUPPORT			false
#define SIP_REGISTER_TIMEOUT		2940
#define SIP_PUBLISH_TIMEOUT			300
#define SIP_USE_OPTIONS				true
#define SIP_P2P_PRESENCE			false
#define SIP_CHAT_WITHOUT_PRESENCE	false
#define SIP_USE_TYPINGSTATE			true


//configure
#define SIP_PROXY_SERVER_NAME		"ser.scut.edu.cn"
#define SIP_LOCAL_PORT				5060
#define AEC_ENABLE					true
#define HALF_DUPLEX_ENABLE			false
#define PIM_ENABLE					false
#define CALLS_ENCRYPTION_ENBALE		false

//media
#define DEFAULT_VIDEO_WIDTH			352
#define DEFAULT_VIDEO_HEIGHT		288

/**
 * @brief 根据用户名和域名获取标准的sip账号 sip:username@realm
 * @param username 用户名
 * @param realm 域名
 */
inline QString getUniSipURI( const QString& username , const QString& realm )
{
	QString uri = "sip:";
	uri = uri + username + "@" + realm;
	return uri;
}

/**
 * @brief 获取标准的SIP账号 sip:username@realm
 * @param username 用户名
 * @param realm 域名
 */
inline QString getUniSipURI( const QString& sipURI )
{
	if( sipURI.startsWith("sip:") )
		return sipURI;
	QString uri = "sip:"+ sipURI;
	return uri;
}

class SDPInfo
{
public:
	QString		_videoCodec;		//视频编码格式，如果为空说明不存在摄像头
	QString		_audioCodec;		//音频编码格式，如果为空说明不存在麦克风
	int			_videoPort;
	int			_videoWidth;
	int			_videoHeight;
	int			_videoFps;			//视频帧率
  int     _audioRate;
  int     _audioBits;
  int     _audioChannel;

	friend QDataStream& operator>>(QDataStream& in, SDPInfo& data)
	{
		in >> data._videoCodec >> data._videoPort >> data._videoWidth >> data._videoHeight >> data._audioCodec >> data._videoFps
      >> data._audioRate >> data._audioBits >> data._audioChannel;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const SDPInfo& data)
	{
    out << data._videoCodec << data._videoPort << data._videoWidth << data._videoHeight << data._audioCodec << data._videoFps
      << data._audioRate << data._audioBits << data._audioChannel;
		return out;
	}
};

enum SipAccountState
{
	/** Unknown state. */
	SipAccountStateUnknown,

	/** Progress state. */
	SipAccountStateProgress,

	/** Connection to the SIP server failed. */
	SipAccountStateServerError,

	/** Connection to the SIP server failed.due to an bad login or password */
	SipAccountStateAuthenticationError,

	/** Connection to the SIP platform failed due to a timeout. */
	SipAccountStateTimeout,

	/** Successfull connection to the SIP platform. */
	SipAccountStateOk,

	/** Line unregistered. */
	SipAccountStateClosed,

	/** Connection to the SIP server failed.due to the sip account bind to a specific terminal*/
	SipAccountStateNotPermitted

};

enum SipCallState
{
	/** Unknown state. */
	SipCallStateUnknown,

	/** An error occured. */
	SipCallStateError,

	/** Phone call resumed (after holding a call). */
	SipCallStateResumed,

	/** Conversation state. */
	SipCallStateTalking,

	/** Outgoing phone call: dialing. */
	SipCallStateDialing,

	/** Outgoing phone call: ringing. */
	SipCallStateRinging,

	/** Phone call closed (call rejected or call hang up). */
	SipCallStateClosed,

	/** Incoming phone call. */
	SipCallStateIncoming,

	/** Phone call hold. */
	SipCallStateHold,

	/** Phone call missed */
	SipCallStateMissed,

	/** Phone call redirected */
	SipCallStateRedirected,

	SipCallStateRingingStart,
	SipCallStateRingingStop,
};