#include "stdafx.h"
#include "SessionManager.h"


void SessionManager::addRecvSession( 
									std::string userId,const int graphId , char * AudioIP,int AudioPort,
									const char* audioCodecMIME , const int audioSampleRate , const int audioBps , const int audioChannels,
									char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	MediaSession* recvSession = new MediaSession();
	recvSession->_userId = userId;
	strcpy( recvSession->_ipAddr , VideoIP );
	recvSession->_videoPort = VideoPort;
	recvSession->_audioPort = AudioPort;
	recvSession->_videoWnd = remoteHwnd;
	recvSession->_graphId = graphId;

	_mediaRecvSessions.push_back( recvSession );
}

void SessionManager::removeRecvSession(std::string userId)
{
	if( _mediaRecvSessions.size()>0 )
	{
		MediaSession* pSession = NULL;
		for( MediaRecvSessions::iterator it = _mediaRecvSessions.begin() ; it!=_mediaRecvSessions.end(); it++ )
		{
			if( (*it)->_userId == userId )
			{
				pSession = *it;
				_mediaRecvSessions.erase( it );
				delete pSession;
				pSession = NULL;
				break;
			}
		}
	}

}

void SessionManager::clearRecvSessions()
{
	if( _mediaRecvSessions.size()>0 )
	{
		for( MediaRecvSessions::iterator it = _mediaRecvSessions.begin() ; it!=_mediaRecvSessions.end(); it++ )
		{
			MediaSession* pSession = *it;
			delete pSession;
			pSession =  NULL;
		}
		_mediaRecvSessions.clear();
	}
}

MediaSession* SessionManager::getRecvSession( const int& graphid)
{
	if( graphid<0 )
	{
		return NULL;
	}
	if( _mediaRecvSessions.size()>0 )
	{
		for( MediaRecvSessions::iterator it = _mediaRecvSessions.begin() ; it!=_mediaRecvSessions.end(); it++ )
		{
			MediaSession* pSession = *it;
			if( graphid == pSession->_graphId )
			{
				return pSession;
			}
		}
	}
	return NULL;
}

void SessionManager::addConfSession( const std::string& focus_uri , const int& callID, const std::string& password /*= "" */ )
{
	if( _confSession==NULL )
	{
		_confSession = new ConferenceSession();
	}
	_confSession->_focusUI = focus_uri;
	_confSession->_callID = callID;
	_confSession->_password = password;
}

void SessionManager::removeConfSession()
{
	if( _confSession )
	{
		delete _confSession;
		_confSession = NULL;
	}
}

ConferenceSession* SessionManager::getConfSession()
{
	return _confSession;
}