/*
 * 所有消息处理模块公共部分的封装 声明文件
 * 2011-12-15   V1.0.0.0    By Lzy
 */

#ifndef COMMON_MESSAGE_PROCESS_H
#define COMMON_MESSAGE_PROCESS_H

#include "DataManager.h"
#include "NetServ.h"
#include "QtDbusServ.h"
#include <Qtcore/QtCore>

#include "json.h"

namespace Common_Message_Space
{
    void SetNewMonitor(const char *pszSipAccount,int nNotifyIndex);//设置新的权限信息并向所有遥控器发送通告
};

#endif