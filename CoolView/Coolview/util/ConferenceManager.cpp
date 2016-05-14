#include "ConferenceManager.h"

#include <WinSock2.h>

#include <QtCore/QVariantMap>
#include <QApplication>

#include <util/ProcessManager.h>

#include "MediaManager.h"
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
#include <dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h>
#include <dbus/sipwrapper/client/CvSipWrapperIf.h>
#include <dbus/sipwrapper/common/SipWrapperCommonService.h>

#include <dbus/txMonitor/client/TxMonitorIf.h>
#include <dbus/txMonitor/common/TxMonitorServiceCommon.h>

#include "../profile/RunningProfile.h"
#include "../config/RunningConfig.h"

#include <CoolviewCommon.h>
#include <log/Log.h>
#include <msdx/config.h>

#include "STcpSender.h"
#include "util\ini\CVIniConfig.h"
#include "QoSDef.h"
#include "NetworkUtil.h"
#include "HttpWrapper.h"

#include "util/ini/TxConfig.h"
#include "util/report/RecordStat.h"

char __globalMessege[512];

#define DEFAULT_IP_ADDRESS  "0.0.0.0"        //默认的一个地址，用来表示单播会议的IP地址或启动graph用，实际不发送数据
//#define DEFAULT_AUDIO_PORT 10000     //默认的音频端口
//#define DEFAULT_VIDEO_PORT  10002        //默认的视频端口
#define LOCALPRE_VIDEO_PORT_BASE 8000
#define LOCALPRE_SMALL_PORT_BASE 8100
#define LOCALREC_AUDIO_PORT_BASE 9000
#define LOCALREC_VIDEO_PORT_BASE 9100

#define MEDIA_CONTROL_ENABLE    //如果定义了该宏，则媒体流的控制需要和会议控制服务器进行协商

#define PPT_TERMINAL_URI	"test_t1@sip.ccnl.scut.edu.cn"
#define MULTICAST_URI       "multicast@sip.ccnl.scut.edu.cn"
#define MULTICAST_SMALL_URI "multicasts@sip.ccnl.scut.edu.cn"


ConfInfo* conf_find_by_uri(const ConferenceVector& confList, const string& uri)
{
    for(ConferenceVector::const_iterator iter = confList.begin(); iter != confList.end(); ++iter)
    {
        if(uri == (*iter)->URI)
            return (*iter);
    }
    return  NULL;
}

//根据uri在成员列表查找指定的会议信息，返回其指针，找不到则返回NULL
MemberInfo* member_find_by_uri(const vector<MemberInfo*>& memberList, const string& uri)
{
    for(vector<MemberInfo*>::const_iterator iter = memberList.begin(); iter != memberList.end(); ++iter)
    {
        if(uri == (*iter)->MemberURI)
            return (*iter);
    }
    return  NULL;
}

//删除列表中包括指定ip的元素
void receiver_remove_by_ip( vector<ReceiverInfo>& receiverList, const string& ip)
{
    vector<ReceiverInfo>::iterator iter;
    for( iter = receiverList.begin(); iter != receiverList.end(); iter++ )
    {
        if(ip == (*iter).recvIP)
        {
            //删除元素，指向下一个迭代器
            iter = receiverList.erase(iter);
            break;
        }
    }
}

void getMediaParamFromSdp( int* videoWidth , int* videoHeight , int* videoFps, char* audioCodec , const char* sdp )
{
    //char audioPort[10];
    //char videoPort[10];
    //char width_str[10];
    //char height_str[10];

    //char* temp, * temp2;
    //if(!sdp || strcmp("", sdp) == 0)
    //    return;

    //temp = strstr((char*)sdp, "m=audio");
    //if( temp == NULL )
    //{
    //    strcpy( audioCodec , "" );
    //}
    //else
    //{
    //    strcpy( audioCodec , "SPEEX" );
    //}

    //temp = strstr((char*)sdp, "m=video");
    //if( temp == NULL )
    //{
    //    //不存在视频设备
    //    *videoWidth = 0;
    //    *videoHeight = 0;
    //}
    //else
    //{
    //    temp = strstr(temp, "CUSTOM");
    //    if(temp)
    //    {
    //        while(*temp != '\r' && *temp != '\n' && *temp != '\0' && *temp != '=')
    //        {
    //            temp++;
    //        }
    //        temp++;
    //        temp2 = temp;
    //        while(*temp2 != '\r' && *temp2 != '\n' && *temp2 != '\0' && *temp2 != ',')
    //        {
    //            temp2++;
    //        }
    //        temp2++;
    //        strncpy(width_str, temp, temp2 - 1 - temp);

    //        temp = temp2;
    //        while(*temp2 != '\r' && *temp2 != '\n' && *temp2 != '\0' && *temp2 != ' ' && *temp2 != ',')
    //        {
    //            temp2++;
    //        }
    //        strncpy(height_str, temp, temp2 - temp);
    //        *videoWidth = atoi(width_str);
    //        *videoHeight = atoi(height_str);
    //    }
    //}

    QString sdpMsg = QString::fromUtf8( sdp );
    if( sdpMsg.contains("m=video") )
    {
        int index1 = sdpMsg.indexOf("CUSTOM=");
        int index2 = sdpMsg.indexOf(" MaxBR");
        QString temp1 = sdpMsg.remove(0, index1 + 7);
        temp1 = temp1.left(index2 - index1 - 7);
        int index3 = temp1.indexOf(",");
        QString swidth = temp1.left(index3);
        QString sheight = temp1.remove(0, index3 + 1);

        *videoWidth = swidth.toInt();
        *videoHeight = sheight.toInt();

        index1 = sdpMsg.indexOf("FPS=");
        if( index1 > 0 )
        {
            temp1 = sdpMsg.remove( 0 , index1 + 4 );
            index2 = sdpMsg.indexOf( " " );
            QString sFps = temp1.left( index2 );
            *videoFps = sFps.toInt();
        }
        else
        {
            *videoFps = 30;
        }
    }
    else
    {
        *videoHeight = 0;
        *videoWidth = 0;
        *videoFps = 0;
    }

    strcpy(audioCodec , "SPEEX");

}

TerminalInfo* terminal_find_by_port( TerminalMap& terminalList , const int& port, int mediaType )
{
	if (mediaType!=0 && mediaType!=1)
		return NULL;
    for( TerminalMap::const_iterator it = terminalList.begin() ; it != terminalList.end() ; ++it )
    {
		for (int i = 0; i < it->second.size(); ++i)
		{
			if (mediaType == 0 && (it->second[i])->videoPort == port)
				return (it->second[i]);
			else if (mediaType == 1 && (it->second[i])->audioPort == port)
				return (it->second[i]);
		}
    }
    return NULL;
}

TerminalInfo* FindTerminal( TerminalMap& terminalMap , string uri )
{
	// 如果输入virtualUri，获得真实uri
	string virtualUri = uri;
	if (uri.find("#") != -1)
	{
		uri = uri.substr(uri.find("#") + 1);
	}
	if (terminalMap.find(uri) == terminalMap.end())
	{
		return NULL;
	}
	// 如果输入的是真实uri
	if (uri == virtualUri)
	{
		return terminalMap[uri][0];
	}
	// 否则返回对应的虚拟终端
    for( TerminalVector::const_iterator it = terminalMap[uri].begin() ; it != terminalMap[uri].end() ; ++it )
    {
        if( virtualUri == (*it)->virtualUri )
        {
            return (*it);
        }
    }
    return NULL;
}

/*
 * 输入一般性的uri字符串，解析出真实uri，虚拟uri，和虚拟终端号
 */
void ParseVirtualUriStr(const string inputUri, string &uri, string &virtualUri, int &virtualIndex)
{
	int nSharpPos = inputUri.find("#");
	if (std::string::npos != nSharpPos)
	{
		// 找到#符号
		virtualUri = inputUri;
		uri = inputUri.substr(nSharpPos + 1);
		virtualIndex = atoi(inputUri.substr(1, nSharpPos - 1).c_str()) - 1;
	}
	else
	{
		uri = inputUri;
		virtualUri = "v1#";
		virtualUri += uri;
		virtualIndex = 0;
	}
}


void TransformConferenceForQt( const ConfInfo& conf , QVariantMap* confMap )
{
    confMap->insert("name" , QString::fromUtf8(conf.Title.c_str()));
    confMap->insert("handle-name" , QString::fromStdString(conf.URI));
    confMap->insert("conference-id" , QString::fromStdString( conf.CID ) );
    confMap->insert("start-time" , QString::fromStdString(conf.StartTime));
    confMap->insert("control-mode" , QString::fromStdString(conf.ControlMode));
    confMap->insert("join-mode" , QString::fromStdString(conf.JoinMode) );
    confMap->insert("duration" , QString::fromStdString(conf.Duration));
    confMap->insert("level" , QString::fromStdString(conf.level));
	confMap->insert("chairman" , QString::fromStdString(conf.HostURI));
	confMap->insert("multicastAddress",QString::fromStdString(conf.multicastAddress));
}

void TransformConfListForQt( const ConferenceVector& confList , QList<QVariantMap>* confMapList )
{
    for( ConferenceVector::const_iterator it = confList.begin(); it != confList.end(); it++ )
    {
        QVariantMap conf;
        TransformConferenceForQt( *(*it) , &conf );
        confMapList->push_back( conf );
    }
}

void TransformMemberForQt( const MemberInfo& member , QVariantMap* memberMap )
{
    memberMap->insert("uri", QString::fromUtf8(member.MemberURI.c_str()));
    memberMap->insert("sdp", QString::fromUtf8(member.sdp.c_str()));
    memberMap->insert("floor" , QString::fromUtf8(member.floor.c_str()));
    memberMap->insert("multicastAddress", QString::fromUtf8(member.multicastAddress.c_str()));
    memberMap->insert("videoPort", member.videoPort);
    memberMap->insert("audioPort", member.audioPort);
    memberMap->insert("name" , QString::fromUtf8(member.name.c_str()));
    memberMap->insert("username" , QString::fromUtf8(member.username.c_str()));
    memberMap->insert("location" , QString::fromUtf8(member.location.c_str() ) );
    memberMap->insert("AllowTerminal" , QString::fromUtf8(member.AllowTerminal.c_str()) );
    memberMap->insert("company" , QString::fromUtf8(member.company.c_str()) );
    memberMap->insert("xmpp" , QString::fromUtf8(member.xmpp.c_str()));
    memberMap->insert("status" , QString::fromUtf8(member.status.c_str()) );
}

void TransformMemberListForQt( const MemberVector& memberList , QList<QVariantMap>* memberMapList )
{
    for( MemberVector::const_iterator it = memberList.begin() ; it != memberList.end() ; it++ )
    {
        QVariantMap member;
        TransformMemberForQt( *(*it) , &member );
        memberMapList->push_back( member );
    }
}

void TransformTerminalForQt( const TerminalInfo&terminal , QVariantMap* terminalMap )
{
	terminalMap->insert("uri", QString::fromUtf8(terminal.uri.c_str()));
    terminalMap->insert("virtualIndex", terminal.virtualIndex);
	terminalMap->insert("virtualUri", QString::fromUtf8(terminal.virtualUri.c_str()));
    terminalMap->insert("virtualCount", terminal.virtualCount);
    terminalMap->insert("name" , QString::fromUtf8(terminal.name.c_str()));
    terminalMap->insert("location" , QString::fromUtf8( terminal.location.c_str() ) );
    terminalMap->insert("status" , QString::fromUtf8(terminal.status.c_str()) );
    terminalMap->insert("sdp" , QString::fromUtf8(terminal.sdp.c_str()));
    terminalMap->insert("videoPort" , QString::number(terminal.videoPort ));
    terminalMap->insert("audioPort" , QString::number(terminal.audioPort) );
    terminalMap->insert("multicastAddress" , QString::fromUtf8(terminal.multicastAddress.c_str()));
	terminalMap->insert("isHandUp",QString :: fromUtf8(terminal.isHandUp.c_str()));
	terminalMap->insert("isSpeaking",QString::fromUtf8(terminal.isSpeaking.c_str()));
	terminalMap->insert("isMainSpeaker",QString::fromUtf8(terminal.isMainSpeaker.c_str()));
	terminalMap->insert("isChairmanTerminal",QString::fromUtf8(terminal.isChairmanTerminal.c_str()));

}

void TransformTerminalListForQt( const TerminalVector& terminalList , QList<QVariantMap>* terminalMapList )
{
    for( TerminalVector::const_iterator it = terminalList.begin() ; it != terminalList.end() ; it++ )
    {
        QVariantMap terminalMap;
        TransformTerminalForQt( *(*it) , &terminalMap );
        terminalMapList->push_back( terminalMap );
    }
}

//发送一条消息给NetworkMeasurer进程
int sendMessageToNetworkMeasurer(const char * message, int length )
{
    if(message == NULL || length <= 0)
        return -1;
    STcpSender tcpsender;
    bool connected = tcpsender.connect("127.0.0.1", 5150);
    if (!connected)
        return -1;
    unsigned	char* data = new unsigned char[length+4];
    //内存拷贝
    int nlen = htonl(length);
    memcpy(data, &nlen, sizeof(nlen));
    memcpy(&data[4], message, length);
    int ret =	tcpsender.send(data, length + 4);
    delete data;
    tcpsender.close();
    return ret;
}


ConferenceManager::ConferenceManager( )
{
    _sipWrapperProxy = new CvSipWrapperIf( SIP_WRAPPER_SERVICE_NAME , SIP_WRAPPER_OBJECT_PATH ,
                                           QDBusConnection::sessionBus() );
    connect( _sipWrapperProxy , SIGNAL(cvMessageReceivedEvent(const QString &, const QString &)) ,
		this , SLOT(cvMessageReceivedSlot(const QString& , const QString&)));

	_httpWrapper = new HttpWrapper();
	connect( _httpWrapper , SIGNAL(RecvTerminalListSignal(TerminalVector)), this , SLOT(RecvTerminalListSlot(TerminalVector)));

	_txMonitor = nullptr; /*new TxMinitorIf(TX_MONITOR_SERVICE_NAME, TX_MONITOR_SERVICE_OBJECT_PATH, 
		QDBusConnection::sessionBus() );*/

    _confState = NotInConference;
    _currentConfInfo = NULL;
	_totalTermianlNum =0;

    //从配置文件加载信息
    const char *strqosuri = CVIniConfig::getInstance().getQosServerSipUri();
    if (strqosuri != NULL && strlen(strqosuri) > 0)
    {
        _qosServerUri = strqosuri;
    }
    else
    {
        //从另一个配置读取
        _qosServerUri = RunningConfig::Instance()->getQosServerURI().toStdString();
    }

    _onlineInfoTimer = new QTimer(this);
	_serverHeartbeatTimer = new QTimer(this);
	_serverTestTimer = new QTimer(this);

	_serverTestCount.start();

	_isServerTest=false;

    connect( _onlineInfoTimer , SIGNAL(timeout()) , this , SLOT(onlineInfoSlot()));
	connect( _serverTestTimer , SIGNAL(timeout()) , this , SLOT(severTestTimeOutSlot()));

}


