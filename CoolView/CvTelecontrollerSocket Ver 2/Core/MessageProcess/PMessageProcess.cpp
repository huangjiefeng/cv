/*
 * 遥控器主动请求消息处理类定义文件
 * V1.0.0.0 2011-12-15    特定消息处理函数外的其它部分完成 By Lzy
 * V0.0.0.0 2011-11-23    By Lzy
 */

#include "PMessageProcess.h"

#include "CommonMessageProcess.h"
using namespace Common_Message_Space;

#include "MouseMove.h"
#include "ContextInput.h"
#include "FileUD.h"

#include "floor_manager.h"

#define ADD_API(BindKeyName,BindFuncName) ms_oFuncP.insert(pair<string,FDealProcess>(BindKeyName,&CPMessageProcess::BindFuncName));

map<string,CPMessageProcess::FDealProcess> CPMessageProcess::ms_oFuncP;
/*
 * 【公开】CPMessageProcess类构造函数
 * @param CNetServ *pNetServ,CNetServ类实例指针，存入值必须有效
 * @param int nSockResIndex,主动请求所在连接的信息索引
 */
CPMessageProcess::CPMessageProcess(CNetServ *pNetServ,int nSockResIndex)
{
    this->m_pNetServ = pNetServ;
    this->m_nSockResIndex = nSockResIndex;
    if (CPMessageProcess::ms_oFuncP.empty() != false)
    {
        /////////////////////////////加入对特定遥控器消息处理函数的注册代码////////////////////////////
            ADD_API("QueryOnline",      QueryOnline)
            ADD_API("RecvMedia",        RecvMedia)
            ADD_API("StopMedia",        StopMedia)
            ADD_API("SetVolumnIndex",   SetVolumnIndex)
            ADD_API("ChangeMediaLoc",   ChangeMediaLoc)
            ADD_API("ChangeLayout",     ChangeLayout)
            ADD_API("ShowRTCP",         ShowRTCP)
			ADD_API("Shutdown",         Shutdown)
			ADD_API("Restart",          Restart)
            ADD_API("StopMyAudio",      StopMyAudio)
            ADD_API("StopMyVideo",      StopMyVideo)
            ADD_API("MouseMove",        MouseMove)
            ADD_API("ReceiveAudio",     ReceiveAudio)
            ADD_API("EnterMeeting",     EnterMeeting)
            ADD_API("QuitMeeting",      QuitMeeting)
            ADD_API("ControllerQuit",   ControllerQuit)
            ADD_API("OpenScreenShare",  OpenScreenShare)
            ADD_API("CloseScreenShare", CloseScreenShare)
			ADD_API("PPTControlCommand", PPTControlCommand)
            ADD_API("ApplyAccess",      ApplyAccess)
            ADD_API("ApplyResult",      ApplyResult)
            ADD_API("SetNotifyIndex",   SetNotifyIndex)
            ADD_API("KeysInput",        KeyInput)
            ADD_API("ContextInput",     ContextInput)
            ADD_API("UpdateFile",       UpdateFile)
			ADD_API("GetTerminalConfig",GetTerminalConfig)
			ADD_API("SetPanelVisible",  SetPanelVisible)
			ADD_API("SegmentVideo",     SegmentVideo)
        ///////////////////////////////////////////////////////////////////////////////////////
    }
}

/*
 * 【私有】【内联】向指定目标发送通告消息
 * @param const char *pData,发送数据所在缓冲区的指针
 * @param int nLen,发送数据的长度
 * @param int nTarget,发送目标，设置为-1表示对所有建立了被动消息接收连接的遥控器发送
 * @return int,返回操作结果 0-正常结束 1——暂存队列已满 负数——要发送的数据超出当前缓冲区大小的值（差值）【返回值定义总是与CNetServ::AddNotify的返回值定义一致】
 */
