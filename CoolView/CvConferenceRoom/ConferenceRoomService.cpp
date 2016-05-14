#include "ConferenceRoomService.h"
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
#include <dbus/conferenceRoom/service/ConferenceRoomAdaptor.h>
//msdx
#include <dbus/msdx/common/MsdxServiceCommon.h>


//channel dispatcher
#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>

//streamedMedia tunnel
#include <dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>

#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
#include <dbus/telecontroller/service/TelecontrollerAdaptor.h>

#include <dbus/kinect/common/KinectServiceCommon.h>

#include "QtConfrenceVideoWindow.h"
#include "VideoFrame.h"
#include <Windows.h>

#include "QoSReportReceiverThread.h"
#include <util/ini/CVIniConfig.h>

#include <QMessageBox>

ConferenceRoomService::ConferenceRoomService()
{
	//开启Conference Room服务
	new ConferenceRoomAdaptor(this);

	//为了适应多屏幕的终端，注册时需要进行区分
	QDBusConnection connection = QDBusConnection::sessionBus();
	QString serviceName = QString(CONF_ROOM_SERVICE_NAME) ;
	QString objectPath = QString(CONF_ROOM_SERVICE_OBJECT_PATH);
	bool ret = connection.registerService(serviceName);
	//if( !ret)
	//	CV_LOG_ERROR( "注册CONF_ROOM_SERVICE失败");

	ret = connection.registerObject( objectPath, this);

	//开启Telecontroller服务
	new TelecontrollerAdaptor(this);
	ret = connection.registerService( _GetTelecontrollerServiceName("confRoom") );
	ret = connection.registerObject( _GetTelecontrollerObjectPath("confRoom"),this );

	///将要使用的tunnelDispatcher服务
	_channelDispatcherProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME , CHANNEL_DISPATCHER_OBJECT_PATH,
		QDBusConnection::sessionBus());

  _mediaWindowManager = MediaWindowManager::getInstance();

	//监听本机的34543端口
  _qosRportThread = new QThread();
  _qosRportThread->start();

	_qosRportReceiver = new QoSReportReceiverThread();
  _qosRportReceiver->Initialize("127.0.0.1", 
    CVIniConfig::getInstance().getConfRoomQosServerUdpPort());
  _qosRportReceiver->moveToThread(_qosRportThread); // 在单独线程中执行
  connect(
    _qosRportReceiver, 
    &QoSReportReceiverThread::SendUDPQoSReportSignal,
    _mediaWindowManager, 
    &MediaWindowManager::UpdateRtcpMessage);

   connect(
    _qosRportReceiver, 
    &QoSReportReceiverThread::SendUDPRecReportSignal,
    _mediaWindowManager, 
    &MediaWindowManager::UpdateRecordMessage);

    _qosRportReceiver->Start();
	// For Test
	/*VideoFrame * frame = MediaWindowManager::getInstance()->AddMediaWindow("Test", "Test", 0, 2, 320, 240);
	MediaWindowManager::getInstance()->ShowVideoFrame( frame );*/
}

ConferenceRoomService::~ConferenceRoomService()
{
	if( _qosRportReceiver )
	{
		_qosRportReceiver->Stop();
	}
}