ConferenceManager::~ConferenceManager()
{
    SAFE_DELETE( _onlineInfoTimer);
	SAFE_DELETE( _serverHeartbeatTimer);
	SAFE_DELETE( _serverTestTimer);

    CVMsgParser cvMessageParser;
    cvMessageParser.ReleaseConfList(_confList);

}

int ConferenceManager::makeJoinConfInvite( const std::string& focus_URI )
{
    switch(_confState)
    {
    case IsInConference:
    {
        //用户已经在一个会议中
        return kInAConference;
    }
    break;
    case ExitingConference:
    {
        //用户正在退出会议
        return kIsLeavingConference;
    }
    break;
    case NotInConference:
    case WaitingPermission:
        //抛弃原来的准入等待，重新申请
    case JoinRejected:
        //可以申请加入一个新的会议
    {
        //显示会议室窗口
        startConferenceRoom();


        //根据uri查找会议信息
        ConfInfo* conf = conf_find_by_uri(_confList, focus_URI);
        if(!conf)
            return kConferenceNotExisted;
        if("password" == conf->JoinMode)
            return kNeedPassword;
        //更新当前会议信息
        _focusURI = focus_URI;
        _confTitle = conf->Title;
        _currentConfInfo = conf;
        _hostURI = conf->HostURI;
        _confMode = conf->ControlMode;
		_speakerURI = ""; 
		_TerminalChairmanURI="";
		_currentScreenShareUri="";
		_totalTermianlNum=0;
        CVMsgParser::ReleaseMemberList( _memberList );

		for (TerminalMap::iterator it = _terminalList.begin(); it != _terminalList.end(); ++it)
		{
			CVMsgParser::ReleaseTerminalList( it->second );
		}
		_terminalList.clear();


        SDPInfo info;
		//TODO:已不再适合多路采集，暂时获取一路有效数据
		info._videoHeight = 0;
		info._videoWidth = 0;
		info._videoFps = 0;
		for (int i = 0; i < RunningConfig::Instance()->VideoCaptureDeviceCount(); ++i)
		{
			VCapSetting setting = RunningConfig::Instance()->VideoCaptureDevice(i);
			if (setting.devicePath == "")
			{
				continue;
			}
			info._videoCodec = setting.videoCodec;
			info._videoHeight = setting.height;
			info._videoWidth = setting.width;
			info._videoFps = setting.fps;
			break;
		}
        info._audioCodec = RunningConfig::Instance()->AudioCodec();
        QByteArray output_bytes;
        QDataStream out(&output_bytes , QIODevice::WriteOnly );
        out.setVersion( QDataStream::Qt_4_4 );
        out << info;
        std::string username = RunningProfile::getInstance()->username();
        RunningProfile::getInstance()->set_current_conference_uri(focus_URI);
        std::string szContent = createJoinConferenceMsg(focus_URI);
        _sipWrapperProxy->makeJoinConfInvite( QString::fromStdString(username) , QString::fromStdString(focus_URI), QString::fromStdString(szContent), output_bytes);
		
        //改变会议状态为正在验证中
        _confState = WaitingPermission;


        return kOk;
    }
    break;
    default:
        break;
    }
    return kUnknown;
}



void ConferenceManager::cvMessageReceivedSlot( const QString &message, const QString &from )
{
    CVMsgParser cvMessageParser;
	cvMessageParser.InitParser( message.toStdString().c_str() );		//For WengoPhone SipWrapper
    
    const char* command = cvMessageParser.GetCommandType();
	//获取终端配置模式 Patrick
	if (strcmp(command,"TerminalConfMode") == 0)
	{
		recvTerminalConfModeMsgHandler(cvMessageParser, from.toStdString());
	}
    //获得会议列表
    if(strcmp(command, "ConfList") == 0)
    {
        recvConfListMsgHandler(cvMessageParser, from.toStdString());
    }
    else if( strcmp(command , "TerminalList") == 0 )
    {
        //获取终端列表
        recvTerminalListCommandMsgHandler( cvMessageParser , from.toStdString() ); //TODO:HTTP
    }
    //登入会议
    else if(strcmp(command, "LoginResult") == 0)
    {
        recvLoginResultCommandMsgHandler(cvMessageParser, from.toStdString());
    }
    //收到MemberList的Info消息
    else if(strcmp(command, "MemberList") == 0)
    {
        recvMemberListCommandMsgHandler(cvMessageParser, from.toStdString());
    }
    //有人加入会议
    else if(strcmp(command, "LoginNotify") == 0 || strcmp(command, "loginnotify") == 0)
    {
        recvLoginNotifyMsgHandler(cvMessageParser, from.toStdString());
    }
    //有人退出会议
    else if(strcmp(command, "LogoutNotify") == 0 || strcmp(command, "logoutnotify") == 0)
    {
        recvLogoutNotifyMsgHandler(cvMessageParser, from.toStdString());
    }
    //受到邀请
    else if(strcmp(command, "Invite") == 0)
    {
        recvInviteMsgHandler(cvMessageParser, from.toStdString());
    }
    //申请即时会议成功
    else if(strcmp(command, "ApplyConfResult") == 0)
    {
        recvApplyConfResultMsgHandler(cvMessageParser, from.toStdString());
    }
    //密码不正确
    else if(strcmp(command, "PasswordIncorrect") == 0)
    {
        recvPasswordIncorrectMsgHandler(cvMessageParser, from.toStdString());
    }
    //被踢出会议
    else if(strcmp(command, "Kicked") == 0)
    {
        recvKickedMsgHandler(cvMessageParser, from.toStdString());
    }
    //收到文本消息
    else if(strcmp(command, "Text") == 0)
    {
        recvTextMsgHandler(cvMessageParser, from.toStdString());
    }
    //收到白板消息
    else if(strcmp(command, "WhiteBoard") == 0)
    {
        recvWhiteBoardMsgHandler(cvMessageParser, from.toStdString());
    }
    //举手发言
    else if(strcmp(command, "HandUp") == 0)
    {
        recvHandUpMsgHandler(cvMessageParser, from.toStdString());
    }
    //取消举手
    else if(strcmp(command, "HandDown") == 0)
    {
        recvHandDownMsgHandler(cvMessageParser, from.toStdString());
    }
#ifndef MEDIA_CONTROL_ENABLE
    //发送一路单播媒体流
    else if(strcmp(command, "Receiver") == 0)
    {
        recvReceiverMsgHandler(cvMessageParser, from.toStdString());
    }
    //结束发送一路单播媒体流
    else if(strcmp(command, "Cancel2Receiver") == 0)
    {
        recvCancel2ReceiverMsgHandler(cvMessageParser, from.toStdString());
    }
#else
    //start---------新增的媒体流控制信令----------zhhua.sun
    else if( strcmp( command , "StartMediaReply") == 0 )
    {
        //focus 返回的创建媒体流的回复
        recvStartMediaReply( cvMessageParser , from.toStdString() );

    }
    else if( strcmp( command , "StartMediaRelay") == 0 )
    {
        ///focus 转发接收方的媒体流的要求
        recvStartSendMediaCommand( cvMessageParser , from.toStdString() );

    }
    else if( strcmp(command , "StopMediaReply") == 0 )
    {
        //focus返回停止媒体流的回复
        recvStopMediaReply( cvMessageParser , from.toStdString() );
    }
    else if( strcmp( command , "StopMediaRelay") == 0 )
    {
        //focus转发接收方停止媒体流的要求
        recvStopSendMediaCommand( cvMessageParser , from.toStdString() );
    }
    else if( strcmp( command , "SendMediaControl") == 0 )
    {
        recvSendMediaControlCommand( cvMessageParser , from.toStdString() );
    }
    //end
#endif

    //发送一路屏幕共享流
    else if(strcmp(command, "ScreenReceiver") == 0)
    {
        recvScreenReceiverMsgHandler(cvMessageParser, from.toStdString());
    }
    //结束发送一路屏幕共享流
    else if(strcmp(command, "ScreenCancel2Receiver") == 0)
    {
        recvScreenCancel2ReceiverMsgHandler(cvMessageParser, from.toStdString());
    }
    else if(strcmp(command, "UnShareScreen") == 0)
    {
        recvUnShareScreenMsgHandler(cvMessageParser, from.toStdString());
    }
    else if( strcmp( command , "EndConference") == 0 )
    {
        cvEndConferenceSignal();
    }
    //qos相关命令
    else if(strcmp(command, "qos") == 0)
    {
        recvQoSMsgHandler(cvMessageParser, from.toStdString());
    }
	//主讲人控制
	else if( strcmp( command, "SetSpeakerTerminal")==0 )
	{
		recvPermitSpeakInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "ForbidSpeaker")==0 )
	{
		recvForbidSpeakInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "TerminalLoginNotify")==0 )
	{
		recvSaviInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "SetChairmanTerminal")==0 )
	{
		recvChairmanTerminalHandler( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "MediaControl")==0 )
	{
		recvMediaControlInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "GetSpeakerTerminal")==0 )
	{
		recvMainSpeakerTermianlInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "GetChairmanTerminal")==0 )
	{
		recvChairmanTermianlInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "GetScreenShareTerminal")==0 )
	{
		recvCurrentScreenShareTerminalInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "ShareScreenControl")==0 )
	{
		recvShareScreenControlInfo( cvMessageParser , from.toStdString() );
	}
	else if( strcmp( command, "Online")==0 )
	{
		recvOnlineMessageInfo( cvMessageParser , from.toStdString() );
	}

}

void ConferenceManager::recvTerminalConfModeMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string confMode = cvMessageParser.GetTerminalConfMode();
	if (confMode == "720PWEBCAM*1")
	{
		//implementation
	}
	else if (confMode == "1080PHD*1")
	{
		//implementation
	}
	else if (confMode == "1080PHD*3")
	{
		//implementation
	}
	else if (confMode == "1080PHD*3+VGA")
	{
		//implementation
	}

}
void ConferenceManager::recvConfListMsgHandler( CVMsgParser& cvMessageParser, const std::string& from )
{
   //cvMessageParser.ParseConfListMsg(_confList);

    ConferenceVector tmpConfVector;
    cvMessageParser.ParseConfListMsg( tmpConfVector );

    cvConfListReceivedSignal( tmpConfVector );

    for( int i = 0 ; i < tmpConfVector.size() ; i++ )
    {
        _confList.push_back( tmpConfVector.at(i) );
    }

    //_currentConfInfo = conf_find_by_uri(_confList, _focusURI);

}

void ConferenceManager::recvLoginResultCommandMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{

}
//接收到MemberList命令SIP消息，更新成员列表
void ConferenceManager::recvMemberListCommandMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    if(!checkIsInConference())
        return;
    if( _currentConfInfo == NULL )
        return;

    MemberVector tmpMemberVector;
    cvMessageParser.ParseMemberListMsg(tmpMemberVector, RunningProfile::getInstance()->user_uri() , _confMode);
    cvMemberListReceivedSignal(tmpMemberVector, _confTitle);

    for( int i = 0 ; i < tmpMemberVector.size() ; i++ )
    {
        _memberList.push_back( tmpMemberVector.at(i) );
    }
}

bool IgnoreTerminal(TerminalInfo & recvTerminal)
{
	if (recvTerminal.name.find("TX") != -1)
	{
		return true;
	}
	else if (recvTerminal.uri == RunningProfile::getInstance()->user_uri() && 
		CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_TX)
	{
		return true;
	}
	return false;
}

//接收到终端列表
//服务器每次应该发来一个终端的一组虚拟终端列表
void ConferenceManager::recvTerminalListCommandMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    if( !checkIsInConference() || _currentConfInfo == NULL)
        return;

	TerminalVector recvTerminals;

	cvMessageParser.ParseTerminalListMsg( recvTerminals , _hostURI , _confMode );
	if (recvTerminals.size() == 0)
	{
		return;
	}

	RecvTerminalListSlot(recvTerminals);
}

void ConferenceManager::RecvTerminalListSlot(TerminalVector recvTerminals)
{
	TerminalVector checkedTerminals;
	std::string recvTerminalUri = recvTerminals[0]->uri;

	// 检查合法性
	// 2013-10-29修订(Liaokz)：因服务器改为一次发一个虚拟终端，故合法性检查不再需要，改为检查已收到终端数是否匹配
	// 如果新虚拟终端数与之前不同，则清空保存的数据
	if (_terminalList[recvTerminalUri].size() > 0 &&
		_terminalList[recvTerminalUri][0]->virtualCount != recvTerminals[0]->virtualCount)
	{
		CVMsgParser::ReleaseTerminalList( _terminalList[recvTerminalUri] );
		_terminalList.erase(recvTerminalUri);
	}
	checkedTerminals = recvTerminals;
	//for( int i = 0; i < recvTerminals.size() ; i++ )
	//{
	//	TerminalInfo* recvTerminal = recvTerminals.at(i);

	//	// 忽略非该终端的虚拟终端
	//	if (recvTerminal->uri != recvTerminals[0]->uri)
	//	{
	//		continue;
	//	}
	//	checkedTerminals.push_back(recvTerminal);
	//}
	// 合法性检查的循环保证了checkedTerminals至少有一个元素

	// 保存终端信息，先判断成员列表里是否存在相对应的成员
	bool bFirstTimeReceived = true; // 用于记录终端是否重复接收.当终端接收不全事,会重复请求终端列表,导致重复收到自己的信息
	// 若无检测可能重复初始化发送流,造成不可预料的问题
	for (TerminalVector::iterator it = checkedTerminals.begin(); 
		it != checkedTerminals.end(); ++it)
	{
		for (TerminalVector::iterator it2 = _terminalList[recvTerminalUri].begin();
			it2 != _terminalList[recvTerminalUri].end(); ++it2)
		{
			if ((*it)->virtualUri == (*it2)->virtualUri)
			{
				// 重复接收终端信息时,删除原信息
				delete (*it2);
				_terminalList[recvTerminalUri].erase(it2);
				bFirstTimeReceived = false;
				break;
			}
		}
		_terminalList[recvTerminalUri].push_back(*it);

		_snprintf(__globalMessege , sizeof(__globalMessege) , "Recv terminal %s(%s)\n",
			(*it)->name.c_str(), (*it)->virtualUri.c_str() );
		CV_LOG_DEBUG( __globalMessege );
	}

	// 当自己的虚拟终端收全了,记录当前终端信息,并初始化发送进程
	if (recvTerminalUri == RunningProfile::getInstance()->user_uri() && 
		_terminalList[recvTerminalUri].size() == checkedTerminals[0]->virtualCount &&
		bFirstTimeReceived)
	{
		//createSendStreamMedia();
		initSendStreamMedia();
	}

	// 通知界面更新
	// 先检查终端数量，并报告给界面，告知其终端总数；界面收到一个中断后，会检查数量是否足够，如果数量不够，每分钟会重新检查一次
	// TODO：这项工作应该由控制器完成
	int num = checkedTerminals[0]->totalTermialNum;
	for( TerminalMap::iterator it = _terminalList.begin(); it != _terminalList.end(); it++ )
	{
		if (it->second.size() > 0 && IgnoreTerminal(*(it->second[0])))
		{
			--num;
		}
	}
	// 数量变更，通知界面
	if(num != _totalTermianlNum)
	{
		_totalTermianlNum = num;
		cvTermianlCountChangeNotify(_totalTermianlNum);
	}

	// 不报告TX终端的存在
	if (_terminalList[recvTerminalUri].size() && 
		!IgnoreTerminal(*_terminalList[recvTerminalUri][0]))
	{
		cvTerminalListReceivedSignal( _terminalList[recvTerminalUri] , _focusURI );
		// 由于发送给的是虚拟终端列表，故每收到一个新的虚拟终端信息，之前收到的终端信息都会被再次发送，信号接收方须能处理
	}
}