inline int CPMessageProcess::_SendAsNotify(const char *pData,int nLen,int nTarget)
{
    return this->m_pNetServ->AddNotify(pData,nLen,nTarget);
}

/*
 * 【私有】【内联】向当前指令发来的连接发送返回消息
 * @param const char *pData,发送数据所在缓冲区的指针
 * @param int nLen,发送数据的长度
 * @return void,返回发送的字节数，出现错误返回SOCKET_ERROR【返回值定义总是与CNetServ::CommandSendBack的返回值定义一致】
 */
inline int CPMessageProcess::_SendAsCommandBack(const char *pData,int nLen)
{
    return this->m_pNetServ->CommandSendBack(pData,nLen,this->m_nSockResIndex);
}

/*
 * 【公开】遥控器主动消息处理函数
 * @param const char *Data,接收数据所在缓冲区的指针
 * @param int Len,接收数据的长度
 */
void CPMessageProcess::MainProcess(const char *pData,int nLen)
{
    Json::Reader f_oDataRead;
    Json::Value f_oJObject;
    string f_sData;
    f_sData.append(pData,nLen);
    if ( ! f_oDataRead.parse(f_sData,f_oJObject))
    {
        printf("ReceiveData Can Not Be Parsed\n");
        return;
    }
    if (this->_PreProcess(f_oJObject) != false)
    {
        map<string,FDealProcess>::iterator iFunI = CPMessageProcess::ms_oFuncP.find(this->m_sKey);
        if (iFunI == CPMessageProcess::ms_oFuncP.end())
            return;
        if ((this->*(iFunI->second))(f_oJObject) != false)
            this->_SurProcess(f_oJObject);
    }
}

//////////////////////////////////对所有遥控器主动请求消息做前期处理的代码在_PreProcess函数中实现////////////////////

/*
 * 【私有】消息预处理函数
 * @info 如果希望触发特定消息处理函数，则必须在本函数内用_SetMessageKey设置与特定消息处理函数关联的消息键
 * @param Json::Value &JObject,数据被解析成Json后的读取对象
 * @return bool,后继执行控制，返回true表示继续执行后面针对特定消息的处理操作，返回false则本次消息处理结束
 */
bool CPMessageProcess::_PreProcess(Json::Value &rJObject)
{
    if (rJObject["Action"].isNull() != false)
    {
        printf("[E]JSon Error,Without Action\n");
        return false;
    }
    printf("%s\n",rJObject["Action"].asCString());
    string strMySip;
    string strAction = rJObject["Action"].asCString();
    int nNotifyIndex;
    //1、预先设置即将执行的指令，并判断当前动作是否需要进行权限验证
    this->_SetMessageKey(strAction.c_str());
    if (strAction == "QueryOnline" || strAction == "SetNotifyIndex" || strAction == "SetPanelVisible")
        return true;
    //2、获取验证所需要的信息
    if (rJObject["MySip"].isNull() == false && rJObject["MyNotifyIndex"].isNull() == false)
    {
        strMySip = rJObject["MySip"].asCString();
        nNotifyIndex = this->JToInt(rJObject,"MyNotifyIndex");
    }
    else
        return false;
    
    //3、检查当前是否已有遥控器获得权限
#ifdef USE_FCS
    if (nNotifyIndex > 0) {
      if (FloorManager::GetInstance()->IsUserHasAllFloors(strMySip)) {
        // 无需再申请权限
        return strAction != "AppyAccess";
      } else if (strAction != "AppyAccess") {
        FloorManager::GetInstance()->ApplyForAllFloors(strMySip, nNotifyIndex);
        return !FloorManager::GetInstance()->IsAnyFloorGranted();
      }
      return true;
    }
    return true;

#else
    CDataManager *pDataManager = CDataManager::GetInstance();
    const char *pszMonitor = pDataManager->GetDataString("MonitorSip");
    if (pszMonitor != NULL && pszMonitor[0] != '\0')
    {
      //3.1、已有遥控器获得控制权
      if (strMySip == pszMonitor)
        return true;
      else
      {
        //3.1.x、检查原先的权限信息是否已经失效
        char szNum[20];
        _snprintf(szNum,20,"%ld",pDataManager->GetDataLong("MonitorNotifyIndex"));
        const char *pszLinkSip = pDataManager->GetDataString(szNum);
        if (pszLinkSip != NULL && strcmp(pszMonitor,pszLinkSip) == 0)//权限不属于当前遥控器
        {
          if (strAction != "ApplyAccess")
            return false;//拒绝除申请权限外的操作
          else
            return true;//允许申请权限的操作执行
        }
      }
    }
    //4、设置新权限并发送通告消息
    SetNewMonitor(strMySip.c_str(),nNotifyIndex);
    if (strAction != "ApplyAccess")
      return true;
    else
      return false;
#endif // USE_FCS
}

