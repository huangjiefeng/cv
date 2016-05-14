#include "MediaManager.h"

#include <dbus/conferenceRoom/client/ConferenceRoomIf.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

#include <dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>

#include <dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h>
#include <dbus/channel/type/screenMedia/client/CvScreenMediaIf.h>

#include <QApplication>
#include <QDesktopWidget>

#include "../config/RunningConfig.h"
#include "config/config_helper.h"
#include "../profile/RunningProfile.h"

#include "ConferenceManager.h"
#include <util/ProcessManager.h>

#include <log/Log.h>

#include "conference_controller_interface.h"


// 还要改recordstatuswidget.h里的MAX_STR_PER_TERMINAL
//const int TERMINAL_STR_NO = 4;
//
//const int port_increment = 2000;



// 该函数将uri，virtualURI转换为@前部的user_id
QString uriToUserId(const QString &uriStr)
{
	QString user_id = uriStr;
	if (user_id.indexOf("@") != -1)
	{
		user_id = user_id.left(user_id.indexOf("@"));
	}
	return user_id;
}

MediaManager::MediaManager()
{

	_confRoomProxy = new ConferenceRoomIf( CONF_ROOM_SERVICE_NAME , CONF_ROOM_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus());
	_channelDispatcherProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME , CHANNEL_DISPATCHER_OBJECT_PATH ,
		QDBusConnection::sessionBus() );

    connect(_channelDispatcherProxy, &ChannelDispatcherIf::NofityChannelStateChanged,
        this, &MediaManager::HandleNofityChannelStateChanged);

	int screenCount = QApplication::desktop()->screenCount();
	_localPreviewScreen = 0;
	//_localPreviewSeet = 6;
	//加入回显的位置信息
	//this->AddRecvMemberPosition( RunningProfile::getInstance()->user_uri().c_str() , _localPreviewScreen, _localPreviewSeet );
	
	_isSendingMedia = false;
	_remoteScreenMember = "";
	_videoDscp = 0;
	_audioDscp = 0;
	_conferenceManager = NULL;

	_rtcpReportTimer = new QTimer(this);
	connect(_rtcpReportTimer , SIGNAL(timeout() ) , this , SLOT(rtcpReportTimerTimeout() ) );

	_lastFileCutTime = QDateTime::currentDateTime();
  _isModelTx = CVIniConfig::getInstance().IsModelTX();

    conference_controller_ = nullptr;
}


MediaManager::~MediaManager()
{
	disconnect(_rtcpReportTimer , SIGNAL(timeout() ) , this , SLOT(rtcpReportTimerTimeout() ) );
	delete _rtcpReportTimer;
	_rtcpReportTimer = NULL;
}


void MediaManager::AddRecvMedia( RecvGraphInfo& info )
{
	QByteArray bytes;
	QDataStream out( &bytes , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );

	int screenIndex;
	int seet;
	bool smallVideo;

	//修正id
	info.user_id = uriToUserId(info.user_id);
	out << info;

	GetRecvMemberPositionFromName( info.user_id, &screenIndex , &seet, &smallVideo);

	// Tx中ConfRoom仅用于与遥控器同步，不控制媒体流，该处绕过ConfRoom直接控制地层
	if (_isModelTx || screenIndex == -1 || seet == -1)
	{
		_channelDispatcherProxy->CreateChannel( userID2MediaID(info.user_id), QString(STREAMED_MEDIA_SERVICE_NAME) , bytes);
	}
  if (screenIndex > -1 && seet > -1)
	{
		_confRoomProxy->AddMediaWindow( false , bytes , screenIndex , seet );
	}
}

void MediaManager::RemoveRecvMedia( const QString& memberUri )
{
	QString user_id = uriToUserId(memberUri);
	int screenIndex;
	int seet;
	bool smallVideo;
	GetRecvMemberPositionFromName( user_id, &screenIndex , &seet, &smallVideo);

	if (CVIniConfig::getInstance().IsModelTX() || screenIndex == -1 || seet == -1)
	{
		_channelDispatcherProxy->ReleaseChannel(userID2MediaID(user_id), QString(STREAMED_MEDIA_SERVICE_NAME));
	}
	if (screenIndex > -1 && seet > -1)
	{
		_confRoomProxy->CloseMediaWindow( user_id );
	}
}


