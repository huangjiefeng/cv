#pragma  once

#define CONF_ROOM_SERVICE_NAME			"com.coolview.ui.conferenceroom"
#define CONF_ROOM_SERVICE_OBJECT_PATH	"/com/coolview/ui/conferenceroom"
#define CONF_ROOM_INTERFACE_NAME		"com.coolview.ui.conferenceroom"

enum ConfRoomDisplayModel
{
	CF_DM_1X1,
	CF_DM_2X2,
	CF_DM_3X3,
	CF_DM_L3X3,				//采用3X3的布局，左上角为大屏
	CF_DM_ALL,
	CF_DM_L4X4,				//采用4X4的布局，左上角为大屏
	CF_DM_L5X5				//采用5X5的布局，左上角为大屏
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