//////////////////////////////////对所有遥控器主动请求消息做最终清理的代码在_SurProcess函数中实现////////////////////

/*
 * 【私有】消息清理处理函数
 * @info 如果前面_PreProcess执行后返回false，或特定消息处理函数执行后返回false，则本函数不会被执行
 * @param Json::Value &JObject,数据被解析成Json后的读取对象
 * @return void
 */
void CPMessageProcess::_SurProcess(Json::Value &rJObject)
{
}

/////////////////////////////////各遥控器主动请求消息的处理函数请写在下面///////////////////////////////////////////

#define FUNC_HEAD()\
    QByteArray SubData;\
    QDataStream SubOut(&SubData,QIODevice::WriteOnly);\
    SubOut.setVersion(QDataStream::Qt_4_4 );\

bool CPMessageProcess::QueryOnline(Json::Value &rJObject)
{
    this->_SendAsCommandBack("Online",7);
    return false;
}

bool CPMessageProcess::RecvMedia(Json::Value &rJObject)
{
    FUNC_HEAD()
    
    QString UserSip;int ScreenIndex;int Seet; int use_small_video;
    UserSip = rJObject["UserSip"].asCString();
    ScreenIndex = this->JToInt(rJObject,"ScreenIndex");
    Seet = this->JToInt(rJObject,"Seet");
    use_small_video = this->JToInt(rJObject, "Stream");

    SubOut << UserSip << ScreenIndex << Seet;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_StartRecvStreamedMedia,SubData);
    return true;
}

bool CPMessageProcess::StopMedia(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString UserSip;int ScreenIndex;
    UserSip = rJObject["UserSip"].asCString();
    ScreenIndex = this->JToInt(rJObject,"ScreenIndex");
    
    SubOut << UserSip << ScreenIndex;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_StopRecvStreamedMedia,SubData);
    return true;
}

bool CPMessageProcess::SetVolumnIndex(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString Direction;int VolumnIndex;
    Direction = rJObject["Direction"].asCString();
    VolumnIndex = this->JToInt(rJObject,"VolumnIndex");
        
    SubOut << Direction << VolumnIndex;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_SetVolume,SubData);
    return true;
}

bool CPMessageProcess::ChangeMediaLoc(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString UserSip;int ScreenIndex;int Seet;
    UserSip = rJObject["UserSip"].asCString();
    ScreenIndex = this->JToInt(rJObject,"ScreenIndex");
    Seet = this->JToInt(rJObject,"Seet");
    
    SubOut << UserSip << ScreenIndex << Seet;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_ChangeMediaWindowPosition,SubData);
    return true;
}

