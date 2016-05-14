#include "terminal-controller.h"
#include<QtGui/QtGui>

#include "util/MediaManager.h"
#include "conferencelist-widget.h"
#include "profile/RunningProfile.h"

#include "config/RunningConfig.h"
#include "DeviceManager/DeviceManager.h"
TerminalController::TerminalController( ConferenceListWidget* parent )
{
	_confListWidget = parent;
}

TerminalController::~TerminalController()
{

}

/******************************************分屏选择****************************************/

void TerminalController::onOneAndOneClicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_1X1 , screenIndex );
}

void TerminalController::onTwoAndTwoClicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_2X2 , screenIndex );
}

void TerminalController::onThreeAndThreeClicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_3X3 , screenIndex );
}

void TerminalController::onLClicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_L3X3, screenIndex );
}

void TerminalController::onAutoLayoutClicked(int screenIndex )
{
	MediaManager::getInstance()->ChangeLayout(CF_DM_AUTO,screenIndex);
}

void TerminalController::onL20Clicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_L1_20 , screenIndex );
}

void TerminalController::onOneAndOneRealClicked( int screenIndex )
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_1X1_REAL , screenIndex );
}

void TerminalController::onAllClicked(int screenIndex)
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_ALL , screenIndex );
}


void TerminalController::onDocumentClicked( int screenIndex )
{
	MediaManager::getInstance()->ChangeLayout( CF_DM_DOC , screenIndex );
}

void TerminalController::onReceiveVideo(QString name,int screenIndex,int seet)
{
	if( _confListWidget && _confListWidget->getConferenceManager() )
	{
		_confListWidget->getConferenceManager()->addRecvStreamMedia( RunningProfile::getInstance()->username() , qPrintable(name) , 
		screenIndex , seet );
	}
}

void TerminalController::onStopVideo(QString name,int screenIndex)
{
	if( _confListWidget && _confListWidget->getConferenceManager() )
	{
		_confListWidget->getConferenceManager()->removeRecvStreamMedia( RunningProfile::getInstance()->username() , qPrintable(name) );
	}
}

void TerminalController::onShowRTCPMessage(bool check , int index)
{
	MediaManager::getInstance()->showRtcpMsg( check ,index );
}

void TerminalController::onChangeVideoSeet( QString memberName,int screenIndex,int seet)
{
	_confListWidget->getConferenceManager()->changeMemberSeat( memberName.toStdString() , screenIndex , seet );
}

void TerminalController::onVoiceChange( QString direction,int volumeIndex )
{
	RunningConfig* config = RunningConfig::Instance();
	if( direction=="in" )
	{
		if( config->AudioCaptureDevice().size()>0 )
		{
			DeviceManager::GetInstatnce()->SetAudioInputVolumn( qPrintable(config->AudioCaptureDevice()) , volumeIndex );
		}
		return;
	}else
	{
		if( config->AudioOutputDevice().size()>0 )
		{
			DeviceManager::GetInstatnce()->SetAudioOutputVolumn(  volumeIndex );
		}
		return;
	}
}


//start....控制音视频发送的启停...zhenHua.sun 20110325
void TerminalController::StopMyAudio( bool disable )
{
	if( _confListWidget && _confListWidget->getConferenceManager() )
	{
		_confListWidget->getConferenceManager()->controlSendMedia( 0 , !disable );
	}
}

void TerminalController::StopMyVideo( bool disable)
{
	if( _confListWidget && _confListWidget->getConferenceManager() )
	{
		_confListWidget->getConferenceManager()->controlSendMedia( 1, !disable );
	}
}

/**
 * @brief 控制本地是否接收远程终端的音频
 * @uri 远程终端的URI
 * @check 是否停止
 */
void TerminalController::ReceiveAudio( QString uri, bool check )
{
	ConferenceManager* confManage = NULL;
	if( _confListWidget )
		confManage = _confListWidget->getConferenceManager();

	if( confManage && confManage->checkIsInConference() )
	{
		MediaManager::getInstance()->ControlAudioRecv( uri , check );
	}
}

void TerminalController::onReceiveDocument( QString uri ,int screen )
{
	ConferenceManager* confManage = NULL;
	if( _confListWidget )
		confManage = _confListWidget->getConferenceManager();

	if( confManage && confManage->checkIsInConference() )
	{
		confManage->createRecvScreenMedia( uri,screen );
	}
}

void TerminalController::onStopDocument( QString uri ,int screen )
{
	ConferenceManager* confManage = NULL;
	if( _confListWidget )
		confManage = _confListWidget->getConferenceManager();

	if( confManage && confManage->checkIsInConference() )
	{
		confManage->removeRecvScreenMedia( uri,screen );
	}
}

void TerminalController::SegmentMedia( QString uri )
{
	ConferenceManager* confManage = NULL;
	if( _confListWidget )
		confManage = _confListWidget->getConferenceManager();

	if( confManage && confManage->checkIsInConference() )
	{
		QString userID = uri;
		int index= userID.indexOf("@");
		if(  index!=-1 )
			userID = userID.left(index);

		MediaManager::getInstance()->SegmentMedia( userID, 0 );
	}
}

//end