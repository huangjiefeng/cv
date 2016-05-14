using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RemoteMonitor
{
    public enum TerminalStatus { Offline, Online, InConference, UnderControl }

    public class Terminal
    {
        public String Name { get; set; }
        public String Sip { get; set; }
        public String Version { get; set; }
        public String Model { get; set; }
        public Int32 ScreenCount { get; set; }
        public String System { get; set; }
        public String Location { get; set; }
        public String Ip { get; set; }
        public String Ipv4 { get; set; }
        public String Ipv6 { get; set; }
        public String Mac { get; set; }
        public Boolean Savi { get; set; }
        public TerminalStatus Status { get; set; }
    }
}