void MediaManager::SegmentMedia( const QString& memberUri, int job_id )
{
  QString userId = uriToUserId(memberUri);
	if (isSendMediaID(userId))
	{
		QDateTime curTime = QDateTime::currentDateTime();
		if (_lastFileCutTime.secsTo(curTime) <= 10) // 两次分割小于10秒，则忽略
		{
			return;
		}
		_lastFileCutTime = curTime;

		RunningConfig* config = RunningConfig::Instance();
		int numberOfStreams = config->VideoCaptureDeviceCount();

		for (int i = 0; i < numberOfStreams; i++)
		{
			QString mediaID = getSendMediaID(i);

			QByteArray recvByteArray;
			QDataStream out( &recvByteArray , QIODevice::WriteOnly );
			out << QString("");
			_channelDispatcherProxy->ModifyChannel(mediaID
				, QString(STREAMED_MEDIA_SERVICE_NAME), Action_CutRecordingVideo, recvByteArray);
		}
	}
	else
	{
		CutRecordInfo info;
    info.job_id = job_id;

		QByteArray recvByteArray;
		QDataStream out( &recvByteArray , QIODevice::WriteOnly );
		out << info;
		_channelDispatcherProxy->ModifyChannel(userID2MediaID(userId)
			, QString(STREAMED_MEDIA_SERVICE_NAME), Action_CutRecordingVideo, recvByteArray);
		//}
	}
}

/** 
 * @brief 创建发送音视频流
 * @param ip 媒体流的目标地址
 * @param audioPort 本地回显媒体流的音频目标端口，应为0
 * @param videoPort 本地回显媒体流的视频目标端口
 * @param smallVideo 是否使用小流
 * @param screenPort 屏幕端口，无用
 * @param audioEnable 是否发送音频
 */
void MediaManager::CreateSendMedia( int virtualIndex, const QString& ip , const int&audioPort , const int&videoPort , const bool smallVideo, const int&screenPort,const bool audioEnable)
{
	if( true/*false == _isSendingMedia*/ ) // TODO:应处理多摄像头的情形
	{
		RunningConfig* config = RunningConfig::Instance();
		const int numberOfStreams = config->VideoCaptureDeviceCount();
		QString mediaID = getSendMediaID(virtualIndex);

		/*for (int i = 0; i < numberOfStreams; i++)
		{*/
		VCapSetting captureDevice = config->VideoCaptureDevice(virtualIndex);
        
    int aec_type = 0;
    ConfigHelper::GetAudioInputTypeByUIOption(config->AudioInputType(), aec_type);
    if (aec_type < 0) aec_type = 0;

		SendGraphInfo sendGraph;

		//如果是主摄像头，建立音频链路
		if (virtualIndex == 0)
		{
			//int screenIndex;
			//int seet;
			//GetRecvMemberPositionFromName( LOCAL_PLAY_USERID , &screenIndex , &seet ); // 已不用 - Liaokz, 2013-10

			//SendGraphInfo sendGraph;
			sendGraph.initial( screenPort, qPrintable(captureDevice.devicePath), qPrintable(config->AudioCaptureDevice()) ,
				qPrintable(config->AudioOutputDevice()) , aec_type,
                qPrintable(ip), audioPort/*9000 + 10 * i*/ , qPrintable(config->AudioCodec()) , 
				config->AudioSampleRate(), config->AudioBitsPerSample(), config->AudioChannel(), audioEnable,
				qPrintable(ip), videoPort/*9100 + 10 * i*/, captureDevice.fps, smallVideo, 0 , captureDevice.width , captureDevice.height, 
				qPrintable(captureDevice.crossBarDisplayName), qPrintable(captureDevice.crossBarType));
			sendGraph.dscp_info.video_dscp = _videoDscp;
			sendGraph.dscp_info.audio_dscp =_audioDscp;
			sendGraph.preview_wnd = NULL;

			/*QByteArray output_array;
			QDataStream out( &output_array , QIODevice::WriteOnly );
			out.setVersion( QDataStream::Qt_4_4 );
			out << sendGraph;*/

			//_confRoomProxy->AddMediaWindow( true , output_array , screenIndex ,seet ); // 已不用 - Liaokz, 2013-10

			//mediaID = QString(SEND_STREAMED_MEDIA_ID);

			_rtcpReportTimer->start(5000);
		}
		//从摄像头，只建立高清视频发送链路，没有音频。
		else
		{
			//SendGraphInfo sendGraph;
			sendGraph.initial( screenPort, qPrintable(captureDevice.devicePath), NULL/*qPrintable(config->AudioCaptureDevice())*/ ,
				NULL/*qPrintable(config->AudioOutputDevice())*/ , aec_type, qPrintable(ip), audioPort/*9000 + 10 * i*/ , NULL/*"SPEEX"*/ , 16000,16,1,false/*audioEnable*/,
				qPrintable(ip), videoPort/*9100 + 10 * i*/, captureDevice.fps, smallVideo, 0 , captureDevice.width , captureDevice.height, qPrintable(captureDevice.crossBarDisplayName),
				qPrintable(captureDevice.crossBarType));
			sendGraph.dscp_info.video_dscp = _videoDscp;
			sendGraph.dscp_info.audio_dscp =_audioDscp;
			sendGraph.preview_wnd = NULL;

			//mediaID = QString(SEND_STREAMED_MEDIA_ID) + QString::number(captureDevice.deviceNo);
		}

		QByteArray output_array;
		QDataStream out( &output_array , QIODevice::WriteOnly );
		out.setVersion( QDataStream::Qt_4_4 );
		out << sendGraph;

		_channelDispatcherProxy->CreateChannel(mediaID, STREAMED_MEDIA_SERVICE_NAME, output_array);
		if (smallVideo)
		{
			QString smallMediaID = getSmallVideoMediaID(virtualIndex);
			_channelDispatcherProxy->CreateChannel(smallMediaID, STREAMED_MEDIA_SERVICE_NAME, output_array);
		}

		//保存mediaID的映射关系
		QString serviceName = QString( STREAMED_MEDIA_SERVICE_NAME ) + QString(".") + mediaID;
		QString objectPath = QString( STREAMED_MEDIA_SERVICE_OBJECT_PATH ) + QString("/") + mediaID;
		CvStreamedMediaIf* streamedMediaIf = new CvStreamedMediaIf( serviceName, objectPath,QDBusConnection::sessionBus() );
		_sendMediaProxyMap.insert(StreamedMediaProxyMap::value_type( mediaID , streamedMediaIf ) );
		//}

		_isSendingMedia = true; // TODO：应处理多摄像头时的情形

	}
	else
	{
		CV_LOG_ERROR("发送媒体流已经创建成功");
	}
}


