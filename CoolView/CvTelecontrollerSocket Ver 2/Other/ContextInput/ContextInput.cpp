/*
 * 把指定的内容传送给网络输入法输出  类定义文件
 * 2012-05-02   V1.0.1.0    根据CNetManager类的调整而做了相应变更
 * 2012-02-28   V1.0.0.0    初版完成 By LZY
 * 2012-01-27   V0.0.0.0    创建文件 By Lzy
 */

#include "ContextInput.h"

CContextInput *CContextInput::m_pInstanceObject = NULL;

/*
 * 【私有】类构造函数，完成模块初始化
 * @
 */
CContextInput::CContextInput(ENUM_IPVersion eIPVer,unsigned int uPort)
{
    this->m_oTCPListenerID = CNetManager::GetInstance()->CreateTCPServ(NULL,eIPVer,uPort,NULL);
    if (this->m_oTCPListenerID.m_nPID < 0)
        printf("[E]CContextInput:Create TCP Serv Failed\n");
}

/*
 * 【私有】类析构函数，释放资源
 */
CContextInput::~CContextInput()
{
    if (this->m_oTCPListenerID.m_nPID >= 0)
    {
        SNodeID oClientID;
        oClientID.m_nPID = CLIENT_NONE;
        CNetManager::GetInstance()->CloseNet(oClientID,this->m_oTCPListenerID,true);
    }
}

/*
 * 【公开】发送要输出的内容给输入法
 * @param in unsigned short usCodePageNum,输出内容的编码页编号
 * @param in const char *pszContext,输出内容
 * @param int nContextLen,输出内容的长度
 * @return void
 */
void CContextInput::SendContext(unsigned short usCodePageNum,const char *pszContext,int nContextLen)
{
    char *pSendBuf = new char[nContextLen + 3];
    if (pSendBuf != NULL)
    {
        SNodeID oClientID;
        oClientID.m_nPID = CLIENT_ALL;
        *((unsigned short *)pSendBuf) = usCodePageNum;
        memcpy(pSendBuf + 2,pszContext,nContextLen);
        CNetManager::GetInstance()->SendNet(oClientID,this->m_oTCPListenerID,pSendBuf,nContextLen + 2);
        delete pSendBuf;
    }
}
