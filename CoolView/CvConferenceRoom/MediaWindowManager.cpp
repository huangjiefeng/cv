#include <QtWidgets/QtWidgets>

#include "MediaWindowManager.h"

#include "VideoFrame.h"
#include "QtConfrenceVideoWindow.h"
#include "QtMeetingPlaceWidget.h"
#include "qtconferenceroom.h"
#include "QtScreenShare.h"
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>

#include <dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h>

#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>

#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>

#include <dbus/kinect/client/KinectIf.h>
#include <dbus/kinect/common/KinectServiceCommon.h>

//core proxy
#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/core/client/CvCoreIf.h>

#include "kinectinfowidget.h"
#include "uicommon.h"
#include <log/Log.h>
#include "util/ini/CvIniConfig.h"
#include "util/ini/TxConfig.h"
#include "util/report/RecordStat.h"

MediaWindowManager::MediaWindowManager()
  : _updateScreenTimer(this)
{
	//根据显示器的数目创建会议室UI
    bool is_model_hd = 
      CVIniConfig::getInstance().IsModelHD();

	const int screenNum = (is_model_hd ? QApplication::desktop()->numScreens() : 
      CTxConfig::getInstance().GetRecUiScreenCount());
	for( int screenIndex=0 ; screenIndex<screenNum ; screenIndex++ )
	{
		QtConferenceRoom* conferenceRoom = new QtConferenceRoom(screenIndex);
		
		_confRoomVector.push_back( conferenceRoom );
        conferenceRoom->setVisible(is_model_hd);
    }

	//connect( QApplication::desktop() , SIGNAL(screenCountChanged(int)) , 
	//	this , SLOT(ScreenCountChanged(int)) );
	
	_isShowingRtcp = false;

	_teleControlProxy = new TelecontrollerIf( _GetTelecontrollerServiceName("sock"),
		_GetTelecontrollerObjectPath("sock") , QDBusConnection::sessionBus() );
	_channelDispatcherProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME,
		CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus() );

	_screenShareWidget = NULL;

	_screenPort = 0;
	_screenIP = "";

	_isSendingScreen = false;

	//kinect相关操作...zhenHua.sun 2011-08-18
	_kinectProxy = new KinectIf( KINECT_SERVICE_NAME,
		KINECT_OBJECT_PATH , QDBusConnection::sessionBus() );
	connect( _kinectProxy , SIGNAL(GestureOccur(const QString, int)), 
		this, SLOT(KinectGestureSlot(const QString,int)) );
	connect( _kinectProxy , SIGNAL(KinectMessage(int, const QString )),
		this, SLOT(KinectMessageSlot(int, const QString)) );

	_kinectInfoWidget = new KinectInfoWidget();
	_kinectInfoWidget->hide();

#ifdef NDEBUG
    connect(&_updateScreenTimer, &QTimer::timeout,
      this, &MediaWindowManager::updateScreenSlot);
    _updateScreenTimer.start(2000);
#endif

  //CoreProxy
  _coreProxy = new CvCoreIf(CVCORE_SERVICE_NAME, CVCORE_SERVICE_OBJECT_PATH, 
    QDBusConnection::sessionBus());

	//更改kinect的位置在第一块屏幕的正中
	QRect screenRect = QApplication::desktop()->availableGeometry();
	int xpos = ( screenRect.width()-_kinectInfoWidget->width() )/2;
	int ypos = ( screenRect.height()-_kinectInfoWidget->height() )/2;
	_kinectInfoWidget->setGeometry( xpos,ypos,_kinectInfoWidget->width(),_kinectInfoWidget->height());
}

MediaWindowManager::~MediaWindowManager()
{
	SAFE_DELETE(_kinectInfoWidget );
}

/** 
 * @brief 根据指定参数返回媒体窗口
 * @return 如果成功则返回媒体窗口，否则返回NULL
 */
