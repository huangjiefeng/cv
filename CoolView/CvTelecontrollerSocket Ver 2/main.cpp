#include <QtCore/QCoreApplication>

#include "CommonMessageProcess.h"
#include "ContextInput.h"
#include "MouseMove.h"
#include "FileUD.h"
#include "NMessageProcess.h"

#include "floor_manager.h"

#include <process.h>

QCoreApplication *g_pApp;

static void __cdecl ProgrammeThreadSub(void *argument)
{
    //1、初始化对象
    int argc = 1;
    g_pApp = new QCoreApplication(argc,(char **)argument);
    CDataManager::GetInstance();
    CQtDBusServ::GetInstance();
    CNetServ::CreateInstance(5000,CNetServ::Type_Both,5001);
    CContextInput::CreateInstance(Version4,5002);
	CFileUpDownServ::CreateInstance(5003,10,100);
#ifdef USE_FCS
    FloorManager::GetInstance()->Initialize();
    FloorManager::GetInstance()->AddFloor(0);
#endif
    //2、启动qt事件循环
    g_pApp->exec();
    //3、退出前清理
#ifdef USE_FCS
    FloorManager::ReleaseInstance();
#endif // USE_FCS
	CFileUpDownServ::ReleaseInstance();
    CContextInput::ReleaseInstance();
    CNetServ::ReleaseInstance();
    Sleep(10);
    CQtDBusServ::ReleaseInstance();
    CDataManager::ReleaseInstance();
    CMouseMove::ReleaseInstance();
    delete g_pApp;
    //4、通知主线程退出
    g_bProgramme_Exit = false;
}

int main(int argc, char *argv[])
{
    g_bProgramme_Exit = false;
    //1、启动程序管理线程
    _beginthread(ProgrammeThreadSub,0,argv);
    //2、等待结束信号

    while (g_bProgramme_Exit == false)
        Sleep(10);
    //自行控制程序退出用的测试代码
    //int ts;
    //g_bProgramme_Exit = true;
    //scanf("%d",&ts);
    
    //3、退出qt事件循环
    g_pApp->exit();
    //4、等待主线程退出通知
    while (g_bProgramme_Exit != false)
        Sleep(5);
    return 0;
}

