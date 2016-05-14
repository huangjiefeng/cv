/*
 * 所有消息处理模块公共部分的封装 定义文件
 * 2011-12-15   V1.0.0.0    By Lzy
 */

#include "CommonMessageProcess.h"

namespace Common_Message_Space
{
    /*
     * 【公开】设置新的权限信息并向所有遥控器发送通告
     * @param const char *pszSipAccount,获得控制权限的Sip帐号名，该指针必须有效
     * @param int nNotifyIndex,获得控制权限的遥控器关联的通告连接索引号
     * @return void
     */
    void SetNewMonitor(const char *pszSipAccount,int nNotifyIndex)
    {
        //设置权限信息
        CDataManager *pDataManager = CDataManager::GetInstance();
        pDataManager->SetDataString(pszSipAccount,false,"MonitorSip");
        const char *pszMonitor = pDataManager->GetDataString("MonitorSip");
        if (pszMonitor != NULL && strcmp(pszMonitor,pszSipAccount) == 0)
            pDataManager->SetDataLong(nNotifyIndex,false,"MonitorNotifyIndex");
        //发送通告消息
        Json::Value oRoot;
        oRoot["info_type"] = Json::Value(TELE_CurrentController);
        oRoot["usersip"] = Json::Value(pszSipAccount);
        Json::FastWriter oWriter;
        string strSend = oWriter.write(oRoot);
        CNetServ::GetInstance()->AddNotify(strSend.c_str(),strSend.length(),NOTIFY_ALL);
    }
    
};