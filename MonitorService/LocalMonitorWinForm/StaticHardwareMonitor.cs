using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Management;
using System.IO;
using System.Net;
using System.Net.NetworkInformation;
using System.Xml.Linq;
using System.Net.Sockets;

namespace LocalMonitorWinForm
{
    class StaticHardwareMonitor : IHardwareMonitor
    {
        private bool GPUEnabled;
        private bool CPUEnabled;
        private bool RAMEnabled;
        private bool HDDEnabled;
        private bool NetworkEnabled;
        private bool OSEnable;
        private XElement xml;
        private bool needRefreshedXML;

        public TerminalInfo Info { get; private set; }

        public StaticHardwareMonitor()
        {
            Info = new TerminalInfo();
            xml = new XElement("TerminalInfo");
            needRefreshedXML = false;
        }

        public bool EnableGPU
        {
            get { return false; }
            set { /*empty*/ }
        }

        public bool EnableCPU
        {
            get { return CPUEnabled; }
            set { CPUEnabled = value; }
        }

        public bool EnableRAM
        {
            get { return RAMEnabled; }
            set { RAMEnabled = value; }
        }

        public bool EnableMainboard
        {
            get { return false; }
            set { /*empty*/ }
        }

        public bool EnableFanController
        {
            get { return false; }
            set { /*empty*/ }
        }

        public bool EnableHDD
        {
            get { return HDDEnabled; }
            set { HDDEnabled = value; }
        }

        public bool EnableNetwork
        {
            get { return NetworkEnabled; }
            set { NetworkEnabled = value; }
        }

        public bool EnableOS
        {
            get { return OSEnable; }
            set { OSEnable = value; }
        }

        public void Update()
        {
            needRefreshedXML = true;
            CheckOperationSystem();
            CheckDriver();
            CheckProcessor();
            CheckNetwork();
            CheckRAM();
        }

        public string GetReport()
        {
            if (needRefreshedXML) { updateXML(); }
            return xml.ToString();
        }

        public void updateXML()
        {
            xml.RemoveAll();

            if (OSEnable)
            {
                // OS Info.
                xml.Add(new XElement("OSName", Info.OsName));
                xml.Add(new XElement("OSVersion", Info.OsVersion));
                xml.Add(new XElement("OSType", Info.OsType));
                xml.Add(new XElement("ProductKey", Info.ProductKey));
                xml.Add(new XElement("InstallDate", Info.InstallDate.ToLongDateString()));
                xml.Add(new XElement("LastBootUpTime", Info.LastBootUpTime));
            }
            if (CPUEnabled)
            {
                // CPU Info.
                xml.Add(new XElement("ProcessorName", Info.ProcessorName));
                xml.Add(new XElement("ProcessorId", Info.ProcessorId));
                xml.Add(new XElement("ProcessorCount", Info.ProcessorCount));
            }
            if (RAMEnabled)
            {
                // MEM Info.
                xml.Add(new XElement("TotalMemory", Info.TotalMemory));
                xml.Add(new XElement("VisibleMemory", Info.VisibleMemory));
                xml.Add(new XElement("MemoryCount", Info.MemoryCount));
            }
            if (HDDEnabled)
            {
                // Disk Info.
                xml.Add(new XElement("DriveCount", Info.DriveCount));
                xml.Add(new XElement("DiskSize", Info.DiskSize));
            }
            if (NetworkEnabled)
            {
                // Network Info.
                xml.Add(new XElement("IPv4", Info.Ipv4));
                xml.Add(new XElement("IPv6", Info.Ipv6));
                xml.Add(new XElement("MAC", Info.Mac));
            }
        }

        public void Dispose()
        {
            // empty
        }

        private void CheckOperationSystem()
        {
            ManagementClass osClass = new ManagementClass("Win32_OperatingSystem");
            foreach (ManagementObject obj in osClass.GetInstances())
            {
                Info.OsName = obj["Caption"].ToString();                                                        // OS名称
                Info.OsVersion = obj["Version"].ToString();                                                     // OS版本
                Info.OsType = obj["OSArchitecture"].ToString();                                                 // OS架构
                Info.LastBootUpTime = StringToDateTime(obj["LastBootUpTime"].ToString());                       // OS最后启动时间
                Info.InstallDate = StringToDateTime(obj["InstallDate"].ToString());                             // OS安装时间
                Info.ProductKey = obj["SerialNumber"].ToString();                                               // OS序列号

                Info.VisibleMemory = MemoryToString(Convert.ToInt64(obj["TotalVisibleMemorySize"]), true);      // 可见内存
            }
        }

