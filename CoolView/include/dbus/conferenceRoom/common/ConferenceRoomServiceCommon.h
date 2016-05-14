#pragma  once

#define CONF_ROOM_SERVICE_NAME			"com.coolview.ui.conferenceroom"
#define CONF_ROOM_SERVICE_OBJECT_PATH	"/com/coolview/ui/conferenceroom"
#define CONF_ROOM_INTERFACE_NAME		"com.coolview.ui.conferenceroom"

//#define LOCAL_PLAY_USERID	"LocalPreview"

#define UiStateTypeAny ""
#define UiStateTypeAudio "audio"
#define UiStateTypeVideo "video"
#define UiStateTypeScreen "screen"
#define UiStateTypeRecord "record"

enum UiMediaState{
	UiMediaState_Initializing,
	UiMediaState_Ready,
	UiMediaState_Run,
	UiMediaState_Stop,
  UiMediaState_Destroy,
  UiMediaState_RecordRun,
  UiMediaState_RecordStop,
  UiMediaState_RecordError, //一般错误
  UiMediaState_RecordSoapError, //Soap调用错误
  UiMediaState_RecordTxError, //Tx返回错误
};

enum RecordCtrlType
{
  RecCtrl_Start,
  RecCtrl_Stop,
};

enum ConfRoomDisplayModel
{
    CF_DM_UNKNOWN = -1,     //未知布局
	CF_DM_1X1,
	CF_DM_2X2,
	CF_DM_3X3,
	CF_DM_L3X3,				//采用3X3的布局，左上角为大屏
	CF_DM_ALL,
	CF_DM_L4X4,				//采用4X4的布局，左上角为大屏
	CF_DM_L5X5,				//采用5X5的布局，左上角为大屏
	CF_DM_L1_5,				//采用3X3的布局，左上角为大屏
	CF_DM_L1_20,			//在1_5的基础上将第一个屏幕之外的所有窗口改成4个
	CF_DM_1X1_REAL,			//仍然采用的是1X1布局，但是根据视频流的实际大小显示视频窗口
	CF_DM_DOC,				//文档阅读布局
	CF_DM_AUTO,             //采用自动布局
    CF_DM_TOP_1_2,          //在屏幕的上半部显示1X2的布局
    CF_DM_4X4,              //采用4X4的布局，共16路
    CF_DM_AIRPLAY,
};

inline void getPrimaryConferenceRoomService( QString* serviceName , QString* objectPath)
{
	*serviceName = QString(CONF_ROOM_SERVICE_NAME) + ".room0";
	*objectPath = QString(CONF_ROOM_SERVICE_OBJECT_PATH) + "/room0";
}

inline void getSecondConferenceRoomService( QString* serviceName , QString* objectPath)
{
	*serviceName = QString(CONF_ROOM_SERVICE_NAME) + ".room1";
	*objectPath = QString(CONF_ROOM_SERVICE_OBJECT_PATH) + "/room1";
}

enum MediaWindowAction
{
	MWA_ChangeWindowPosition,		//移动窗口
	MWA_ChangeMediaState,			//更改媒体流的状态
	MWA_AddScreenRecv,				//接收屏幕流
	MWA_RemoveScreenRecv,			//停止屏幕流
	MWA_ChangeMediaSipState,		//更新SIP信令的状态
	MWA_ControlScreenShare,			//控制屏幕共享权限，参数为true（主讲人）/false（非主讲人），
	MWA_AddMainSpeakerScreenRecv,   //接收主讲人屏幕流
	MWA_RemoveSendShareScreen,	    //移除发送屏幕共享
	MWA_PPTControlCommand           //执行PPT页面变化处理
};

#include <dbus/msdx/common/MsdxServiceCommon.h>
class UserMediaHandle
{
public:

	RecvGraphInfo	info;
	int				seet;	//窗口所在位置
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, UserMediaHandle& data)
	{
		in >> data.info >>data.seet;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, UserMediaHandle& data)
	{
		out <<data.info << data.seet;
		return out;
	}

#endif
};
