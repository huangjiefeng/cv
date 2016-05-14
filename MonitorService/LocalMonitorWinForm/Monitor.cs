using System;
using System.Collections.Generic;

using System.Diagnostics;
using System.ServiceModel;
using System.Net.NetworkInformation;
using System.Net;
using System.Net.Sockets;
using System.Management;
using System.Globalization;
using System.Xml.Linq;
using System.IO;
using System.Linq;
using System.Threading;
using System.Text.RegularExpressions;
using SharpPcap;
using PacketDotNet;
using OpenHardwareMonitor.Utilities;
using OpenHardwareMonitor.GUI;
using OpenHardwareMonitor.Hardware;

namespace LocalMonitorWinForm
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
    class Monitor : IMonitorService, IDisposable
    {
        public TerminalInfo TerminalInfo { get; private set; }
        
        private Dictionary<String, Int32> endPoint2PidMap;
        private Dictionary<Int32, ProcessDynamicInfo> pid2InfoMap;

        private ICaptureDevice capDev;
        private Queue<Packet> packetQueue = new Queue<Packet>();

        private int packetMiss = 0;
        private ulong byteMiss = 0;

        private Timer netstatTimer;
        private Timer pid2NetworkMapTimer;

        private PerformanceCounter pcCpuUsage = new PerformanceCounter("Processor", "% Processor Time", "_Total");
        private PerformanceCounter pcAvailableMem = new PerformanceCounter("Memory", "Available MBytes");

        // hardware dynamic info monitor added by CBJ
        private DynamicHardwareMonitor hardwareMonitor;
        private PersistentSettings settings;
        private const string CONFIG_FILE_NAME = "cfg/HardwareMonitor.config";
        
        private Timer refreshTimer;
        private double refreshInterval = 1.0;

        private Timer stopBeepingTimer;


#region Alerm Trigger 

        private delegate void TriggerAction();

        private class Trigger
        {
            public float threshold;
            public TriggerAction action;
        }

        private List<Trigger> cpuTriggers = new List<Trigger>();
        private List<Trigger> hddTriggers = new List<Trigger>();
        private List<Trigger> mainboardTriggers = new List<Trigger>();

        private const float CPU_DEFAULT_THRESHOLD = 85.0f;
        private const float HDD_DEFAULT_THRESHOLD = 85.0f;
        private const float MAINBOARD_DEFAULT_THRESHOLD = 85.0f;

        private bool beeping;
        private bool can_beep;

        private void InitializeMonitorTriggers()
        {
            StopBeeping();
            beeping = false;
            can_beep = true;

            // 由CoolView主进程进行操作
            // 因此屏蔽一下触发器
            //InitializeCPUTriggers();
            //InitializeHDDTriggers();
            //InitializeMainboardTriggers();
        }

        private void InitializeCPUTriggers()
        {
            string cpu_threshold_format = "cpu_temperature_threshold_{0:D}";
            string cpu_action_format = "cpu_temperature_action_{0:D}";
            string cpu_triggers_count = "cpu_triggers_count";
            int triggers_count = settings.GetValue(cpu_triggers_count, 0);
            for (int i = 0; i < triggers_count; ++i)
            {
                Trigger trigger = new Trigger();
                trigger.threshold = settings.GetValue(string.Format(cpu_threshold_format, i), CPU_DEFAULT_THRESHOLD);
                string action = settings.GetValue(string.Format(cpu_action_format, i), "undefined");
                trigger.action = GetTriggerActionFromString(action);
                if (trigger.action != null)
                {
                    cpuTriggers.Add(trigger);
                }
            }
        }

        private TriggerAction GetTriggerActionFromString(string action)
        {
            switch (action)
            {
              case "shutdown":
                return ShutDown;
              case "restart":
                return Restart;
              case "beep":
                return Beep;
              default:
                return null;
            }
        }

        private void InitializeHDDTriggers()
        {
            string hdd_threshold_format = "hdd_temperature_threshold_{0:D}";
            string hdd_action_format = "hdd_temperature_action_{0:D}";
            string hdd_triggers_count = "hdd_triggers_count";
            int triggers_count = settings.GetValue(hdd_triggers_count, 0);
            for (int i = 0; i < triggers_count; ++i)
            {
                Trigger trigger = new Trigger();
                trigger.threshold = settings.GetValue(string.Format(hdd_threshold_format, i), HDD_DEFAULT_THRESHOLD);
                string action = settings.GetValue(string.Format(hdd_action_format, i), "undefined");
                trigger.action = GetTriggerActionFromString(action);
                if (trigger.action != null)
                {
                    hddTriggers.Add(trigger);
                }
            }
        }

        private void InitializeMainboardTriggers() 
        {
            string mainboard_threshold_format = "mainboard_temperature_threshold_{0:D}";
            string mainboard_action_format = "mainboard_temperature_action_{0:D}";
            string mainboard_triggers_count = "mainboard_triggers_count";
            int triggers_count = settings.GetValue(mainboard_triggers_count, 0);
            for (int i = 0; i < triggers_count; ++i)
            {
                Trigger trigger = new Trigger();
                trigger.threshold = settings.GetValue(string.Format(mainboard_threshold_format, i), MAINBOARD_DEFAULT_THRESHOLD);
                string action = settings.GetValue(string.Format(mainboard_action_format, i), "undefined");
                trigger.action = GetTriggerActionFromString(action);
                if (trigger.action != null)
                {
                    mainboardTriggers.Add(trigger);
                }
            }
        }

        // OK
        public void ShutDown()
        {
            if (beeping)
                StopBeeping();
            Shell.Execute("shutdown", "/s /t 0");
        }

        // OK
        public void Restart()
        {
            if (beeping)
                StopBeeping();
            Shell.Execute("shutdown", "/r /t 0");
        }

        public void BeepForAWhile(int ms)
        {
            beeping = true;
            uint freq = 1193180000 / 440000; // 440Hz

            Ring0.WriteIoPort(0x43, 0xB6);
            Ring0.WriteIoPort(0x42, (byte)(freq & 0xFF));
            Ring0.WriteIoPort(0x42, (byte)(freq >> 9));

            System.Threading.Thread.Sleep(100);

            Ring0.WriteIoPort(0x61, (byte)(Ring0.ReadIoPort(0x61) | 0x03));

            stopBeepingTimer = new Timer(
                new TimerCallback(StopBeepingForTimer),
                null, 
                ms, 
                System.Threading.Timeout.Infinite);
        }

        private void Beep()
        {
            if (can_beep && !beeping)
            {
                can_beep = false;
                beeping = true;
                uint freq = 1193180000 / 440000; // 440Hz

                Ring0.WriteIoPort(0x43, 0xB6);
                Ring0.WriteIoPort(0x42, (byte)(freq & 0xFF));
                Ring0.WriteIoPort(0x42, (byte)(freq >> 9));

                System.Threading.Thread.Sleep(100);

                Ring0.WriteIoPort(0x61, (byte)(Ring0.ReadIoPort(0x61) | 0x03));
            }
        }

        private void StopBeepingForTimer(object state)
        {
            StopBeeping();
        }

        private void StopBeeping()
        {
            beeping = false;
            Ring0.WriteIoPort(0x61, (byte)(Ring0.ReadIoPort(0x61) & 0xFC));
        }

        private void CheckTriggers()
        {
            if (!can_beep && beeping)
            {
                StopBeeping();
            }
            CheckTemperatureOfCPU();
            CheckTemperatureOfHDD();
            CheckTemperatureOfMainboard();
        }

        private void CheckTemperatureOfCPU()
        {
            float? temperature = hardwareMonitor.GetMaxTemperatureOfCPU();
            if (temperature.HasValue)
            {
                AppendLog(String.Format("Current CPU Temperature: {0:F1} °C", temperature.Value));
                foreach (Trigger trigger in cpuTriggers)
                {
                    if (temperature.Value > trigger.threshold)
                    {
                        AppendLog("Activate Trigger!");
                        trigger.action();
                    }
                }
            }
        }

        private void CheckTemperatureOfMainboard()
        {
            float? temperature = hardwareMonitor.GetMinTemperatureOfMainboard();
            if (temperature.HasValue)
            {
                AppendLog(String.Format("Current Mainboard Temperature: {0:F1} °C", temperature.Value));
                foreach (Trigger trigger in mainboardTriggers)
                {
                    if (temperature.Value > trigger.threshold)
                    {
                        AppendLog("Activate Trigger!");
                        trigger.action();
                    }
                }
            }
        }

        private void CheckTemperatureOfHDD()
        {
            float? temperature = hardwareMonitor.GetMaxTemperatureOfHDD();
            if (temperature.HasValue)
            {
                AppendLog(String.Format("Current HardDisk Temperature: {0:F1} °C", temperature.Value));
                foreach (Trigger trigger in hddTriggers)
                {
                    if (temperature.Value > trigger.threshold)
                    {
                        AppendLog("Activate Trigger!");
                        trigger.action();
                    }
                }
            }
        }



#endregion
        
        private Thread workingThread;

        public Monitor()
        {
            AppendLog("--------------------Start Monitor-----------------");
            InitializeTerminalInfo();

            endPoint2PidMap = new Dictionary<String, Int32>();
            GetNetstat(null);

            pid2InfoMap = new Dictionary<Int32, ProcessDynamicInfo>();

            capDev = CaptureDeviceList.Instance[0];
            capDev.OnPacketArrival += new PacketArrivalEventHandler(capDev_OnPacketArrival);
            
            settings = new PersistentSettings();
            settings.Load(CONFIG_FILE_NAME);
            hardwareMonitor = new DynamicHardwareMonitor(settings, new UpdateVisitor());

            InitializeTimers();
            InitializeMonitorTriggers();
            StopBeeping();
        }

        public void Dispose()
        {
            if (settings != null)
            {
                SaveConfiguration(settings, CONFIG_FILE_NAME);
            }
        }

        private void capDev_OnPacketArrival(object sender, CaptureEventArgs e)
        {
            lock (packetQueue)
            {
                try
                {
                    packetQueue.Enqueue(IpPacket.ParsePacket(e.Packet.LinkLayerType, e.Packet.Data));
                    if (packetQueue.Count == 1)
                        System.Threading.Monitor.Pulse(packetQueue);
                }
                catch (Exception ex)
                {
                    AppendLog("Error: capDev_OnPacketArrival" + ex.Message);
                }
            }
        }

        private void AppendLog(string message)
        {
            System.Console.WriteLine(message);
        }

        public void StartCapture()
        {
            AppendLog("------------------Start Capture----------------");
            workingThread = new Thread(new ThreadStart(ProcessPacketArrival));
            workingThread.Start();

            capDev.Open();
            capDev.Filter = "tcp or udp";
            capDev.StartCapture();
        }

        public void StopCapture()
        {
            AppendLog("------------------Stop Capture----------------");
            capDev.StopCapture();
            capDev.Close();

            netstatTimer.Change(Timeout.Infinite, Timeout.Infinite);
            pid2NetworkMapTimer.Change(Timeout.Infinite, Timeout.Infinite);
            refreshTimer.Change(Timeout.Infinite, Timeout.Infinite);

            workingThread.Abort();
        }

        private void InitializeTerminalInfo()
        {
            AppendLog("InitializeTerminalInfo");

            TerminalInfo = new TerminalInfo();

            ManagementClass osClass = new ManagementClass("Win32_OperatingSystem");
            foreach (ManagementObject obj in osClass.GetInstances())
            {
                TerminalInfo.OsName = obj["Caption"].ToString();                                                        // OS名称
                TerminalInfo.OsVersion = obj["Version"].ToString();                                                     // OS版本
                TerminalInfo.OsType = obj["OSArchitecture"].ToString();                                                 // OS架构
                TerminalInfo.LastBootUpTime = StringToDateTime(obj["LastBootUpTime"].ToString());                       // OS最后启动时间
                TerminalInfo.InstallDate = StringToDateTime(obj["InstallDate"].ToString());                             // OS安装时间
                TerminalInfo.ProductKey = obj["SerialNumber"].ToString();                                               // OS序列号
                TerminalInfo.VisibleMemory = MemoryToString(Convert.ToInt64(obj["TotalVisibleMemorySize"]), true);      // 可见内存
            }

            Int64 memorySize = 0;
            Int32 memeryCount = 0;
            ManagementClass memoryClass = new ManagementClass("Win32_PhysicalMemory");
            foreach (ManagementObject obj in memoryClass.GetInstances())
            {
                memorySize += Convert.ToInt64(obj["Capacity"]);
                memeryCount++;
            }
            TerminalInfo.TotalMemory = MemoryToString(Convert.ToInt64(memorySize), false);                              // 总内存
            TerminalInfo.MemoryCount = memeryCount;                                                                     // 内存条数量

            ManagementClass processClass = new ManagementClass("Win32_Processor");
            foreach (ManagementObject obj in processClass.GetInstances())
            {
                TerminalInfo.ProcessorName = obj["Name"].ToString();                                                    // CPU名称
                TerminalInfo.ProcessorId = obj["ProcessorId"].ToString();                                               // CPU ID
                TerminalInfo.ProcessorCount = Int32.Parse(obj["NumberOfLogicalProcessors"].ToString());                 // CPU数量
            }

            DriveInfo[] driveInfos = DriveInfo.GetDrives();
            Int64 diskSize = 0;
            Int32 driverCount = 0;
            TerminalInfo.DriveCount = driveInfos.Length;                                                                
            foreach (DriveInfo drive in driveInfos)
            {
                if (drive.DriveType == DriveType.Fixed)
                {
                    diskSize += drive.TotalSize;
                    driverCount++;
                }
            }
            TerminalInfo.DiskSize = MemoryToString(diskSize, false);                                                    // 磁盘容量
            TerminalInfo.DriveCount = driverCount;                                                                      // 分驱数量

            IPAddress[] ips = Dns.GetHostEntry(Dns.GetHostName()).AddressList;
            TerminalInfo.Ipv4 = "";
            TerminalInfo.Ipv6 = "";
            foreach (IPAddress ip in ips)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    TerminalInfo.Ipv4 = ip.ToString();                                                                  // IPv4
                }
                else if (!ip.IsIPv6LinkLocal && !ip.IsIPv6Multicast && !ip.IsIPv6SiteLocal/* && !ip.IsIPv6Teredo*/)
                {
                    TerminalInfo.Ipv6 = ip.ToString();                                                                  // IPv6
                }
            }

            NetworkInterface[] nis = NetworkInterface.GetAllNetworkInterfaces();
            TerminalInfo.Mac = "";
            foreach (NetworkInterface ni in nis)
            {
                if (ni.NetworkInterfaceType == NetworkInterfaceType.Ethernet)
                {
                    String temp = ni.GetPhysicalAddress().ToString();
                    String mac = temp.Substring(0, 2);
                    for(int i=0 ;i<5;i++)
                    {
                        mac += "-" + temp.Substring(i * 2 + 2, 2);
                    }
                    TerminalInfo.Mac = mac;                                                                             // MAC
                }
            }
        }

        private void InitializeTimers()
        {
            AppendLog("InitializeTimers");
            netstatTimer = new Timer(new TimerCallback(GetNetstat), null, 10 * 1000, 10 * 1000);
            pid2NetworkMapTimer = new Timer(new TimerCallback(ClearPid2NetworkMap), null, 60 * 60 * 1000, 60 * 60 * 1000);
            refreshTimer = new Timer(new TimerCallback(RefreshInfo), null, 0, Convert.ToInt32(1000 * refreshInterval));
        }

        private void RefreshInfo(Object state)
        {
            RefreshHardwareInfo(state);
            RefreshProcessInfo(state);
        }

        private void RefreshHardwareInfo(Object state)
        {
            AppendLog("RefreshHardwareInfo");
            hardwareMonitor.Update();

            CheckTriggers();
        }

        public String GetNetwork(Int32 pid)
        {
            AppendLog("GetNetwork");
            String value = "";
            List<int> keys = pid2InfoMap.Keys.ToList();
            foreach (int key in keys)
            {
                ProcessDynamicInfo ns = pid2InfoMap[key];
                value += key + "\tsend:" + ns.ByteSend + ", receive:" + ns.ByteReceive +"\r\n";
            }
            value += "miss packet:" + packetMiss + ", miss byte:" + byteMiss;

            return value;
        }

        private void ProcessPacketArrival()
        {
            while (true)
            {
                Packet packet;

                lock (packetQueue)
                {
                    while (packetQueue.Count <= 0)
                        System.Threading.Monitor.Wait(packetQueue);
                    packet = packetQueue.Dequeue().PayloadPacket;
                }

                try
                {
                    String src = "";
                    String dst = "";
                    ulong len = 0;

                    IpPacket ipPacket = packet as IPv4Packet;

                    if (ipPacket == null)
                        ipPacket = packet as IPv6Packet;

                    len = (ulong)(ipPacket.TotalLength);
                    src = ipPacket.SourceAddress.ToString();
                    dst = ipPacket.DestinationAddress.ToString();

                    if (src.Equals("127.0.0.1") || src.Equals("0.0.0.0"))
                        src = TerminalInfo.Ipv4;
                    else if (src.Equals("[::1]"))
                        src = TerminalInfo.Ipv6;

                    if (dst.Equals("127.0.0.1") || dst.Equals("0.0.0.0"))
                        dst = TerminalInfo.Ipv4;
                    else if (dst.Equals("[::1]"))
                        dst = TerminalInfo.Ipv6;

                    Packet payload = packet.PayloadPacket;
                    TcpPacket tcp = payload as TcpPacket;
                    UdpPacket udp = payload as UdpPacket;

                    if (tcp != null)
                    {
                        src += ":" + tcp.SourcePort;
                        dst += ":" + tcp.DestinationPort;
                    }
                    else if (udp != null)
                    {
                        src += ":" + udp.SourcePort;
                        dst += ":" + udp.DestinationPort;
                    }

                    lock (endPoint2PidMap)
                    {
                        bool find = false;
                        if (endPoint2PidMap.ContainsKey(src))
                        {
                            find = true;
                            Int32 pid = endPoint2PidMap[src];
                            if (pid2InfoMap.ContainsKey(pid))
                            {
                                pid2InfoMap[pid].ByteSend += len;
                            }
                            else
                            {
                                ProcessDynamicInfo ns = new ProcessDynamicInfo();
                                ns.ByteSend = len;
                                pid2InfoMap.Add(pid, ns);
                            }
                            TerminalInfo.ByteSent += len;
                        }

                        if (endPoint2PidMap.ContainsKey(dst))
                        {
                            find = true;
                            Int32 pid = endPoint2PidMap[dst];
                            if (pid2InfoMap.ContainsKey(pid))
                            {
                                pid2InfoMap[pid].ByteReceive += len;
                            }
                            else
                            {
                                ProcessDynamicInfo ns = new ProcessDynamicInfo();
                                ns.ByteReceive = len;
                                pid2InfoMap.Add(pid, ns);
                            }
                            TerminalInfo.ByteReceived += len;
                        }
                    
                        if (!find)
                        {
                            packetMiss++;
                            byteMiss += len;
                        }
                    }
                }
                catch (Exception ex)
                {
                    AppendLog("Error: ProcessPacketArrival" + ex.Message);
                }
            }
        }

        public String GetTerminalInfo()
        {
            AppendLog("GetTerminalInfo");
            TerminalInfo.AvailableMemory = pcAvailableMem.NextValue().ToString("####MB");
            TerminalInfo.CpuTotalUsage = pcCpuUsage.NextValue();

            DriveInfo[] driveInfos = DriveInfo.GetDrives();
            Int64 freeSize = 0;
            TerminalInfo.DriveCount = driveInfos.Length;
            foreach (DriveInfo drive in driveInfos)
            {
                if (drive.DriveType == DriveType.Fixed)
                {
                    freeSize += drive.AvailableFreeSpace;
                }
            }
            TerminalInfo.AvailableSize = MemoryToString(freeSize, false);

            XElement value = new XElement("terminalInfo");

            // OS Info.
            value.Add(new XElement("osName", TerminalInfo.OsName));
            value.Add(new XElement("osVersion", TerminalInfo.OsVersion));
            value.Add(new XElement("osType", TerminalInfo.OsType));
            value.Add(new XElement("productKey", TerminalInfo.ProductKey));
            value.Add(new XElement("installDate", TerminalInfo.InstallDate.ToLongDateString()));
            value.Add(new XElement("lastBootUpTime", TerminalInfo.LastBootUpTime));

            // CPU Info.
            value.Add(new XElement("processorName", TerminalInfo.ProcessorName));
            value.Add(new XElement("processorId", TerminalInfo.ProcessorId));
            value.Add(new XElement("processorCount", TerminalInfo.ProcessorCount));
            value.Add(new XElement("cpuTotalUsage", TerminalInfo.CpuTotalUsage.ToString("0.00") + "%"));

            // MEM Info.
            value.Add(new XElement("totalMemory", TerminalInfo.TotalMemory));
            value.Add(new XElement("visibleMemory", TerminalInfo.VisibleMemory));
            value.Add(new XElement("memoryCount", TerminalInfo.MemoryCount));
            value.Add(new XElement("availableMemory", TerminalInfo.AvailableMemory));

            // Disk Info.
            value.Add(new XElement("driveCount", TerminalInfo.DriveCount));
            value.Add(new XElement("diskSize", TerminalInfo.DiskSize));
            value.Add(new XElement("availableSize", TerminalInfo.AvailableSize));

            // Network Info.
            value.Add(new XElement("ipv4", TerminalInfo.Ipv4));
            value.Add(new XElement("ipv6", TerminalInfo.Ipv6));
            value.Add(new XElement("mac", TerminalInfo.Mac));
            value.Add(new XElement("byteReceived", TerminalInfo.ByteReceived.ToString("#,###B")));
            value.Add(new XElement("byteSent", TerminalInfo.ByteSent.ToString("#,###B")));

            return value.ToString();
        }

        public String GetHardwareInfo()
        {
            AppendLog("GetHardwareInfo");
            return hardwareMonitor.GetReport();
        }

        public String GetProcessInfo(String filter)
        {
            AppendLog("GetProcessInfo");
            String[] filters = filter.Split(new Char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            XElement root = new XElement("processInfo");
            Process[] processes = Process.GetProcesses();
            foreach (Process p in processes)
            {
                if (filters.Length==0 || filters.Contains(p.ProcessName) && pid2InfoMap.ContainsKey(p.Id))
                {
                    try
                    {
                        ProcessDynamicInfo info = pid2InfoMap[p.Id].Clone();
                        XElement infoXml = new XElement("info");
                        infoXml.Add(
                            new XElement("pid", p.Id),
                            new XElement("name", p.ProcessName),
                            new XElement("priority", p.PriorityClass.ToString()),
                            new XElement("cpu", (info.CpuUsage*100.0).ToString("0.00")),
                            new XElement("mem", (p.WorkingSet64 / 1024).ToString()),
                            new XElement("threadCount", p.Threads.Count),
                            new XElement("send", info.SendSpeed.ToString()),
                            new XElement("receive", info.ReceiveSpeed.ToString())
                            );
                        root.Add(infoXml);
                    }
                    catch (Exception ex)
                    {
                        // 这里会捕获到拒绝访问异常，可能对应的进程正在执行，进程的某些属性拒绝访问。
                        //AppendLog("Error: GetProcessInfo" + ex.Message);
                    }
                }
            }

            return root.ToString();
        }

        public String[] GetThreadInfo(int pid)
        {
            AppendLog("GetThreadInfo");
            Process p = Process.GetProcessById(pid);
            if (p != null)
            {
                ProcessThreadCollection threads = p.Threads;
                String[] infoString = new String[threads.Count];

                for (int i = 0; i < threads.Count; ++i)
                {
                    ProcessThread thread = threads[i];
                    String tid = thread.Id.ToString();
                    String priority = thread.PriorityLevel.ToString();
                    String state = thread.ThreadState.ToString();
                    String cpuTime = thread.TotalProcessorTime.TotalSeconds.ToString();

                    infoString[i] = tid + "," + priority + "," + state + "," + cpuTime;
                }
                return infoString;
            }
            return null;
        }

        // OK
        public bool KillProcess(int PID)
        {
            Process process = Process.GetProcessById(PID);
            if (process != null)
            {
                try
                {
                    process.Kill();
                }
                catch
                {
                    return false;
                }
            }
            return true;
        }

        public bool SetProcessPriority(int PID, String priority)
        {
            Process process = Process.GetProcessById(PID);
            if (process != null)
            {
                try
                {
                    if (priority.Equals("AboveNormal"))
                    {
                        process.PriorityClass = ProcessPriorityClass.AboveNormal;
                    }
                    else if (priority.Equals("BelowNormal"))
                    {
                        process.PriorityClass = ProcessPriorityClass.BelowNormal;
                    }
                    else if (priority.Equals("High"))
                    {
                        process.PriorityClass = ProcessPriorityClass.High;
                    }
                    else if (priority.Equals("Idle"))
                    {
                        process.PriorityClass = ProcessPriorityClass.Idle;
                    }
                    else if (priority.Equals("Normal"))
                    {
                        process.PriorityClass = ProcessPriorityClass.Normal;
                    }
                    else if (priority.Equals("RealTime"))
                    {
                        process.PriorityClass = ProcessPriorityClass.RealTime;
                    }
                    else
                    {
                        return false;
                    }
                }
                catch
                {
                    return false;
                }
            }
            return true;
        }

        public bool SetThreadPriority(int PID, int TID, String priority)
        {
            Process process = Process.GetProcessById(PID);
            if (process == null)
            {
                return false;
            }

            ProcessThreadCollection threads = process.Threads;

            foreach (ProcessThread thread in threads)
            {
                if (thread.Id == TID)
                {
                    try
                    {
                        if (priority.Equals("AboveNormal"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.AboveNormal;
                        }
                        else if (priority.Equals("BelowNormal"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.BelowNormal;
                        }
                        else if (priority.Equals("Highest"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.Highest;
                        }
                        else if (priority.Equals("Idle"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.Idle;
                        }
                        else if (priority.Equals("Lowest"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.Lowest;
                        }
                        else if (priority.Equals("Normal"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.Normal;
                        }
                        else if (priority.Equals("TimeCritical"))
                        {
                            thread.PriorityLevel = ThreadPriorityLevel.TimeCritical;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    catch
                    {
                        return false;
                    }
                    return true;
                }
            }
            return false;
        }

       

        // OK. Read info from netstat -ano , periodically.
        private void GetNetstat(Object state)
        {
            AppendLog("Into GetNetstat");
            try
            {
                Dictionary<String, Int32> tempMap = new Dictionary<String, Int32>();

                Process netstat = new Process();
                netstat.StartInfo.FileName = "cmd.exe";
                netstat.StartInfo.UseShellExecute = false;
                netstat.StartInfo.RedirectStandardInput = true;
                netstat.StartInfo.RedirectStandardOutput = true;
                netstat.StartInfo.RedirectStandardError = true;
                netstat.StartInfo.CreateNoWindow = true;
                netstat.Start();

                netstat.StandardInput.WriteLine("netstat -ano");
                netstat.StandardInput.WriteLine("exit");

                // Read each line.
                StreamReader reader = netstat.StandardOutput;
                Boolean start = false;
                Char[] separator = new Char[] { ' ', '\t' };
                while (!reader.EndOfStream)
                {
                    String line = reader.ReadLine().Trim();

                    // Skip header info.
                    if (!start)
                    {
                        if (line.StartsWith("TCP") || line.StartsWith("UDP"))
                        {
                            start = true;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    // Read connection info line.
                    String[] parts = line.Split(separator, StringSplitOptions.RemoveEmptyEntries);
                    try
                    {
                        String[] ipParts = parts[1].Split(':');
                        String localAddr = "";
                        if (ipParts.Length == 2)        // Ipv4
                        {
                            if (ipParts[0].Equals("127.0.0.1") || ipParts[0].Equals("0.0.0.0"))
                                localAddr = TerminalInfo.Ipv4 + ":" + ipParts[1];
                            else
                                localAddr = parts[1];
                        }
                        else
                        {
                            if (ipParts[0].Equals("[::1]"))
                                localAddr = TerminalInfo.Ipv6 + ":" + ipParts[ipParts.Length - 1];
                            else
                                localAddr = parts[1];
                        }
                        tempMap.Add(localAddr, Convert.ToInt32(parts[parts.Length - 1]));
                    }
                    catch (Exception) { }
                }

                lock (endPoint2PidMap)
                {
                    endPoint2PidMap = tempMap;
                }

                netstat.WaitForExit();
                netstat.Close();
                reader.Close();
            }
            catch (Exception ex)
            {
                AppendLog("Error: GetNetstat" + ex.Message);
            }
            AppendLog("Exit GetNetstat");
        }

        // OK. Clear [Pid->Network] map, periodically.
        private void ClearPid2NetworkMap(Object state)
        {
            AppendLog("Into ClearPid2NetworkMap");
            Int32[] pids = pid2InfoMap.Keys.ToArray();
            foreach (Int32 pid in pids)
            {
                try
                {
                    if (Process.GetProcesses().Where((p) => { return p.Id == pid; }).Any())
                    {
                        pid2InfoMap.Remove(pid);
                    }
                }
                catch (Exception ex)
                {
                    AppendLog("Error: ClearPid2NetworkMap" + ex.Message);
                }
            }
            AppendLog("Exit ClearPid2InfoMap");
        }


        private void RefreshProcessInfo(Object state)
        {
            AppendLog("RefreshProcessInfo");
            Process[] processes = Process.GetProcesses();
            double coreCount = TerminalInfo.ProcessorCount * 1.0;
            foreach (Process p in processes)
            {
                if (pid2InfoMap.ContainsKey(p.Id))
                {
                    try
                    {
                        ProcessDynamicInfo info = pid2InfoMap[p.Id];
                        info.OldTotalProcessorTime = info.TotalProcessorTime;

                        // 下面的语句会抛出拒绝访问的异常
                        // 可能是p对应的进程在执行，拒绝访问这个属性。
                        info.TotalProcessorTime = p.TotalProcessorTime;
                        
                        info.CpuUsage = (info.TotalProcessorTime.TotalSeconds - info.OldTotalProcessorTime.TotalSeconds) / refreshInterval / coreCount;
                        info.ReceiveSpeed = Convert.ToInt32((info.ByteReceive - info.OldByteReceive) / refreshInterval);
                        info.SendSpeed = Convert.ToInt32((info.ByteSend - info.OldByteSend) / refreshInterval);
                        info.OldByteReceive = info.ByteReceive;
                        info.OldByteSend = info.ByteSend;
                    }
                    catch (Exception ex)
                    {
                        //AppendLog("Error: RefreshProcessInfo" + ex.Message);
                    }
                }
                else
                {
                    pid2InfoMap.Add(p.Id, new ProcessDynamicInfo());
                }
            }
        }

        private static DateTime StringToDateTime(String datetimeStr)
        {
            String dt = datetimeStr.Substring(0, datetimeStr.IndexOf('.'));
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

        private void SaveConfiguration(PersistentSettings settings, string filename)
        {
            try
            {
                settings.Save(filename);
            }
            catch (UnauthorizedAccessException)
            {
                System.Console.WriteLine("Access to the path '" + filename + "' is denied. " +
                  "The current settings could not be saved.");
            }
            catch (IOException)
            {
                System.Console.WriteLine("The path '" + filename + "' is not writable. " +
                  "The current settings could not be saved.");
            }
        }
    }

    class Shell
    {
        public static void Execute(String AppName, String args)
        {
            Process p = new Process();
            p.StartInfo.FileName = AppName;
            p.StartInfo.Arguments = args;
            p.StartInfo.CreateNoWindow = true;
            // p.StartInfo.EnvironmentVariables
            // p.StartInfo.Password
            // p.StartInfo.UserName
            p.StartInfo.UseShellExecute = true;
            // p.StartInfo.WorkingDirectory
            // p.StartInfo.RedirectStandardInput
            p.Start();
        }
    }

}