void ConferenceManager::createSendStreamMedia()
{
	if (!CVIniConfig::getInstance().IsModelHD())
	{
		// 如果不是正常终端，不启动本地媒体流
		return;
	}

	RunningConfig* pConfig = RunningConfig::Instance();

	bool enableSmallVideo = pConfig->isEnableSmallVideo();

	// 根据摄像头数量创建本地媒体流
	const int numberOfStreams = pConfig->VideoCaptureDeviceCount();

	for (int i = 0; i < numberOfStreams; i++)
	{
		VCapSetting vcap = pConfig->VideoCaptureDevice(i);
		if (vcap.devicePath != "")
		{
			//启动媒体流传输
			//之所以要在创建发送流的时候指定本地回显端口，是因为这里存在一个进程同步问题
			//一般的，必须要在发送流成功运行后，才能添加发送端口。
			//而本地回显接收进程与发送进程几乎同时创建，故无法保证addRecvStreamMedia调用时发送流已经运行
			//因此发送流应在创建完成后主动发送视频流给本地回显
			// Liaokz补充说明
			MediaManager::getInstance()->CreateSendMedia(i, 
				"127.0.0.1" ,
				0, //发送本地回显视频，不进行音频发送
				(enableSmallVideo ? LOCALPRE_SMALL_PORT_BASE + 10 * i : LOCALPRE_VIDEO_PORT_BASE + 10 * i),
				enableSmallVideo, 0);
		}

		//TODO:暂时在此创建本地回显
		std::string localID = getLocalPreviewMediaID(i).toStdString();
		addRecvLocalMedia(localID, 0, 6 - i, true);
	}
}

void ConferenceManager::initSendStreamMedia()
{
	if (!CVIniConfig::getInstance().IsModelHD())
	{
		// 如果不是正常终端，不初始化
		return;
	}

	TerminalVector myVirtualTerminals = _terminalList[RunningProfile::getInstance()->user_uri()];
	if (myVirtualTerminals.size() == 0)
	{
		return;
	}

	//在报告会议中，只有主讲人可以发送音频
	if(myVirtualTerminals[0]->isSpeaking=="false")
	{
		MediaManager::getInstance()->ControlAudioSend(false);
	}
	else
	{
		MediaManager::getInstance()->ControlAudioSend(true);
	}

	RunningConfig* pConfig = RunningConfig::Instance();
	bool enableSmallVideo = pConfig->isEnableSmallVideo();

	for (int i = 0; i < myVirtualTerminals.size(); i++)
	{
		TerminalInfo * curVirtualTerminal = myVirtualTerminals[i];
		VCapSetting vsetting = RunningConfig::Instance()->VideoCaptureDevice(curVirtualTerminal->virtualIndex);
		if (vsetting.devicePath == "")
		{
			continue;
		}

		//发送给组播地址
		if( checkSupportMulticast( _currentConfInfo->multicastAddress ) )
		{
			_receiverInfo.AddRemoteReceiver(curVirtualTerminal->virtualIndex, 
				MULTICAST_URI, 
				curVirtualTerminal->multicastAddress.c_str(), 
				curVirtualTerminal->videoPort, 
				RunningConfig::Instance()->AudioCodec() == "" ? 0 : curVirtualTerminal->audioPort,
				false);

			MediaManager::getInstance()->AddSendMedia(curVirtualTerminal->virtualIndex, // 像改虚拟终端对应的发送进程添加发送端
				curVirtualTerminal->multicastAddress.c_str(), 
				RunningConfig::Instance()->AudioCodec() == "" ? 0 : curVirtualTerminal->audioPort, 
				curVirtualTerminal->videoPort, 
				false, 0);

			if(enableSmallVideo)
			{
				//将小流也发向组播地址
				_receiverInfo.AddRemoteReceiver(curVirtualTerminal->virtualIndex, 
					MULTICAST_SMALL_URI, 
					curVirtualTerminal->multicastAddress.c_str(), 
					curVirtualTerminal->smallVideoPort, 
					0,
					true);

				MediaManager::getInstance()->AddSendMedia(curVirtualTerminal->virtualIndex, 
					curVirtualTerminal->multicastAddress.c_str(),
					0, // 小流无音频
					curVirtualTerminal->smallVideoPort,
					true, 0, false);
			}
		}

		//开启本地录制
		if (pConfig->isEnableLocalRecord())
		{
			addRecvLocalMedia(getLocalRecordMediaID(curVirtualTerminal->virtualIndex).toStdString(), -1, -1);
		}
	}

	// 开启录制分割计时器.注意:不管本地录制是否启用都应启动计时器,为远程TX提供分段数据
	int sec = CTxConfig::getInstance().GetRecLocalDuration();
	if (sec > 0)
	{
		QObject::connect(&_localVideoAutoCutTimer, SIGNAL(timeout()), this, SLOT(SegmentLocalMedia()));
		_localVideoAutoCutTimer.start(sec * 1000);
	}
}

void ConferenceManager::resetSendStreamMedia()
{
	//删除除了本地回显以外的发送端
	ReceiverInfoManager::InfoList list = _receiverInfo.ToInfoList();
	for (ReceiverInfoManager::InfoList::iterator it = list.begin();
		 it != list.end(); ++it)
	{
		if (isLocalPreviewMediaID(it->MemberURI.c_str()))
		{
			continue;
		}

		//停止发送
		MediaManager::getInstance()->RemoveSendMedia( it->virtualIndex, it->recvIP.c_str(),
			it->audioPort,
			it->videoPort,
			it->smallVideo);
		//删除发送信息 - 使用URI,避免无法区分本地回显和本地录制
		_receiverInfo.RemoveRemoteReceiverByURI(it->virtualIndex, it->MemberURI.c_str());
	}
}

//接收到LoginNotify命令SIP消息，有人加入会议
void ConferenceManager::recvLoginNotifyMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    if(!checkIsInConference())
        return;

    int loginType = cvMessageParser.GetLoginNotifyType();
    if( loginType == 0 )
    {
        MemberInfo* member;
        member = cvMessageParser.ParseMemberLoginNotifyMsg( _memberList , _hostURI , _confMode ) ;
        if( member )
        {
            cvMemberLoginSignal( *member );
            return;
        }
    }
    else if( loginType == 1 )
    {
		// case:服务器会发送一台登录终端的所有虚拟终端的列表
		TerminalVector recvTerminals;
		TerminalVector checkedTerminals;

        cvMessageParser.ParseTerminalLoginNotifyMsg( recvTerminals, _hostURI , _confMode );
		if (recvTerminals.size() == 0)
		{
			return;
		}

		// 检查合法性
		//for( int i = 0; i < recvTerminals.size() ; i++ )
		//{
		//	TerminalInfo* recvTerminal = recvTerminals.at(i);

		//	// 忽略非该终端的虚拟终端
		//	if (recvTerminal->uri != recvTerminals[0]->uri)
		//	{
		//		continue;
		//	}
		//	checkedTerminals.push_back(recvTerminal);
		//}
		// 本循环保证了checkedTerminals至少有一个元素
		checkedTerminals = recvTerminals; //TODO:因服务器又改回一次发一个虚拟终端，故注释上部分代码

		string loginUri = checkedTerminals[0]->uri;
		if(loginUri == _TerminalChairmanURI)
		{
			checkedTerminals[0]->isChairmanTerminal="true";
			checkedTerminals[0]->isSpeaking="true";
		}
		
		// 更新终端信息
		for (TerminalVector::iterator it = checkedTerminals.begin(); 
			it != checkedTerminals.end(); ++it)
		{
			for (TerminalVector::iterator it2 = _terminalList[loginUri].begin();
				it2 != _terminalList[loginUri].end(); ++it2)
			{
				if ((*it)->virtualUri == (*it2)->virtualUri)
				{
					// 重复接收终端信息时,删除原信息
					delete (*it2);
					_terminalList[loginUri].erase(it2);
					break;
				}
			}
			_terminalList[loginUri].push_back(*it);
		}

		// 通知界面
		if (!IgnoreTerminal(*checkedTerminals[0]))
		{
			cvTerminalLoginSignal( checkedTerminals );
			// Auto recv
			//addRecvStreamMedia( RunningProfile::getInstance()->getUserName() , terminal->uri, 0, 1);
		}
    }

}

//接收到LogoutNotify命令SIP消息，有人退出会议
void ConferenceManager::recvLogoutNotifyMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    if(!checkIsInConference())
        return;

	// 服务器只发送退出终端的URI，需要找到其所有虚拟终端并设置下线
    string logoutUserUri = cvMessageParser.GetMemberUri();

    //单播会议下，如果有人退出检查是否有发向对方的媒体流，有则停止发送
    if(!checkCurrentIsMulticastConference())
    {
		const int virtualCount = RunningConfig::Instance()->VideoCaptureDeviceCount();
		//检查每一路发送流
		for (int i = 0; i < virtualCount; ++i)
		{
			ReceiverInfo * receiver  = _receiverInfo.FindRemoteReceiverByURI(i, logoutUserUri);
			if (receiver)
			{
				//关闭发送媒体流
				MediaManager::getInstance()->RemoveSendMedia( receiver->virtualIndex, receiver->recvIP.c_str() , receiver->audioPort , receiver->videoPort );
				//删除发送信息
				_receiverInfo.RemoveRemoteReceiverByIP(receiver->virtualIndex, receiver->recvIP);
			}
		}
    }

    //关闭接收的屏幕流
    MediaManager::getInstance()->RemoveRecvScreenMedia(logoutUserUri.c_str() );

	if (_terminalList.find(logoutUserUri) != _terminalList.end())
	{
		TerminalVector & virtualList = _terminalList[logoutUserUri];
		for (TerminalVector::iterator it = virtualList.begin(); it != virtualList.end(); ++it)
		{
			TerminalInfo * terminal = *it;
            //如果有接收其媒体流，关闭接收媒体流
			MediaManager::getInstance()->RemoveRecvMedia( terminal->virtualUri.c_str() );

			//更新状态
			terminal->status = "Not Available";
			if(terminal->uri == _TerminalChairmanURI)
			{
				terminal->isChairmanTerminal="false";
				if(_confMode == "report")
					terminal->isSpeaking="false";
				cvUpgradeTerminalInfoSignal(*terminal);
				_TerminalChairmanURI ="";
			}
		}

		//通知界面变更状态
		emit cvTerminalLogoutSignal(QString::fromStdString(virtualList[0]->uri));
	}

    //更新成员列表或成员列表
    //_SipAccountLogout( logoutUserUri );
    //cvMessageParser.ParseMemberListByNotifyMsg(_memberList, _hostURI, _confMode);
}

//接收到Invite命令SIP消息，受到邀请
void ConferenceManager::recvInviteMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    ConfInfo* conf = conf_find_by_uri(_confList, fromUri);
    if(conf)
    {
        string inviter = cvMessageParser.GetInviterUri();
        cvConfrenceInviteSignal( conf, inviter);
    }
}
//接收到ApplyConfResult命令SIP消息，申请即时会议成功
void ConferenceManager::recvApplyConfResultMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    string uri = cvMessageParser.GetFocusURI();
    if(uri != "")
    {
        makeJoinConfInvite(uri);
    }
}

//接收到PasswordIncorrect命令SIP消息，密码不正确
void ConferenceManager::recvPasswordIncorrectMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    cvPasswordIncorrectSignal();
}

void ConferenceManager::recvTextMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    string userId = fromUri;
    int index = userId.find('@');
    if(index != std::string::npos)
    {
        userId = userId.substr(0, index);
    }
    string msg = cvMessageParser.GetTextMessage();
    cvTextMsgReceivedSignal( msg, userId);
}

//接收到LoginResult命令SIP消息，被踢出会议
void ConferenceManager::recvKickedMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    cvKickedSignal();
}

//接收到WhiteBoard命令SIP消息，收到白板消息
void ConferenceManager::recvWhiteBoardMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    string userId = fromUri;
    int index = userId.find('@');
    if(index != std::string::npos)
    {
        userId = userId.substr(0, index);
    }
    string msg = cvMessageParser.GetWhiteBoardMessage();
    cvWhiteBoardMsgReceivedSignal( msg, userId);
}

//接收到HandUp命令SIP消息，举手发言
void ConferenceManager::recvHandUpMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    QString userUri =  QString::fromStdString(cvMessageParser.GetTerminalUri());
	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , userUri.toStdString() );
	if(remoteTerminal!= NULL)
	{
		remoteTerminal->isHandUp="true";
		cvUpgradeTerminalInfoSignal(*remoteTerminal);
	}
	//emit cvMemberHandUpSignal(userUri);
    //MemberInfo* member =  member_find_by_uri(_memberList,userUri);
    //if (member)
    //{
    //	member->hand = 1;
    //	//加入请求发言队列
    //	_requestForSpeakList.push_back(member);
    //}
    //cvMemberListReceivedSignal( _memberList, _confTitle);
}

void ConferenceManager::recvHandDownMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    QString userUri =  QString::fromStdString(cvMessageParser.GetTerminalUri());
	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , userUri.toStdString() );
	if(remoteTerminal!= NULL)
	{
		remoteTerminal->isHandUp="false";
		cvUpgradeTerminalInfoSignal(*remoteTerminal);
	}
    //emit cvMemberHandDownSignal( userUri );
}

/*  已过期不用*/
//void ConferenceManager::recvReceiverMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
//{
//    //获取对方地址，单播/组播IP地址
//    string recvIP = cvMessageParser.GetReceiverIP();
//    string remoteIPAddr;					//远程地址其实可以直接使用recvIP
//    int videoPort = 0, audioPort = 0;
//
//    //判断是否组播会议 , 单播会议目标端口是对方的，组播会议目标端口是自己的
//    if(checkCurrentIsMulticastConference())
//    {
//        // 组播会议使用组播地址、发送到自己的端口
//        getCurrentMulticastAddress(&remoteIPAddr);		//此时remoteIPAddr==recvIP
//    }
//    else
//    {
//        // 单播会议使用对方IP地址、发送到自己的端口
//        remoteIPAddr = recvIP;
//    }
//    //发送到自己的端口
//    MemberInfo* currentMember = NULL;
//    for( MemberVector::iterator it = _memberList.begin() ; it != _memberList.end() ; it++ )
//    {
//        MemberInfo*  member = (MemberInfo*) * it;
//        if( member->MemberURI == RunningProfile::getInstance()->user_uri() )
//        {
//            currentMember = *it;
//            break;
//        }
//    }
//
//    if (!currentMember)
//        return;
//    videoPort = currentMember->videoPort;
//    audioPort = currentMember->audioPort;
//
//
//
//    if( _confMode == "host" && currentMember->MemberURI != _hostURI && _memberList[0]->permitSpeak == 0)
//    {
//        MediaManager::getInstance()->AddSendMedia( remoteIPAddr.c_str() , 0 , videoPort,false, 0 );
//    }
//    else
//    {
//        MediaManager::getInstance()->AddSendMedia( remoteIPAddr.c_str() , audioPort , videoPort,false, 0 );
//    }
//
//    //记录对方信息，以便删除
//	ReceiverInfo::AddRemoteReceiver( _sendIPList, fromUri, remoteIPAddr, videoPort, audioPort );
//}

