#pragma once

#define CHANNEL_DISPATCHER_SERVICE_NAME    "com.dcampus.coolview.channelDispatcher"
#define CHANNEL_DISPATCHER_OBJECT_PATH    "/com/dcampus/coolview/channelDispatcher"


enum MediaState
{
    Media_NoInitial,
    Media_Running,
    Media_Pause,
    Media_Stop,
    Media_Destroyed
};

enum MediaDirection
{
    MediaDirection_Unknow,
    MediaDirection_In,
    MediaDirection_out,
    MediaDirection_ScreenOut,
    MediaDirection_PlayOut
};


enum MediaModifyType
{
    MMT_EnableAutoResync = 9,   //开启或关闭接收端的自动同步纠正，MMT结构值是个bool值    
    MMT_CutRecordingVideo,
    MMT_ShowConsoleWindow,
};