VideoFrame* MediaWindowManager::AddMediaWindow( const QString& userID ,const QString& displayName, const int screenIndex, const int seet ,const int videoWidth , const int videoHeight)
{
	if( screenIndex > (_confRoomVector.size()-1) )
	{
		//如果屏幕的索引值大于最大屏幕索引值screen num -1 ，那么直接返回null
		return NULL;
	}
	VideoFrame *vFrame = NULL;
	//首先判断是否存在同样名字的videoFrame，有的话直接拿来用
	vFrame = FindMediaWindow( userID );
	if( vFrame )
	{
		if( vFrame->isUsed )
		{
			//如果窗口已经存在并且正在显示，那么只需要调整窗口为新位置
			this->ChangeWindowPostion( userID , screenIndex , seet );
			return NULL;
		}else
		{
			vFrame->isUsed = true;
			vFrame->_screenIndex = screenIndex;
			vFrame->_seet = seet;
			vFrame->LinkWin->setUserId( userID );
			vFrame->LinkWin->setDisplayName( displayName );
			vFrame->_videoWidth = videoWidth;
			vFrame->_videoHeight = videoHeight;
			vFrame->LinkWin->show();
		}
	}else
	{
		//如果没有可用的窗口，则新建一个
		//VideoFrame *vFrame = NULL;
		if( screenIndex>0 )
			vFrame = new VideoFrame( _confRoomVector.at(screenIndex)->getMeetingPlaceWidget());
		else
			vFrame = new VideoFrame( _confRoomVector.at(0)->getMeetingPlaceWidget() );

		QtConfrenceVideoWindow *child = new QtConfrenceVideoWindow();
		vFrame->layout.addWidget(child);
		vFrame->LinkWin = child;//Add By LZY 2010-10-09 加入关联的窗口指针
		vFrame->LinkWin->setUserId( userID );
		vFrame->LinkWin->setDisplayName( displayName );
		vFrame->isUsed = true;
		vFrame->_screenIndex = screenIndex;
		vFrame->_seet = seet;
		vFrame->_videoWidth = videoWidth;
		vFrame->_videoHeight = videoHeight;
		//video_frame_map[child] = vFrame;
		_memberFrames.push_back(vFrame);
		child->show();
	}

	if( vFrame && vFrame->LinkWin  )
	{
		//对于新加入的窗口要判断是否显示rtcp信息
		if( _isShowingRtcp /*&& userID!=LOCAL_PLAY_USERID*/ )
			vFrame->LinkWin->showRtcpMessage();
		else
			vFrame->LinkWin->hideRtcpMessage();

    //新窗口显示等待数据标识
    ChangeMediaState(vFrame, "", UiMediaState_Initializing, false);
	}

	return vFrame;
}

void MediaWindowManager::CloseMediaWindow( const QString& userID )
{
	VideoFrame* frame = FindMediaWindow( userID );

	if( frame )
	{
		if( frame->LinkWin )
		{
			this->MemberLocationRemoveNotify( userID ,  frame->_screenIndex , frame->_seet );
		}

		//对于本地回显窗口，窗口的资源不能被释放
		/*if( userID==LOCAL_PLAY_USERID )
			return;*/

		frame->Release();
	}
}


void MediaWindowManager::ClearMediaWindow()
{
	//_memberFramesLock.lock();

	MediaWindowVector localPreviewWindows;
	//注意不要清除本地回显的窗口
	for( int i=0  ; i<_memberFrames.size() ; i++ )
	{
		VideoFrame* frame = _memberFrames.at(i);
		if (frame == NULL)
		{
			continue;
		}

		if (isLocalPreviewMediaID(frame->LinkWin->getUserId()))
		{
			localPreviewWindows.push_back(frame);
			continue;
		}
		//在meetingplace中消除对视频窗口的关联
		HideVideoFrame( frame );

		frame->isUsed = false;
		if( frame->LinkWin )
		{
			frame->LinkWin->close();
			delete frame->LinkWin;
			frame->LinkWin = NULL;
		}
		delete frame;
		frame = NULL;
	}
	_memberFrames.clear();
	_memberFrames = localPreviewWindows;
	//_memberFramesLock.unlock();

	_screenPort = 0;
	_screenIP = "";

	for(int i=0 ; i<_confRoomVector.size();i++ )
	{
		if( _confRoomVector.at(i)&& _confRoomVector.at(i)->getScreenShareWidget() )
		{
			//退出会议的同时将之前打开了的ppt关掉  zhongBao.liang
			_confRoomVector.at(i)->getScreenShareWidget()->killPPT();
			_confRoomVector.at(i)->getScreenShareWidget()->release();
		}	
	}

}