/**
 * @brief 添加发送目标
 * @param ip 媒体流的目标地址
 * @param audioPort 媒体流的音频目标端口
 * @param videoPort 媒体流的视频目标端口
 * @param smallVideo 是否采用小流
 * @param screenPort 屏幕端口，无用
 * @param audioEnable 是否发送音频
 */
void MediaManager::AddSendMedia( int virtualIndex, const QString& ip , const int&audioPort , const int&videoPort ,const bool smallVideo,const int&screenPort, const bool audioEnable)
{
  if( !_isSendingMedia ) {
    CV_LOG_ERROR("发送媒体流尚未创建，无法添加新的发送目标");
  }

  //等待发送媒体流进程的启动。由于发送媒体流进程是最先启动的，因此可以采用下面的工作方式
  /*while(!ProcessManager::isRunning(TEXT("CvStreamedMedia.exe") ) )
  {
	  Sleep(2000);
  }*/
    
  // TODO: 临时方案
  // 由于只有virtualIndex为0的streamMedia包含音频, 因此需要分开发送音视频
  // 其中音频发送的是virtualIndex为0的音频，
  // 而视频发送的是virtualIndex的视频（分大小流）

  // 查找virtualIndex对应的媒体流，并发送其视频流
  NetInfo netInfo;
  netInfo.ip_addr = ip;
  netInfo.audio_port = 0; // 不指定音频
  netInfo.video_port = videoPort;
  netInfo.enable_small_video = smallVideo;
    
  QByteArray output_array;
  QDataStream out( &output_array , QIODevice::WriteOnly );
  out.setVersion( QDataStream::Qt_4_4 );
  out << netInfo;

  if (smallVideo) {
    // 查找virtualIndex对应的小流媒体流，并发送其视频流
    CvStreamedMediaIf streamedMeidaProxy( getStreamedMediaServiceName(getSmallVideoMediaID(virtualIndex)) ,
      getStreamedMediaObjectPath( getSmallVideoMediaID(virtualIndex) ),QDBusConnection::sessionBus() );
    streamedMeidaProxy.SetMediaInfo( Action_AddSendDst , output_array );
  } else {
    // 查找virtualIndex对应的大流媒体流，并发送其视频流
    CvStreamedMediaIf * pSendMediaProxy = NULL;
    auto it = _sendMediaProxyMap.find(getSendMediaID(virtualIndex));
    if (it != _sendMediaProxyMap.end()) {
      pSendMediaProxy = (*it).second;
      pSendMediaProxy->SetMediaInfo( Action_AddSendDst , output_array );
    } else {
      // TODO: 未发现有该媒体流
      // 写日志
      return;
    }
  }

  if (audioEnable) {
    // 查找virtualIndex为0的媒体流，并发送其音频流
    CvStreamedMediaIf *stream_media_with_audio = NULL;
    auto it = _sendMediaProxyMap.find(getSendMediaID(0));
    if (it != _sendMediaProxyMap.end()) {
      stream_media_with_audio = (*it).second;

      NetInfo info;
      info.ip_addr = ip;
      info.audio_port = audioPort;
      info.video_port = 0; // 不指定视频流
      info.enable_small_video = false;

      QByteArray output_array;
      QDataStream out( &output_array , QIODevice::WriteOnly );
      out.setVersion( QDataStream::Qt_4_4 );
      out << info;

      stream_media_with_audio->SetMediaInfo(Action_AddSendDst, output_array);
    }
  }
}