bool CPMessageProcess::ChangeLayout(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString LayoutType;int ScreenIndex;
    LayoutType = rJObject["LayoutType"].asCString();
    ScreenIndex = this->JToInt(rJObject,"ScreenIndex");
    int LayoutIndex = CF_DM_ALL;
    if (LayoutType == "1")
        LayoutIndex = CF_DM_1X1 ;
    else if (LayoutType == "2" )
        LayoutIndex = CF_DM_2X2 ;
    else if (LayoutType == "3")
        LayoutIndex = CF_DM_3X3;
    else if (LayoutType == "A")
        LayoutIndex = CF_DM_ALL;
    else if (LayoutType == "L")
        LayoutIndex = CF_DM_L3X3;
    else if (LayoutType == "L4")
        LayoutIndex = CF_DM_L4X4;
    else if (LayoutType == "L5")
        LayoutIndex = CF_DM_L5X5;
    else if (LayoutType == "L20")
        LayoutIndex = CF_DM_L1_20;
    else if (LayoutType=="D" )
        LayoutIndex = CF_DM_DOC;
    else if (LayoutType == "AirPlay")
        LayoutIndex = CF_DM_AIRPLAY;
    else if (LayoutType == "AUTO")
        LayoutIndex = CF_DM_AUTO;
    else if (LayoutType == "T1x2")
        LayoutIndex = CF_DM_TOP_1_2;
    else if (LayoutType == "4")
        LayoutIndex = CF_DM_4X4;
    
    SubOut <<ScreenIndex<<LayoutIndex;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_ChangeLayout,SubData);
    return true;
}

bool CPMessageProcess::ShowRTCP(Json::Value &rJObject)
{
    FUNC_HEAD()

    bool Visibility;int ScreenIndex;
    Visibility = rJObject["Visibility"].asBool();
    ScreenIndex = this->JToInt(rJObject,"ScreenIndex");

    CQtDBusServ::GetInstance()->m_pConfRoomProxy->ShowRtcpMsg(ScreenIndex,Visibility);
    return true;
}

bool CPMessageProcess::Shutdown(Json::Value &rJObject)
{
    //system("shutdown -s -t 30");
    QByteArray EmptyBytes;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_ShutDown,EmptyBytes);
    CQtDBusServ::GetInstance()->m_pTeleProxy->TeleInfo(TELE_ExitProcess,-1,EmptyBytes);
    return true;
}

bool CPMessageProcess::Restart(Json::Value &rJObject)
{
    //system("shutdown -s -t 30");
    QByteArray EmptyBytes;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_Restart,EmptyBytes);
    CQtDBusServ::GetInstance()->m_pTeleProxy->TeleInfo(TELE_ExitProcess,-1,EmptyBytes);
    return true;
}

bool CPMessageProcess::StopMyAudio(Json::Value &rJObject)
{
    FUNC_HEAD()

    int check = rJObject["Check"].asInt();

    SubOut << check;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_StopLocalAudio ,SubData);
    return true;
}

bool CPMessageProcess::StopMyVideo(Json::Value &rJObject)
{
    FUNC_HEAD()

    int check = rJObject["Check"].asInt();
    int video_index = rJObject["VideoIndex"].asInt();

    SubOut << check << video_index;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_StopLocalVideo , SubData );
    return true;
}

bool CPMessageProcess::MouseMove(Json::Value &rJObject)
{
    CMouseMove::GetInstance()->RunMove(rJObject);
    return false;
}

bool CPMessageProcess::ReceiveAudio(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString memberURI = rJObject["UserSip"].asCString();
    bool check = rJObject["Check"].asBool();

    SubOut << memberURI << check;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_ControlAudioRecv , SubData);
    return true;
}

bool CPMessageProcess::EnterMeeting(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString meetingURI = rJObject["Uri"].asCString();
    
    SubOut << meetingURI;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_EnterMeeting, SubData);
    return true;
}

bool CPMessageProcess::QuitMeeting(Json::Value &rJObject)
{
    SetNewMonitor("",-1);
    QByteArray EmptyBytes;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_QuitMeeting, EmptyBytes);
    return true;
}

bool CPMessageProcess::ControllerQuit(Json::Value &rJObject)
{
    SetNewMonitor("",-1);
    return true;
}