// 会在UpdateRtcpMessage中多线程调用，需作同步。且应该由调用者管理同步锁
VideoFrame* MediaWindowManager::FindMediaWindow( const QString& userID )
{
	VideoFrame * pFrame = NULL;

	MediaWindowVector::iterator it = _memberFrames.begin();
	for( ; it!=_memberFrames.end();it++ )
	{
		if( (*it)->LinkWin && (*it)->LinkWin->getUserId()==userID )
		{
			pFrame = (*it);
			break;
		}
	}

	return pFrame;
}

void MediaWindowManager::ShowRtcpMessage( const int screenIndex, const bool show )
{
	//_memberFramesLock.lock();
	MediaWindowVector::iterator it = _memberFrames.begin();
	for( ; it!=_memberFrames.end();it++ )
	{
		if( (*it)->isUsed /*&& ((*it)->LinkWin->getUserId()!=LOCAL_PLAY_USERID)*/ )
		{
			if( show )
				(*it)->LinkWin->showRtcpMessage();
			else
				(*it)->LinkWin->hideRtcpMessage();
		}
	}
	//_memberFramesLock.unlock();
	_isShowingRtcp = show;
}

void MediaWindowManager::ShowVideoFrame( VideoFrame* frame )
{
	if( frame->_screenIndex>_confRoomVector.size()-1 )
	{
		CV_LOG_ERROR("视频窗口所在的屏幕并不存在！");
		return ;
	}
	if( frame->_screenIndex>0 )
	{
		_confRoomVector.at(frame->_screenIndex)->getMeetingPlaceWidget()->showVideoWindow( frame );
	}else
	{
		_confRoomVector.at(0)->getMeetingPlaceWidget()->showVideoWindow( frame );
	}

}

void MediaWindowManager::HideVideoFrame( VideoFrame* frame )
{
	if( frame->_screenIndex>_confRoomVector.size()-1 )
	{
		CV_LOG_ERROR("视频窗口所在的屏幕并不存在！");
		return ;
	}
	if( frame->_screenIndex>0 )
	{
		_confRoomVector.at(frame->_screenIndex)->getMeetingPlaceWidget()->hideVideoWindow( frame );
	}else
	{
		_confRoomVector.at(0)->getMeetingPlaceWidget()->hideVideoWindow( frame );
	}

}


void MediaWindowManager::ChangeLayout( int screenIndex, int displayModel )
{
	if( screenIndex>_confRoomVector.size()-1 || _confRoomVector.size()==0  )
	{
		_snprintf(__global_msg , sizeof(__global_msg), "改变布局时出错，屏幕索引值为%d，屏幕数目为:%d" , screenIndex, _confRoomVector.size() );
		CV_LOG_ERROR(__global_msg);
		return ;
	}

	int index = 0;
	if( screenIndex>0 )
		index = screenIndex;

	QtConferenceRoom* pConfRoom = _confRoomVector.at(index);
	if( pConfRoom )
	{
		if( displayModel==CF_DM_DOC )
		{
			//如果更换成文档布局
			//初始化共享屏幕
			pConfRoom->showScreenShare( );
            pConfRoom->getScreenShareWidget()->SetSharedMode(QtScreenShare::kDocumentMode);

		} else if (displayModel == CF_DM_AIRPLAY) {
            pConfRoom->showScreenShare();
            pConfRoom->getScreenShareWidget()->SetSharedMode(QtScreenShare::kAirPlayMode);
        } else {
            pConfRoom->getScreenShareWidget()->SetSharedMode(QtScreenShare::kVideoAudioMode);
			pConfRoom->showMeetringPlace();
			pConfRoom->getMeetingPlaceWidget()->updateDisplay( static_cast<ConfRoomDisplayModel>(displayModel) );
		}


		this->LayoutChangedNofity();
	}

}

void MediaWindowManager::ChangeWindowPostion( const QString& userID , const int newScreenIndex, const int newSeet )
{
	VideoFrame* frame = FindMediaWindow( userID );
	if( frame )
	{
		if( /*userID!=LOCAL_PLAY_USERID &&*/ frame->_screenIndex==newScreenIndex && frame->_seet==newSeet )
		{
			//非本地回显的情况下，位置不变的时候不进行任何操作
			return;
		}

		if( frame->_screenIndex!=newScreenIndex )
		{
			//移动到另外一个屏幕的时候graph会重新启动，因此音视频都会回到run的状态
			this->ChangeMediaState( frame , "video" , UiMediaState_Run ,true);
			this->ChangeMediaState( frame , "audio" , UiMediaState_Run ,true);
		}

		if( frame->_screenIndex!=newScreenIndex 
			|| frame->_seet != newSeet )
		{
			this->HideVideoFrame( frame );	
		}
		frame->_seet = newSeet;
		frame->_screenIndex = newScreenIndex;
		this->ShowVideoFrame( frame );
	}

	this->MemberLocationChangeNotify( userID , newScreenIndex , newSeet );
}

