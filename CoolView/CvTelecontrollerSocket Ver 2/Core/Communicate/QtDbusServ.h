/*
 * QtDbus通信控制类声明文件
 * V1.0.0.0	2012-12-15	DBus对象整合完成 By Lzy
 * V0.0.0.0	2011-12-14  创建文件 By Lzy
 */

#ifndef QT_DBUS_SERV_H
#define QT_DBUS_SERV_H

#include <QObject>
#include <dbus/telecontroller/service/TelecontrollerAdaptor.h>
#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
#include <dbus/core/client/CvCoreIf.h>
#include <dbus/conferenceRoom/client/ConferenceRoomIf.h>
#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

class CQtDBusServ : public QObject
{
	Q_OBJECT
public:
	//------函数------
    static CQtDBusServ *GetInstance();//获取CQtDBusServ单例的静态函数
    static void ReleaseInstance();//释放CQtDBusServ单例的静态函数
	//------变量------
	CvCoreIf *m_pCoreProxy;//coolview程序服务接口
	ConferenceRoomIf *m_pConfRoomProxy;//会议室程序服务接口
	TelecontrollerIf *m_pConfTeleProxy;//会议室程序的telecontroller专用接口
    TelecontrollerIf *m_pTeleProxy;//指向自己的dbus接口

private:
    //------函数------
    CQtDBusServ();
    ~CQtDBusServ();
    CQtDBusServ(const CQtDBusServ&){};
    //------变量------
    static CQtDBusServ *m_pQtDbusServ;//CQtDBusServ的静态对象
    TelecontrollerAdaptor *m_pDbusAdaptor;

public Q_SLOTS: // METHODS

	/*
     * 【公开】接收到终端Dbus消息时触发的响应槽
     * @param int nInfoIndex,通告消息标识
     * @param int nNotifyId,通告消息接收对象（遥控器）的ID,负值表示通告所有遥控器
     * @param const QByteArray &rInputArray 通告消息数据
     * @return void
     */
	void TeleInfo(int nInfoIndex,int nNotifyId,const QByteArray &rInputArray);
};

#endif