void ConferenceManager::recvScreenReceiverMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{

}

void ConferenceManager::recvUnShareScreenMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{

}

//已过期不用，接收到Cancel2Receiver命令SIP消息，结束媒体流的发送
//void ConferenceManager::recvCancel2ReceiverMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
//{
//    //组播会议不停止发送
//    if(checkCurrentIsMulticastConference())
//        return;
//
//    string recvIP = cvMessageParser.GetReceiverIP();
//
//	const int virtualCount = RunningConfig::Instance()->VideoCaptureDeviceNumber();
//	for (int i = 0; i < virtualCount; ++i)
//	{
//		ReceiverInfo * receiver  = _receiverInfo.FindRemoteReceiverByIP(i, recvIP);
//		if (receiver)
//		{
//			//关闭发送媒体流
//			MediaManager::getInstance()->RemoveSendMedia( receiver->virtualIndex, receiver->recvIP.c_str() , receiver->audioPort , receiver->videoPort );
//			//删除发送信息
//			_receiverInfo.RemoveRemoteReceiverByIP(receiver->virtualIndex, receiver->recvIP);
//		}
//	}
//	//ReceiverInfo * receiver  = ReceiverInfo::FindRemoteReceiverByIP(_receiverInfo, recvIP);
// //   if (receiver)
// //   {
// //       MediaManager::getInstance()->RemoveSendMedia(  receiver->recvIP.c_str() , receiver->audioPort , receiver->videoPort );
// //       //删除发送信息
//	//	ReceiverInfo::RemoveRemoteReceiverByIP(_receiverInfo, recvIP);
// //   }
//}

void ConferenceManager::recvScreenCancel2ReceiverMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{

}

void ConferenceManager::recvQoSMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
    RunningConfig* config = RunningConfig::Instance();
    string strType = cvMessageParser.GetQosType();
    const char* type = strType.c_str();
    if(strcmp(type, "LoginResult") == 0)    //申请加入会议结果
    {
        //sucess表示允许，正在返回LoginPermission消息
        //failure表示失败，正在与qos服务器协商
    }
    else if(strcmp(type, "LoginPermission") == 0)   //qos允许进入会议
    {
        MsgLoginPermissionInfo info;
        cvMessageParser.ParseLoginPermissionMsg(info);

        if( _confState == WaitingPermission )
        {
            //开始进入当前会议
            joinConference(_focusURI);
            //记录DSCP值
            int videoDscp =  info.dscp;
            int audioDscp =  info.dscp;
#ifdef _DEBUG
            if (CVIniConfig::getInstance().isUseCustomedDscp())  //强制使用ini配置中的DSCP值，该方式有利于测试，正常发布时应使用服务器分配的
            {
                videoDscp = CVIniConfig::getInstance().getVideoDscp();
                audioDscp = CVIniConfig::getInstance().getAudioDscp();
            }
#endif
            MediaManager::getInstance()->SetDSCP(videoDscp, audioDscp);
			
			//首先向服务器查询终端列表
			GetTermialList();


        }
    }
    else if(strcmp(type, "LoginRejection") == 0)    //qos不允许进入会议
    {
        if( _confState == WaitingPermission ) //只有在等待验证的时候才有效, zhenHua.sun 20110225
        {
            MsgLoginRejectionInfo info;
            cvMessageParser.ParseLoginRejectionMsg(info);
            //不允许进入当前会议
            rejectToJoinConference(_focusURI, info.description);
        }
    }
    else  if(strcmp(type, "QoSServerInfo") == 0)		//QoS服务器信息
    {
        MsgQoSServerInfo info;
        if ( cvMessageParser.ParseQoSServerInfoMsg(info) == 0 )
            saveQoSServerInfo(info);
    }
}

//判断当前会议是否组播会议
bool ConferenceManager::checkCurrentIsMulticastConference()
{
    if(!checkIsInConference())
        return false;

    return checkSupportMulticast( _currentConfInfo->multicastAddress );
}

std::string ConferenceManager::createJoinConferenceMsg( const string& confUri )
{
    string cid = "";

    //根据uri查找会议的cid
    ConfInfo* conf = conf_find_by_uri(_confList, confUri);
    if(conf)
        cid = conf->CID;

    string reporterUri = RunningProfile::getInstance()->user_uri();
    string reporterIP = RunningProfile::getInstance()->ip_address();
    char  strGateway[60];
    memset(strGateway, 0, 60);
    GetGatewayByHostIP(reporterIP.c_str(), strGateway);
    string reporterGateway(strGateway);

    //先设置一个默认的带宽限值
    string minRate = "50";
    stringstream ss;
    ss << computeRequiredBandwidth();
    string maxRate = ss.str();

	int virtualLineCount = RunningConfig::Instance()->VideoCaptureDeviceCount();
	char sVirtualLineCount[10] = {0};
	itoa(virtualLineCount, sVirtualLineCount, 10);

    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"Login\">";
    szContent += ("<cid>" + cid + "</cid>");
    szContent += ("<focusuri>" + confUri + "</focusuri>");
    szContent += ("<uri>" + reporterUri + "</uri>");
    szContent += ("<minRate>" + minRate + "</minRate>");
    szContent += ("<maxRate>" + maxRate + "</maxRate>");
    szContent += ("<ip>" + reporterIP + "</ip>");
	szContent += ("<gateway>" + reporterGateway + "</gateway>");
	szContent += (std::string("<virtualTerminalCount>") + sVirtualLineCount + "</virtualTerminalCount>");
    szContent += "</coolview>";
    return szContent;
}

void ConferenceManager::exitConference()
{
    if( _currentConfInfo )
    {
        if( checkIsInConference() )
		{
			//停止接收进程
            MediaManager::getInstance()->ExitConference();
			//停止发送
			resetSendStreamMedia();
		}

        _sipWrapperProxy->exitConference( QString::fromStdString(_currentConfInfo->URI) );
        _confState = NotInConference;
        _hostURI = "";
        _confMode = "";
        _confTitle = "";
		_speakerURI = "";
		_TerminalChairmanURI="";
		_currentScreenShareUri="";
		_totalTermianlNum=0;
		CVMsgParser::ReleaseMemberList( _memberList );

		for (TerminalMap::iterator it = _terminalList.begin(); it != _terminalList.end(); ++it)
		{
			CVMsgParser::ReleaseTerminalList( it->second );
		}
		_terminalList.clear();

		CVMsgParser ::ReleaseConfList(_confList);

        ///停止在线状态计时器
        _onlineInfoTimer->stop();
		_serverHeartbeatTimer->stop();
        _currentConfInfo = NULL;
		_localVideoAutoCutTimer.stop();

        CVIniConfig::getInstance().setCurrentConferenceUri("");
        CVIniConfig::getInstance().setCurrentConferenceCid(-1);

    }
}

void ConferenceManager::MemberHandup( const std::string memberURI , const bool handup )
{
    if( _currentConfInfo )
    {
        std::string command;
        if(handup)
            command = "HandUp";
        else
            command = "HandDown";

        std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"" + command + "\"><uri>" + memberURI + "</uri></coolview>";

        _sipWrapperProxy->makeInfoCall( QString::fromStdString( RunningProfile::getInstance()->current_conference_uri() ) , QString::fromStdString(szContent) );
    }

}

void ConferenceManager::startConference( const std::string& username , const std::string& sipServer )
{
    //连接focus服务器，获取会议列表
    QString szContent = "<?xml version=\"1.0\"?><coolview command=\"GetConfList\"></coolview>";
    _sipWrapperProxy->startConference( QString::fromStdString(username) , QString::fromStdString(sipServer), szContent );

}

void ConferenceManager::makeInfoCall( const std::string& fousURI , const std::string& szContent )
{
    _sipWrapperProxy->makeInfoCall( QString::fromStdString(fousURI) , QString::fromStdString(szContent) );
}


void ConferenceManager::addRecvStreamMedia( const std::string username , const std::string remoteMemberURI , const int screenIndex , const int seet)
{
	if( !checkIsInConference() )
	{
		qDebug("没有加入任何会议!");
		return;
	}

	//Step1: 收集信息
	bool isRecord = false; //是否为录制进程
	bool isMulti =false;  //是否是组播
	bool usingSmallVideo = seet > 1; //是否使用小流
	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , remoteMemberURI );

    if( remoteTerminal == NULL )
    {
		return;
	}
	//如果用户要求接受的是本地回显，则转调用创建本地接收进程的方法
	if (remoteTerminal->uri == RunningProfile::getInstance()->user_uri())
	{
		addRecvLocalMedia(getLocalPreviewMediaID(remoteTerminal->virtualIndex).toStdString(), 
			screenIndex, seet, usingSmallVideo);
		return;
	}

	if (CVIniConfig::getInstance().IsModelTX())
	{
		isRecord = true;
		usingSmallVideo = false; // TX录制只使用大流
	}
	else if (!RunningConfig::Instance()->isEnableSmallVideo())
	{
		usingSmallVideo = false;
	}

    //处理终端的媒体流
    //获取远程终端的媒体参数 - 本地回显和接收远程统一处理 - Liaokz, 2013-9
	int videoWidth;
	int videoHeight;
	int videoFps;
	char audioCodec[20] = {0};
	getMediaParamFromSdp( &videoWidth , &videoHeight , &videoFps, audioCodec, remoteTerminal->sdp.c_str() );
	std::string sAudioCodec( audioCodec );
	int remoteAudioPort = sAudioCodec.size() > 0 ? remoteTerminal->audioPort : 0;
	int remoteVideoPort;

	// 初始化媒体流信息
	if( usingSmallVideo) 
	{
		remoteVideoPort = (videoHeight == 0 || videoWidth == 0) ? 0 : remoteTerminal->smallVideoPort;

		//全高清摄像头的视频参数除以4后事460x270，但是编码器并不支持该视频格式，因此统一为320x180
		//...zhenhua.sun 2012/2/22
		videoWidth = MSDX_CONF_SMALL_VIDEO_WIDTH;
		videoHeight = MSDX_CONF_SMALL_VIDEO_HEIGHT;
		videoFps /=2;
	}else
	{
		remoteVideoPort = (videoHeight == 0 || videoWidth == 0) ? 0 : remoteTerminal->videoPort;
	}

	std::string multiAddr =_currentConfInfo->multicastAddress;
	string localAddress = "127.0.0.1";

	//Step2: 请求发送端发送数据
	// 接收远程组播
    if( ConferenceManager::checkSupportMulticast( multiAddr ) )
    {
        localAddress = multiAddr;
		isMulti=true;
    }
	// 接收远程单播
    else
    {
		localAddress = RunningProfile::getInstance()->ip_address();
        //如果不是组播会议，那么需要发送Sip消息要求对方用户发送媒体流
		QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StartMedia\"><uri>" +
			QString::fromStdString(remoteTerminal->uri) + "</uri>" + 
			"<virtualUri>" + QString::fromStdString(remoteTerminal->virtualUri) + "</virtualUri>" + 
			"<ip>" + localAddress.c_str() + "</ip><type>%1</type></coolview>";
		if( usingSmallVideo )
			szContent = szContent.arg("smallStream");
		else
			szContent = szContent.arg("stream");
        QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
        _sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
    }

	//Step3: 通知媒体流控制器建立接收进程
	//必须先添加接收成员的位置消息
	MediaManager::getInstance()->AddRecvMemberPosition( remoteTerminal->virtualUri.c_str() , screenIndex , seet, usingSmallVideo);

    RecvGraphInfo info;
    info.initial( qPrintable(RunningConfig::Instance()->AudioCaptureDevice()), qPrintable(RunningConfig::Instance()->AudioOutputDevice()),
                    localAddress.c_str(), remoteAudioPort  , audioCodec , 16000, 16 , 1 , localAddress.c_str() ,
                    remoteVideoPort , 0 , videoWidth , videoHeight , videoFps, remoteTerminal->virtualUri.c_str(), remoteTerminal->name.c_str(),
                    //RunningConfig::getInstance()->isEnableRecvAutoResync());
					false, "", CTxConfig::getInstance().GetRecLocalDuration() );

	if (isRecord)
	{
		QString file = getRecordPath(QString::fromUtf8(remoteTerminal->name.c_str()), remoteTerminal->virtualIndex);
		info.record_info.file_name = file;
	}

    MediaManager::getInstance()->AddRecvMedia( info );

	//Step4: 后续工作
	//音频同步
	if("true"!=remoteTerminal->isSpeaking)
	{
		//更改会议室中视频窗口上的音频图标
		MediaManager::getInstance()->ChangeMediaState( remoteTerminal->virtualUri.c_str(), "audio" , UiMediaState_Stop ,true );
		//MediaManager::getInstance()->ControlAudioRecv( remoteUsername ,false );
	}
	if(isMulti)
	{
		MediaManager::getInstance()->ChangeMediaState(remoteTerminal->virtualUri.c_str() , "video" , UiMediaState_Ready ,true);
	}

	//通知界面已创建接收进程
	streamMediaReceiveStartedSignal(QString::fromStdString(remoteTerminal->virtualUri));

    _snprintf(__globalMessege , sizeof(__globalMessege) , "@Recv Media From %s, WIDTH:%d , HEIGHT:%d, VideoPort:%d , AudioPort:%d \n",
                remoteTerminal->virtualUri.c_str() , videoWidth , videoHeight , remoteTerminal->videoPort , remoteTerminal->audioPort );
    CV_LOG_DEBUG( __globalMessege );
    
}

