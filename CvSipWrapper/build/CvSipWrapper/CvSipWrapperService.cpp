#include "CvSipWrapperService.h"
#include <dbus/sipwrapper/service/CvSipWrapperAdaptor.h>
#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include <PhApiWrapper.h>

#include <dbus/performance/process/common/PerformanceProcessCommon.h>
#include <dbus/performance/process/service/CvPerfProcessAdaptor.h>
#include <dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h>
#include <dbus/performance/monitor/client/CvPerfMonitorIf.h>

#include <log/log.h>

QString uniformSipAddress( QString sipURI )
{
	if( !sipURI.startsWith("sip:") )
	{
		return "sip:" + sipURI;
	}
	return sipURI;
}

CvSipWrapperService::CvSipWrapperService()
{
	new CvSipWrapperAdaptor( this );
	QDBusConnection connec = QDBusConnection::sessionBus();
	bool res = connec.registerService( SIP_WRAPPER_SERVICE_NAME );
	res = connec.registerObject( SIP_WRAPPER_OBJECT_PATH , this);


	_phapiWrapper = PhApiWrapper::getInstance();

	_phapiWrapper->phoneLineStateChangedEvent +=
		boost::bind(&CvSipWrapperService::phoneLineStateChangedEventHandler, this, _1, _2, _3);
	_phapiWrapper->phoneCallStateChangedEvent +=
		boost::bind(&CvSipWrapperService::phoneCallStateChangedEventHandler, this, _1, _2, _3, _4);
	_phapiWrapper->cvMessageReceivedEvent +=
		boost::bind(&CvSipWrapperService::cvMessageReceivedEventHandler,this,_2,_3);

	_phapiWrapper->setSipOptions("sip.register_timeout", QString::number(SIP_REGISTER_TIMEOUT).toStdString() );
	_phapiWrapper->setSipOptions("sip.publish_timeout", QString::number(SIP_PUBLISH_TIMEOUT).toStdString() );
/*	if(SIP_USE_OPTIONS)
		_phapiWrapper->setSipOptions("sip.use_options_request", "true");
	else
		*/
	_phapiWrapper->setSipOptions("sip.use_options_request", "false");

	if(SIP_P2P_PRESENCE)
		_phapiWrapper->setSipOptions("sip.p2p_presence", "true");
	else
		_phapiWrapper->setSipOptions("sip.p2p_presence", "false");

	if(SIP_CHAT_WITHOUT_PRESENCE)
		_phapiWrapper->setSipOptions("sip.chat.without.presence", "true");
	else
		_phapiWrapper->setSipOptions("sip.chat.without.presence", "false");

	if(SIP_USE_TYPINGSTATE)
		_phapiWrapper->setSipOptions("sip.use_typing_state", "true");
	else
		_phapiWrapper->setSipOptions("sip.use_typing_state", "false");


	_perfMonitorProxy = new CvPerfMonitorIf( PERFORMANCE_MONITOR_SERVICE_NAME  , PERFORMANCE_MONITOR_OBJECT_PATH ,
		QDBusConnection::sessionBus() );
	new CvPerfProcessAdaptor( this );
	res = connec.registerService( __GetCvPerfProcessServiceName(CV_SIPWRAPPER_PROC_NAME) );
	res = connec.registerObject( __GetCvPerfProcessObjectPath(CV_SIPWRAPPER_PROC_NAME) , this );
	_state = SIP_IDLE;
	_confURI = "";
	_helloTimer = NULL;
	this->registerProcess();

	_confInviteTimer = new QTimer();
	connect( _confInviteTimer , SIGNAL(timeout()) , this ,SLOT(autoJoinConference()));
}

CvSipWrapperService::~CvSipWrapperService()
{
	_phapiWrapper->phoneLineStateChangedEvent -=
		boost::bind(&CvSipWrapperService::phoneLineStateChangedEventHandler, this, _1, _2, _3);
	_phapiWrapper->phoneCallStateChangedEvent -=
		boost::bind(&CvSipWrapperService::phoneCallStateChangedEventHandler, this, _1, _2, _3, _4);
	_phapiWrapper->cvMessageReceivedEvent -=
		boost::bind(&CvSipWrapperService::cvMessageReceivedEventHandler,this,_2,_3);
}


