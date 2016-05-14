using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RemoteMonitor
{
    class Process
    {
        public Int32 Pid { get; set; }
        public String Name { get; set; }
        public Int32 ThreadCount { get; set; }
        public String Priority { get; set; }
        public Double Cpu { get; set; }
        public Int32 Memory { get; set; }
        public Int32 SendSpeed { get; set; }
        public Int32 ReceiveSpeed { get; set; }
    }
}