void MediaManager::RemoveSendMedia( int virtualIndex, const QString&ip , const int&audioPort , const int&videoPort,  const bool isSmallVideo )
{
  // TODO: 临时方案
  // 由于只有virtualIndex为0的streamMedia包含音频, 因此需要分开处理音视频
  // 其中音频发送的是virtualIndex为0的音频，
  // 而视频发送的是virtualIndex的视频（分大小流）

  NetInfo info;
  info.ip_addr = ip;
  info.audio_port = 0; // 不指定音频流
  info.video_port = videoPort;
  info.enable_small_video = isSmallVideo;

  QByteArray output_bytes;
  QDataStream out(&output_bytes , QIODevice::WriteOnly );
  out.setVersion( QDataStream::Qt_4_4 );
  out << info;

  if (isSmallVideo) {
    //  查找virtualIndex对应的小流媒体流，停止发送其视频到指定的目标。
    CvStreamedMediaIf streamedMeidaProxy( getStreamedMediaServiceName(getSmallVideoMediaID(virtualIndex)) ,
		getStreamedMediaObjectPath( getSmallVideoMediaID(virtualIndex) ),QDBusConnection::sessionBus() );
	streamedMeidaProxy.SetMediaInfo( Action_RemoveSendDst , output_bytes );
  } else {
    //  查找virtualIndex对应的大流媒体流，停止发送其视频到指定的目标。
    CvStreamedMediaIf * pSendMediaProxy = NULL;
    auto it = _sendMediaProxyMap.find(getSendMediaID(virtualIndex));
    if (it != _sendMediaProxyMap.end()) {
      pSendMediaProxy = (*it).second;
      pSendMediaProxy->SetMediaInfo(Action_RemoveSendDst, output_bytes);
    } else {
      // 未发现有该媒体流
      // TODO：写日志
      return;
    }
  }

  // 查找virtualIndex为0的媒体流，并停止发送其音频流到指定的目标。
  CvStreamedMediaIf *stream_media_with_audio = NULL;
  auto it = _sendMediaProxyMap.find(getSendMediaID(0));
  if (it != _sendMediaProxyMap.end()) {
    stream_media_with_audio = (*it).second;

    NetInfo info;
    info.ip_addr = ip;
    info.audio_port = audioPort;
    info.video_port = 0; // 不指定视频流
    info.enable_small_video = false;

    QByteArray output_array;
    QDataStream out( &output_array , QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_4_4 );
    out << info;

    stream_media_with_audio->SetMediaInfo(Action_RemoveSendDst, output_array);
  }
}



