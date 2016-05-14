#pragma once

#define CVCORE_SERVICE_NAME			"com.dcampus.coolview.core"
#define CVCORE_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/core"


enum CoreAction
{
	CoreAction_SendQoS,
	CoreAction_StartRecvStreamedMedia,		//开始接收某一路视频流
	CoreAction_StopRecvStreamedMedia,		//停止接收某一路视频流
	CoreAction_SetVolume,					//设置终端音量
	CoreAction_ShutDown,					//退出CoolView
	CoreAction_Restart,						//重启CoolView
	CoreAction_ChangeMediaWindowPosition,	//更改媒体窗口的位置
	CoreAction_StopLocalAudio,				//停止本终端的音频发送
	CoreAction_StopLocalVideo,				//停止本终端的视频发送
	CoreAction_StartSendScreenMedia,		//创建发送屏幕流
	CoreAction_StartRecvScreenMedia,		//创建接收屏幕流，参数为memberUR和screenIndex
	CoreAction_StopRecvScreenMedia,			//停止接收屏幕流，参数为memberUR和screenIndex
	CoreAction_ControlAudioRecv,			//控制接收远程终端音频，参数为memberUR和enable
	CoreAction_ControlVideoRecv,			//控制接收远程终端视频，参数为memberUR和enable
	CoreAction_EnterMeeting,				//遥控终端进入指定会议
	CoreAction_ChangeLayout,				//遥控终端改变布局
	CoreAction_QuitMeeting,					//遥控终端退出会议
	CoreAction_CloseScreenRecv,             //屏幕共享接收关闭
	CoreAction_OpenScreenShare,				//遥控终端打开屏幕共享
	CoreAction_CloseScreenShare,            //遥控终端停止屏幕共享
	CoreAction_ShareScreenControl,			//向服务器发送开启或关闭共享屏幕消息，通知其他非主讲人终端开启或关闭接收屏幕共享
	CoreAction_PPTControlCommand,			//执行PPT页面改变指令
	CoreAction_GetTerminalConfig,           //遥控器获取终端配置请求
	CoreAction_SetPanelVisible,             //设置终端控制面板显示与否
	CoreAction_GetPannelState ,             //获取终端控制面板显示与否
	CoreAction_RecvMonitorCofinfo ,         //从监控模块中接收到监控信息
	CoreAction_RecoveryMeidaProcess,        //恢复媒体进程
	CoreAction_SegmentVideo,                //视频分段，用于向Tx发送分集信息
	CoreAction_ControlVideoRecord,          //视频录制控制
};