//void MediaWindowManager::UpdateRtcpMessage( const RtpStatItemInfo& stateInfo )
//{
//	_memberFramesLock.lock();
//	VideoFrame* frame = FindMediaWindow( stateInfo.user_id );
//	if( frame )
//	{
//		if( frame->LinkWin )
//		{
//			frame->LinkWin->updateRtcpEventHandler( stateInfo );
//		}
//	}
//	_memberFramesLock.unlock();
//}

void MediaWindowManager::UpdateRtcpMessage( const RtpStatItem& stateItem )
{
	//_memberFramesLock.lock();
	if( stateItem.media_type==eMT_Video )
	{
		VideoFrame* frame = FindMediaWindow( stateItem.member_id );
		if( frame )
		{
			if( frame->LinkWin )
			{
				frame->LinkWin->updateRtcpEventHandler( stateItem );
			}
		}
	}
	//_memberFramesLock.unlock();
}

void MediaWindowManager::UpdateRecordMessage( const RecStatItem& stateItem )
{
	//_memberFramesLock.lock();
    QString member_id = stateItem.user_name;
    //将本地录制的进程id替换成回放进程id，以便匹配窗口
    member_id = member_id.replace(LOCAL_RECORD_MEDIA_ID, LOCAL_PREVIEW_MEDIA_ID);
    VideoFrame* frame = FindMediaWindow(member_id);
    if( frame )
    {
        if( frame->LinkWin )
        {
            frame->LinkWin->updateRecordEventHandler( stateItem );
        }
    }
	//_memberFramesLock.unlock();
}

void MediaWindowManager::ScreenCountChanged( int newCount )
{
	if (!CVIniConfig::getInstance().IsModelHD())
	{
		return;
	}

	if( newCount==_confRoomVector.size() || newCount==0 )
	{
		//如果屏幕的数量与UI保持一致，或者屏幕数量更改为0时，
		//不进行任何操作
		return;
	}
	if( newCount < _confRoomVector.size() )
	{
		//显示器减少了，去除多余的界面
		for( int screenIndex=_confRoomVector.size()-1 ; screenIndex>=newCount ; screenIndex-- )
		{
			QtConferenceRoom* pConfRoom = _confRoomVector.at(screenIndex);

			//_memberFramesLock.lock();
			int memberFrameCount = _memberFrames.size();
			//关闭多余显示器上的视频窗口
			MediaWindowVector::iterator it = _memberFrames.begin();
			for( ; it!=_memberFrames.end() ; )
			{
				VideoFrame*	frame = *it;
				if( frame->_screenIndex==screenIndex )
				{
					//在meetingplace中消除对视频窗口的关联
					HideVideoFrame( frame );
					frame->isUsed = false;

					if( frame->LinkWin )
					{
						//释放媒体流
						_channelDispatcherProxy->ReleaseChannel( userID2MediaID(frame->LinkWin->getUserId()), STREAMED_MEDIA_SERVICE_NAME );

						frame->LinkWin->close();
						delete frame->LinkWin;
						frame->LinkWin = NULL;
					}

					delete frame;
					frame = NULL;

					it = _memberFrames.erase(it);
				}else
				{

					it++;
				}
			}

			//_memberFramesLock.unlock();

			//关闭会议室UI
			delete pConfRoom;
			pConfRoom = NULL;
			_confRoomVector.pop_back();
		}
	}else
	{
		//如果显示器增加了，那么添加新的界面UI
		int addUiNum = newCount - _confRoomVector.size();
		for( int i=0 ; i<addUiNum ; i++ )
		{
			QtConferenceRoom* confRoom = new QtConferenceRoom( _confRoomVector.size() );
			confRoom->show();
			_confRoomVector.push_back(confRoom);
		}
	}
	this->LayoutChangedNofity();
}