bool CPMessageProcess::OpenScreenShare(Json::Value &rJObject)
{
    FUNC_HEAD()

    QString fileName = rJObject["FileName"].asCString();
    int ScreenIndex = this->JToInt(rJObject,"ScreenIndex");
    
    SubOut << fileName << ScreenIndex;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_OpenScreenShare, SubData);
    return true;
}

bool CPMessageProcess::CloseScreenShare(Json::Value &rJObject)
{
    QByteArray EmptyBytes;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_CloseScreenShare, EmptyBytes);
    return true;
}

bool CPMessageProcess::PPTControlCommand(Json::Value &rJObject)
{
    FUNC_HEAD()

	int commandType = this->JToInt(rJObject,"Type");
	SubOut << commandType;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_PPTControlCommand, SubData);
    return true;
}

bool CPMessageProcess::ApplyAccess(Json::Value &rJObject)
{
    //预处理中可认为已保证了当前权限在其它帐号手上，下面的操作可省去一些检查
#ifdef USE_FCS
  std::string strMySip = rJObject["MySip"].asCString();
  int nNotifyIndex = this->JToInt(rJObject,"MyNotifyIndex");
  FloorManager::GetInstance()->ApplyForAllFloors(strMySip, nNotifyIndex);
#else
  const char *pszChairman = CDataManager::GetInstance()->GetDataString("Chairman");
  string strApplySip = rJObject["MySip"].asCString();
  int nMonitorNotifyIndex = CDataManager::GetInstance()->GetDataLong("MonitorNotifyIndex");
  if (nMonitorNotifyIndex < 0 || (pszChairman != NULL && strApplySip == pszChairman))
    SetNewMonitor(strApplySip.c_str(),this->JToInt(rJObject,"MyNotifyIndex"));
  else
  {
    Json::Value oRoot;
    oRoot["info_type"] = Json::Value(TELE_ApplyAccess);
    oRoot["usersip"] = Json::Value(strApplySip);
    oRoot["index"] = Json::Value(this->JToInt(rJObject,"MyNotifyIndex"));
    Json::FastWriter oWriter;
    string strSend = oWriter.write(oRoot);
    this->_SendAsNotify(strSend.c_str(),strSend.length(),nMonitorNotifyIndex);
  }
#endif // USE_FCS
  return true;
}

bool CPMessageProcess::ApplyResult(Json::Value &rJObject)
{
  
#ifdef USE_FCS
  bool bResult = rJObject["Result"].asBool();
  std::string requester_uri = rJObject["UserSip"].asCString();
  std::string chair_uri = rJObject["MySip"].asCString();
  if (bResult == true) {
    FloorManager::GetInstance()->AcceptAccess(chair_uri, requester_uri);
  } else {
    FloorManager::GetInstance()->DenyAccess(chair_uri, requester_uri);
  }
#else
  bool bResult = rJObject["Result"].asBool();
  int nNotifyIndex = this->JToInt(rJObject,"Index");
  if (bResult == true)
    SetNewMonitor(rJObject["UserSip"].asCString(),nNotifyIndex);
  //单独通知申请权限的遥控器申请结果
  Json::Value oRoot;
  oRoot["info_type"] = Json::Value(TELE_ApplyResult);
  oRoot["result"] = Json::Value(bResult);
  Json::FastWriter oWriter;
  string strSend = oWriter.write(oRoot);
  this->_SendAsNotify(strSend.c_str(),strSend.length(),nNotifyIndex);
#endif // USE_FCS

    return true;
}

bool CPMessageProcess::SetNotifyIndex(Json::Value &rJObject)
{
    string strMySip = rJObject["MySip"].asCString();
    int nNotifyIndex = rJObject["MyNotifyIndex"].asInt();
    char szNum[20];
    _snprintf(szNum,20,"%d",nNotifyIndex);
    CDataManager::GetInstance()->SetDataString(strMySip.c_str(),false,szNum);
    FloorManager::GetInstance()->UpdateUserNotifyIndex(strMySip, nNotifyIndex);
    return true;
}