void ConferenceManager::addRecvLocalMedia(const std::string localMediaID, const int screenIndex, const int seet, bool smallvideo)
{
	//Step1: 收集信息
	std::string localAddress = "127.0.0.1";
	std::string baseMediaID;
	int videoPort = 0;
	int audioPort = 0;
	int virtualIndex = 0;
	int videoWidth = 0;
	int videoHeight = 0;
	int videoFps = 0;
	RunningConfig * config = RunningConfig::Instance();

	//根据虚拟终端号设置信息
	if (isLocalPreviewMediaID(localMediaID.c_str()))
	{
		virtualIndex = getVirtualIndexFromLocalPreviewMediaID(localMediaID.c_str());
		if (!config->isEnableSmallVideo())
		{
			smallvideo = false;
		}
		if (smallvideo)
		{
			videoPort = LOCALPRE_SMALL_PORT_BASE + 10 * virtualIndex;
		} 
		else
		{
			videoPort = LOCALPRE_VIDEO_PORT_BASE + 10 * virtualIndex;
		}
		baseMediaID = LOCAL_PREVIEW_MEDIA_ID;
	}
	else if (isLocalRecordMediaID(localMediaID.c_str()))
	{
		virtualIndex = getVirtualIndexFromLocalRecordMediaID(localMediaID.c_str());
		smallvideo = false;
		videoPort = LOCALREC_VIDEO_PORT_BASE + 10 * virtualIndex;
		audioPort = LOCALREC_AUDIO_PORT_BASE + 10 * virtualIndex;
		baseMediaID = LOCAL_RECORD_MEDIA_ID;
	}
	else
	{
		return;
	}

	//根据该路视频配置需要修改信息
	VCapSetting vsetting = config->VideoCaptureDevice(virtualIndex);

	if (vsetting.devicePath == "")
	{
		//底层应根据此信息决定是否创建媒体流接收进程
		videoPort = 0;
		audioPort = 0;
	}
	videoWidth = vsetting.width;
	videoHeight = vsetting.height;
	videoFps = vsetting.fps;

	if (smallvideo)
	{
		videoWidth = MSDX_CONF_SMALL_VIDEO_WIDTH;
		videoHeight = MSDX_CONF_SMALL_VIDEO_HEIGHT;
	}

	//Step2: 添加发送端
	MediaManager::getInstance()->AddSendMedia(virtualIndex, localAddress.c_str(), audioPort, videoPort, smallvideo, 0);
	_receiverInfo.AddRemoteReceiver(virtualIndex, baseMediaID, localAddress.c_str(), videoPort, audioPort, smallvideo);

	//Step3: 通知媒体流控制器创建接收进程
	MediaManager::getInstance()->AddRecvMemberPosition( localMediaID.c_str() , screenIndex , seet, smallvideo);
	//TODO: 硬编码下本地回显不创建音频链路时退出会崩溃，这里暂时让其接收一个没有数据的端口
	if (videoPort != 0 && audioPort == 0)
	{
		audioPort = 8500 + 10 * virtualIndex;
	}

	RecvGraphInfo info;
	info.initial( qPrintable(config->AudioCaptureDevice()), qPrintable(config->AudioOutputDevice()),
		localAddress.c_str(), audioPort  , qPrintable(config->AudioCodec()) , config->AudioSampleRate(), config->AudioBitsPerSample(), config->AudioChannel(),
		localAddress.c_str(), videoPort , NULL, videoWidth , videoHeight , videoFps, localMediaID.c_str(), localMediaID.c_str(),
		//RunningConfig::getInstance()->isEnableRecvAutoResync());
		false);

	if (isLocalRecordMediaID(localMediaID.c_str()))
	{
		//如果创建本地录制，则必须在会议中
		if (_terminalList.find(RunningProfile::getInstance()->user_uri()) == _terminalList.end())
		{
			return;
		}
		TerminalVector vec = _terminalList[RunningProfile::getInstance()->user_uri()];
		for (TerminalVector::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			if ((*it)->virtualIndex == virtualIndex)
			{
				QString file = getRecordPath(QString::fromUtf8((*it)->name.c_str()), (*it)->virtualIndex);
				info.record_info.file_name = file;
				break;
			}
		}
		if (info.record_info.file_name.isEmpty())
		{
			return;
		}
	}

	MediaManager::getInstance()->AddRecvMedia( info );

	//Step4
	MediaManager::getInstance()->ChangeMediaState(localMediaID.c_str(), "video", UiMediaState_Ready, true);
	MediaManager::getInstance()->ChangeMediaState(localMediaID.c_str(), "audio", UiMediaState_Ready, true);
	if (vsetting.devicePath == "")
	{
		MediaManager::getInstance()->ChangeMediaState(localMediaID.c_str(), "video", UiMediaState_Stop, true);
	}
}

void ConferenceManager::removeRecvStreamMedia( const std::string username, const std::string remoteMemberURI )
{
    if( !checkIsInConference() )
    {
        qDebug("没有加入任何会议!");
        return;
    }

    TerminalInfo* remoteTerminal = FindTerminal( _terminalList , remoteMemberURI );
	if (NULL == remoteTerminal)
	{
		return;
	}

	if(remoteTerminal->uri == RunningProfile::getInstance()->user_uri())
	{
		removeRecvLocalMedia(getLocalPreviewMediaID(remoteTerminal->virtualIndex).toStdString());
		return;
	}

	//判断对方是否为小流
	int index, seet;
	bool isSmallVideo = false;
	MediaManager::getInstance()->GetRecvMemberPositionFromURI(QString::fromStdString(remoteMemberURI), 
		&index, &seet, &isSmallVideo);
	/*if(RunningConfig::Instance()->isEnableSmallVideo() && seet > 1)
	{
		isSmallVideo = true;
	}*/

    std::string multiAddr = _currentConfInfo->multicastAddress;
    string localAddress;

	// 停止接收单播
    if( !ConferenceManager::checkSupportMulticast( multiAddr ) )
    {
        //如果不是组播，则使用虚拟终端号发出停止对方发送媒体流的请求
        localAddress = RunningProfile::getInstance()->ip_address();

		QString szContent = QString("<?xml version=\"1.0\"?><coolview command=\"StopMedia\"><uri>") + 
			QString::fromStdString(remoteTerminal->uri) + "</uri><virtualUri>" + QString::fromStdString(remoteTerminal->virtualUri) + 
			"</virtualUri><ip>" + localAddress.c_str() + "</ip><type>%1</type></coolview>";
		szContent = szContent.arg(isSmallVideo ? "stream_small" : "stream");

        QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
        _sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent);
    }
	// else 组播，什么也不干

    //停止接收进程
    MediaManager::getInstance()->RemoveRecvMedia( remoteTerminal->virtualUri.c_str() );

    //删除位置消息
    MediaManager::getInstance()->RemoveRecvMemberPosition( remoteTerminal->virtualUri.c_str() );

	//通知界面响应
	streamMediaReceiveStoppedSignal(QString::fromStdString(remoteTerminal->virtualUri));
    
}

void ConferenceManager::removeRecvLocalMedia( const std::string localMediaID )
{
	std::string localAddress = "127.0.0.1";
	std::string baseMediaID;
	int virtualIndex = 0;
	int videoPort = 0;
	int audioPort = 0;

	if (isLocalPreviewMediaID(localMediaID.c_str()))
	{
		virtualIndex = getVirtualIndexFromLocalPreviewMediaID(localMediaID.c_str());
		baseMediaID = LOCAL_PREVIEW_MEDIA_ID;
	}
	else if (isLocalRecordMediaID(localMediaID.c_str()))
	{
		virtualIndex = getVirtualIndexFromLocalRecordMediaID(localMediaID.c_str());
		baseMediaID = LOCAL_RECORD_MEDIA_ID;
	}
	else
	{
		return;
	}

	//因为存在本地回显、本地录制等IP均为127.0.0.1的接收进程，故需要以URI查找
	ReceiverInfo * receiver  = _receiverInfo.FindRemoteReceiverByURI(virtualIndex, baseMediaID);
	if (receiver)
	{
		assert(receiver->virtualIndex == virtualIndex);
		//移除发送端
		MediaManager::getInstance()->RemoveSendMedia( receiver->virtualIndex, 
			receiver->recvIP.c_str() , 
			receiver->audioPort , 
			receiver->videoPort,
			receiver->smallVideo);
		//删除发送信息
		_receiverInfo.RemoveRemoteReceiverByURI(receiver->virtualIndex, baseMediaID);
	}

	//停止接收进程
	MediaManager::getInstance()->RemoveRecvMedia( localMediaID.c_str() );

	//删除位置消息
	MediaManager::getInstance()->RemoveRecvMemberPosition( localMediaID.c_str() );
}

void ConferenceManager::changeMemberSeat( const std::string remoteMemberURI , const int screenIndex , const int seat )
{
	std::string uri = remoteMemberURI;
	if (checkIsInConference())
	{
		//当在会议中时,检查URI是否是本终端的URI,若是则转换为本地回显ID
		TerminalInfo* remoteTerminal = FindTerminal( _terminalList , remoteMemberURI );
		if (remoteTerminal && remoteTerminal->uri == RunningProfile::getInstance()->user_uri())
		{
			uri = getLocalPreviewMediaID(remoteTerminal->virtualIndex).toStdString();
		}
	}
	MediaManager::getInstance()->ChangeMemberSeat(uri.c_str(), screenIndex, seat);
}

void ConferenceManager::removeRecvScreenMedia(  const QString& memberURI, const int screenIndex)
{
    if( !checkIsInConference() )
    {
        qDebug("没有加入任何会议!");
        return;
    }

    TerminalInfo* remoteTerminal = FindTerminal( _terminalList , memberURI.toStdString());
    if( remoteTerminal != NULL )
    {
        std::string multiAddr = remoteTerminal->multicastAddress;
        string localAddress;

		//由于屏幕流采用TCP，因此不支持组播...zhhua.sun 2012/02/28

		//发出停止对方发送媒体流的请求
		localAddress = RunningProfile::getInstance()->ip_address();

		QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StopMedia\"><uri>" +
			QString::fromStdString(remoteTerminal->uri) + "</uri><ip>" + localAddress.c_str() + "</ip><type>screen</type></coolview>";
		QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
		_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent);

        //释放屏幕流
        MediaManager::getInstance()->RemoveRecvScreenMedia( memberURI );

        _snprintf(__globalMessege , sizeof(__globalMessege) , "@Stop Recv Screen From %s,Port:%d \n", PPT_TERMINAL_URI, _currentConfInfo->pptport );
        CV_LOG_DEBUG( __globalMessege );
    }

}

void ConferenceManager::sendTextMessage( const std::string& membername , const std::string& message )
{
    QString cvMessage = "<?xml version=\"1.0\"?><coolview command=\"Text\"><message>" + QString::fromStdString(message) + "</message></coolview>";
    QString username = QString::fromStdString(RunningProfile::getInstance()->username());
    if( membername == "ALL" )
    {
        for( MemberVector::iterator it  = _memberList.begin() ; it != _memberList.end() ; it++ )
        {
            MemberInfo* member = *it;
            QString memberURI = "sip:" + QString::fromStdString( member->MemberURI);
            _sipWrapperProxy->startConference( username, memberURI , cvMessage );
        }
    }
    else
    {
        for( MemberVector::iterator it  = _memberList.begin() ; it != _memberList.end() ; it++ )
        {
            MemberInfo* member = *it;
            if( member->MemberURI.find(membername) != -1 )
            {
                QString memberURI = "sip:" + QString::fromStdString( member->MemberURI);
                _sipWrapperProxy->startConference( username, memberURI, cvMessage);
                break;
            }
        }
    }
}

bool ConferenceManager::getCurrentMulticastAddress( string* ipaddress )
{
    if(!checkIsInConference())
        return false;

    //从会议信息判断
    if( _currentConfInfo && checkSupportMulticast(_currentConfInfo->multicastAddress ) )
    {
        *ipaddress = _currentConfInfo->multicastAddress;
        return true;
    }

    //从成员列表判断
    int memberCount = _memberList.size();
    for( MemberVector::iterator it = _memberList.begin() ; it != _memberList.end(); it++ )
    {
        MemberInfo* member = *it;
        if( checkSupportMulticast(member->multicastAddress) )
        {
            *ipaddress = member->multicastAddress;
            return true;
        }
    }

    return false;


}

int ConferenceManager::joinConference( const string& curFocusURI )
{
    //控制服务器允许进入会议，开始进入一个新会议

    //检查当前用户是否已经进入了一个会议，则不处理
    if(checkIsInConference() && _memberList.size() > 0 && _receiverInfo.ReceiverCount() > 0)
    {
        return	-1;
    }
    //根据记录的会议URI找到指定会议信息，并进入
    ConfInfo* conf = conf_find_by_uri(_confList, curFocusURI);
    if(!conf)
        return -1;

	//向主持人请求当前主讲人的信息...zhenHua.sun 2011-08-17
	_speakerURI = "";
	_TerminalChairmanURI ="";
	_currentScreenShareUri="";

    _focusURI = curFocusURI;
    _confTitle = conf->Title;
    _confMode = conf->ControlMode;
    _hostURI = conf->HostURI;
    _currentConfInfo = conf;
	_totalTermianlNum=0;

	_currentConfInfo = conf_find_by_uri(_confList, _focusURI);
	//如果是讨论会的话则允许任何用户发布屏幕共享
	if( _confMode == "discuss" )
	{
		MediaManager::getInstance()->ControlScreenShare( true );
	}else
	{
		MediaManager::getInstance()->ControlScreenShare( false );
	}
	

    //释放成员信息
    if(_memberList.size() > 0)
		CVMsgParser::ReleaseMemberList(_memberList);

	for (TerminalMap::iterator it = _terminalList.begin(); it != _terminalList.end(); ++it)
	{
		CVMsgParser::ReleaseTerminalList( it->second );
	}
	_terminalList.clear();

    //引发成员列表更新事件
    cvMemberListReceivedSignal(_memberList, _confTitle);

    string currentUser = RunningProfile::getInstance()->user_uri();

    //改变状态
    _confState = IsInConference;
    emit cvJoinConferenceSuccessSignal(_focusURI);

    ////启动媒体传输，但不发送数据
    //if(_confMode == "host" && currentUser != _hostURI)
    //{
    //    MediaManager::getInstance()->AddSendMedia(DEFAULT_IP_ADDRESS , 0 ,
    //            RunningConfig::getInstance()->VideoCodec() == "" ? 0 : DEFAULT_VIDEO_PORT
    //                                             );
    //}
    //else
    //{
    //    MediaManager::getInstance()->AddSendMedia(DEFAULT_IP_ADDRESS ,
    //            RunningConfig::getInstance()->AudioCodec() == "" ? 0 : DEFAULT_AUDIO_PORT,
    //            RunningConfig::getInstance()->VideoCodec() == "" ? 0 : DEFAULT_VIDEO_PORT
    //                                             );
    //}


    //For Testing:向Qos服务器报告登陆信息，发送到模拟器
    //sendQosJoinConfReport(curFocusURI);

    //启动计时器，定时向会议服务器发送在线消息
    _onlineInfoTimer->start(5000);
	//启动心跳超时计时器
	_serverHeartbeatTimer->start(180000);

    //记录当前的会议信息，用于QoS信息的发送..zhenHua.sun 20110315
    CVIniConfig::getInstance().setCurrentConferenceUri(_currentConfInfo->URI.c_str());
    CVIniConfig::getInstance().setCurrentConferenceCid(atoi(_currentConfInfo->CID.c_str()));

    return 0;
}
/**
 * @brief 拒绝加入指定会议，通常用于QoS控制
 */
int ConferenceManager::rejectToJoinConference( const string& curFocusURI, const string& reason )
{
    if (_confState != WaitingPermission)
    {
        //没有申请加入，不处理拒绝消息
        return -1;
    }
    //处理

    //通知上层被拒绝加入会议
    ConfInfo* conf = conf_find_by_uri( _confList , curFocusURI );
    if( conf == NULL )
        return -2;
    
    cvJoinRejectionSignal( curFocusURI , reason );

    //改变状态
    _confState = JoinRejected;
    return 0;
}