void MediaWindowManager::MemberLocationUpdateNotify( const int controllerIndex/*=-1*/ )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_MemberLocation;
	out << (int)0;							//正在接收成员的数据，获得具体值之后再重写它

	//首先检查音视频窗口
	int recvUserNum = 0;
	//_memberFramesLock.lock();

	MediaWindowVector::iterator it = _memberFrames.begin();
	for( ; it!=_memberFrames.end() ; it++ )
	{
		VideoFrame* frame = *it;
		if( frame->isUsed&&!frame->LinkWin->isHidden() )
		{
			if( frame->LinkWin )
			{
				MemberLocation location;
				location._memberName = frame->LinkWin->getUserId();
				location._screenIndex = ScreenHelper::Instance()->ScreenIndexToPosIndex(frame->_screenIndex);
				location._seet = frame->_seet;
				if( frame->LinkWin )
				{
					if( frame->LinkWin->isAudioEnable() )
					{
						location._audioEnable = true;
					}else
					{
						location._audioEnable = false;
					}
					if( frame->LinkWin->isVideoEnable() )
					{
						location._videoEnable = true;
					}else
					{
						location._videoEnable = false;
					}

				}
				out << location;
				recvUserNum++;
			}
		}
	}

	//_memberFramesLock.unlock();

	//然后检查屏幕共享窗口
	for( int i=0 ; i<_confRoomVector.size();i++ )
	{
		QtConferenceRoom* room = _confRoomVector.at(i);

		MemberLocation location;
		QString name = room->getScreenShareWidget()->UserID();
		int index = name.indexOf("@");
		if( index>=0 )
			name = name.left( index );
		location._memberName = name;
		location._screenIndex = ScreenHelper::Instance()->ScreenIndexToPosIndex(i);
		location._seet = 10;			//由于在遥控器上屏幕共享采用的是第10个窗口，所以这里是10
		location._audioEnable = false;
		location._videoEnable = false;
		out << location;
		recvUserNum++;
		
	}

	//返回写指针，设置接收人员数目
	out.device()->seek( sizeof(int) );
	out << recvUserNum;

	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo(TELE_MemberLocation , controllerIndex,output_array );
	}

}

void MediaWindowManager::LayoutChangedNofity( const int controllerIndex/*=-1 */ )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_Layout;
	out << _confRoomVector.size();
	for( int i=0 ; i<_confRoomVector.size() ; i++ )
	{
		ScreenInfo screen;
		screen._screenIndex = ScreenHelper::Instance()->ScreenIndexToPosIndex(i);
		if( _confRoomVector.at(i)->getMeetingPlaceWidget()->isHidden() )
		{
			//屏幕正在显示文档流
            switch (_confRoomVector.at(i)->getScreenShareWidget()->GetSharedMode()) {
              case QtScreenShare::kDocumentMode:
                screen._layout = "D";
                break;
              case QtScreenShare::kAirPlayMode:
                screen._layout = "AirPlay";
            }
		}else
		{
			switch( _confRoomVector.at(i)->getMeetingPlaceWidget()->_displayModel )
			{
			case CF_DM_1X1:
				screen._layout = "1";
				break;
			case CF_DM_2X2:
				screen._layout = "2";
				break;
			case CF_DM_3X3:
				screen._layout = "3";
				break;
			case CF_DM_L3X3:
			case CF_DM_L1_5:
				screen._layout = "L1_5";
				break;
			case CF_DM_L1_20:
				screen._layout = "L1_20";
				break;
			case CF_DM_AUTO:
				screen._layout = "AUTO";
				break;
            case CF_DM_TOP_1_2:
                screen._layout = "T1x2";
                break;
            case CF_DM_4X4:
                screen._layout = "4";
                break;
			default:
				screen._layout = "3";
			}
		}

		out << screen;
	}

	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo( TELE_Layout , controllerIndex, output_array );
	}
}

/**
 * @brief 更改接收媒体流的状态
 * @param frame 媒体流窗口
 * @param mediaType audio为音频，video为视频
 * @param state 媒体流的状态，参看枚举类UiMediaState
 * @param isAudioSend 如果为True说明当前的媒体流为发送流，否则为false
 * @param enable 如果接收则为True，否则为false
 */
