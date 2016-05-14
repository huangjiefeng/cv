using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RemoteMonitor
{
    public class TerminalInfo
    {
        // CPU
        public String ProcessorName { get; set; }
        public String ProcessorId { get; set; }
        public Int32 ProcessorCount { get; set; }
        //public Double CpuTemperature { get; set; }              // Dynamic Info
        public String CpuTotalUsage { get; set; }               // Dynamic Info

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
        public String ByteReceived { get; set; }                  // Dynamic Info
        public String ByteSent { get; set; }                     // Dynamic Info

        // OS
        public String OsName { get; set; }
        public String OsVersion { get; set; }
        public String OsType { get; set; }
        public String ProductKey { get; set; }
        public String InstallDate { get; set; }
        public String LastBootUpTime { get; set; }
    }
}