void ConferenceRoomService::AddMediaWindow( bool send, const QByteArray &input_garray, int screenIndex, int seet )
{
    screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
	VideoFrame* videoFrame = NULL;
	QString mediaID;
	QByteArray output_garray;
	QDataStream inDataStream( input_garray );
	inDataStream.setVersion( QDataStream::Qt_4_4 );

	if( send )
	{
		QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("以外的收到本地发送流标识"));
		return;
		//否则安排回显窗口
		/*SendGraphInfo info;
		inDataStream>> info;*/

		//MediaWindowManager::getInstance()->SetScreenPort( info.net_info.screen_port );
		//MediaWindowManager::getInstance()->SetScreenIP( info.net_info.ip_addr );

		//videoFrame = MediaWindowManager::getInstance()->FindMediaWindow(LOCAL_PLAY_USERID);
		//if(videoFrame )
		//{
		//	//在旧位置上显示视频
		//	MediaWindowManager::getInstance()->ShowVideoFrame( videoFrame);
		//	//在新位置上显示该视频窗口
		//	//MediaWindowManager::getInstance()->ChangeWindowPostion( LOCAL_PLAY_USERID , screenIndex , seet );
		//	return;
		//}

		//videoFrame = MediaWindowManager::getInstance()->AddMediaWindow( LOCAL_PLAY_USERID , LOCAL_PLAY_USERID,screenIndex , seet, info.video_info.width, info.video_info.height );
		//if( videoFrame ==NULL )
		//	return;

		//MediaWindowManager::getInstance()->ShowVideoFrame( videoFrame );
		//info.preview_wnd = (int)videoFrame->LinkWin->getVideoFrame()->winId();

		/*QDataStream outDataStream( &output_garray , QIODevice::WriteOnly  );
		outDataStream.setVersion( QDataStream::Qt_4_4 );
		outDataStream << info;

		mediaID = SEND_STREAMED_MEDIA_ID;*/
		//把smallvideograph加入...没办法，暂时这么弄
		/*QString serviceName = QString( STREAMED_MEDIA_SERVICE_NAME ) + QString(".") + userID2MediaID("localpreview");
		QString objectPath = QString( STREAMED_MEDIA_SERVICE_OBJECT_PATH ) + QString("/") + userID2MediaID("localpreview");
		CvStreamedMediaIf* streamedMediaIf = new CvStreamedMediaIf( serviceName, objectPath,QDBusConnection::sessionBus() );
		_mediaProxyMap.insert(StreamedMediaProxyMap::value_type( userID2MediaID("localpreview") , streamedMediaIf ) );*/

	}else
	{
		RecvGraphInfo info;
		inDataStream >> info;

		videoFrame = MediaWindowManager::getInstance()->AddMediaWindow( 
      info.user_id , info.user_name, screenIndex , seet, 
      info.video_info.width, info.video_info.height );
		if( videoFrame==NULL )
			return;

		MediaWindowManager::getInstance()->ShowVideoFrame( videoFrame ); //确保可见
		info.recv_wnd = (int)videoFrame->LinkWin->getVideoFrame()->winId();
    qDebug() << "Show LinkWin:" << videoFrame->LinkWin->getVideoFrame()->winId();

		QDataStream outDataStream( &output_garray , QIODevice::WriteOnly  );
		outDataStream.setVersion( QDataStream::Qt_4_4 );
		outDataStream << info;

		//否则根据mediaID创建媒体流进程
		if (info.net_info.video_port != 0 || info.net_info.audio_port != 0)
		{
			mediaID = userID2MediaID( info.user_id);	
		}
	}

	if (CVIniConfig::getInstance().IsModelHD() && 
		!mediaID.isEmpty())
	{
		//要求channelDispatcher创建相应的媒体流进程，如果已经创建则会初始化他们
		_channelDispatcherProxy->CreateChannel( mediaID , STREAMED_MEDIA_SERVICE_NAME , 
			output_garray );
		StreamedMediaProxyMap::iterator it = _mediaProxyMap.find( mediaID );
		if( it!=_mediaProxyMap.end() )
		{
			delete it->second;
		}
		//保存mediaID的映射关系
		QString serviceName = QString( STREAMED_MEDIA_SERVICE_NAME ) + QString(".") + mediaID;
		QString objectPath = QString( STREAMED_MEDIA_SERVICE_OBJECT_PATH ) + QString("/") + mediaID;
		CvStreamedMediaIf* streamedMediaIf = new CvStreamedMediaIf( serviceName, objectPath,QDBusConnection::sessionBus() );
		//connect(streamedMediaIf , SIGNAL(QosNofity(const QByteArray& , int )) , this ,SLOT(QoSNotifySlot(const QByteArray& , int)) );

		_mediaProxyMap[mediaID] = streamedMediaIf;
	}

	//通告遥控器
	if( videoFrame && videoFrame->LinkWin )
		MediaWindowManager::getInstance()->MemberLocationAddNotify( videoFrame->LinkWin->getUserId() , videoFrame->_screenIndex , videoFrame->_seet , 
			videoFrame->LinkWin->isAudioEnable() , videoFrame->LinkWin->isVideoEnable() );
}


void ConferenceRoomService::ChangeLayout( int screenIndex, int displayModel )
{
    screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
	MediaWindowManager::getInstance()->ChangeLayout( screenIndex ,displayModel );
}

void ConferenceRoomService::CloseMediaWindow( const QString &user_id )
{
	VideoFrame* frame = MediaWindowManager::getInstance()->FindMediaWindow( user_id );
	if( frame )
	{
		if( CVIniConfig::getInstance().IsModelHD() /*&&
			user_id!=LOCAL_PLAY_USERID*/ )
		{
			//释放相应的channel
			_channelDispatcherProxy->ReleaseChannel( userID2MediaID(user_id), STREAMED_MEDIA_SERVICE_NAME );
    }
    MediaWindowManager::getInstance()->HideVideoFrame( frame );
		MediaWindowManager::getInstance()->CloseMediaWindow( user_id );
	}
}

void ConferenceRoomService::CloseWindow()
{

}