void MediaManager::ChangeMemberSeat( const QString&memberURI , const int newScreenIndex , const int newSeet )
{
	QString user_id = uriToUserId(memberURI);
	QByteArray output_bytes;
	QDataStream out( &output_bytes , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << newScreenIndex << newSeet;
	_confRoomProxy->ModifyMediaWindow( user_id , MWA_ChangeWindowPosition, output_bytes );

	//if( oldScreenIndex!=newScreenIndex )
	//{
	//	//如果用户的显示屏幕发生了改变，那么应该关闭原有的窗口
	//	RemoveRecvMedia( userID );
	//}

	////然后添加新的媒体窗口
	//AddRecvMemberPosition( userID , newScreenIndex , newSeet );
	//RecvGraphInfo info;
	//info.initial( "" , 0 , "" , 0 , 0 , 0 , "", 0 , 0 , 0 ,0 ,qPrintable(userID) );
	//AddRecvMedia( info);
}

void MediaManager::ExitConference()
{
	for( RecvMemberVector::iterator it = _recvMemberVector.begin() ; it!=_recvMemberVector.end() ;it++ )
	{
		RecvMemberPosition * pos = (*it);
		if (pos == NULL || isLocalPreviewMediaID(pos->_memberURI) ||
        isLocalRecordMediaID(pos->_memberURI))
		{
			continue;
		}
		RemoveRecvMedia(pos->_memberURI);
	}

  if (_isModelTx) {
    _channelDispatcherProxy->Destroy();
  }
	_confRoomProxy->ExitConference();
	//_isSendingMedia = false;
	
	_rtcpReportTimer->stop();
	//清空用户的位置信息
	clearRecvMemberPosition();
}


void MediaManager::SetDSCP( const int vdieoDscp , const int audioDscp )
{
	_videoDscp = vdieoDscp;
	_audioDscp = audioDscp;

	//下面代码的执行只在媒体流发送进程CvStreamedMedia_send已经启动时才有效
	QosDscpInfo info;
	info.video_dscp = vdieoDscp;
    info.audio_dscp = audioDscp;
	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << info;
	//获取发送媒体进程的服务接口，目前qos只针对发送进程
	CvStreamedMediaIf streamedMediaIf( getStreamedMediaServiceName(QString(SEND_STREAMED_MEDIA_ID) ) ,
		getStreamedMediaObjectPath(QString(SEND_STREAMED_MEDIA_ID)) , QDBusConnection::sessionBus() );
	streamedMediaIf.SetMediaInfo( Action_SetQoSInfo , byteArray );
}



void MediaManager::showRtcpMsg( bool visiable , int screenIndex/*=-1 */ )
{
	_confRoomProxy->ShowRtcpMsg( screenIndex , visiable );
}

void MediaManager::ChangeLayout( const ConfRoomDisplayModel displayModel , int screenIndex/*=-1 */ )
{
    static int try_times = 15; //仅在第一次尝试等待
    while (try_times-- > 0) {
        //之前尝试用_confRoomProxy->isValid()判定，但发现无效（参考信号槽原理）
        if (ProcessManager::isRunning(TEXT("CvConferenceRoom.exe")) && 
            ProcessManager::isRunning(TEXT("CvTelecontrollerSocket.exe"))) {
            try_times = 0;
            Sleep(200); //确保已注册
            break;
        }
        Sleep(200);
    }
    _confRoomProxy->ChangeLayout( screenIndex , displayModel );
}

void MediaManager::AddRecvMemberPosition( const QString& memberURI , const int screenIndex , const int seet, const bool smallVideo )
{
	RecvMemberVector::iterator it = _recvMemberVector.begin();
	for(  ; it!=_recvMemberVector.end() ; it++ )
	{
		if( (*it)->_memberURI == memberURI )
		{
			//更新当前视频窗口位置
			(*it)->_screenIndex = screenIndex;
			(*it)->_seat = seet;
			(*it)->_smallVideo = smallVideo;
			break;
		}
	}
	if( it==_recvMemberVector.end() )
	{
		//还没有接收该用户，则添加进去
		RecvMemberPosition* recvMember = new RecvMemberPosition();
		recvMember->_memberURI = memberURI;
		recvMember->_screenIndex = screenIndex;
		recvMember->_seat = seet;
		recvMember->_smallVideo = smallVideo;


		string mediaID;
		if( isLocalPreviewMediaID(memberURI) )
		{
			int index = getVirtualIndexFromLocalPreviewMediaID(memberURI);
			mediaID = getSendMediaID(index).toStdString();
		}else
		{
			QString user_id = uriToUserId(memberURI);
			mediaID = userID2MediaID(user_id).toStdString();
		}
		recvMember->_rtcpVideoReportReader->setReader( mediaID );
		recvMember->_rtcpAudioReportReader->setReader( mediaID );
		_recvMemberVector.push_back( recvMember );
	}
}

void MediaManager::RemoveRecvMemberPosition( const QString& memberURI )
{
	RecvMemberVector::iterator it = _recvMemberVector.begin();
	for(  ; it!=_recvMemberVector.end() ; it++ )
	{
		if( (*it)->_memberURI == memberURI )
		{
			RecvMemberPosition * recvMember = *it;
			if (recvMember)
			{
				delete recvMember;
			}
			_recvMemberVector.erase( it );
			break;
		}
	}
}

int MediaManager::GetRecvMemberPositionFromURI( const QString&memberURI , int* screenIndex , int* seet, bool *smallVideo )
{
	RecvMemberVector::iterator it = _recvMemberVector.begin();
	for(  ; it!=_recvMemberVector.end() ; it++ )
	{
		if( (*it)->_memberURI == memberURI )
		{
			if (screenIndex) *screenIndex = (*it)->_screenIndex;
			if (seet) *seet = (*it)->_seat;
			if (smallVideo) *smallVideo = (*it)->_smallVideo;
			return 1;
		}
	}
	if( it==_recvMemberVector.end() )
	{
		if (screenIndex) *screenIndex=-1;
		if (seet) *seet = -1;
		if (smallVideo) *smallVideo = false;
	}
	return -1;
}


int MediaManager::GetRecvMemberPositionFromName( const QString &userId , int* screenIndex , int* seet, bool *smallVideo )
{
	RecvMemberVector::iterator it = _recvMemberVector.begin();

	for(  ; it!=_recvMemberVector.end() ; it++ )
	{
		if ((*it) == NULL)
		{
			continue;
		}
		QString user_id_to_check = uriToUserId((*it)->_memberURI);

		if( user_id_to_check == userId )
		{
			if (screenIndex) *screenIndex = (*it)->_screenIndex;
			if (seet) *seet = (*it)->_seat;
			if (smallVideo) *smallVideo = (*it)->_smallVideo;
			return 1;
		}
	}
	if( it==_recvMemberVector.end() )
	{
		if (screenIndex) *screenIndex=-1;
		if (seet) *seet = -1;
		if (smallVideo) *smallVideo = false;
	}
	return -1;
}

void MediaManager::ControlVideoSend(int virtualIndex, bool videoSend )
{
	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion(QDataStream::Qt_4_4);
	out << videoSend;

	//获取发送媒体进程的服务接口，目前qos只针对发送进程
    
	CvStreamedMediaIf streamedMediaIf( getStreamedMediaServiceName(getSendMediaID(virtualIndex)),
		getStreamedMediaObjectPath(QString(SEND_STREAMED_MEDIA_ID)) , QDBusConnection::sessionBus() );
	streamedMediaIf.SetMediaInfo( Action_ControlVideoSend , byteArray );

	QString localID = getLocalPreviewMediaID(virtualIndex);
	if( videoSend )
		ChangeMediaState( localID , UiStateTypeVideo , UiMediaState_Run, true);
	else
		ChangeMediaState( localID , UiStateTypeVideo , UiMediaState_Stop,true);
}

void MediaManager::ControlVideoSend( bool videoSend ) {
  QByteArray byteArray;
  QDataStream out( &byteArray , QIODevice::WriteOnly );
  out.setVersion(QDataStream::Qt_4_4);
  out << videoSend;

  //获取发送媒体进程的服务接口，目前qos只针对发送进程
  CvStreamedMediaIf streamedMediaIf( getStreamedMediaServiceName(QString(SEND_STREAMED_MEDIA_ID) ) ,
    getStreamedMediaObjectPath(QString(SEND_STREAMED_MEDIA_ID)) , QDBusConnection::sessionBus() );
  streamedMediaIf.SetMediaInfo( Action_ControlVideoSend , byteArray );

  for (int i = 0; i < RunningConfig::Instance()->VideoCaptureDeviceCount(); ++i) {
    QString localID = getLocalPreviewMediaID(i);
    if( videoSend )
      ChangeMediaState( localID , UiStateTypeVideo, UiMediaState_Run, true);
    else
       ChangeMediaState( localID , UiStateTypeVideo, UiMediaState_Stop,true);
  }
}

void MediaManager::ControlAudioSend( bool audioSend )
{
	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << audioSend;

	//获取发送媒体进程的服务接口，目前qos只针对发送进程
	CvStreamedMediaIf streamedMediaIf( getStreamedMediaServiceName(QString(SEND_STREAMED_MEDIA_ID) ) ,
		getStreamedMediaObjectPath(QString(SEND_STREAMED_MEDIA_ID)) , QDBusConnection::sessionBus() );
	streamedMediaIf.SetMediaInfo( Action_ControlAudioSend , byteArray );

	for (int i = 0; i < RunningConfig::Instance()->VideoCaptureDeviceCount(); ++i)
	{
		QString localID = getLocalPreviewMediaID(i);
		if( audioSend )
			ChangeMediaState( localID , UiStateTypeAudio, UiMediaState_Run, true);
		else
			ChangeMediaState( localID , UiStateTypeAudio, UiMediaState_Stop,true);
	}
}

//注意:此方法仅供TX调用以在退出时清空媒体流.如果普通终端调用,将导致发送进程被终止!
void MediaManager::ClearStreamedMedia()
{
	_remoteScreenMember = "";
	//TODO
	while (!_recvMemberVector.empty())
	{
		RecvMemberVector::iterator it = _recvMemberVector.begin();
		RecvMemberPosition * pos = *it;
		if (NULL == pos)
		{
			_recvMemberVector.erase(it);
			continue;
		}
		RemoveRecvMedia(pos->_memberURI);
		RemoveRecvMemberPosition(pos->_memberURI);
	}
	_channelDispatcherProxy->Destroy();
	_sendMediaProxyMap.clear();
	_isSendingMedia = false;
	//ExitConference();
}

void MediaManager::ChangeMediaState( const QString&memberURI , const QString&mediaType , const UiMediaState state ,bool isAudioSend)
{
	QString user_id = uriToUserId(memberURI);

	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out<<mediaType<<state<<isAudioSend;

	_confRoomProxy->ModifyMediaWindow( user_id ,MWA_ChangeMediaState, byteArray);
}


void MediaManager::CreateSendScreenMedia( const int port ,const QString&dst, const QRect wnd )
{
	ScreenMediaInfo info;
	info._port = port;
	info._ipAddress = dst;
	info._screenWnd = wnd;

	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << info;
	_channelDispatcherProxy->CreateChannel( SEND_SCREEN_MEDIA_ID, SCREEN_MEDIA_SERVICE_NAME,
		outputBytes );

}

void MediaManager::AddSendScreenMediaDst( const int port , const QString&dst )
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << dst << port;
	CvScreenMediaIf proxy( getScreenMediaServiceName(SEND_SCREEN_MEDIA_ID),
		getScreenMediaObjectPath(SEND_SCREEN_MEDIA_ID) , QDBusConnection::sessionBus() );
	proxy.SetMediaInfo( ScreenMediaAction_AddSendDst , outputBytes );
}

