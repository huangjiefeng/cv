/*
 * 专用于处理鼠标动作的类的函数定义文件
 * V1.0.0.1 2011-11-23  把线程函数设置为类成员私有函数，把RunMove函数中JSon::Value类型的传入从指针改为引入
 * V0.0.0.0 2011-04-18 创建文件
 */

#include "MouseMove.h"
#include <process.h>
#include <vector>
using namespace std;

CMouseMove *CMouseMove::m_pInstanceObject = NULL;

struct SMouseMoveNode//单个鼠标消息的结构体
{
    int m_nPoint_X;
    int m_nPoint_Y;
    char m_chMethod;
};

/*
 * 【私有】【线程函数】鼠标动作执行函数，将被封在一条单独的线程中执行
 * @param void *pMoveData,传入的为vector<SMouseMoveNode>的指针，将在函数内被转换，同时最终完成后被释放
 * @return void
 */
void CMouseMove::_CMouseMoveSub(void *pMoveData)
{
    vector<SMouseMoveNode> *pSMouseMoveArray = (vector<SMouseMoveNode> *)pMoveData;
    int nArraySize = pSMouseMoveArray->size();
    SMouseMoveNode oLastNode,*pPreNode;
    oLastNode.m_chMethod = MOUSE_NONE;
    POINT oCurrentPoint;//读取鼠标当前位置值用
    int nScale = 2;//点倍率
    for (int nLoopVar = 0;nLoopVar < nArraySize;++nLoopVar)
    {
        pPreNode = &(pSMouseMoveArray->at(nLoopVar));
        if (oLastNode.m_chMethod != MOUSE_NONE)
        {
            GetCursorPos(&oCurrentPoint);
            SetCursorPos(oCurrentPoint.x + nScale * (pPreNode->m_nPoint_X - oLastNode.m_nPoint_X),oCurrentPoint.y + nScale * (pPreNode->m_nPoint_Y - oLastNode.m_nPoint_Y));
        }
        if (pPreNode->m_chMethod == MOUSE_LEFT_DOWN)
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        else if (pPreNode->m_chMethod == MOUSE_LEFT_UP)
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        else if (pPreNode->m_chMethod == MOUSE_RIGHT_DOWN)
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        else if (pPreNode->m_chMethod == MOUSE_RIGHT_UP)
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        oLastNode.m_chMethod = pPreNode->m_chMethod;
        oLastNode.m_nPoint_X = pPreNode->m_nPoint_X;
        oLastNode.m_nPoint_Y = pPreNode->m_nPoint_Y;
        Sleep(1);
    }
    delete pSMouseMoveArray;
    CMouseMove::GetInstance()->m_bCMouseMoveRunning = false;
}

/*
 * 【公开】【静态】获取类对象实例
 * @return CMouseMove *,返回实例指针
 */
CMouseMove *CMouseMove::GetInstance()
{
    if (CMouseMove::m_pInstanceObject == NULL)
        CMouseMove::m_pInstanceObject = new CMouseMove();
    return CMouseMove::m_pInstanceObject;
}

/*
 * 【公开】【静态】释放类对象实例
 * @return void
 */
void CMouseMove::ReleaseInstance()
{
    if (CMouseMove::m_pInstanceObject != NULL)
    {
        EnterCriticalSection(&(CMouseMove::m_pInstanceObject->m_oCMouseMoveSection));
        while (CMouseMove::m_pInstanceObject->m_bCMouseMoveRunning == true)
            Sleep(5);
        CMouseMove::m_pInstanceObject->m_bCMouseMoveRunning = true;
        DeleteCriticalSection(&(CMouseMove::m_pInstanceObject->m_oCMouseMoveSection));
        delete CMouseMove::m_pInstanceObject;
        CMouseMove::m_pInstanceObject = NULL;
    }
}

/*
 * 【公开】执行鼠标动作调用函数，但如果上一次的执行未完成则会返回失败
 * @param Json::Value &rJObject,存放动作的Json串
 * @return bool,返回动作是否被执行
 */
bool CMouseMove::RunMove(Json::Value &rJObject)
{
    if (this->m_bCMouseMoveRunning != true)
    {
        if (TryEnterCriticalSection(&(this->m_oCMouseMoveSection)) != false)
        {
            Json::Value oDataArray;
            oDataArray = rJObject["MMove"];
            if (oDataArray.isArray() == true)
            {
                int nArraySize = oDataArray.size();
                vector<SMouseMoveNode> *pSMouseMoveArray = new (vector<SMouseMoveNode>);
                SMouseMoveNode oSMouseMoveNode;
                for (int nLoopVar = 0;nLoopVar < nArraySize;++nLoopVar)
                {
                    oSMouseMoveNode.m_nPoint_X = oDataArray[nLoopVar]["X"].asInt();
                    oSMouseMoveNode.m_nPoint_Y = oDataArray[nLoopVar]["Y"].asInt();
                    oSMouseMoveNode.m_chMethod = oDataArray[nLoopVar]["M"].asInt();
                    pSMouseMoveArray->push_back(oSMouseMoveNode);
                }
                this->m_bCMouseMoveRunning = true;
                _beginthread(_CMouseMoveSub,0,pSMouseMoveArray);//启动线程
            }
            LeaveCriticalSection(&(this->m_oCMouseMoveSection));
            return true;
        }
    }
    return false;
}

/*
 * 【私有】类构造函数，完成内部变量初始化
 */
CMouseMove::CMouseMove()
{
    this->m_bCMouseMoveRunning = false;
    InitializeCriticalSection(&(this->m_oCMouseMoveSection));
}
