/************************************************************************/
/* @author zhenHua.sun
 * @date 2010-08-31
 * @brief 存储视频会议过程中的即时状态，如进入了哪些会议，创建了那些会话
          （即音视频的收发等信息）。以供各个模块的读取
 */
/************************************************************************/
#ifndef RUNNING_PROFILE_H
#define RUNNING_PROFILE_H

#include "SessionManager.h"



class RunningProfile
{
public:
	static RunningProfile& getInstance()
	{
		static RunningProfile runningProfile;
		return runningProfile;
	}

	/**
	 * @brief 获取会话管理器
	 */
	SessionManager& getSessionManager()
	{
		return _sessionManager;
	}

private:
	RunningProfile(){}
	~RunningProfile(){}

	////以下函数只定义不实现，防止出现类拷贝
	////杜绝 CClass object = CClass::getInstance()的出现
	RunningProfile( const RunningProfile& );
	RunningProfile& operator= ( const RunningProfile& );

	SessionManager _sessionManager;


};




#endif