void MediaWindowManager::ChangeMediaState( const VideoFrame* frame , const QString& mediaType , const UiMediaState state,bool isAudioSend)
{
	if( mediaType=="screen" )
	{
		QVector<QtConferenceRoom*>::iterator it = _confRoomVector.begin();
		QtConferenceRoom* pConfRoom;
		for( ; it!=_confRoomVector.end() ; it++ )
		{
			pConfRoom = *it;
			pConfRoom->getScreenShareWidget()->setMediaState(state);
		}
  }
  else /*if( mediaType=="audio" || mediaType=="video" )*/
  {
    if( frame && frame->LinkWin )
    {
      frame->LinkWin->setMediaState( mediaType , state ,isAudioSend);

      if( state==UiMediaState_Stop )
      {
        QString mediaState = "stop";
        this->MediaStateChangedNotify( frame->LinkWin->getUserId() , mediaType , mediaState );
      }else if( state==UiMediaState_Run )
      {
        QString mediaState = "run";
        this->MediaStateChangedNotify( frame->LinkWin->getUserId() , mediaType , mediaState  );
      }

    }
  }
	
}

/**
 * @brief 添加屏幕接收流，用于接收远程终端的屏幕信息。如果当前终端已经共享了屏幕并创建了屏幕发送进程，那么
          程序会先释放掉这些资源。也即无法在发送的同时接收远程终端的屏幕。
 * @param userID 远程终端的终端名
 * @param ip 远程终端的IP
 * @param port 屏幕共享端口
 * @param screenIndex 当前终端显示远程屏幕的显示器
 */
void MediaWindowManager::AddScreenMediaRecv( const QString& userID , const QString&ip , const int port ,const int screenIndex)
{
	if( screenIndex>_confRoomVector.size()-1 || _confRoomVector.size()==0  )
	{
		_snprintf(__global_msg , sizeof(__global_msg), "添加屏幕流时出错，屏幕索引值为%d，屏幕数目为:%d" , screenIndex, _confRoomVector.size() );
		CV_LOG_ERROR(__global_msg);
		return ;
	}


	
	//首先释放原来的屏幕接收流
	_channelDispatcherProxy->ReleaseChannel( RECV_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME);


	//告知屏幕刷屏
	int index = 0;
	if( screenIndex>0 )
		index = screenIndex;
	QtConferenceRoom* pConfRoom = _confRoomVector.at(index);
	if( pConfRoom && pConfRoom->getScreenShareWidget() )
	{
		//先关闭已经打开的文档并释放媒体流
		pConfRoom->getScreenShareWidget()->release();

		
		pConfRoom->getScreenShareWidget()->recvScreen(true );
		pConfRoom->getScreenShareWidget()->UserID(userID );
		pConfRoom->getScreenShareWidget()->setMediaState( UiMediaState_Initializing );
		
		ScreenMediaInfo info;
		info._port = port;
		info._ipAddress = ip;

		//添加新的屏幕接收流
		QByteArray outputBytes;
		QDataStream out(&outputBytes, QIODevice::WriteOnly );
		out << info;
		_channelDispatcherProxy->CreateChannel( RECV_SCREEN_MEDIA_ID, SCREEN_MEDIA_SERVICE_NAME,
			outputBytes );

		QString memberName = userID;
		int index = memberName.indexOf("@");
		if( index>-1 )
		{
			memberName = memberName.left(index);
		}
		this->MemberLocationAddNotify( memberName, screenIndex , 10 , false , false );
	}
}


void MediaWindowManager::AddMainSpeakerScreenMediaRecv( const QString& userID , const QString&ip , const int port )
{

	
	//首先释放原来的屏幕接收流
	_channelDispatcherProxy->ReleaseChannel( RECV_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME);


	for(int i=0 ; i<_confRoomVector.size();i++ )
	{
		if( _confRoomVector.at(i)&& _confRoomVector.at(i)->getScreenShareWidget() )
		{
			QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
			//先关闭已经打开的文档并释放媒体流
			pConfRoom->getScreenShareWidget()->closeFile();
			pConfRoom->getScreenShareWidget()->stopScreenGraph();


			pConfRoom->getScreenShareWidget()->recvScreen(true );
			pConfRoom->getScreenShareWidget()->UserID(userID );
			pConfRoom->getScreenShareWidget()->setMediaState( UiMediaState_Initializing );
		}	
	}

	ScreenMediaInfo info;
	info._port = port;
	info._ipAddress = ip;

	//添加新的屏幕接收流
	QByteArray outputBytes;
	QDataStream out(&outputBytes, QIODevice::WriteOnly );
	out << info;
	_channelDispatcherProxy->CreateChannel( RECV_SCREEN_MEDIA_ID, SCREEN_MEDIA_SERVICE_NAME,
		outputBytes );
}


