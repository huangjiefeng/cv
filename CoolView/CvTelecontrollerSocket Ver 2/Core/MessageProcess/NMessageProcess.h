/*
 * 通告消息处理类声明文件
 * V1.0.0.0	2011-12-15	特定消息处理函数外的其它部分完成 By Lzy
 * V0.0.0.0 2011-11-23  By Lzy
 */

#ifndef N_MESSAGE_PROCESS_H
#define N_MESSAGE_PROCESS_H

#include <map>
#include <Qtcore/QtCore>

using namespace std;

//定义程序退出控制变量
#ifdef N_MESSAGE_PROCESS_CPP
bool g_bProgramme_Exit;
#else
extern bool g_bProgramme_Exit;
#endif

class CNMessageProcess
{
public:
    CNMessageProcess();
    void MainProcess(int nInfoIndex,int nNotifyId,const QByteArray &rInputArray);//接收到的通告消息的处理函数

private:
    //------类型定义------
    typedef bool (CNMessageProcess::*FDealProcess)(int nNotifyId,QDataStream &rQStream);//各消息实现函数的原型定义，返回值为true时清理函数_SurProcess才会被执行
    //------变量------
    static map<int,FDealProcess> ms_oFuncP;//消息函数指针
    int m_nKey;//用于搜索关联函数的键
    //------函数------
    
    /*
     * 【私有】【内联】设置消息键值
     * @param int nMessageKey,作为消息键的数值
     * @return void
     */
    void _SetMessageKey(int nMessageKey)
    {
        this->m_nKey = nMessageKey;
    }

    CNMessageProcess(const CNMessageProcess&){};//空复制构造函数，阻止类复制
    void operator=(const CNMessageProcess&){};//空复制/赋值函数，阻止类复制
    bool _PreProcess(int nInfoIndex,int nNotifyId,const QByteArray &rInputArray);//消息预处理函数
    void _SurProcess(int nInfoIndex,int nNotifyId,const QByteArray &rInputArray);//消息清理处理函数

    ////////////////////其他自定义本类内专用的功能函数////////////////////////////
    
    ////////////////特定消息处理函数在下面定义////////////////////////
    bool _ExitProcess(int nNotifyId,QDataStream &rQStream);
    bool _Layout(int nNotifyId,QDataStream &rQStream);
    bool _MemberLocation(int nNotifyId,QDataStream &rQStream);
    bool _MediaState(int nNotifyId,QDataStream &rQStream);
    bool _AddMemberLocation(int nNotifyId,QDataStream &rQStream);
    bool _RemoveMemeberLocation(int nNotifyId,QDataStream &rQStream);
    bool _ChangeMemberLocation(int nNotifyId,QDataStream &rQStream);
    bool _PostMeetingInfo(int nNotifyId,QDataStream &rQStream);
	bool _ScreenShareState(int nNotifyId,QDataStream &rQStream);
	bool _TerminalInfo(int nNotifyId,QDataStream &rQStream);
	bool _PanelState(int nNotifyId,QDataStream &rQStream);
};

#endif