/*
 * 通告订阅请求与连接关闭时的处理封装 定义文件
 * V1.0.0.0 2011-12-09  By Lzy 完成函数定义，函数内容待填充
 * V0.0.0.0 2011-12-08  By Lzy
 */

#include "OMessageProcess.h"

#include "CommonMessageProcess.h"
using namespace Common_Message_Space;

#include "floor_manager.h"

namespace Notify_Process_Space
{
    /*
     * 【公开】当有新的通告订阅连接被建立时被调用的初始化函数
     * @param int nConnectionIndex,连接的标识号
     * @return void
     */
    void NewNotifyConnectionCreated(int nConnectionIndex)
    {
        QByteArray array;
        CQtDBusServ::GetInstance()->m_pConfTeleProxy->TeleInfo( TELE_QueryMemberLocation, nConnectionIndex , array );
        CQtDBusServ::GetInstance()->m_pConfTeleProxy->TeleInfo( TELE_QueryLayout , nConnectionIndex , array );
		CQtDBusServ::GetInstance()->m_pConfTeleProxy->TeleInfo( TELE_QueryScreenShareState , nConnectionIndex , array );

		QByteArray SubData;
		QDataStream SubOut(&SubData,QIODevice::WriteOnly);
		SubOut.setVersion(QDataStream::Qt_4_4 );
		SubOut << nConnectionIndex;
		CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_GetPannelState, SubData);

        //发送通告连接索引号
        Json::Value oRoot;
        oRoot["info_type"] = Json::Value(TELE_NewNotifyIndex);
        oRoot["notify_index"] = Json::Value(nConnectionIndex);
        Json::FastWriter oWriter;
        string strSend = oWriter.write(oRoot);
        CNetServ::GetInstance()->AddNotify(strSend.c_str(),strSend.length(),nConnectionIndex);

        //发送拥有终端控制权的sip账号信息
        oRoot.clear();
        oRoot["info_type"] = Json::Value(TELE_CurrentController);
#ifdef USE_FCS
        std::string monitorSip = FloorManager::GetInstance()->GetFirstChairURI();
        oRoot["usersip"] = Json::Value(monitorSip);
#else
        const char *pMonitorSip = CDataManager::GetInstance()->GetDataString("MonitorSip");
        if (pMonitorSip != NULL)
          oRoot["usersip"] = Json::Value(pMonitorSip);
        else
          oRoot["usersip"] = Json::Value("");
#endif //  USE_FCS
        strSend = oWriter.write(oRoot);
        CNetServ::GetInstance()->AddNotify(strSend.c_str(),strSend.length(),nConnectionIndex);
    }

    /*
     * 【公开】当通告订阅连接被检测出已断开时被调用的清理函数
     * @param int nConnectionIndex,连接的标识号
     * @return void
     */
    void NotifyConnectionClosed(int nConnectionIndex)
    {
        char szNum[20];
        CDataManager *pDataManager = CDataManager::GetInstance();
        _snprintf(szNum,20,"%d",nConnectionIndex);
        const char *pszOldLinkSip = pDataManager->GetDataString(szNum);
#ifdef USE_FCS
        if (pszOldLinkSip != NULL) {
          FloorManager::GetInstance()->OnUserOffline(pszOldLinkSip);
        }
#else
        const char *pszOldMonitorSip = pDataManager->GetDataString("MonitorSip");
        if (pszOldLinkSip != NULL && pszOldMonitorSip != NULL && strcmp(pszOldMonitorSip,pszOldLinkSip) == 0)
          SetNewMonitor("",-1);
#endif // USE_FCS
        pDataManager->SetDataString(NULL,false,szNum);
    }
};
