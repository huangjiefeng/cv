/*
 * 遥控器主动请求消息处理类声明文件
 * V1.0.0.0 2011-12-15    特定消息处理函数外的其它部分完成 By Lzy
 * V0.0.0.0 2011-11-23    By Lzy
 */

#ifndef P_MESSAGE_PROCESS_H
#define P_MESSAGE_PROCESS_H

#include "json.h"
#include <map>
#include <string>

using namespace std;

class CNetServ;

class CPMessageProcess
{
public:
    CPMessageProcess(CNetServ *pNetServ,int nSockResIndex);
    void MainProcess(const char *pData,int nLen);//遥控器主动消息处理函数

private:
    //------类型定义------
    typedef bool (CPMessageProcess::*FDealProcess)(Json::Value &rJObject);//各消息实现函数的原型定义，返回值为true时清理函数_SurProcess才会被执行
    //------变量------
    CNetServ *m_pNetServ;//CNetServ类实例指针，当进行网络操作时需要使用
    int m_nSockResIndex;//当前消息请求方的连接信息索引，当进行网络操作时需要用到
    static map<string,FDealProcess> ms_oFuncP;//消息函数指针
    string m_sKey;//用于搜索关联函数的键
    //------函数------
    
    /*
     * 【私有】【内联】设置消息键值
     * @param const char *pszMessageKey,作为消息键的字符串
     * @return void
     */
    void _SetMessageKey(const char *pszMessageKey)
    {
        if (pszMessageKey != NULL)
            this->m_sKey = pszMessageKey;
        else
            this->m_sKey.clear();
    }

    CPMessageProcess(const CPMessageProcess&){};//空复制构造函数，阻止类复制
    void operator=(const CPMessageProcess&){};//空复制/赋值函数，阻止类复制
    bool _PreProcess(Json::Value &rJObject);//消息预处理函数
    void _SurProcess(Json::Value &rJObject);//消息清理处理函数
    inline int _SendAsNotify(const char *pData,int nLen,int nTarget);//向指定目标发送通告消息
    inline int _SendAsCommandBack(const char *pData,int nLen);//向当前指令发来的连接发送返回消息

    ////////////////////其他自定义本类内专用的功能函数////////////////////////////
    
    /*
     * 【私有】【内联】把Json数据指定对象转换成数字返回，转换的对象只能是int类型或字符串类型
     * @param Json::Value &rJObject,JSon对象引用
     * @param const char *pszName,对象名
     * @return int,返回转换后的值
     */
    int JToInt(Json::Value &rJObject,const char *pszName)
    {
        if (rJObject[pszName].isInt())
            return rJObject[pszName].asInt();
        else
            return atoi(rJObject[pszName].asCString());
    }

    ////////////////特定消息处理函数在下面定义////////////////////////
    bool QueryOnline(Json::Value &rJObject);
    bool RecvMedia(Json::Value &rJObject);
    bool StopMedia(Json::Value &rJObject);
    bool SetVolumnIndex(Json::Value &rJObject);
    bool ChangeMediaLoc(Json::Value &rJObject);
    bool ChangeLayout(Json::Value &rJObject);
    bool ShowRTCP(Json::Value &rJObject);
    bool Shutdown(Json::Value &rJObject);
    bool Restart(Json::Value &rJObject);
    bool StopMyAudio(Json::Value &rJObject);
    bool StopMyVideo(Json::Value &rJObject);
    bool MouseMove(Json::Value &rJObject);
    bool ReceiveAudio(Json::Value &rJObject);
    bool EnterMeeting(Json::Value &rJObject);
    bool QuitMeeting(Json::Value &rJObject);
    bool ControllerQuit(Json::Value &rJObject);
    bool OpenScreenShare(Json::Value &rJObject);
    bool CloseScreenShare(Json::Value &rJObject);
	bool PPTControlCommand(Json::Value &rJObject);
    bool ApplyAccess(Json::Value &rJObject);
    bool ApplyResult(Json::Value &rJObject);
    bool SetNotifyIndex(Json::Value &rJObject);
    bool KeyInput(Json::Value &rJObject);
    bool ContextInput(Json::Value &rJObject);
    bool UpdateFile(Json::Value &rJObject);
	bool GetTerminalConfig(Json::Value &rJObject);
	bool SetPanelVisible(Json::Value &rJObject);
	bool SegmentVideo(Json::Value &rJObject);
};

#endif