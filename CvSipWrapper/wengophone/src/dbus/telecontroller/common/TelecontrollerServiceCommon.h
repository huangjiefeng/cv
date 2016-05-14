#pragma once

#define TELECONTROLLER_SERVICE_NAME			"com.dcampus.coolview.controller.telecontroller"
#define TELECONTROLLER_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/controller/telecontroller"

//-------Add By LZY 2010-10-06-------------
#define CHANGE_MEETINGLIST 0 //会议列表变更
#define CHANGE_MEMBERLIST 1 //与会人员列表变更
#define CHANGE_LAYOUT 2 //视频布局变更
#define CHANGE_UI 3 //界面驻态变更
#define CHANGE_INVITE 4 //收到邀请
#define CHANGE_KICK 5 //被踢出会议
#define CHANGE_CONTACT 6 //联系人信息发生变化
#define CHANGE_LOCALVIDEO 7 //本地视频的发送设置发生了变化
#define CHANGE_LOCALAUDIO 8 //本地音频的发送设置发生了变化
//-------End of Add By LZY----------------