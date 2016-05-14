#pragma once

#define MOBICONTROLLER_SERVICE_NAME			"com.dcampus.coolview.controller.mobicontroller"
#define MOBICONTROLLER_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/controller/mobicontroller"

#include <QtCore/QtCore>

enum TeleCommandIndex
{
    Index_RecvMedia,
    Index_StopMedia,
    Index_ContactMember,     //Telecontroller的旧命令，不建议使用
    Index_StopMember         //Telecontroller的旧命令，不建议使用
};


class DeviceInfo
{
public:
    QString         _deviceName;
    QString         _ipAddress;
};