void ConferenceManager::sendRTPStatToQoSServer( const RtpStatItem& rtpstat )
{
    if( !checkIsInConference() )
        return;
    //检查rtpstat是否合法
    if(rtpstat.local_addr_len == 0 ||  rtpstat.rem_addr_len == 0)
        return;

    string cid = _currentConfInfo != NULL ? _currentConfInfo->CID : "0";
    RunningProfile* profile = RunningProfile::getInstance();
    string reporterUri = profile->user_uri();
    string reporterIP = profile->ip_address();
    string senderUri = "";
    string senderIP = "";
    int senderPort = 0;
    string receiverUri = "";
    string receiverIP = "";
    int receiverPort = 0;
    int bandwidth = 0;
    if(rtpstat.rtp_endpoint_type == eET_Sender)  //发送者报告
    {
        senderUri = reporterUri;
        senderIP = reporterIP;
        senderPort = rtpstat.local_port;
        receiverIP = rtpstat.rem_addr;
        receiverPort = rtpstat.rem_port;
		for (int i = 0; i < RunningConfig::Instance()->VideoCaptureDeviceCount(); ++i)
		{
			ReceiverInfo * recvInfo = _receiverInfo.FindRemoteReceiverByIP(i, rtpstat.rem_addr);
			if (recvInfo)
			{
				receiverUri = recvInfo->MemberURI;
				break;
			}
		}
        bandwidth = rtpstat.send_bandwidth;
    }
    else    //接收者报告
    {
        senderIP = rtpstat.rem_addr;
        senderPort = rtpstat.rem_port;
        //根据端口查找发送端的uri
        TerminalInfo * info = terminal_find_by_port(_terminalList, rtpstat.local_port, (int)rtpstat.media_type);
        if (info)
            senderUri  = info->uri;
        receiverUri = reporterUri;
        receiverIP = reporterIP;
        receiverPort = rtpstat.local_port;
        bandwidth = rtpstat.recv_bandwidth;
    }
    string flowClass;
    if(rtpstat.media_type == eMT_Video)
        flowClass = "video";
    else if(rtpstat.media_type == eMT_Audio)
        flowClass = "audio";
    else
        flowClass = "app";

    char msg[1400];
    memset(msg, 0, sizeof(msg));
    const char *fmtQosReport = "<?xml version=\"1.0\"?>"
                               "<coolview command=\"qos\" type=\"clientQosReport\">"
                               "<cid>%s</cid>"
                               "<messageSenderIP>%s</messageSenderIP>"
                               "<messageSenderUri>%s</messageSenderUri>"
                               "<seq>%d</seq>"
                               "<endpointType>%s</endpointType>"
                               "<senderIP>%s</senderIP>"
                               "<senderUri>%s</senderUri>"
                               "<senderPort>%d</senderPort>"
                               "<receiverIP>%s</receiverIP>"
                               "<receiverUri>%s</receiverUri>"
                               "<receiverPort>%d</receiverPort>"
                               "<flowClass>%s</flowClass>"
                               "<bandwidth>%d</bandwidth>"
                               "<lossRate>%.2f</lossRate>"
                               "<delay>%d</delay>"
                               "<jitter>%d</jitter>"
                               "<interval>%d</interval>"
                               "<timestamp>%I64d</timestamp>"
                               "</coolview>";
    int msgLenth =	sprintf(msg, fmtQosReport,
                            cid.c_str(),
                            reporterIP.c_str(),
                            reporterUri.c_str(),
                            rtpstat.seqnumber,
                            rtpstat.rtp_endpoint_type == eET_Sender ? "sender" : "receiver",
                            senderIP.c_str(),
                            senderUri.c_str(),
                            senderPort,
                            receiverIP.c_str(),
                            receiverUri.c_str(),
                            receiverPort,                          
                            flowClass.c_str(),
                            bandwidth,
                            rtpstat.lost,
                            rtpstat.delay,
                            rtpstat.jitter,
                            rtpstat.interval,
                            rtpstat.timestamp);

    //取消sip发送报告，使用UDP方式发送
    //_sipWrapperProxy->sendQosPara( QString::fromStdString(profile->getUserName()) ,getUniSipURI(_qosServerUri.c_str()) ,  msg );

    sendUdpMessageToQosServer(msg, msgLenth);

	receiveRtpStatSignal(rtpstat);

}

void ConferenceManager::sendRecStatToServer(const RecStatItem& recstat)
{
	if (recstat.statType == REC_STAT_FILE_CLOSE)
	{
		QString strFileName = QString::fromLocal8Bit(recstat.rec.file);

		QByteArray data;
		QDataStream out( &data , QIODevice::WriteOnly );
		out << QString(strFileName);

		_txMonitor->ReportRecStat(RecStat_FileClose, data);
	}

	receiveRecStatSignal(recstat);
}

void ConferenceManager::recvStartMediaReply( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
    if( !checkIsInConference() )
    {
        qDebug("没有加入任何会议!");
        return;
    }

    //MemberInfo * remoteMember = member_find_by_uri( _memberList, cvMessageParser.GetMemberUri() );
    //if( remoteMember==NULL )
    //{
    //	return;
    //}

    QString remoteURI = cvMessageParser.GetMemberUri().c_str();
    QString type = cvMessageParser.GetType().c_str();
	QString remoteVirtualURI = cvMessageParser.GetVirtualUri().c_str();

    //读取会议允许值
    string permission = cvMessageParser.GetPermission();
    if( permission != "true" )
	{
		//TODO:设置为特殊状态
		MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , "video" , UiMediaState_Ready,true); //先关闭等待标识
		MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , "video" , UiMediaState_Stop,true);
		MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , "audio" , UiMediaState_Stop,true );
        return;
	}

	if (remoteVirtualURI.isEmpty()) // 兼容旧版没有virtualUri的情况
	{
		remoteVirtualURI = "v1#" + remoteURI;
	}

    //更新媒体流的状态
    if( type == "screen" )
    {
        MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , type , UiMediaState_Ready,true);
    }
    if( type == "stream" || type=="smallStream" )
    {
        MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , "video" , UiMediaState_Ready,true);
		MediaManager::getInstance()->ChangeMediaState(remoteVirtualURI , "audio" , UiMediaState_Ready,true );

		//通知界面对方开始发送数据
		streamMediaReceiveStartedSignal(QString::fromStdString(remoteVirtualURI.toStdString()));
    }

    //std::string multiAddr = remoteMember->multicastAddress;
    //string localAddress;
    //if( ConferenceManager::checkSupportMulticast( multiAddr ) )
    //{
    //	getCurrentMulticastAddress(&localAddress);
    //}else
    //{
    //	localAddress = RunningProfile::getInstance()->getIpAddress();
    //}

    //int videoWidth;
    //int videoHeight;
    //char audioCodec[20] = {0};
    //getMediaParamFromSdp( &videoWidth , &videoHeight ,audioCodec, remoteMember->Sdp.c_str() );
    //std::string sAudioCodec( audioCodec );
    //int remoteAudioPort = sAudioCodec.size()>0 ? remoteMember->audioPort : 0;
    //int remoteVideoPort = (videoHeight==0||videoWidth==0) ? 0 : remoteMember->videoPort;

    ////获取远程用户的名字
    //QString remoteUsername = QString::fromStdString(remoteMember->MemberURI);
    //int index = remoteUsername.indexOf( "@" );
    //if( index>0 )
    //	remoteUsername = remoteUsername.left(index );
    //RecvGraphInfo info;
    //info.initial( localAddress.c_str(), remoteAudioPort  , "SPEEX" , 16000, 16 , 1 , localAddress.c_str() ,
    //	remoteVideoPort , 0 , videoWidth , videoHeight , qPrintable(remoteUsername));
    //MediaManager::getInstance()->AddRecvMedia( info );
}

void ConferenceManager::recvStartSendMediaCommand( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
	string remoteURI = cvMessageParser.GetMemberUri(); // 对方URI
	string myVirtualURI = cvMessageParser.GetVirtualUri(); // 本机虚拟终端号
	string type = cvMessageParser.GetType();

	if (myVirtualURI.empty()) // 兼容非多路支持的旧版
	{
		myVirtualURI = "v1#" + RunningProfile::getInstance()->user_uri();
	}

	TerminalInfo* currentTerminal = FindTerminal( _terminalList , myVirtualURI );

    string permission = "false";
	if( type=="" || type=="stream" || type=="smallStream" )
	{
		//检查type是否能被处理
		if (currentTerminal && 
			RunningConfig::Instance()->VideoCaptureDevice(currentTerminal->virtualIndex).devicePath != "")
		{
			permission = "true";
		}
		if (type=="smallStream" && !RunningConfig::Instance()->isEnableSmallVideo())
		{
			//本终端不支持小流时
			permission = "false";
		}
	}
	else if (type=="screen")
	{
		permission = "true";
	}

    if( remoteURI.size() > 0 )
    {
        QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StartMediaRelayReply\"><uri>" +
                            QString::fromStdString(remoteURI) + "</uri><virtualUri>" + 
							QString::fromStdString(myVirtualURI) + "</virtualUri><permission>" +
                            QString::fromStdString(permission) + "</permission><type>" +
                            QString::fromStdString(type) + "</type></coolview>";
        _sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
    }

    if( permission == "false" )
    {
		return;
	}

    //获取对方地址，单播/组播IP地址
    string remoteIP = cvMessageParser.GetReceiverIP();

    if( type == "" || type == "stream" || type=="smallStream")
    {
        int videoPort = 0, audioPort = 0;

        //判断是否组播会议 , 单播会议目标端口是对方的，组播会议目标端口是自己的
        if(checkCurrentIsMulticastConference())
        {
            // 组播会议中进入会议即已经在发送媒体流，因此不进行任何操作
            return;
        }

        // 单播会议使用对方IP地址、发送到自己的端口
        if (!currentTerminal)
            return;

		audioPort = RunningConfig::Instance()->AudioCodec() == "" ? 0 : currentTerminal->audioPort;

		bool enableSmallStream = false;
		if( type=="smallStream" )
			enableSmallStream = true; // TODO:如果本终端不支持小流，将导致对方收不到数据

		{
			VCapSetting setting = RunningConfig::Instance()->VideoCaptureDevice(currentTerminal->virtualIndex);
			if( setting.videoCodec == "")
			{
				videoPort = 0;
			}else
			{
				videoPort = enableSmallStream ? currentTerminal->smallVideoPort : currentTerminal->videoPort;
			}
		}

		MediaManager::getInstance()->AddSendMedia( currentTerminal->virtualIndex, remoteIP.c_str(), audioPort , videoPort, enableSmallStream,  0);

        //记录对方信息，以便删除
		_receiverInfo.AddRemoteReceiver(currentTerminal->virtualIndex, remoteURI, remoteIP, videoPort, audioPort, enableSmallStream);

		_snprintf( __globalMessege , sizeof(__globalMessege) , "@Start Send Media To:%s , IP:%s , videoPort:%d , audioPort:%d, audioEnable:%d" ,
                    remoteURI.c_str() , remoteIP.c_str() , videoPort , audioPort, true );
        CV_LOG_DEBUG(__globalMessege);

			
		/************************************************************************/  
		//下面三行代码已经荒废，原本实现“在接收方要求接收视频之前，询问发送方，是否愿意发送给对方”的功能   			
		/*TerminalInfo* terminal = FindTerminal( _terminalList , remoteURI );
		if( terminal )
		{
			cvRemoteTerminalAskForMediaSignal( remoteURI , terminal->name);
		}*/
		/************************************************************************/

    }
    else if( type == "screen" )
    {
		//由于屏幕流采用TCP，因此不支持组播...zhhua.sun 2012/02/28
		MediaManager::getInstance()->AddSendScreenMediaDst( _currentConfInfo->pptport , remoteIP.c_str() );
			
		_snprintf( __globalMessege , sizeof(__globalMessege) , "@Start Send Media To:%s , IP:%s , pptPort:%d" ,
			remoteURI.c_str() , _currentConfInfo->multicastAddress.c_str() , _currentConfInfo->pptport );
        CV_LOG_DEBUG(__globalMessege);
    }
}

void ConferenceManager::recvStopMediaReply( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
    ////关闭媒体流
    if( !checkIsInConference() )
    {
        qDebug("没有加入任何会议!");
        return;
    }

    //MemberInfo * remoteMember = member_find_by_uri( _memberList, cvMessageParser.GetMemberUri() );
    //if( remoteMember==NULL )
    //{
    //	return;
    //}

    //读取会议允许值
    string permission = cvMessageParser.GetPermission();
    if( permission != "true" )
        return;

    //////获取远程用户的名字
    //QString remoteUsername = remoteMember->MemberURI.c_str();
    //int index = remoteUsername.indexOf( "@" );
    //if( index>0 )
    //	remoteUsername = remoteUsername.left(index );
    //MediaManager::getInstance()->RemoveRecvMedia( remoteUsername );
    //
    ////删除位置消息
    //MediaManager::getInstance()->RemoveRecvMemberPosition( remoteMember->MemberURI.c_str() );
}

void ConferenceManager::recvStopSendMediaCommand( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
    //是否允许
    string permission = "true";

    string remoteURI = cvMessageParser.GetMemberUri();
	string myVirtualURI = cvMessageParser.GetVirtualUri();
	string type = cvMessageParser.GetType();

	if (myVirtualURI.empty()) // 兼容非多路支持的旧版
	{
		myVirtualURI = "v1#" + RunningProfile::getInstance()->user_uri();
	}

    if( remoteURI.size() > 0 )
    {
        QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StopMediaRelayReply\"><uri>" +
                            QString::fromStdString(remoteURI) + "</uri><permission>" +
                            QString::fromStdString(permission) + "</permission><type>" +
                            QString::fromStdString(type) + "</type></coolview>";
        _sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
    }

    if( permission == "false" )
    {
		return;
	}

    string remoteIP = cvMessageParser.GetReceiverIP();

    if( type == "" || type == "stream" || type == "stream_small")
    {
        //组播会议不停止发送
        if(checkCurrentIsMulticastConference())
			return;

		// 单播会议使用对方IP地址，停止发送到自己的端口
		TerminalInfo* currentTerminal = FindTerminal( _terminalList , myVirtualURI );
		if (!currentTerminal)
			return;

		ReceiverInfo * receiver = _receiverInfo.FindRemoteReceiverByIP(currentTerminal->virtualIndex, remoteIP);
        if (receiver)
        {
            _snprintf( __globalMessege , sizeof(__globalMessege) , "@Stop Send Media To:%s , IP:%s , videoPort:%d , audioPort:%d, smallVideo:%d" ,
                        receiver->MemberURI.c_str() , receiver->recvIP.c_str() , receiver->videoPort , receiver->audioPort, receiver->smallVideo );
            CV_LOG_DEBUG(__globalMessege);
			assert(receiver->smallVideo == (type == "stream_small"));

            MediaManager::getInstance()->RemoveSendMedia( currentTerminal->virtualIndex, receiver->recvIP.c_str() ,
                    receiver->audioPort ,
                    receiver->videoPort,
					receiver->smallVideo);
            //删除发送信息
			_receiverInfo.RemoveRemoteReceiverByIP(currentTerminal->virtualIndex, remoteIP);
        }
    }
    else if( type == "screen" )
	{
		//由于屏幕流采用TCP，因此不支持组播...zhhua.sun 2012/02/28
		MediaManager::getInstance()->RemoveSendScreenMediaDst( _currentConfInfo->pptport , remoteIP.c_str() );
    }
}