void ConferenceRoomService::ExitConference()
{
	if( _mediaProxyMap.size()>0 )
	{
		//释放所有的媒体流channel
		//2013-10-30 Liaokz补充：由于需求改为退出会议不结束本地回显，故不能销毁ChannelDispatcher，
		//非本地回显接收进程的结束工作改在CoolView中进行
		/*if( NConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_HD)
		{
			_channelDispatcherProxy->Destroy();
		}*/	

		StreamedMediaProxyMap::iterator it = _mediaProxyMap.begin();
		for( ; it!=_mediaProxyMap.end(); )
		{
			if (isLocalPreviewMediaID(it->first))
			{
				++it;
				continue;
			}
			CvStreamedMediaIf* streamedMediaIf = it->second;
			//streamedMediaIf->Stop();
			delete streamedMediaIf;
			streamedMediaIf = NULL;
			StreamedMediaProxyMap::iterator itOld = it++;
			_mediaProxyMap.erase(itOld); //Map中erase不影响迭代器次序
		}
		//_mediaProxyMap.clear();
	}
	
	
	MediaWindowManager::getInstance()->ClearMediaWindow();

	//ChangeLayout( 0 , CF_DM_L1_5 );
	//ChangeLayout( 1 , CF_DM_L1_5 );
}

/**
 * @brief 对媒体窗口的属性进行更改
 * @param user_id 用户名，通常是sip帐号的用户名称，不包括"@realm"字段
 * @param actionIndex 操作类型，查看MediaWindowAction枚举类型
 * @param arguments 操作所需要的参数
 */
void ConferenceRoomService::ModifyMediaWindow( const QString &user_id, int actionIdex, const QByteArray &arguments )
{
	QDataStream in( arguments);
	in.setVersion( QDataStream::Qt_4_4 );
	switch( actionIdex )
	{
	case MWA_ChangeWindowPosition:
		{
			int screenIndex;
			int seet;
			in >> screenIndex >> seet;
            screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
			VideoFrame* frame = MediaWindowManager::getInstance()->FindMediaWindow(user_id);
			if( frame )
			{
				//只有在同一个屏幕内的视频窗口才能进行拖动
				//if( user_id!=LOCAL_PLAY_USERID ||
				//	( user_id==LOCAL_PLAY_USERID && screenIndex==frame->_screenIndex)  )
				//{
				//	//更改媒体窗口
				//	MediaWindowManager::getInstance()->ChangeWindowPostion( user_id , screenIndex , seet );
				//}else
				//{
				//	//将已经隐藏掉的窗口显示出来
				//	MediaWindowManager::getInstance()->ShowVideoFrame(frame);
				//}

				//不再限制
				MediaWindowManager::getInstance()->ChangeWindowPostion( user_id , screenIndex , seet );
			}
			
			return;
		}
	case MWA_ChangeMediaState:
		{
			QString mediaType;
			int state;
			bool isAudioSend;
			in >> mediaType >> state>>isAudioSend;
			if( mediaType!="screen" )
			{
				VideoFrame* frame = MediaWindowManager::getInstance()->FindMediaWindow(user_id);
				if( frame )
				{
					MediaWindowManager::getInstance()->ChangeMediaState( frame , mediaType , static_cast<UiMediaState>(state),isAudioSend);
				}
			}else
			{
				MediaWindowManager::getInstance()->ChangeMediaState( NULL , "screen" , static_cast<UiMediaState>(state),isAudioSend);
			}

			return;
		}
	case MWA_AddScreenRecv:
		{
			QString ip;
			int port;
			int screenIndex;
			in >> ip >> port >> screenIndex;
            screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
			MediaWindowManager::getInstance()->AddScreenMediaRecv( user_id , ip , port , screenIndex );
			return;
		}
	case MWA_RemoveScreenRecv:
		{
			MediaWindowManager::getInstance()->RemoveScreenMediaRecv( user_id );
			return;
		}
	case MWA_ControlScreenShare:
		{
			bool control;
			in >> control;
			MediaWindowManager::getInstance()->ControlScreenShare( control );
			return;
		}
	case MWA_AddMainSpeakerScreenRecv:
		{
			QString ip;
			int port;
			in >> ip >> port;
			MediaWindowManager::getInstance()->AddMainSpeakerScreenMediaRecv( user_id , ip , port );
			return;
		}
	case  MWA_RemoveSendShareScreen:
		{
			
			MediaWindowManager::getInstance()->RemoveSendShareScreen();
			return;
		}
	case  MWA_PPTControlCommand:
		{
			int commd;
			in >> commd;
			int type=-1;
			switch(commd)
			{
				case 0 :
					{
						type=KG_RightHandLift;
						break;
					}
				case 1:
					{
						type=KG_LeftHandLift;
						break;
					}
			}
	
			MediaWindowManager::getInstance()->KinectGestureSlot("",type);
			return;
		}
	}
}

