using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LocalMonitorWinForm
{
    class TerminalInfo
    {
        // CPU
        public String ProcessorName { get; set; }
        public String ProcessorId { get; set; }
        public Int32 ProcessorCount { get; set; }
        //public Double CpuTemperature { get; set; }              // Dynamic Info
        public Double CpuTotalUsage { get; set; }               // Dynamic Info

        // Memeory
        public String TotalMemory { get; set; }
        public String VisibleMemory { get; set; }
        public Int32 MemoryCount { get; set; }
        public String AvailableMemory { get; set; }                  // Dynamic Info

        // Disk
        public Int32 DriveCount { get; set; }
        public String DiskSize { get; set; }
        public String AvailableSize { get; set; }

        // Network
        public String Ipv4 { get; set; }
        public String Ipv6 { get; set; }
        public String Mac { get; set; }
        public UInt64 ByteReceived { get; set; }                  // Dynamic Info
        public UInt64 ByteSent { get; set; }                     // Dynamic Info

        // OS
        public String OsName { get; set; }
        public String OsVersion { get; set; }
        public String OsType { get; set; }
        public String ProductKey { get; set; }
        public DateTime InstallDate { get; set; }
        public DateTime LastBootUpTime { get; set; }

    }
}
