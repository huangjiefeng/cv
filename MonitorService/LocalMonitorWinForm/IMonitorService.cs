using System;
using System.ServiceModel;
using System.Collections.Generic;
using System.Xml.Linq;

namespace LocalMonitorWinForm
{
    // 注意: 使用“重构”菜单上的“重命名”命令，可以同时更改代码和配置文件中的接口名“IMonitorService”。
    [ServiceContract]
    public interface IMonitorService
    {
        [OperationContract]
        String GetTerminalInfo();

        [OperationContract]
        String GetHardwareInfo();

        // 获取进程信息（列表）
        [OperationContract]
        String GetProcessInfo(String filter);

        // 获取指定进程的线程信息（列表）
        [OperationContract]
        String[] GetThreadInfo(int pid);


        [OperationContract]
        bool KillProcess(int PID);

        [OperationContract]
        bool SetProcessPriority(int PID, String priority);

        [OperationContract]
        bool SetThreadPriority(int PID, int TID, String priority);

        [OperationContract(IsOneWay = true)]
        void ShutDown();

        [OperationContract(IsOneWay = true)]
        void Restart();

        [OperationContract(IsOneWay = true)]
        void BeepForAWhile(int ms);
    }
}