void ConferenceRoomService::ShowRtcpMsg( int screenIndex, bool show )
{
    screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
	MediaWindowManager::getInstance()->ShowRtcpMessage( screenIndex , show );
}

void ConferenceRoomService::UpdateRtcpMsg( const QByteArray &input_garray )
{
	QDataStream in( input_garray );
	in.setVersion( QDataStream::Qt_4_4 );
}

void ConferenceRoomService::resizeMediaWindow( const QString& userID , const int wnd )
{
	int xpos = 0, ypos = 0, width = 0, height = 0;
	int graphID = 0;
	int *value = NULL;
	RECT WindowRect;
	if( userID.length()<=0 )
	{
		return;
	}
	if (wnd)
	{
		QString mediaID;
		/*if( userID==LOCAL_PLAY_USERID )
		{
		    mediaID = SMALLVIDEO_STREAMED_MEDIA_ID;
		}else
		{*/
		mediaID = userID2MediaID(userID);
		//}
		StreamedMediaProxyMap::iterator it = _mediaProxyMap.find(mediaID );
		if( it!=_mediaProxyMap.end() )
		{
			HWND remoteHwnd = (HWND)wnd;
			GetWindowRect(remoteHwnd, &WindowRect);
			width = WindowRect.right - WindowRect.left;
			height = WindowRect.bottom - WindowRect.top;
			if (width == 0 || height == 0)
			{
				return;
			}
			if ( "screen_share"!= userID )
			{
				if ((width / height) >= (176 / 144))
				{
					xpos = (width - height * 176 / 144) / 2;
					width = height * 176 / 144;
				}
				else
				{
					ypos = (height - width * 144 / 176) / 2;
					height = width * 144 / 176;
				}
			}
			WndInfo info;
			info.initial( -1 , qPrintable(userID) , wnd , xpos , ypos , width , height );

			QByteArray outArray;
			QDataStream outDataStream(&outArray , QIODevice::WriteOnly);
			outDataStream.setVersion( QDataStream::Qt_4_4 );
			outDataStream << info;

			it->second->SetMediaInfo( Action_SetVideoWindow , outArray );
		}
	}
}

void ConferenceRoomService::TeleInfo( int info_index, int subscribe_id, const QByteArray &input_array )
{
	switch( info_index )
	{
	case TELE_QueryLayout:
		{
			MediaWindowManager::getInstance()->LayoutChangedNofity(subscribe_id);
			return;
		}
	case TELE_QueryMemberLocation:
		{
			MediaWindowManager::getInstance()->MemberLocationUpdateNotify(subscribe_id);
			return;
		}
	case TELE_QueryScreenShareState :
		{
			MediaWindowManager::getInstance()->ScreenShareStateUpdateNotify(subscribe_id);
			return;
		}
	}
}

void ConferenceRoomService:: OpenScreenShareFile(const QString &fileName, int screenIndex)
{
    screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	QString tempPath = appDir +  "\\share\\"+fileName;
	MediaWindowManager::getInstance()->OpenScreenShareFile(tempPath,screenIndex);
	
}

void ConferenceRoomService::RecoveryRecvMediaProcess(const QByteArray &input_garray, int screenIndex, int seet)
{
    screenIndex = ScreenHelper::Instance()->PosIndexToScreenIndex(screenIndex);
	VideoFrame* videoFrame = NULL;
	QString mediaID;
	QByteArray output_garray;
	QDataStream inDataStream( input_garray );
	inDataStream.setVersion( QDataStream::Qt_4_4 );
	
	RecvGraphInfo info;
	inDataStream >> info;

	//videoFrame = MediaWindowManager::getInstance()->AddMediaWindow( info.user_id , info.user_name, screenIndex , seet, info.video_info.width, info.video_info.height );
	videoFrame = MediaWindowManager::getInstance()->FindMediaWindow(info.user_id);
	if( videoFrame==NULL )
		return;

	//MediaWindowManager::getInstance()->ShowVideoFrame( videoFrame );

	info.recv_wnd = (int)videoFrame->LinkWin->getVideoFrame()->winId();

	QDataStream outDataStream( &output_garray , QIODevice::WriteOnly  );
	outDataStream.setVersion( QDataStream::Qt_4_4 );
	outDataStream << info;

	//否则根据mediaID创建媒体流进程
	mediaID = userID2MediaID( info.user_id);	

	//要求channelDispatcher创建媒体流进程，如果已经创建则会初始化他们
	if (CVIniConfig::getInstance().IsModelHD())
	{
		_channelDispatcherProxy->CreateChannel( mediaID , STREAMED_MEDIA_SERVICE_NAME , 
			output_garray );
	}



}