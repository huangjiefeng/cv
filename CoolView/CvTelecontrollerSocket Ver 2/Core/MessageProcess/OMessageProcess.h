/*
 * 通告订阅请求与连接关闭时的处理封装 声明文件
 * V0.0.0.0 2011-12-08  By Lzy
 */

#ifndef O_MESSAGE_PROCESS_H
#define O_MESSAGE_PROCESS_H

namespace Notify_Process_Space
{
    void NewNotifyConnectionCreated(int nConnectionIndex);//当有新的通告订阅连接被建立时被调用的初始化函数
    void NotifyConnectionClosed(int nConnectionIndex);//当通告订阅连接被检测出已断开时被调用的清理函数
};

#endif