void CvSipWrapperService::initialSipWrapper()
{
	_phapiWrapper->setSIP( SIP_PROXY_SERVER_NAME , SIP_PROXY_PORT , SIP_LOCAL_PORT );
	_phapiWrapper->setNatType( EnumNatType::NatTypeSymmetric );
	_phapiWrapper->setVideoWidthAndHeight( DEFAULT_VIDEO_WIDTH , DEFAULT_VIDEO_HEIGHT );
	StringList codecList;
	codecList.push_back("SPEEX/16000/16/1");
	_phapiWrapper->setAudioCodecList( codecList );
	_phapiWrapper->enableAEC( AEC_ENABLE );
	_phapiWrapper->enableHalfDuplex( HALF_DUPLEX_ENABLE );
	_phapiWrapper->enablePIM( PIM_ENABLE );
	_phapiWrapper->setCallsEncryption( CALLS_ENCRYPTION_ENBALE );
}



void CvSipWrapperService::acceptSubscription( int sid )
{

}
void CvSipWrapperService::addVirtualLine( const QString &displayName, const QString &username, const QString &identity, const QString &password, const QString &realm, const QString &proxyServer, const QString &registerServer, int address_family )
{
	UserIDMap::iterator it = _userMap.find( username );
	if( it!=_userMap.end() )
	{
		qDebug("Release old user profile");
		//释放原来的登录用户
		this->removeVirtualLine( username , true );
	}

	int lineID = _phapiWrapper->addVirtualLine( 
		qPrintable(displayName), qPrintable(username),qPrintable(identity),
		qPrintable(password),qPrintable(realm) , qPrintable(proxyServer) , qPrintable( registerServer), address_family );
	
	if( lineID<0 )
	{
		SIP_LOG_ERROR( "添加用户信息时发生错误！");
		return;
	}

	_userMap.insert( UserIDMap::value_type(username, lineID) );

	//更新进程映像
	_state = SIP_LOGIN;
	_username = username;
	_password = password;
	_realm = realm;
	_ipFamily = address_family;
	this->sendHelloInfoToMonitor( );

}
void CvSipWrapperService::init( int ipv6_enable )
{
	this->initialSipWrapper();
	_phapiWrapper->init( ipv6_enable );
}

bool CvSipWrapperService::isInitialized()
{

	return _phapiWrapper->isInitialized();
}