        private void CheckRAM()
        {
            Int64 memorySize = 0;
            Int32 memeryCount = 0;
            ManagementClass memoryClass = new ManagementClass("Win32_PhysicalMemory");
            foreach (ManagementObject obj in memoryClass.GetInstances())
            {
                memorySize += Convert.ToInt64(obj["Capacity"]);
                memeryCount++;
            }
            Info.TotalMemory = MemoryToString(Convert.ToInt64(memorySize), false);                              // 总内存
            Info.MemoryCount = memeryCount;                                                                     // 内存条数量
        }

        private void CheckProcessor()
        {
            ManagementClass processClass = new ManagementClass("Win32_Processor");
            foreach (ManagementObject obj in processClass.GetInstances())
            {
                Info.ProcessorName = obj["Name"].ToString();                                                    // CPU名称
                Info.ProcessorId = obj["ProcessorId"].ToString();                                               // CPU ID
                Info.ProcessorCount = Int32.Parse(obj["NumberOfLogicalProcessors"].ToString());                 // CPU数量
            }
        }

        private void CheckDriver()
        {
            DriveInfo[] driveInfos = DriveInfo.GetDrives();
            Int64 diskSize = 0;
            Int32 driverCount = 0;
            Info.DriveCount = driveInfos.Length;
            foreach (DriveInfo drive in driveInfos)
            {
                if (drive.DriveType == DriveType.Fixed)
                {
                    diskSize += drive.TotalSize;
                    driverCount++;
                }
            }
            Info.DiskSize = MemoryToString(diskSize, false);                                                    // 磁盘容量
            Info.DriveCount = driverCount;     
        }

        private void CheckNetwork()
        {
            IPAddress[] ips = Dns.GetHostEntry(Dns.GetHostName()).AddressList;
            Info.Ipv4 = "";
            Info.Ipv6 = "";
            foreach (IPAddress ip in ips)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    Info.Ipv4 = ip.ToString();                                                                  // IPv4
                }
                else if (!ip.IsIPv6LinkLocal && !ip.IsIPv6Multicast && !ip.IsIPv6SiteLocal/* && !ip.IsIPv6Teredo*/)
                {
                    Info.Ipv6 = ip.ToString();                                                                  // IPv6
                }
            }

            NetworkInterface[] nis = NetworkInterface.GetAllNetworkInterfaces();
            Info.Mac = "";
            foreach (NetworkInterface ni in nis)
            {
                if (ni.NetworkInterfaceType == NetworkInterfaceType.Ethernet)
                {
                    String temp = ni.GetPhysicalAddress().ToString();
                    String mac = temp.Substring(0, 2);
                    for (int i = 0; i < 5; i++)
                    {
                        mac += "-" + temp.Substring(i * 2 + 2, 2);
                    }
                    Info.Mac = mac;                                                                             // MAC
                }
            }
        }

        private static DateTime StringToDateTime(String datetimeStr)
        {
            String dt = datetimeStr.Split('.')[0];
            int year = Convert.ToInt32(dt.Substring(0, 4));
            int month = Convert.ToInt32(dt.Substring(4, 2));
            int day = Convert.ToInt32(dt.Substring(6, 2));
            int hour = Convert.ToInt32(dt.Substring(8, 2));
            int minute = Convert.ToInt32(dt.Substring(10, 2));
            int second = Convert.ToInt32(dt.Substring(12, 2));
            return new DateTime(year, month, day, hour, minute, second);
        }

        private static String MemoryToString(Int64 size, bool isKB)
        {
            double val = 1.0 * size / 1024 / 1024;
            if (!isKB)
                val /= 1024;
            return val.ToString(".##") + "GB";
        }

    }
}
