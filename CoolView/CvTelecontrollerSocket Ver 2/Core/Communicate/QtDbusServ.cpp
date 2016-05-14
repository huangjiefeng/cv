/*
 * QtDbus通信控制类定义文件
 * V1.0.0.0	2012-12-15	DBus对象整合完成 By Lzy
 * V0.0.0.0	2011-12-14  创建文件 By Lzy
 */

#include "QtDbusServ.h"
#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/conferenceRoom/ConfRoomClient.h>
#include "NMessageProcess.h"

CQtDBusServ *CQtDBusServ::m_pQtDbusServ = NULL;

/*
 * 【公开】【静态】获取CQtDBusServ单例的静态函数
 * @return CQtDBusServ *,返回CQtDBusServ的单例对象
 */
CQtDBusServ *CQtDBusServ::GetInstance()
{
    if (CQtDBusServ::m_pQtDbusServ == NULL)
        CQtDBusServ::m_pQtDbusServ = new CQtDBusServ();
    return CQtDBusServ::m_pQtDbusServ;
}

/*
 * 【公开】【静态】释放CQtDBusServ单例的静态函数
 * @return void
 */
void CQtDBusServ::ReleaseInstance()
{
    if (CQtDBusServ::m_pQtDbusServ != NULL)
    {
        delete CQtDBusServ::m_pQtDbusServ;
        CQtDBusServ::m_pQtDbusServ = NULL;
    }
}

/*
 * 【私有】构造函数，初始化Dbus消息收发对象
 */
CQtDBusServ::CQtDBusServ()
{
	QDBusConnection oConnect = QDBusConnection::sessionBus();
    this->m_pCoreProxy = new CvCoreIf(CVCORE_SERVICE_NAME,CVCORE_SERVICE_OBJECT_PATH,oConnect);
    this->m_pConfRoomProxy = new ConferenceRoomIf(CONF_ROOM_SERVICE_NAME,CONF_ROOM_SERVICE_OBJECT_PATH,oConnect);
	this->m_pConfTeleProxy = new TelecontrollerIf(_GetTelecontrollerServiceName("confRoom"),_GetTelecontrollerObjectPath("confRoom"),oConnect);
	this->m_pTeleProxy = new TelecontrollerIf(_GetTelecontrollerServiceName("sock"),_GetTelecontrollerObjectPath("sock"),oConnect);
    this->m_pDbusAdaptor = new TelecontrollerAdaptor(this);
	oConnect.registerService(QString(TELECONTROLLER_SERVICE_NAME) + ".sock");
	oConnect.registerObject(QString(TELECONTROLLER_SERVICE_OBJECT_PATH) + "/sock" ,this);
}

/*
 * 【私有】析构函数，释放资源
 */
CQtDBusServ::~CQtDBusServ()
{
	delete this->m_pDbusAdaptor;
    delete this->m_pConfRoomProxy;
    delete this->m_pCoreProxy;
	delete this->m_pConfTeleProxy;
}

/*
 * 【公开】【DBus槽函数】接收到终端Dbus消息时触发的响应槽
 * @param int nInfoIndex,通告消息标识
 * @param int nNotifyId,通告消息接收对象（遥控器）的ID,负值表示通告所有遥控器
 * @param const QByteArray &rInputArray 通告消息数据
 * @return void
 */
void CQtDBusServ::TeleInfo(int nInfoIndex,int nNotifyId,const QByteArray &rInputArray)
{
	CNMessageProcess oNMessageProcess;
    oNMessageProcess.MainProcess(nInfoIndex,nNotifyId,rInputArray);
}
