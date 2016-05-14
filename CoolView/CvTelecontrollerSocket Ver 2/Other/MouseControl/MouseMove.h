/*
 * 专用于处理鼠标动作的类的函数声明文件
 * V1.0.0.1 2011-11-23  把线程函数设置为类成员私有函数，把RunMove函数中JSon::Value类型的传入从指针改为引入
 * V0.0.0.0 2011-04-18  创建文件
 */

#ifndef MOUSE_MOVE_H
#define MOUSE_MOVE_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include "windows.h"
#include "json.h"

//------鼠标动作定义------
#define MOUSE_LEFT_DOWN 1//鼠标左键按下
#define MOUSE_LEFT_UP 2//鼠标左键释放
#define MOUSE_RIGHT_DOWN 3//鼠标右键按下
#define MOUSE_RIGHT_UP 4//鼠标右键释放
#define MOUSE_MOVE 5//鼠标移动
#define MOUSE_NONE 0//不表示任何鼠标动作，仅作为无效节点看待
//----------------------

class CMouseMove
{
public:
    static CMouseMove *GetInstance();//获取类对象实例
    static void ReleaseInstance();//释放类对象实例
    bool RunMove(Json::Value &rJObject);//执行鼠标动作调用函数，但如果上一次的执行未完成则会返回失败
private:
    static CMouseMove *m_pInstanceObject;//静态对象
    CRITICAL_SECTION m_oCMouseMoveSection;//访问CMouseMoveRunning的临界区
    bool m_bCMouseMoveRunning;//动作执行标记
    //------------
    CMouseMove();
    CMouseMove(const CMouseMove&){;};
    static void _CMouseMoveSub(void *pMoveData);//鼠标动作运行线程函数
};

#endif