void MediaManager::RemoveSendScreenMediaDst( const int port , const QString&dst )
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << dst << port;
	CvScreenMediaIf proxy( getScreenMediaServiceName(SEND_SCREEN_MEDIA_ID),
		getScreenMediaObjectPath(SEND_SCREEN_MEDIA_ID) , QDBusConnection::sessionBus() );
	proxy.SetMediaInfo( ScreenMediaAction_RemoveSendDst , outputBytes );
}

void MediaManager::RemoveRecvScreenMedia( const QString&remoteURI )
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	_confRoomProxy->ModifyMediaWindow( remoteURI ,MWA_RemoveScreenRecv , outputBytes );
}

void MediaManager::CreateRecvScreenMedia( const QString&memberURI , const int port ,const QString&src, const int wnd, const int screenIndex )
{

	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << src << port << screenIndex;

	_confRoomProxy->ModifyMediaWindow( memberURI , MWA_AddScreenRecv, outputBytes );
}

void MediaManager::EnableRecvAutoResync( bool enable )
{
	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << enable;
	_channelDispatcherProxy->ModifyChannel("", STREAMED_MEDIA_SERVICE_NAME, MMT_EnableAutoResync, byteArray);
}

void MediaManager::ControlVideoRecv( const QString memberURI, const bool videoRecv )
{
	QString memberName = memberURI;
	int index= memberName.indexOf("@");
	if(  index!=-1 )
		memberName = memberName.left(index);

	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << videoRecv;

	QString mediaID = userID2MediaID( memberName );
	CvStreamedMediaIf streamProxy( getStreamedMediaServiceName(mediaID ), 
		getStreamedMediaObjectPath(mediaID), QDBusConnection::sessionBus() );
	streamProxy.SetMediaInfo( Action_ControlVideoRecv , byteArray );

	if( videoRecv )
		ChangeMediaState( memberURI , UiStateTypeVideo, UiMediaState_Run,false);
	else
		ChangeMediaState( memberURI , UiStateTypeVideo, UiMediaState_Stop,false);
}

