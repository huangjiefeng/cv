#include <QtCore/QCoreApplication>
#include "CvChannelDispatcherService.h"

#include <Windows.h>
#include "util/ProcessManager.h"
#include "log/Log.h"

int main(int argc, char *argv[])
{
    //现在的ChannelDispatcher由DBus以Service方式自动开启,不需要检测
    //int procCount = 0;
    //if( ProcessManager::isRunning(TEXT("CvChannelDispatcher.exe") , &procCount) )
    //{
    //    //防止出现两个程序
    //    if( procCount>1 )
    //        return 0;
    //}

    //SetConsoleTitleA( "CvChannelDispatcher.exe" );

    //更改log文件名
    SetLogComponent("CvChannel" );

    QCoreApplication a(argc, argv);

    CvChannelDispatcherService service;
    if (0 != service.Initial())
    {
        return 0;
    }

    //::SetPriorityClass( ::GetCurrentProcess() , HIGH_PRIORITY_CLASS );

    return a.exec();
}