void CvSipWrapperService::makeJoinConfInvite( const QString &username, const QString &focusURI, const QString &szContent, const QByteArray &sdpMessage )
{

	_snprintf(__global_msg , sizeof(__global_msg) , "准备登录视频会议[%s]" , qPrintable(focusURI) );
	SIP_LOG_DEBUG(__global_msg);

	UserIDMap::iterator it = _userMap.find( username );
	if( it==_userMap.end() )
	{
		_snprintf(__global_msg , sizeof(__global_msg) , "无法找到用户[%s]" , qPrintable(username) );
		SIP_LOG_ERROR( __global_msg );
		return;
	}
	int lineID = it->second;
	ConferenceIDMap::iterator confIt = _confMap.find( focusURI );
	if( confIt!=_confMap.end() )
	{
		//删除记录信息
		//因为删除会话再申请的话终端端口会更改，因此在这里不关闭会话
		_confMap.erase( confIt );
	}

	if( sdpMessage.size()>0 )
	{
		SDPInfo info;
		QDataStream in(sdpMessage);
		in.setVersion(QDataStream::Qt_4_4);
		in >> info;
		//设置媒体流信息
		_phapiWrapper->setVideoWidthAndHeight( info._videoWidth , info._videoHeight );

		QString videoCodec = info._videoCodec;
		if( info._videoCodec=="H264" )
		{
			//底层的sdp profil里面没有H264的类型，因此在这里转换为H263
			videoCodec = "H263";
		}

    char video_codec[32] = {0};
    char audio_codec[32] = {0};
    strncpy_s(video_codec, qPrintable(videoCodec), sizeof(video_codec));
    strncpy_s(audio_codec, qPrintable(info._audioCodec), sizeof(audio_codec));

    MediaInfo mediaInfo = {0};
    mediaInfo.video_codec = video_codec;
    mediaInfo.video_width = info._videoWidth;
    mediaInfo.video_height = info._videoHeight;
    mediaInfo.video_fps = info._videoFps;
    mediaInfo.audio_codec = audio_codec;
    mediaInfo.audio_rate = info._audioRate;
    mediaInfo.audio_bits = info._audioBits;
    mediaInfo.audio_channel = info._audioChannel;

		int callID = _phapiWrapper->makeJoinConfInvite( lineID , qPrintable(uniformSipAddress(focusURI)) , qPrintable(szContent), mediaInfo);
		if( callID <0 )
		{
			_snprintf( __global_msg , sizeof(__global_msg) , "进入会议[%s]时发生错误:无法创建Dialog", qPrintable(focusURI) );
			SIP_LOG_ERROR( __global_msg );
			return;
		}

		SIP_LOG_ERROR( "成功创建会议Dialog." );

		//记录callID与focusUI的映射
		_confMap.insert( ConferenceIDMap::value_type( focusURI , callID) );

		//更新进程映像
		_state = SIP_MEETING;
		_confURI = focusURI;
		_joinConfContent = szContent;
		_sdpMsg = sdpMessage;
		this->sendHelloInfoToMonitor( );

		//开启会议定时器
		_confInviteTimer->start(3000);
	}else
	{
		SIP_LOG_ERROR("SDP信息为空，无法进入会议" );
	}
}
void CvSipWrapperService::makeInfoCall( const QString &sipURI, const QString &szContent )
{
	ConferenceIDMap::iterator it = _confMap.find( sipURI );
	if( it!=_confMap.end() )
	{
		//if( szContent.indexOf("Online")>0 )
		//{
		//	QDateTime timeNow = QDateTime::currentDateTime();
		//	_snprintf( __global_msg , sizeof(__global_msg) , "Online Info On %s" , qPrintable(timeNow.toString("MM-dd hh:mm:ss")) );
		//	MONITOR_LOG_DEBUG(__global_msg);
		//}
		char content[1024] = {0};
		strcpy( content , qPrintable(szContent) );
		_phapiWrapper->makeInfoCall( it->second , content);
	}else
	{
		_snprintf(__global_msg , sizeof(__global_msg) , "无法找到会议[%s]!\n" , qPrintable(sipURI) );
		SIP_LOG_ERROR(__global_msg);
	}

}


void CvSipWrapperService::registerVirtualLine( const QString &username ,const QString &content)
{
	UserIDMap::iterator it = _userMap.find( username );
	if( it==_userMap.end() )
	{
		_snprintf(__global_msg , sizeof(__global_msg) , "无法找到用户[%s]" , qPrintable(username) );
		SIP_LOG_ERROR(__global_msg);
		return;
	}

	_phapiWrapper->registerVirtualLine( it->second, qPrintable(content) , "application/coolview" );

	_registContent = content;

}

void CvSipWrapperService::rejectSubscription( int sid )
{
}

void CvSipWrapperService::removeVirtualLine( const QString &username, bool force )
{
	UserIDMap::iterator it = _userMap.find( username );
	if( it!=_userMap.end() )
	{
		_phapiWrapper->removeVirtualLine( it->second , force );
		_userMap.erase(it);
	}else
	{
		_snprintf(__global_msg ,sizeof(__global_msg) ,"无法找到用户[%s]", qPrintable(username) );
		SIP_LOG_ERROR(__global_msg);
	}
	
	//更新进程映像
	_state = SIP_IDLE;
	this->sendHelloInfoToMonitor( );
}