void MediaManager::ControlAudioRecv( const QString memberURI, const bool audioRecv )
{
	QString memberName = memberURI;
	int index= memberName.indexOf("@");
	if(  index!=-1 )
		memberName = memberName.left(index);

	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << audioRecv;

	QString mediaID = userID2MediaID( memberName );
	CvStreamedMediaIf streamProxy( getStreamedMediaServiceName(mediaID ), 
		getStreamedMediaObjectPath(mediaID), QDBusConnection::sessionBus() );
	streamProxy.SetMediaInfo( Action_ControlAudioRecv , byteArray );

	if( audioRecv )
		ChangeMediaState( memberURI , UiStateTypeAudio , UiMediaState_Run,false);
	else
		ChangeMediaState( memberURI , UiStateTypeAudio , UiMediaState_Stop,false);

}

void MediaManager::EnableAdaptiveCodeRate( bool enable )
{
    QByteArray byteArray;
    QDataStream out( &byteArray , QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_4_4 );
    out << enable;
    CvStreamedMediaIf streamedMediaIf( getStreamedMediaServiceName(QString(SEND_STREAMED_MEDIA_ID) ) ,
        getStreamedMediaObjectPath(QString(SEND_STREAMED_MEDIA_ID)) , QDBusConnection::sessionBus() );
    streamedMediaIf.SetMediaInfo( Action_AdaptiveControlCodeRate , byteArray );
}

