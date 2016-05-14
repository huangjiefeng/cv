/************************************************************************/
/* @author zhenHua.sun
 * @date 2010-09-01
 * @brief 管理会议过程中建立的会话，如音视频的收发
 */
/************************************************************************/
#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H
#include <string>
using namespace std;
class MediaSession
{
public:
	string			_userId;
	int				_graphId;
	char			_ipAddr[48];
	unsigned short	_audioPort;
	unsigned short	_videoPort;
	HWND			_videoWnd;
};

class ConferenceSession
{
public:
	string			_focusUI;
	string			_password;
	int				_callID;
};

typedef std::vector<MediaSession*>  MediaRecvSessions;

class SessionManager
{
public:
	SessionManager()
	{
		_mediaRecvSessions.clear();
		_confSession = NULL;
	}
	~SessionManager()
	{
		_mediaRecvSessions.clear();
		if( _confSession )
		{
			delete _confSession;
			_confSession = NULL;
		}
	}

	MediaRecvSessions _mediaRecvSessions;

public:
	/**
	 * @brief 往内部队列中添加一个媒体接收会话
	 */
	void addRecvSession( std::string userId, const int graphId, char * AudioIP,int AudioPort,const char* audioCodecMIME , const int audioSampleRate , const int audioBps , const int audioChannels,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight );
	/**
	 * @brief 往内部队列中删除一个媒体接收会话
	 */
	void removeRecvSession( std::string userId);
	/**
	 * @brief 清空媒体接收会话
	 */
	void clearRecvSessions();
	/**
	 * @brief 获取指定IP获取对应的session
	 * @return 如果没找到则返回空指针，否则返回
	 */
	MediaSession* getRecvSession( const int& graphid );

	/**
	 * @brief 加入会议会话
	 */
	void addConfSession( const std::string& focus_uri , const int& callID, const std::string& password = "" );

	/**
	 * @brief 销毁会议会话
	 */
	void removeConfSession( );

	/**
	 * @brief 获取会议会话
	 * @return 如果没有进入任何会议将返回NULL
	 */
	ConferenceSession* getConfSession();

private:
	
	//保存用户进入的会议信息
	ConferenceSession*		_confSession;
};

#endif