void MediaWindowManager::RemoveScreenMediaRecv( const QString& userID,const int screenIndex/*=-1 */ )
{
	if( screenIndex>_confRoomVector.size()-1 || _confRoomVector.size()==0  )
	{
		_snprintf(__global_msg , sizeof(__global_msg), "添加屏幕流时出错，屏幕索引值为%d，屏幕数目为:%d" , screenIndex, _confRoomVector.size() );
		CV_LOG_ERROR(__global_msg);
		return ;
	}

	if( screenIndex>=0&& screenIndex<_confRoomVector.size()  )
	{
		//直接获取会议室的指针并停止屏幕流
		QtConferenceRoom* pConfRoom = _confRoomVector.at(screenIndex );
		if( pConfRoom && pConfRoom->getScreenShareWidget() && pConfRoom->getScreenShareWidget()->UserID()==userID )
		{
			//释放屏幕接收流
			_channelDispatcherProxy->ReleaseChannel( RECV_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME);

			//停止刷屏
			pConfRoom->getScreenShareWidget()->recvScreen(false );
			pConfRoom->getScreenShareWidget()->UserID("");
			pConfRoom->getScreenShareWidget()->setMediaState(UiMediaState_Destroy);

			QString memberName = userID;
			int index = memberName.indexOf("@");
			if( index>-1 )
			{
				memberName = memberName.left(index);
			}
			this->MemberLocationRemoveNotify( memberName , screenIndex , 10 );
			return;
		}
	}else
	{
		//否则枚举所有共享屏幕来查找userID
		int screenSize = _confRoomVector.size();
		for( int i=0 ; i<screenSize; i++ )
		{
			QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
			if( pConfRoom && pConfRoom->getScreenShareWidget()&&
				(userID=="" || pConfRoom->getScreenShareWidget()->UserID()==userID) )
			{

				//释放屏幕接收流
				_channelDispatcherProxy->ReleaseChannel( RECV_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME);

				//停止刷屏
				pConfRoom->getScreenShareWidget()->recvScreen(false );
				pConfRoom->getScreenShareWidget()->UserID("");
				pConfRoom->getScreenShareWidget()->setMediaState(UiMediaState_Destroy);

				QString memberName = userID;
				int index = memberName.indexOf("@");
				if( index>-1 )
				{
					memberName = memberName.left(index);
				}
				this->MemberLocationRemoveNotify( memberName , i , 10 );

			}
		}
	}
}

void MediaWindowManager::MediaStateChangedNotify( const QString&memberName , const QString&mediaType, const QString&mediaState, const int controllerIndex/*=-1 */ )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_MediaState;

	MediaStateNotify notify;
	notify._memberName = memberName;
	notify._mediaState = mediaState;
	notify._mediaType = mediaType;
	out << notify;

	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo(TELE_MediaState , controllerIndex,output_array );
	}
}

void MediaWindowManager::MemberLocationAddNotify( const QString& memberName , const int screen , const int seet , const bool audioEnable , const bool videoEnable )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_AddMemberLocation;

	MemberLocation location;
	location._memberName = memberName;
	location._screenIndex = ScreenHelper::Instance()->ScreenIndexToPosIndex(screen);
	location._seet = seet;

	location._audioEnable = audioEnable;
	location._videoEnable = videoEnable;

	out << location;

	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo(TELE_AddMemberLocation,-1,output_array );
	}
}

void MediaWindowManager::MemberLocationRemoveNotify( const QString& memberName , const int screen , const int seet  )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_RemoveMemeberLocation;

	out << memberName;
	out << ScreenHelper::Instance()->ScreenIndexToPosIndex(screen);
	out << seet;
	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo(TELE_RemoveMemeberLocation,-1,output_array );
	}
}

void MediaWindowManager::MemberLocationChangeNotify( const QString& memberName , const int newScreenIndex, const int newSeet )
{
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion( QDataStream::Qt_4_4 );
	out << TELE_ChangeMemberLocation;

	out << memberName;
	out << ScreenHelper::Instance()->ScreenIndexToPosIndex(newScreenIndex);
	out << newSeet;
	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo(TELE_ChangeMemberLocation,-1,output_array );
	}
}