void MediaManager::clearRecvMemberPosition()
{
	RecvMemberVector localPreviewPos;

	for( RecvMemberVector::iterator it = _recvMemberVector.begin() ; it!=_recvMemberVector.end() ;it++ )
	{
		RecvMemberPosition * recvMember = *it;
		if (isLocalPreviewMediaID(recvMember->_memberURI))
		{
			localPreviewPos.push_back(recvMember);
			continue;
		}
		if( recvMember!=NULL )
		{
			delete recvMember;
			*it = NULL;
		}
	}

	_recvMemberVector.clear();

	//重新加入本地回显的位置信息
	_recvMemberVector = localPreviewPos;
}

void MediaManager::rtcpReportTimerTimeout()
{
	if( _conferenceManager!=NULL )
	{
		
		RtpStatItem item;
		for( RecvMemberVector::iterator it = _recvMemberVector.begin(); it!=_recvMemberVector.end() ; it++ )
		{
			///发送自己的报告存在问题，不处理他
			if( (*it)->_memberURI != RunningProfile::getInstance()->user_uri().c_str() )
			{
				(*it)->_rtcpAudioReportReader->readRtcpReport(item );
				_conferenceManager->sendRTPStatToQoSServer(item );
				(*it)->_rtcpVideoReportReader->readRtcpReport( item );
				_conferenceManager->sendRTPStatToQoSServer(item );
			}

		}
	}

    if (conference_controller_) {
      RtpStatItem item;
      for( RecvMemberVector::iterator it = _recvMemberVector.begin(); it!=_recvMemberVector.end() ; it++ )
      {
        ///发送自己的报告存在问题，不处理他
        if( (*it)->_memberURI != RunningProfile::getInstance()->user_uri().c_str() )
        {
          (*it)->_rtcpAudioReportReader->readRtcpReport(item );
          conference_controller_->HandleReceiveUDPQoSReportSlot(item );
          (*it)->_rtcpVideoReportReader->readRtcpReport( item );
          conference_controller_->HandleReceiveUDPQoSReportSlot(item );
        }

      }

    }

}

/**
 * @brief 控制共享屏幕的界面的按钮，如果当前终端为主讲人则把“屏幕共享”按钮显示出来，否则隐藏它
 * @param control 如果为true，则显示按钮，否则隐藏按钮
 */
void MediaManager::ControlScreenShare( const bool control )
{
	QByteArray byteArray;
	QDataStream out( &byteArray , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << control;
	_confRoomProxy->ModifyMediaWindow( "", MWA_ControlScreenShare, byteArray );
}


/**
 * @brief 根据指定参数创建主讲人的接收屏幕流
 * @param port 屏幕流占用的端口
 * @dst 目标地址，如果不是组播则设置为"0.0.0.0"
 * @wnd 共享的屏幕窗口
 */
void MediaManager::CreateMainSpeakerRecvScreenMedia( const QString&memberURI , const int port ,const QString&src, const int wnd)
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << src << port;

	_confRoomProxy->ModifyMediaWindow( memberURI , MWA_AddMainSpeakerScreenRecv, outputBytes );

}


void MediaManager:: RemoveScreenShareSend()
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );

	_confRoomProxy->ModifyMediaWindow( "" , MWA_RemoveSendShareScreen, outputBytes );
}

void MediaManager:: PPTControlCommand(int type)
{
	QByteArray outputBytes;
	QDataStream out(&outputBytes,QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out<<type;

	_confRoomProxy->ModifyMediaWindow( "" , MWA_PPTControlCommand, outputBytes );

}

void MediaManager::RecoveryRecvMedia( RecvGraphInfo& info )
{

	QByteArray bytes;
	QDataStream out( &bytes , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << info;
	int screenIndex;
	int seet;
	bool smallVideo;
	GetRecvMemberPositionFromName( info.user_id, &screenIndex , &seet, &smallVideo);
	_confRoomProxy->RecoveryRecvMediaProcess( bytes , screenIndex , seet );

}

void MediaManager::HandleNofityChannelStateChanged( const QString &channel_id, 
                                                   const QString &user_id, 
                                                   const QString &channel_type, 
                                                   int channel_state )
{
    if (channel_type == STREAMED_MEDIA_SERVICE_NAME) {
        if (isSendMediaID(channel_id)) {
            emit NotifySendMediaStateChanged(user_id, channel_state);
        } 
        else {
            emit NotifyRecvMediaStateChanged(user_id, channel_state);
        }
    }
}