void CvSipWrapperService::sendQosPara( const QString &currentUserID, const QString &focusURI, const QString &szContent )
{
	UserIDMap::iterator it = _userMap.find( currentUserID );
	if( it!=_userMap.end() )
	{
		_phapiWrapper->sendQosPara( it->second , qPrintable(getUniSipURI(focusURI)), qPrintable(szContent) );
	}else
	{
		_snprintf(__global_msg ,sizeof(__global_msg) ,"无法找到用户[%s]", qPrintable(currentUserID  ) );
		SIP_LOG_ERROR(__global_msg);
		return;
	}
}

void CvSipWrapperService::sendSipMsg( const QString &currentUserID, const QString &remoteURI, const QString &msgContent )
{
	UserIDMap::iterator it = _userMap.find( currentUserID );
	if( it!=_userMap.end() )
	{
		_phapiWrapper->sendSipMessage( it->second , qPrintable(remoteURI) , qPrintable(msgContent) );
	}else
	{
		qDebug("无法找到用户[%s]！" , qPrintable(currentUserID ) );
		return;
	}
}

void CvSipWrapperService::setNatType( int natType )
{
	_phapiWrapper->setNatType((EnumNatType::NatType)natType);
}

void CvSipWrapperService::setProxy( const QString &address, int port, const QString &login, const QString &password )
{
	_phapiWrapper->setProxy( qPrintable(address) , port , qPrintable(login) , qPrintable(password) );
}

void CvSipWrapperService::setSIP( const QString &server, int serverPort, int localPort )
{
	_phapiWrapper->setSIP(qPrintable(server) , serverPort , localPort );
}

void CvSipWrapperService::setSipOptions( const QString &optname, const QString &optval )
{
	_phapiWrapper->setSipOptions( qPrintable(optname) , qPrintable(optval) );
}

void CvSipWrapperService::setTunnel( const QString &address, int port, bool ssl )
{
	_phapiWrapper->setTunnel( qPrintable(address ) , port , ssl );
}

void CvSipWrapperService::setUaName( const QString &name )
{
	_phapiWrapper->setUaName(qPrintable(name) );
}

void CvSipWrapperService::startConference( const QString &username, const QString &confURI, const QString &szContent )
{
	UserIDMap::iterator it = _userMap.find( username );
	if( it!=_userMap.end() )
	{
		_phapiWrapper->startConference( it->second , qPrintable(confURI), qPrintable(szContent) );
	}else
	{
		qDebug("无法找到用户[%s]", qPrintable(username) );
		return;
	}
}

void CvSipWrapperService::exitConference( const QString &confURI )
{
	ConferenceIDMap::iterator it = _confMap.find( confURI );
	if( it!=_confMap.end() )
	{
		_phapiWrapper->closeCall( it->second );
		_phapiWrapper->exit_conference( qPrintable(confURI) );
		_confMap.erase( it );
	}else
	{
		_snprintf( __global_msg , sizeof(__global_msg ) , "退出会议时无法找到会议[%s]的callID" , qPrintable(confURI) );
		SIP_LOG_ERROR( __global_msg);
	}

	//更新进程映像
	_state = SIP_LOGIN;
	_confURI = "";
	_joinConfContent = "";
	_sdpMsg = "";
	this->sendHelloInfoToMonitor( );

	//停止会议计时器
	_confInviteTimer->stop();
}

void CvSipWrapperService::terminate()
{
	_userMap.clear();
	_phapiWrapper->terminate();
}