void MediaWindowManager::KinectGestureSlot( const QString &to, int type )
{
	//QMessageBox::information(0,"Kinect", QString::number(type) , QMessageBox::Ok );

	//将指定的姿势转换为PPT控制命令
	QtScreenShare::PPTControlType pptControlType = QtScreenShare::PPTControl_Unknow;
	switch( type )
	{
	case KG_LeftHandLift:
		pptControlType = QtScreenShare::PPTControl_Pre;
		break;
	case KG_RightHandLift:
		pptControlType = QtScreenShare::PPTControl_Next;
		break;
	case KG_RightHandUp:
		pptControlType = QtScreenShare::PPTControl_Close;
		break;
	}

	//直接获取会议室的指针并停止屏幕流
	for( int i=0 ; i<_confRoomVector.size(); i++ )
	{
		QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
		if( pConfRoom && pConfRoom->getScreenShareWidget())
		{
			//控制屏幕上PPT的播放
			pConfRoom->getScreenShareWidget()->controlPPT( pptControlType );
		}
	}
}

void MediaWindowManager::KinectMessageSlot( int type, const QString &message )
{
	static QString lastMessage = "";
	switch( type )
	{
	case KM_Normal:
		_kinectInfoWidget->hide();
		break;
	case KM_Information:
	case KM_Warning:
	case KM_Error:
		{
			if( _kinectInfoWidget->isHidden()&&lastMessage!=message )
			{
				//同一个消息只显示一次
				_kinectInfoWidget->SetInfoText( message );
				_kinectInfoWidget->show();
				lastMessage = message;
			}

		}
		
		break;
	}
}

void MediaWindowManager::ControlScreenShare( const bool control )
{
	int screenSize = _confRoomVector.size();
	for( int i=0 ; i<screenSize; i++ )
	{
		QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
		if( pConfRoom && pConfRoom->getScreenShareWidget() )
		{
			pConfRoom->getScreenShareWidget()->ControlScreenShare( control );
		}
	}
}


void MediaWindowManager:: OpenScreenShareFile(const QString &filePath, int screenIndex)
{
	if(screenIndex >= _confRoomVector.size() || screenIndex<0)
		return;

	QtConferenceRoom* pConfRoom = _confRoomVector.at(screenIndex);
	pConfRoom->getScreenShareWidget()->openFile(filePath,screenIndex);


}


void MediaWindowManager:: RemoveSendShareScreen()
{
	
	for(int i=0; i<_confRoomVector.size();i++)
	{
		QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
		pConfRoom->getScreenShareWidget()->closeFile();
		pConfRoom->getScreenShareWidget()->stopScreenGraph();
	}

}


void MediaWindowManager:: ScreenShareStateUpdateNotify( const int controllerIndex /*=-1*/)
{
	bool shareState = false;
	
	for(int i=0; i<_confRoomVector.size();i++)
	{
		QtConferenceRoom* pConfRoom = _confRoomVector.at(i);
		if(pConfRoom->getScreenShareWidget()->getScreenShareState())
		{
			shareState = true;
			break;
		}
		
	}

	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out<<TELE_ScreenShareState;
	out << shareState;

	if( _teleControlProxy )
	{
		_teleControlProxy->TeleInfo( TELE_ScreenShareState , controllerIndex, output_array );
	}

}

void MediaWindowManager::ControlVideoRecording( const QString userId, int type, QByteArray param )
{
  if (_coreProxy) {
    QByteArray output_array;
    QDataStream out(&output_array , QIODevice::WriteOnly );
    out << userId << type << param;

    _coreProxy->TeleCommand(CoreAction_ControlVideoRecord, output_array);
  }
}

void MediaWindowManager::updateScreenSlot()
{
  ScreenHelper *helper = ScreenHelper::Instance();
  helper->Update();
  int screen_count = helper->GetScreenCount();
  if (CVIniConfig::getInstance().isAutoSetPrimaryScreen() && 
      helper->SetPrimaryScreen(screen_count - 1)){
    for (int i = 0; i < screen_count; ++i) {
      _confRoomVector[i]->showMaximizedToScreen();
    }
  }
}
