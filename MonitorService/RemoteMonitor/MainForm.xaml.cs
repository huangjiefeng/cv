using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using CoolViewManager.Utility;
using System.Xml.Linq;
using Newtonsoft.Json;

using RemoteMonitor.MonitorServiceNS;
using System.Timers;

namespace RemoteMonitor
{
    /// <summary>
    /// MainForm.xaml 的交互逻辑
    /// </summary>
    public partial class MainForm : Window
    {
        private MonitorServiceClient client;
        private Timer refreshTimer;

        public MainForm()
        {
            InitializeComponent();
        }

        public void LoadOnlineTerminalList()
        {
            List<KeyValuePair<String, String>> parameters = new List<KeyValuePair<string, string>>();
            parameters.Add(new KeyValuePair<string, string>("start", "0"));
            parameters.Add(new KeyValuePair<string, string>("limit", "100000"));
            String jsonValue = CoolViewHttpClient.Get("/terminal/getTerminalList", parameters);
            XElement xmlValue = JsonConvert.DeserializeXNode(jsonValue, "result").Root;
            if (Convert.ToInt32(xmlValue.Element("code").Value) != 200)
                throw new Exception("获取终端列表失败");

            uiTerminalList.Items.Clear();
            foreach (XElement item in xmlValue.Elements("items"))
            {
                String statusString = item.Element("status").Value;
                /*String sip = item.Element("sip").Value.Split('@')[0];*/
                if (!statusString.Equals("offline")/* || sip.StartsWith("scut_t_")*/)
                {
                    Terminal terminal = new Terminal
                    {
                        Name = item.Element("terminalName").Value,
                        Sip = item.Element("sip").Value.Split('@')[0],
                        Ip = item.Element("ip").Value,
                        Ipv4 = item.Element("ipv4").Value,
                        Ipv6 = item.Element("ipv6").Value,
                        Mac = item.Element("mac").Value,
                        Model = item.Element("model").Value,
                        ScreenCount = Convert.ToInt32(item.Element("screenCount").Value),
                        Version = item.Element("version").Value,
                        System = item.Element("company").Value,
                        Location = item.Element("location").Value,
                        Savi = Convert.ToBoolean(item.Element("savi").Value),
                    };

                    uiTerminalList.Items.Add(terminal);
                }
            }
        }

        public void LoadTerminalInfo()
        {
            try
            {
                XElement root = XElement.Parse(client.GetTerminalInfo());
                TerminalInfo terminalInfo = new TerminalInfo()
                {
                    OsName = root.Element("osName").Value,
                    OsType = root.Element("osType").Value,
                    OsVersion = root.Element("osVersion").Value,
                    ProductKey = root.Element("productKey").Value,
                    InstallDate = root.Element("installDate").Value,
                    LastBootUpTime = root.Element("lastBootUpTime").Value,

                    ProcessorName = root.Element("processorName").Value,
                    ProcessorId = root.Element("processorId").Value,
                    ProcessorCount = Convert.ToInt32(root.Element("processorCount").Value),
                    CpuTotalUsage = root.Element("cpuTotalUsage").Value,
                    TotalMemory = root.Element("totalMemory").Value,
                    AvailableMemory = root.Element("availableMemory").Value,
                    MemoryCount = Convert.ToInt32(root.Element("memoryCount").Value),
                    VisibleMemory = root.Element("visibleMemory").Value,
                    DiskSize = root.Element("diskSize").Value,
                    DriveCount = Convert.ToInt32(root.Element("driveCount").Value),
                    AvailableSize = root.Element("availableSize").Value,
                    Ipv4 = root.Element("ipv4").Value,
                    Ipv6 = root.Element("ipv6").Value,
                    Mac = root.Element("mac").Value,
                    ByteReceived = root.Element("byteReceived").Value,
                    ByteSent = root.Element("byteSent").Value,
                };

                this.Dispatcher.Invoke(new Action(() => uiTerminalInfoGrid.DataContext = terminalInfo));
            }
            catch (Exception) { }
        }

        public void LoadProcessInfo()
        {
            try
            {
                XElement root = XElement.Parse(client.GetProcessInfo(""));

                List<Process> processList = new List<Process>();

                foreach (XElement info in root.Elements())
                {
                    Process process = new Process()
                    {
                        Pid = Convert.ToInt32(info.Element("pid").Value),
                        Name = info.Element("name").Value,
                        Priority = info.Element("priority").Value,
                        ThreadCount = Convert.ToInt32(info.Element("threadCount").Value),
                        Cpu = Convert.ToDouble(info.Element("cpu").Value.TrimEnd('%')),
                        Memory = Convert.ToInt32(info.Element("mem").Value.TrimEnd('K', 'B')),
                        SendSpeed = Convert.ToInt32(info.Element("send").Value.TrimEnd('B', 'p', 's')),
                        ReceiveSpeed = Convert.ToInt32(info.Element("receive").Value.TrimEnd('B', 'p', 's'))
                    };

                    processList.Add(process);
                }

                this.Dispatcher.Invoke(new Action(() => uiProcessList.ItemsSource = processList));
            }
            catch (Exception) { }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            refreshTimer = new Timer(2000);
            refreshTimer.Elapsed += new ElapsedEventHandler(refreshTimer_Elapsed);
            refreshTimer.Enabled = true;

            LoadOnlineTerminalList();
        }

        void refreshTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (client != null)
            {
                try
                {
                    LoadTerminalInfo();
                    LoadProcessInfo();
                }
                catch (Exception) { }
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            App.LoginForm.Close();
        }

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            LoadOnlineTerminalList();
        }

        private void uiTerminalList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (client != null)
            {
                client.Close();
            }

            Terminal terminal = uiTerminalList.SelectedItem as Terminal;
            client = new MonitorServiceClient();
            client.Endpoint.Address = new System.ServiceModel.EndpointAddress(new Uri("http://" + terminal.Ipv4 + ":8010/MonitorService/"));
            //client.Open();
        }
    }
}