void CvSipWrapperService::phoneLineStateChangedEventHandler( SipWrapper & sender, int lineId, EnumPhoneLineState::PhoneLineState state )
{
	UserIDMap::iterator it = _userMap.begin();
	for( ; it!=_userMap.end(); it++ )
	{
		if( it->second == lineId )
		{
			//并发送信号
			switch( state )
			{
			case EnumPhoneLineState::PhoneLineStateUnknown:
				this->phoneLineStateChangedSignal(it->first, SipAccountStateUnknown );
				break;
			case EnumPhoneLineState::PhoneLineStateProgress:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateProgress );
				break;
			case EnumPhoneLineState::PhoneLineStateServerError:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateServerError);
				break;
			case EnumPhoneLineState::PhoneLineStateAuthenticationError:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateAuthenticationError);
				break;
			case EnumPhoneLineState::PhoneLineStateTimeout:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateTimeout );
				break;
			case EnumPhoneLineState::PhoneLineStateOk:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateOk);
				if( _state==SIP_MEETING && _confURI!="" )
				{
					//如果SipWrapper的状态是进行会议，那么根据会议URI恢复SipWrapper的状态
					_snprintf( __global_msg , sizeof(__global_msg) , "恢复Sip登录状态成功，重新登录会议[%s]" , qPrintable(_confURI) );
					SIP_LOG_DEBUG( __global_msg);
					this->makeJoinConfInvite( _username , _confURI , _joinConfContent , _sdpMsg  );
				}
				break;
			case EnumPhoneLineState::PhoneLineStateClosed:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateClosed);
				break;
			case EnumPhoneLineState::PhoneLineStateNotPermitted:
				this->phoneLineStateChangedSignal(it->first , SipAccountStateNotPermitted);
				break;
			}
		}
	}
}

void CvSipWrapperService::phoneCallStateChangedEventHandler( SipWrapper & sender, int callId, EnumPhoneCallState::PhoneCallState state, const std::string & from )
{
	if( state==EnumPhoneCallState::PhoneCallStateIncoming )
	{
		//告诉远程终端正在响铃
		//Sends SIP code 180
		//TODO automatically??
		bool enableVideo = true;
		this->_phapiWrapper->sendRingingNotification(callId, enableVideo);


		//新加入的call是没有对应的callID的，这时应该加入map里面
		QString fromURI = from.c_str();
		if( fromURI.indexOf("unknow")!=-1 )
		{
			_snprintf(__global_msg,sizeof(__global_msg),"来自未知终端[%s]的SIP呼叫",from.c_str() );
			SIP_LOG_ERROR( __global_msg );
		}
		_confMap.insert( ConferenceIDMap::value_type( fromURI , callId) );

		this->phoneCallStateChangedSignal( fromURI , SipCallStateIncoming , fromURI );

		return;
	}

	ConferenceIDMap::iterator it = _confMap.begin();
	for( ; it!=_confMap.end(); it++ )
	{
		if( it->second == callId )
		{
			//并发送信号
			switch( state )
			{
				/** Unknown state. */
			case EnumPhoneCallState::PhoneCallStateUnknown:
				this->phoneCallStateChangedSignal( it->first , SipCallStateUnknown , QString::fromStdString(from) );
				break;
					/** An error occured. */
			case EnumPhoneCallState::PhoneCallStateError:
				this->phoneCallStateChangedSignal( it->first , SipCallStateError, QString::fromStdString(from) );
				break;
					/** Phone call resumed (after holding a call). */
			case EnumPhoneCallState::PhoneCallStateResumed:
				this->phoneCallStateChangedSignal( it->first , SipCallStateResumed , QString::fromStdString(from) );
				break;
					/** Conversation state. */
			case EnumPhoneCallState::PhoneCallStateTalking:
				this->phoneCallStateChangedSignal( it->first , SipCallStateTalking , QString::fromStdString(from) );
				break;
					/** Outgoing phone call: dialing. */
			case EnumPhoneCallState::PhoneCallStateDialing:
				this->phoneCallStateChangedSignal( it->first , SipCallStateDialing , QString::fromStdString(from) );
				break;
					/** Outgoing phone call: ringing. */
			case EnumPhoneCallState::PhoneCallStateRinging:
				this->phoneCallStateChangedSignal( it->first , SipCallStateRinging , QString::fromStdString(from) );
				break;
					/** Phone call closed (call rejected or call hang up). */
			case EnumPhoneCallState::PhoneCallStateClosed:
				this->phoneCallStateChangedSignal( it->first , SipCallStateClosed , QString::fromStdString(from) );
				break;
					/** Phone call hold. */
			case EnumPhoneCallState::PhoneCallStateHold:
				this->phoneCallStateChangedSignal( it->first , SipCallStateHold , QString::fromStdString(from) );
				break;
					/** Phone call missed */
			case EnumPhoneCallState::PhoneCallStateMissed:
				this->phoneCallStateChangedSignal( it->first , SipCallStateMissed , QString::fromStdString(from) );
				break;
					/** Phone call redirected */
			case EnumPhoneCallState::PhoneCallStateRedirected:
				this->phoneCallStateChangedSignal( it->first , SipCallStateRedirected , QString::fromStdString(from) );
				break;
			case EnumPhoneCallState::PhoneCallStateRingingStart:
				this->phoneCallStateChangedSignal( it->first , SipCallStateRingingStart, QString::fromStdString(from) );
				break;
			case EnumPhoneCallState::PhoneCallStateRingingStop:
				this->phoneCallStateChangedSignal( it->first , SipCallStateRingingStop, QString::fromStdString(from) );
				break;
			}
			break;
		}
	}
}

