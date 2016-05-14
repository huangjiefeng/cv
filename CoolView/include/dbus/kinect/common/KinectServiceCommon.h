#ifndef _KINECT_SERVICE_COMMON_H_
#define _KINECT_SERVICE_COMMON_H_

#define KINECT_SERVICE_NAME		"com.dcampus.coolview.kinect"
#define KINECT_OBJECT_PATH		"/com/dcampus/coolview/kinect"

enum KinectGesture
{
	/* 举起左手到顶部 */
	KG_LeftHandUp,

	/* 举起右手到顶部 */
	KG_RightHandUp,

	/* 举起左手到左边 */
	KG_LeftHandLift,

	/* 举起右手到右边 */
	KG_RightHandLift,
};

enum KinectMessageType
{
	KM_Information,		//提示信息
	KM_Warning,			//警告信息
	KM_Error,			//错误信息
	KM_Normal,			//kinect工作正常
};

#endif