using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LocalMonitorWinForm
{
    public class ProcessDynamicInfo
    {
        public UInt64 ByteSend { get; set; }
        public UInt64 ByteReceive { get; set; }
        public UInt64 OldByteSend { get; set; }
        public UInt64 OldByteReceive { get; set; }
        public TimeSpan TotalProcessorTime { get; set; }
        public TimeSpan OldTotalProcessorTime { get; set; }
        public DateTime LastCheckTime { get; set; }

        public UInt64 PrivateWorkingSet { get; set; }

        public Double CpuUsage { get; set; }
        public Int32 SendSpeed { get; set; }
        public Int32 ReceiveSpeed { get; set; }

        public ProcessDynamicInfo()
        {
            ByteSend = 0;
            ByteReceive = 0;
            OldByteSend = 0;
            OldByteReceive = 0;
            TotalProcessorTime = TimeSpan.FromSeconds(0);
            OldTotalProcessorTime = TimeSpan.FromSeconds(0);
            PrivateWorkingSet = 0;
        }

        public ProcessDynamicInfo Clone()
        {
            ProcessDynamicInfo value = new ProcessDynamicInfo();
            value.ByteSend = this.ByteSend;
            value.ByteReceive = this.ByteReceive;
            value.OldByteSend = this.OldByteSend;
            value.OldByteReceive = this.OldByteReceive;
            value.TotalProcessorTime = this.TotalProcessorTime;
            value.OldTotalProcessorTime = this.OldTotalProcessorTime;
            value.CpuUsage = this.CpuUsage;
            value.SendSpeed = this.SendSpeed;
            value.ReceiveSpeed = this.ReceiveSpeed;

            return value;
        }
    }
}
