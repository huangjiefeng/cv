#pragma  once

#ifdef CONFROOMCLIENT_EXPORTS    //主要在项目属性中设置此预处理器
#define CONF_ROOM_CLIENT_API __declspec(dllexport)
#else
#define CONF_ROOM_CLIENT_API __declspec(dllimport)
#endif

#include <dbus/msdx/common/MsdxServiceCommon.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

class ConferenceRoomIf;
class CONF_ROOM_CLIENT_API ConfRoomDBusClient
{
public:
	static ConfRoomDBusClient* getInstance()
	{
		static ConfRoomDBusClient client;
		return &client;
	}

	/**
	 * @brief 通知会议室里创建一路发送流，并指定本地回显的窗口位置
	 * @param sendInfo 发送信息
	 * @param seet	窗口位置，如果为负则随机分配
	 */
	void AddSendMedia( SendGraphInfo& sendInfo , const int seet );

	/**
	 * @brief 通知会议室创建一路接收流，并指定视频窗口窗口位置
	 * @param sendInfo 发送信息
	 * @param seet	窗口位置，如果为负则随机分配
	 */
	void AddRecvMedia( RecvGraphInfo& recvInfo , const int seet );

	/**
	 * @brief 通知会议室关闭某一路视频流
	 * @param userID 用户名
	 */
	void CloseMedia( std::string userID );

	/**
	 * @brief 调整显示模式
	 */
	void SetDisplayModel( ConfRoomDisplayModel displayModel);

	/**
	 * @brief 显示RTCP信息条
	 */
	void ShowRtcpMessage( const bool show );

	/**
	 * @brief 退出会议
	 */
	void ExitConference( );

	/**
	 * @brief 关闭Coolview会议室窗口
	 */
	void CloseConfRoomWindow();

private:
	ConfRoomDBusClient();
	~ConfRoomDBusClient();

private:
	//client proxy
	ConferenceRoomIf*  _conRoomIf;
	
};