// 已不使用 - Liaokz
void ConferenceManager::_SipAccountLogout( const std::string& sipURI )
{
    //查找成员列表
    for( int i = 0 ; i < _memberList.size() ; i++ )
    {
        MemberInfo* member = _memberList.at(i);
        if( sipURI == member->MemberURI )
        {
            member->status = "Not Available";
			//如果是主席用户账号退出了，要更新主席终端账号
			if(sipURI == _currentConfInfo->HostURI)
			{			
				TerminalInfo* remoteTerminal = FindTerminal( _terminalList ,_TerminalChairmanURI );
				if(remoteTerminal!= NULL)
				{
					remoteTerminal->isChairmanTerminal="false";
					if(_confMode == "report")
					remoteTerminal->isSpeaking="false";
					cvUpgradeTerminalInfoSignal(*remoteTerminal);
				}
				_TerminalChairmanURI ="";
			}
			
            return;
        }
    }

    //查找终端列表
    if (_terminalList.find(sipURI) != _terminalList.end())
    {
		TerminalVector & virtualList = _terminalList[sipURI];
		for (TerminalVector::iterator it = virtualList.begin(); it != virtualList.end(); ++it)
		{
            TerminalInfo* terminal = *it;

            terminal->status = "Not Available";
			if(sipURI == _TerminalChairmanURI)
			{
				
				TerminalInfo* remoteTerminal = FindTerminal( _terminalList ,_TerminalChairmanURI );
				if(remoteTerminal!= NULL)
				{
					remoteTerminal->isChairmanTerminal="false";
					if(_confMode == "report")
						remoteTerminal->isSpeaking="false";
					cvUpgradeTerminalInfoSignal(*remoteTerminal);
				}
				_TerminalChairmanURI ="";
			}
		}
    }
}

int ConferenceManager::computeRequiredBandwidth()
{
    //根据当前视频配置计算所需要的带宽，由于参考带宽是离散值，因此取分辨率和帧率不小于当前分辨率和帧率的最小带宽。
    static int resolutionTable[] = {1279 * 719, 1280 * 720, 1280 * 720, 1280 * 720, 1280 * 720, 1920 * 1080};
    static int fpsTable[] = {50, 10, 15, 30, 50, 25};
    static int bandwidthTable[] = {1024, 1.5 * 1024, 2.5 * 1024, 4 * 1024, 6 * 1024};

    int requiredBandwidth = 0;
    int tableSize =  sizeof(bandwidthTable) / sizeof(bandwidthTable[0]);
    //int resolution = RunningConfig::Instance()->VideoHeight() * RunningConfig::Instance()->VideoWidth(); //已不再适合多路采集
	int resolution = 1920 * 1080; // TODO:暂时使用支持的最大分辨率
    int fps = 30;//RunningConfig::Instance()->VideoFps(); // TODO:暂时使用支持的最大分辨率

    for(int i = 0; i < tableSize; i++)
        if(resolutionTable[i] >= resolution && fpsTable[i] >= fps)
        {
            requiredBandwidth = bandwidthTable[i];
            break;
        };
    if(requiredBandwidth == 0)
        requiredBandwidth = bandwidthTable[tableSize - 1];
    return requiredBandwidth;

}

void ConferenceManager::saveQoSServerInfo( const MsgQoSServerInfo & serverInfo )
{
    //保存qos server sip uri
    if (!serverInfo.sipUri.empty())
    {
        _qosServerUri = serverInfo.sipUri;
        CVIniConfig &inst = CVIniConfig::getInstance();
        inst.setQosServerHostIP(serverInfo.ip.c_str());
        inst.setQosServerSipUri(serverInfo.sipUri.c_str());
        inst.setQosServerTcpPort(serverInfo.tcpPort);
        inst.setQosServerUdpPort(serverInfo.operationUdpPort);
        inst.saveConfig();

        //CVIniConfig::getInstance().setQosServerSipUri(_qosServerUri.c_str());
        RunningConfig::Instance()->setQosServerURI(QString::fromStdString(_qosServerUri));
    }

    //发送一条xml消息给网络测量进程，通知其修改QoS服务器信息
    char content[500];
    memset(content, 0, sizeof(content));
    const char* formatBuffer = "<?xml version=\"1.0\"?>"
                               "<coolview command=\"qos\" type=\"QoSServerInfo\">"
                               "<ip>%s</ip>"
                               "<simulatetestTCPPort>%d</simulatetestTCPPort>"
                               "<simulatetestUDPPort>%d</simulatetestUDPPort>"
                               "</coolview>";
    sprintf(content,
            formatBuffer,
            serverInfo.ip.c_str(),
            serverInfo.tcpPort,
            serverInfo.simulateTestUdpPort);

    sendMessageToNetworkMeasurer(content, strlen(content));
}

void ConferenceManager::onlineInfoSlot()
{
    char* szContent = "<?xml version=\"1.0\"?><coolview command=\"Online\"><type>heartbeat</type></coolview>";
    if( checkIsInConference() )
    {
        makeInfoCall( RunningProfile::getInstance()->current_conference_uri() , szContent );
    }
}

/**
 * @brief 告知所有参会终端的媒体发送状态，并进行对媒体流进行控制
 * @param type 媒体类型，0为音频，1为视频
 * @param enable 如果发送媒体流，则为true，否则为false
 */
void ConferenceManager::controlSendMedia( const int&type , const bool enable )
{
    QString sEnable;
    if( enable )
        sEnable = "true";
    else
        sEnable = "false";

		if( type == 0 )
		{
			
			MediaManager::getInstance()->ControlAudioSend( enable );
			QString szContent = "<?xml version=\"1.0\"?><coolview command=\"SendMediaControl\"><uri>" +
				QString::fromStdString(RunningProfile::getInstance()->user_uri()) + "</uri><type>audio</type><enable>" + sEnable + "</enable></coolview>";
			_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
			//if(RunningProfile:: getInstance()->getUserURI() == _TerminalChairmanURI)
			//{

			//}
			//else
			//{
			//	cvModifyStopMyAudioState(false);
			//}
		}
		else
		{
			
			MediaManager::getInstance()->ControlVideoSend(enable);
			QString szContent = "<?xml version=\"1.0\"?><coolview command=\"SendMediaControl\"><uri>" +
								QString::fromStdString(RunningProfile::getInstance()->user_uri()) + "</uri><type>video</type><enable>" + sEnable + "</enable></coolview>";
			_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
		}


}

void ConferenceManager::recvSendMediaControlCommand( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
    string uri = cvMessageParser.GetMemberUri();
    string mediaType = cvMessageParser.GetType();
    string enable  = cvMessageParser.GetEnable();

    if( uri.size() > 0 && mediaType.size() > 0 && enable.size() > 0 )
    {
        //if( enable=="true" )
        //	cvRTSendMediaStateChanged( uri , mediaType , true );
        //else
        //	cvRTSendMediaStateChanged(uri , mediaType , false );
        if( enable == "true" )
            MediaManager::getInstance()->ChangeMediaState( uri.c_str() , mediaType.c_str() , UiMediaState_Run,true);
        else
            MediaManager::getInstance()->ChangeMediaState( uri.c_str() , mediaType.c_str(), UiMediaState_Stop,true);
    }
}

void ConferenceManager::createSendScreenMedia( const QRect wnd )
{
    if( this->checkIsInConference())
    {
        MediaManager::getInstance()->CreateSendScreenMedia( _currentConfInfo->pptport , _currentConfInfo->multicastAddress.c_str(), wnd );
        _snprintf( __globalMessege , sizeof(__globalMessege ), "Screen Send Media:%d , %s,%x", _currentConfInfo->pptport , _currentConfInfo->multicastAddress.c_str() , wnd );
        CV_LOG_DEBUG(__globalMessege );
    }
}

void ConferenceManager::createRecvScreenMedia( const QString& memberURI, const int screenIndex  )
{
    //对于发送PPT的终端无法创建屏幕流
    if( this->checkIsInConference() )
    {
        string localAddress;
        TerminalInfo* remoteTerminal = FindTerminal( _terminalList , memberURI.toStdString() );
        if( remoteTerminal != NULL )
        {
            if( ConferenceManager::checkSupportMulticast( _currentConfInfo->multicastAddress ) )
            {
                localAddress = _currentConfInfo->multicastAddress;
            }
            else
            {
                localAddress = RunningProfile::getInstance()->ip_address();

                //如果不是组播会议，那么需要发送Sip消息要求对方用户发送媒体流

                QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StartMedia\"><uri>" +
                                    QString::fromStdString(remoteTerminal->uri) + "</uri><ip>" + localAddress.c_str() + "</ip><type>screen</type></coolview>";
                QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
                _sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
            }

            MediaManager::getInstance()->CreateRecvScreenMedia( memberURI, _currentConfInfo->pptport , localAddress.c_str(), 0 , screenIndex );
        }

    }
}

void ConferenceManager::sendUdpMessageToQosServer( char * msg, int msgLenth )
{
    if(!_qosReportSender.isConnected())
    {
        CVIniConfig &inst = CVIniConfig::getInstance();
        if(!_qosReportSender.connect(inst.getQosServerHostIP(), inst.getQosServerUdpPort()))
        {
            return;
        }
    }
    _qosReportSender.send(msg, msgLenth);
}

///接收到选择主讲人的信息
void ConferenceManager::recvPermitSpeakInfo( CVMsgParser& cvMessageParser , const std::string& fromUri )
{
	
	string uri = cvMessageParser.GetTerminalUri();
	
	if (_speakerURI==uri)
	{
		return;
	}

	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri );
	if(remoteTerminal!= NULL)
	{
		remoteTerminal->isSpeaking="true";
		remoteTerminal->isMainSpeaker="true";
	}
	if( uri==RunningProfile::getInstance()->user_uri() )
	{
		MediaManager::getInstance()->ControlAudioSend( true );
		MediaManager::getInstance()->ControlScreenShare(true);
	} 
	else
	{
		
		MediaManager::getInstance()->ChangeMediaState(uri.c_str(),"audio",UiMediaState_Run,true);
	}
	cvSetMemberWindowToFirstPositionSignal(QString ::fromStdString(uri));
	if(remoteTerminal!= NULL)
	{
		cvUpgradeTerminalInfoSignal(*remoteTerminal);
	}
	forbidSpeakByURI(_speakerURI);
	_speakerURI=uri;
	

}

///接收到取消主讲人的信息
void ConferenceManager::recvForbidSpeakInfo( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
	string uri = cvMessageParser.GetTerminalUri();
	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri );
	if(remoteTerminal!= NULL)
	{
		remoteTerminal->isMainSpeaker="false";
		remoteTerminal->isHandUp="false";
	}

	if(_confMode=="report")
	{
		if( uri==RunningProfile::getInstance()->user_uri() )
		{
			MediaManager::getInstance()->ControlScreenShare(false);
		}

		if(remoteTerminal!= NULL)
		{
			remoteTerminal->isSpeaking="false";
		}
		if( uri==RunningProfile::getInstance()->user_uri() )
		{
			MediaManager::getInstance()->ControlAudioSend( false );
		}
		else
		{
			
			MediaManager::getInstance()->ChangeMediaState(uri.c_str(),"audio",UiMediaState_Stop,true);
		}
	}
	
	if(remoteTerminal!= NULL)
	{
		cvUpgradeTerminalInfoSignal(*remoteTerminal);
	}
}

void ConferenceManager:: forbidSpeakByURI(const std::string& Uri)
{
	string uri = Uri;

	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri );
	if(remoteTerminal!= NULL)
	{
		remoteTerminal->isMainSpeaker="false";
		remoteTerminal->isHandUp="false";
	}
	if( uri==RunningProfile::getInstance()->user_uri() )
	{
		MediaManager::getInstance()->ControlScreenShare(false);
	}
	if(_confMode=="report")
	{

		if(remoteTerminal!= NULL)
		{
			remoteTerminal->isSpeaking="false";
		}
		if( uri==RunningProfile::getInstance()->user_uri() )
		{
			MediaManager::getInstance()->ControlAudioSend( false );
		}
		else
		{

			MediaManager::getInstance()->ChangeMediaState(uri.c_str(),"audio",UiMediaState_Stop,true);
		}
	}

	if(remoteTerminal!= NULL)
	{
		cvUpgradeTerminalInfoSignal(*remoteTerminal);
	}
	
}

void ConferenceManager::queryMainSpeaker()
{
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"GetSpeakerTerminal\"></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
}


void ConferenceManager::queryChairmanTermianl()
{
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"GetChairmanTerminal\"></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
}

void ConferenceManager::queryCurrentScreenShareTermianl()
{
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"GetScreenShareTerminal\"></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
}


void ConferenceManager:: HandUpHandler(const bool handup)
{
	if(handup)
	{
		QString szContent = "<?xml version=\"1.0\"?><coolview command=\"HandUp\"><terminalUri>"+ QString::fromStdString(RunningProfile::getInstance()->user_uri())+"</terminalUri></coolview>";
		_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
	}
	else
	{
		QString szContent = "<?xml version=\"1.0\"?><coolview command=\"HandDown\"><terminalUri>"+ QString::fromStdString(RunningProfile::getInstance()->user_uri())+"</terminalUri></coolview>";
		_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
	}
}

string ConferenceManager:: getConfMode()
{

	return _confMode;
}

void ConferenceManager:: AllowSpeakHandler(QString uri)
{
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"SetSpeaker\"><terminalUri>"+uri+"</terminalUri></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
}

void ConferenceManager:: ForbidSpeakHandler(QString uri)
{
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"ForbidSpeaker\"><terminalUri>"+uri+"</terminalUri></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),szContent);
}

void ConferenceManager:: TerminalLogin(const std::string& username , const std::string& sipServer,QString ipv4 ,QString ipv6,QString myVersion,int screenNum)
{
	QString myIp= QString::fromStdString(RunningProfile::getInstance()->ip_address());
	QString uri=QString::fromStdString(RunningProfile::getInstance()->user_uri());
	QString cvMessage = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"TerminalLogin\"><ip>"+myIp+"</ip><ipv4>"
		+ipv4+"</ipv4><ipv6>"+ipv6+"</ipv6><version>"+myVersion+"</version><screenCount>"+QString::number(screenNum)+"</screenCount></coolview>";
	_sipWrapperProxy->startConference( QString::fromStdString(username) , QString::fromStdString(sipServer), cvMessage );
}


void ConferenceManager:: recvSaviInfo( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
	string res = cvMessageParser.GetResult();
		if(res== "true")
			RecvSaviSignal(true);
		else
			RecvSaviSignal(false);

	startConference( RunningProfile::getInstance()->username() , 
			RunningConfig::Instance()->GlobalService().toStdString());

}


void ConferenceManager:: recvChairmanTerminalHandler( CVMsgParser& cvMessageParser, const std::string& fromUri)
{

		string uri = cvMessageParser.GetTerminalUri();

		if(_TerminalChairmanURI == uri)
			return;

		TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri );
		if(remoteTerminal!= NULL)
		{
			remoteTerminal->isChairmanTerminal="true";
			remoteTerminal->isSpeaking="true";
			cvUpgradeTerminalInfoSignal(*remoteTerminal);
		}

		TerminalInfo* oldChairmanTerminal = FindTerminal( _terminalList , _TerminalChairmanURI );
		if(oldChairmanTerminal!= NULL)
		{
			oldChairmanTerminal->isChairmanTerminal="false";
			if(_confMode == "report")
			oldChairmanTerminal->isSpeaking="false";
			cvUpgradeTerminalInfoSignal(*oldChairmanTerminal);
		}

		_TerminalChairmanURI=uri;

}

