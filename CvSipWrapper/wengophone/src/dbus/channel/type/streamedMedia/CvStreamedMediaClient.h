#pragma  once

#ifdef CVSTREAMEDMEDIACLIENT_EXPORTS    //主要在项目属性中设置此预处理器
#define STREAMED_MEDIA_CLIENT_API __declspec(dllexport)
#else
#define STREAMED_MEDIA_CLIENT_API __declspec(dllimport)
#endif

#include <QtCore/QtCore>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>
 
class ChannelDispatcherIf;
class CvStreamedMediaIf;
class ConferenceRoomIf;

class STREAMED_MEDIA_CLIENT_API CvStreamedMediaClient
{
public:
	static CvStreamedMediaClient* getInstance()
	{
		static CvStreamedMediaClient client;
		return &client;
	}

	
	/**
	 * @brief 根据输入参数创建发送Graph
	 * @param send_info 发送相关参数
	 * @return 非负表明工作正常
	 */
	int createSendGraph( const QByteArray& send_info );

	/**
	 * @brief 停止发往指定IP的媒体流
	 * @param net_info 指定了将停止的目标IP与端口
	 * @return 非负表明工作正常
	 */
	int stopSend( const QByteArray& net_info);

	/**
	 * @brief 根据输入参数创建接收Graph
	 * @param userID 用于表示媒体流
	 * @param recv_info 接收媒体流的相关参数
	 * @return 如果成功创建则返回graphID ，否则返回负值
	 */
	int createRecvGraph( const QString& userID , const QByteArray& recv_info );

	/**
	 * @brief 根据UserID销毁对应的RecvGraph
	 * @param userID RecvGraph对应的userID
	 * @return 非负表示工作正常
	 */
	int stopRecv( const QString& userID );

	/**
	 * @brief 退出会议。销毁RecvGraph
	 * @return 非负表示工作正常
	 */
	int exitConference( );

	/**
	 * @brief 设置Qos信息
	 * @param userID 用户名
	 * @param qos_info qos信息
	 * @return 非负表示工作正常
	 */
	int setQosInfo( const QString& userID , const QByteArray& qos_info );

private:
	CvStreamedMediaClient();
	~CvStreamedMediaClient();


	ChannelDispatcherIf*		_channelDispatcherProxy;
	ConferenceRoomIf*		_confRoomProxy;
};