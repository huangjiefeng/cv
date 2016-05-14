using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LocalMonitorWinForm
{
    public interface IHardwareMonitor : IDisposable
    {
        bool EnableGPU { set; get; }
        bool EnableCPU { set; get; }
        bool EnableRAM { set; get; }
        bool EnableMainboard { set; get; }
        bool EnableFanController { set; get; }
        bool EnableHDD { set; get; }
        bool EnableNetwork { set; get; }
        
        void Update();
        string GetReport();
    }
}
