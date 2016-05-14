#ifndef MSDX_CLIENT_SERVICE_COMMON_H
#define MSDX_CLIENT_SERVICE_COMMON_H

#define MSDX_CLIENT_SERVICE_NAME 			"com.coolview.msdx.client"
#define MSDX_CLIENT_SERVICE_OBJECT_PATH 	"/com/coolview/msdx/client"
#include <QtCore/QtCore>
#include <QtCore/QDataStream>
//重定义一些数据结构
typedef unsigned __int64 uint64_t;
typedef unsigned int uint32_t;

#include <RtpStat.h>

enum MsdxClientServiceMethod
{
	DT_SEND_RTP_STAT_INFO,
	DT_RECV_RTP_STAT_INFO,
	DT_TRAN_STAT_INFO
};


//=================================返回值============================

#define		EVENT_R_JOB_DONE				1		//工作已完成
#define		EVENT_R_OK						0
#define		EVENT_R_NO_INITIAL				-1		//事件没有被创建
#define		EVENT_R_DATA_UNKNOW				-2		//共享内存里的数据无法解析
#define		EVENT_R_TIME_OUT				-3		//等待的事件超时
#define		EVENT_R_THREAD_ERROR			-4		//监听线程出现错误
#define		EVENT_R_THREAD_TERMINATE_FAIL	-5		//监听线程退出失败
#define		EVENT_R_JOB_FAIL				-6		//工作失败




#endif