bool CPMessageProcess::KeyInput(Json::Value &rJObject)
{
    Json::Value DataArray = rJObject["KeyCodes"];
    if (DataArray.isArray() == true)
    {
        int ArraySize = DataArray.size();
        if(ArraySize < 1 || ArraySize > 2)
            return true;

        int index = 0;
        int Code0 = DataArray[index]["Code"].asInt();
        keybd_event(Code0, 0, 0, 0);
        if(ArraySize == 2){
            int Code1 = DataArray[1]["Code"].asInt();
            keybd_event(Code1, 0, 0, 0);
            keybd_event(Code1, 0, KEYEVENTF_KEYUP, 0);
        }
        keybd_event(Code0, 0, KEYEVENTF_KEYUP, 0);
    }
    return true;
}

bool CPMessageProcess::ContextInput(Json::Value &rJObject)
{
    string strContext = rJObject["Context"].asCString();
    int nCodePageNum = rJObject["CodePageNum"].asInt();
    CContextInput::GetInstance()->SendContext(nCodePageNum,strContext.c_str(),strContext.length());
    return true;
}

bool CPMessageProcess::UpdateFile(Json::Value &rJObject)
{
    string strFilePath = ".\\share\\";
	system("mkdir share");
	strFilePath += rJObject["FileName"].asCString();
	string strTmpFilePath = strFilePath;
    strTmpFilePath.append(".tmp");//设置上传信息记录文件的文件名
    long long llTmp;
    llTmp = rJObject["Size_1"].asInt();
    llTmp += (long long)(rJObject["Size_2"].asInt()) << 16;
    llTmp += (long long)(rJObject["Size_3"].asInt()) << 32;
    llTmp += (long long)(rJObject["Size_4"].asInt()) << 48;
	int nReturn = CFileUpDownServ::GetInstance()->RegUpdateTrans(strFilePath.c_str(),strTmpFilePath.c_str(),llTmp,
        rJObject["ConnectionCount"].asUInt(),rJObject["BlockCount"].asInt(),rJObject["OverWrite"].asBool(),llTmp);
    Json::Value oRoot;
    oRoot["UpdateID"] = nReturn;
    oRoot["FinishedSize_1"] = (int)(llTmp & 0x0ffff);//填入64位数中从低位算起0-15位
    oRoot["FinishedSize_2"] = (int)((llTmp >> 16) & 0x0ffff);//填入64位数中从低位算起16-31位
    oRoot["FinishedSize_3"] = (int)((llTmp >> 32) & 0x0ffff);//填入64位数中从低位算起32-47位
    oRoot["FinishedSize_4"] = (int)((llTmp >> 48) & 0x0ffff);//填入64位数中从低位算起48-63位
    Json::FastWriter oWriter;
    string strSend = oWriter.write(oRoot);
    this->_SendAsCommandBack(strSend.c_str(),strSend.length());
    return true;
}

bool CPMessageProcess::GetTerminalConfig(Json::Value &rJObject)
{
    QByteArray EmptyBytes;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand( CoreAction_GetTerminalConfig, EmptyBytes);
    return true;
}

bool CPMessageProcess::SetPanelVisible(Json::Value &rJObject)
{
    FUNC_HEAD()

    bool visible;
	visible = rJObject["Visible"].asBool();
    
    SubOut << visible;
    CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_SetPanelVisible, SubData);
    return true;
}

bool CPMessageProcess::SegmentVideo( Json::Value &rJObject )
{
	QByteArray EmptyBytes;
	CQtDBusServ::GetInstance()->m_pCoreProxy->TeleCommand(CoreAction_SegmentVideo, EmptyBytes);
	return true;
}