void ConferenceManager::recvMediaControlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string uri = cvMessageParser.GetTerminalUri();
	string type =cvMessageParser.GetType();
	string enable =cvMessageParser.GetEnable();


	if(enable =="false")
	{
		if(type =="audio")
		{
			TerminalInfo* remoteTerminal = FindTerminal( _terminalList ,uri );
			if(remoteTerminal!= NULL)
			{
				remoteTerminal->isSpeaking="false";
				cvUpgradeTerminalInfoSignal(*remoteTerminal);
			}
			if( uri==RunningProfile::getInstance()->user_uri() )
			{
				MediaManager::getInstance()->ControlAudioSend( false );

			}
			else
			{			

				MediaManager::getInstance()->ChangeMediaState(uri.c_str(),"audio",UiMediaState_Stop,true);
			}

		}		

	}
	else
	{
		TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri );
		if(type =="audio")
		{
			if(remoteTerminal!= NULL)
			{
				remoteTerminal->isSpeaking="true";
				cvUpgradeTerminalInfoSignal(*remoteTerminal);
			}
			if( uri==RunningProfile::getInstance()->user_uri() )
			{
				MediaManager::getInstance()->ControlAudioSend( true );
			} 
			else
			{			

				MediaManager::getInstance()->ChangeMediaState(uri.c_str(),"audio",UiMediaState_Run,true);
			}

		}
	}
	
	

}

//告知其他终端创建/销毁当前终端的屏幕流接收进程
void ConferenceManager:: ScreenShareControlHandler(const bool enable)
{
	QString Enable="true";
	if(!enable)
	Enable="false";

	QString uri=QString::fromStdString(RunningProfile::getInstance()->user_uri());
	int port =_currentConfInfo->pptport;
	
	string localAddress="";
	TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri.toStdString() );
	if( remoteTerminal != NULL )
	{
		//由于屏幕流采用TCP实现，因此不能支持组播...zhenhua.sun 2012/02/28
		localAddress = RunningProfile::getInstance()->ip_address();
	}

	QString cvMessage = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"ShareScreenControl\"><terminalUri>"+uri+"</terminalUri><enable>"+
		Enable+"</enable><ip>"+QString::fromStdString(localAddress)+"</ip><port>"+QString::number(port)+"</port></coolview>";
	_sipWrapperProxy->makeInfoCall(_currentConfInfo->URI.c_str(),cvMessage);

	if(!enable)
	{
		MediaManager::getInstance()->RemoveScreenShareSend();
	}
}



void ConferenceManager::ChairmanChangeNotify(bool enable) {
	TelecontrollerIf *teleControlProxy = new TelecontrollerIf( _GetTelecontrollerServiceName("sock"),
		_GetTelecontrollerObjectPath("sock") , QDBusConnection::sessionBus() );;
	
	QString uri= "";
	if(enable)
		uri=QString ::fromStdString(_currentConfInfo->Chairman);

	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_PostMeetingInfo;
	out << uri;
	if( teleControlProxy )
	{
		teleControlProxy->TeleInfo(TELE_PostMeetingInfo,-1,output_array );
	}
}


void ConferenceManager::recvMainSpeakerTermianlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string uri = cvMessageParser.GetTerminalUri();
	_speakerURI=uri;

}

void ConferenceManager::recvChairmanTermianlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string uri = cvMessageParser.GetTerminalUri();
	_TerminalChairmanURI=uri;

}

void ConferenceManager::recvCurrentScreenShareTerminalInfo(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string uri = cvMessageParser.GetTerminalUri();
	_currentScreenShareUri=uri;

	if(_currentScreenShareUri != "" && _currentScreenShareUri != RunningProfile::getInstance()->user_uri())
	{
		recvMainSpeakerShareScreen(QString::fromStdString(_currentScreenShareUri));
	}	
}

void ConferenceManager::recvShareScreenControlInfo( CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	
	string uri = cvMessageParser.GetTerminalUri();
	string enable =cvMessageParser.GetEnable();

	if(enable == "true")
	{
		//对于发送PPT的终端无法创建屏幕流
		if( this->checkIsInConference() )
		{
			string localAddress;
			TerminalInfo* remoteTerminal = FindTerminal( _terminalList , uri);
			if( remoteTerminal != NULL )
			{
				//由于屏幕流采用TCP，因此不支持组播...zhhua.sun 2012/02/28
				localAddress = RunningProfile::getInstance()->ip_address();

				//先创建接收进程，在要求屏幕发送方连接到当前的接收进程
				MediaManager::getInstance()->CreateMainSpeakerRecvScreenMedia( QString:: fromStdString(uri), _currentConfInfo->pptport , localAddress.c_str(), 0 );

				Sleep(2000);


				////送Sip消息要求对方用户发送媒体流
				QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StartMedia\"><uri>" +
					QString::fromStdString(remoteTerminal->uri) + "</uri><ip>" + localAddress.c_str() + "</ip><type>screen</type></coolview>";
				QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
				_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );

				
			}

		}
	}
	else
	{
		MediaManager::getInstance()->RemoveRecvScreenMedia();
	}
}



void ConferenceManager::recvMainSpeakerShareScreen(QString & Uri)
{

	//对于发送PPT的终端无法创建屏幕流
	if( this->checkIsInConference() )
	{
		string localAddress;
		TerminalInfo* remoteTerminal = FindTerminal( _terminalList , Uri.toStdString());
		if( remoteTerminal != NULL )
		{
			//由于屏幕流采用TCP，因此不支持组播...zhhua.sun 2012/02/28
			localAddress = RunningProfile::getInstance()->ip_address();
			
			//先创建接收进程，在要求屏幕发送方连接到当前的接收进程
			MediaManager::getInstance()->CreateMainSpeakerRecvScreenMedia( Uri, _currentConfInfo->pptport , localAddress.c_str(), 0 );

			Sleep(2000);


			////如果不是组播会议，那么需要发送Sip消息要求对方用户发送媒体流
			QString szContent = "<?xml version=\"1.0\"?><coolview command=\"StartMedia\"><uri>" +
				QString::fromStdString(remoteTerminal->uri) + "</uri><ip>" + localAddress.c_str() + "</ip><type>screen</type></coolview>";
			QString remoteUniURI = QString("sip:") + _currentConfInfo->URI.c_str();
			_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
		}


	}


}


void ConferenceManager::TerminalLogOutSlot()
{
	string username=RunningProfile::getInstance()->username();
	QString cvMessage = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"TerminalLogOut\"></coolview>";
	_sipWrapperProxy->startConference( QString::fromStdString(username) , RunningConfig::Instance()->GlobalService(), cvMessage );
}


void ConferenceManager:: GetTermialList()
{
	if (RunningConfig::Instance()->isEnableHttp())
	{
		_httpWrapper->RequestTerminalList(QString::fromStdString(_currentConfInfo->CID));
	}
	else
	{
		QString szContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"GetTerminalList\"></coolview>";
		_sipWrapperProxy->makeInfoCall( _currentConfInfo->URI.c_str() , szContent );
	}
}

void ConferenceManager:: QueryCoferenceInfoBeforeFinishTerminal()
{
	queryMainSpeaker();
	queryChairmanTermianl();
	queryCurrentScreenShareTermianl();
}

void ConferenceManager:: StartServerTsetSlot()
{
	_isServerTest=true;
	
	string username=RunningProfile::getInstance()->username();
	QString cvMessage = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><coolview command=\"Online\"><type>test</type></coolview>";
	_sipWrapperProxy->startConference( QString::fromStdString(username) , RunningConfig::Instance()->GlobalService(), cvMessage );

	_serverTestCount.restart();
	_serverTestTimer->start(30000);

}

void ConferenceManager::recvOnlineMessageInfo(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	string type = cvMessageParser.GetType();

	//如果是手动测试终端与服务器的延迟，则进入分支
	if(type == "test")
	{  
		//没有超时
		if(_isServerTest)
		{
			int intervalTime = _serverTestCount.elapsed();
			_serverTestTimer->stop();
			_isServerTest=false;
			cvServerTestResultSignal(false,intervalTime);
		}
		else
		{
			return;
		}
		

	}
	else
	{
		//一般的心跳回复
		
		QString fromStr = RunningConfig::Instance()->GlobalService();
		int index = fromStr.indexOf( ":" );
		if( index > 0 )
			fromStr = fromStr.right( index );
		//收到来自GlobalService的心跳回复
		if(fromUri == fromStr.toStdString() )
		{
			_serverHeartbeatTimer->stop();
			//重新启动心跳超时计时器,如果等待三次都没有收到心跳，表示回复超时
			_serverHeartbeatTimer->start(180000);
		}
			
	}
}

void ConferenceManager:: severTestTimeOutSlot()
{
	_serverTestTimer->stop();
	_isServerTest=false;
	cvServerTestResultSignal(true,30000);
}

void ConferenceManager:: serverHeartbeatMessageTimeOutSlot()
{
	_serverHeartbeatTimer->stop();
	
	//超时处理
}

void ConferenceManager::RecoveryMediaProcess(QString mediaType, bool isSend, QString removeUserID)
{
	if("stream" != mediaType)
	{
		return;
	}

	if(!isSend)
	{
		bool isMulti =false;  //是否是组播
		bool usingSmallVideo = false; //是否使用小流
		int seet = -1;
		int screenIndex = -1;
			
		//MediaManager::getInstance()->AddRecvMemberPosition( remoteMemberURI.c_str() , screenIndex , seet );
		MediaManager::getInstance()->GetRecvMemberPositionFromName(removeUserID, &screenIndex, &seet, &usingSmallVideo);
			
		if(seet<0 || screenIndex<0)
			return;
			
		removeUserID =removeUserID+"@sip.ccnl.scut.edu.cn";
		//处理终端的媒体流
		TerminalInfo* remoteTerminal = FindTerminal( _terminalList , removeUserID.toStdString() );
		if( remoteTerminal == NULL )
		{
			return;
		}

		//获取远程终端的媒体参数
		int videoWidth;
		int videoHeight;
		int videoFps;
		char audioCodec[20] = {0};
		getMediaParamFromSdp( &videoWidth , &videoHeight , &videoFps, audioCodec, remoteTerminal->sdp.c_str() );
		std::string sAudioCodec( audioCodec );
		int remoteAudioPort = sAudioCodec.size() > 0 ? remoteTerminal->audioPort : 0;
		int remoteVideoPort;

		//如果不是第一个窗口，则使用小流
		if( RunningConfig::Instance()->isEnableSmallVideo() && 
			seet>1 )
			usingSmallVideo = true;

		std::string multiAddr =_currentConfInfo->multicastAddress;
		string localAddress;
		if( ConferenceManager::checkSupportMulticast( multiAddr ) )
		{
			localAddress = multiAddr;
			isMulti=true;
		}
		else
		{
			localAddress = RunningProfile::getInstance()->ip_address();
		}

		//建立媒体流
		if( usingSmallVideo) 
		{
			remoteVideoPort = (videoHeight == 0 || videoWidth == 0) ? 0 : remoteTerminal->smallVideoPort;

			//全高清摄像头的视频参数除以4后事460x270，但是编码器并不支持该视频格式，因此统一为320x180
			//...zhenhua.sun 2012/2/22
			videoWidth = MSDX_CONF_SMALL_VIDEO_WIDTH;
			videoHeight = MSDX_CONF_SMALL_VIDEO_HEIGHT;
			videoFps /=2;
		}else
		{
			remoteVideoPort = (videoHeight == 0 || videoWidth == 0) ? 0 : remoteTerminal->videoPort;
		}
		//获取远程用户的名字
		QString remoteUsername = QString::fromStdString(remoteTerminal->uri);
		int index = remoteUsername.indexOf( "@" );
		if( index > 0 )
			remoteUsername = remoteUsername.left(index );
		RecvGraphInfo info;
		info.initial( qPrintable(RunningConfig::Instance()->AudioCaptureDevice()), qPrintable(RunningConfig::Instance()->AudioOutputDevice()),
			localAddress.c_str(), remoteAudioPort  , "SPEEX" , 16000, 16 , 1 , localAddress.c_str() ,
			remoteVideoPort , 0 , videoWidth , videoHeight , videoFps, qPrintable(remoteUsername), remoteTerminal->name.c_str(),
			//RunningConfig::getInstance()->isEnableRecvAutoResync());
			false );
				
		MediaManager::getInstance()->RecoveryRecvMedia( info );
	}
}

void ConferenceManager::SegmentLocalMedia()
{
	RunningConfig* pConfig = RunningConfig::Instance();
	const int numberOfStreams = pConfig->VideoCaptureDeviceCount();
	for (int i = 0; i < numberOfStreams; i++)
	{
		QString sendID = getSendMediaID(i);
		MediaManager::getInstance()->SegmentMedia(sendID, 0);
	}
}

QString ConferenceManager::getRecordPath(QString terminalName, const int virtualIndex)
{
	// init file path
	if (_recordingPath.size() == 0)
	{
		QString recDir = CTxConfig::getInstance().GetRecPath();
		if (recDir.indexOf(":") == -1)
		{
			QString appDir = QCoreApplication::applicationDirPath();
			appDir.replace("/" , "\\" );
			if (recDir[0] == '\\')
			{
				recDir = appDir + recDir;
			}
			else
			{
				recDir = appDir + "\\" + recDir;
			}
		}
		if (recDir[recDir.size()-1] == '\\')
		{
			recDir = recDir.mid(0, recDir.size() - 1);
		}
		_recordingPath = recDir;
	}

	QString confTimeTitle;
	int pos = _currentConfInfo->StartTime.find(' ');
	if (pos != -1)
	{
		confTimeTitle = QString("%1_%2").arg(QString::fromUtf8(_currentConfInfo->StartTime.substr(0, pos).c_str())).arg(QString::fromUtf8(_confTitle.c_str()));
	}
	else
	{
		confTimeTitle = QString::fromUtf8(_confTitle.c_str());
	}

	QString curTerminalName = "DefaultName";
	TerminalVector currentTerminal = _terminalList[RunningProfile::getInstance()->user_uri()];
	if (currentTerminal.size() > 0)
	{
		curTerminalName = QString::fromUtf8(currentTerminal[0]->name.c_str());
	}
	if (curTerminalName[curTerminalName.length()-1] == ')') // TODO:多路摄像头时临时去除本终端名末尾的编号
	{
		curTerminalName = curTerminalName.mid(0, curTerminalName.length()-3);
	}
	if (terminalName[terminalName.length()-1] == ')') // TODO:多路摄像头时临时去除被录制终端名末尾的编号
	{
		terminalName = terminalName.mid(0, terminalName.length()-3);
	}

	// 路径：{录制缓存位置}/{tx终端名}/{会议名}/{日期}/{终端名}/{文件名}
	// 文件名：{终端名}-{日期时间}-{片段号}-{媒体流编号}-{视频格式}.mp4
	QString file = QString("%1\\%2\\%3\\%4\\%5\\%5_%6_%7_%8_%9.mp4")
		.arg(_recordingPath).arg(curTerminalName)
		.arg(confTimeTitle).arg(TX_FNFMT_DATE).arg(terminalName)
		.arg(TX_FNFMT_DATETIME).arg(TX_FNFMT_EPISODE).arg(virtualIndex).arg(TX_FNFMT_VINFO);

	return file;
}