void CvSipWrapperService::cvMessageReceivedEventHandler( const std::string & message,const std::string & from )
{
	QString qMessage = QString::fromStdString(message);
	this->cvMessageReceivedEvent(qMessage , QString::fromStdString(from));

	//如果接收到登录通告，这时停止会议计时器
	if( _confMap.size()>0 && qMessage.indexOf("TerminalList"))
	{
		_confInviteTimer->stop();
	}
}


void CvSipWrapperService::ExitProcess()
{
	this->unregisterProcess();
	QCoreApplication::quit();
}

//----------------------------------监控相关（20110225 zhenHua.sun)--------------------------
void CvSipWrapperService::registerProcess()
{
	//开启定时器，定时向监控进程发送Hello报文
	if( _helloTimer==NULL )
	{
		_helloTimer = new QTimer(this);
	}
	int monitorInterval = 1000;
	_helloTimer->start( monitorInterval/2 );
	connect( _helloTimer , SIGNAL(timeout()) , this ,SLOT(sendHelloInfoToMonitor()));

	MonitoredProcess process;
	process._processID = ::GetCurrentProcessId();
	process._processName = CV_SIPWRAPPER_PROC_NAME;
	process._reportEmail = "zhhua.sun@mail.scut.edu.cn";
	process._processDirPath = QCoreApplication::applicationDirPath();
	qDebug( "Process Dir : %s" , qPrintable(process._processDirPath) );
	process._monitorInterval = monitorInterval;

	process.AddTrigger(Trigger::CPU_USAGE , 12, MonitorTriggerAction::NO_ACTION );		//CPU超过12%时重启并恢复该进程
	process.AddTrigger( Trigger::MEM_USAGE , 30 , MonitorTriggerAction::NO_ACTION );
	process.AddTrigger( Trigger::TIME_TO_LIVE , 3 , MonitorTriggerAction::RECOVER_PROCESS );

	QByteArray output_bytes;
	QDataStream out( &output_bytes , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4);
	out << process;
	_perfMonitorProxy->RegisterProcess( output_bytes );
}

void CvSipWrapperService::unregisterProcess()
{
	_helloTimer->stop();
	disconnect( _helloTimer , SIGNAL(timeout()) , this ,SLOT(sendHelloInfoToMonitor()));
	_perfMonitorProxy->UnregisterProcess( ::GetCurrentProcessId() );
}

void CvSipWrapperService::sendHelloInfoToMonitor()
{
	QByteArray output_bytes;
	QDataStream out( &output_bytes , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4);
	out << this->_state << this->_username << this->_registContent << this->_password << this->_realm << this->_confURI 
		<< this->_ipFamily << this->_joinConfContent << this->_sdpMsg ;

	_perfMonitorProxy->HelloInfo( ::GetCurrentProcessId() , output_bytes );
}

