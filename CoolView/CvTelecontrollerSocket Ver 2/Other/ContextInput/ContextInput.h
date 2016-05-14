/*
 * 把指定的内容传送给网络输入法输出  头文件
 * 2012-05-02   V1.0.1.0    根据CNetManager类的调整而做了相应变更
 * 2012-02-28   V1.0.0.0    初版完成 By LZY
 * 2012-01-27   V0.0.0.0    创建文件 By Lzy
 */

#ifndef CONTEXT_INPUT_H
#define CONTEXT_INPUT_H

#define LOOP_CONTEXT_COUNT 15 //循环队列的队列长度

#include "NetManager.h"

class CContextInput
{
public:
    /*
     * 【公开】【内联】【静态】创建类对象实例
     * @param in ENUM_IPVersion eIPVer,监听输入法连接的IP版本
     * @param in unsigned int uPort,监听端口号
     * @return CContextInput *,返回创建好的对象
     */
    static CContextInput *CreateInstance(ENUM_IPVersion eIPVer,unsigned int uPort)
    {
        if (CContextInput::m_pInstanceObject != NULL)
            delete CContextInput::m_pInstanceObject;
        CContextInput::m_pInstanceObject = new CContextInput(eIPVer,uPort);
        return CContextInput::m_pInstanceObject;
    }

    /*
     * 【公开】【内联】【静态】获取类对象实例
     * @return CContextInput *,返回类对象实例，如果实例未构造则返回NULL
     */
    static CContextInput *GetInstance()
    {
        return CContextInput::m_pInstanceObject;
    }

    /*
     * 【公开】【内联】【静态】释放类对象实例
     * @return void
     */
    static void ReleaseInstance()
    {
        if (CContextInput::m_pInstanceObject != NULL)
            delete CContextInput::m_pInstanceObject;
        CContextInput::m_pInstanceObject = NULL;
    }

    void SendContext(unsigned short usCodePageNum,const char *pszContext,int nContextLen);//发送要输出的内容给输入法
private:
    //------结构体------
    struct SContextNode
    {
        char m_szContext[256];//存放要发送给输入法的字符串（已包含编码页信息）
        int m_nContextLen;//m_szContext中有效内容总长
    };
    //------变量------
    static CContextInput *m_pInstanceObject;//静态对象
    /*CRITICAL_SECTION m_oCContextInputSection;//写入内容到循环队列的临界区
    SContextNode m_sLoopContext[LOOP_CONTEXT_COUNT];//循环队列
    int m_nLoopWrite,m_nLoopRead;//循环队列的写入/读取指针*/
    SNodeID m_oTCPListenerID;//监听输入法连接的连接索引结构
    //------函数------
    CContextInput(ENUM_IPVersion eIPVer,unsigned int uPort);
    ~CContextInput();
    CContextInput(const CContextInput&){;};
};

#endif