void CvSipWrapperService::Recover( const QByteArray &processImage )
{
	QDataStream in( processImage );
	in.setVersion( QDataStream::Qt_4_4);
	int state;
	in >> state >> _username >>this->_registContent>> _password >> _realm >> _confURI >> _ipFamily >> this->_joinConfContent >> this->_sdpMsg;
	_state = static_cast<SipWrapperState>(state);

	//qDebug( "State:%d , Username:%s , password:%s , Realm:%s , ConfURI:%s , IPFamily:%d" , 
	//	state , qPrintable(_username) , qPrintable(_password) , qPrintable(_realm) , qPrintable(_confURI) , _ipFamily);
	_snprintf( __global_msg , sizeof(__global_msg ) ,  
		"Recover By CvMonitor:\n\t\tState:%d , Username:%s , password:%s , Realm:%s , ConfURI:%s , IPFamily:%d" , 
		state , qPrintable(_username) , qPrintable(_password) , qPrintable(_realm) , qPrintable(_confURI) , _ipFamily);
	SIP_LOG_DEBUG(__global_msg );

	switch( _state )
	{
	case SIP_IDLE:
		return;
	case SIP_LOGIN:
		{
			//说明已经登录进Sip服务器了，利用已有的信息调用相关的注册服务
			QString displayName = _username + "@" + _realm;
			if( _ipFamily==AF_INET6 )
			{
				this->init( true );
			}else
			{
				this->init( false );
			}
			this->addVirtualLine( displayName , _username ,_username ,_password ,_realm , _realm , _realm , _ipFamily );
			this->registerVirtualLine( _username, _registContent );
		}

	case SIP_MEETING:
		{
			//说明已经登录进Sip服务器了，利用已有的信息调用相关的注册服务
			QString displayName = _username + "@" + _realm;
			if( _ipFamily==AF_INET6 )
			{
				this->init( true );
			}else
			{
				this->init( false );
			}
			this->addVirtualLine( displayName , _username ,_username ,_password ,_realm , _realm , _realm , _ipFamily );
			this->registerVirtualLine( _username,_registContent );

			_state = SIP_MEETING;
		}
		return;
	}
}

void CvSipWrapperService::autoJoinConference()
{
	//超时未接收到终端列表，重新发送加入会议的邀请
	_snprintf( __global_msg , sizeof(__global_msg) , "超时未接收到终端列表，重新发出加入会议[%s]的邀请" , qPrintable(_confURI) );
	SIP_LOG_DEBUG( __global_msg);
	if( _confURI!="" && _sdpMsg.size()>0 )
		this->makeJoinConfInvite( _username , _confURI , _joinConfContent , _sdpMsg  );
}

void CvSipWrapperService::makeCall( const QString &username, const QString &memberURI, const QByteArray &sdpMessage )
{
	UserIDMap::iterator it = _userMap.find( username );
	if( it!=_userMap.end() )
	{

		if( sdpMessage.size()>0 )
		{
			SDPInfo info;
			QDataStream in(sdpMessage);
			in.setVersion(QDataStream::Qt_4_4);
			in >> info;

			//设置媒体流信息
			_phapiWrapper->setVideoWidthAndHeight( info._videoWidth , info._videoHeight );

			int callID = _phapiWrapper->makeCall2( it->second, qPrintable(uniformSipAddress(memberURI) ) , true , 
				info._videoWidth,info._videoHeight, info._videoFps,qPrintable(info._videoCodec) );
			if( callID <0 )
			{
				_snprintf( __global_msg , sizeof(__global_msg) , "与[%s]建立会话时发生错误！", qPrintable(memberURI) );
				SIP_LOG_ERROR( __global_msg );
				return;
			}

			SIP_LOG_ERROR( "成功创建会议Dialog." );

			//记录callID与focusUI的映射
			_confMap.insert( ConferenceIDMap::value_type( memberURI , callID) );

		}else
		{
			SIP_LOG_ERROR("SDP信息为空，进行SIP呼叫" );
		}	
	}else
	{
		qDebug("无法找到用户[%s]", qPrintable(username) );
		return;
	}


}

void CvSipWrapperService::acceptCall( const QString &username, const QString &memberURI, bool enableVideo, const QByteArray &sdpMessage )
{

}

void CvSipWrapperService::closeCall( const QString &username, const QString &memberURI )
{

}

void CvSipWrapperService::holdCall( const QString &username, const QString &memberURI )
{

}

void CvSipWrapperService::rejectCall( const QString &username, const QString &memberURI )
{

}

void CvSipWrapperService::resumeCall( const QString &username, const QString &memberURI )
{

}