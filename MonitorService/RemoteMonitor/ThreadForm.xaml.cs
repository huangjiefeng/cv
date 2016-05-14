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
using System.Windows.Shapes;

using System.Timers;
using System.Diagnostics;

using RemoteMonitor.MonitorServiceNS;

namespace RemoteMonitor
{
    /// <summary>
    /// ThreadForm.xaml 的交互逻辑
    /// </summary>
    public partial class ThreadForm : Window
    {
        public delegate void refresh(string[] infoString);

        private Dictionary<int, double> lastCpuTime = new Dictionary<int, double>(64);
        private const int INTERVAL = 1000;
        private Timer timer;
        private int pid = 0;

        private String targetURL = "";
        private int cpuCount = System.Environment.ProcessorCount;

        private MonitorServiceClient myMonitorServiceClient;

        private ThreadInfoCollection threads = new ThreadInfoCollection();

        public ThreadForm(String url, int pid)
        {
            InitializeComponent();

            targetURL = url;
            this.pid = pid;
            myMonitorServiceClient = new MonitorServiceClient();
            myMonitorServiceClient.Endpoint.Address = new System.ServiceModel.EndpointAddress(url);
            //myMonitorServiceClient.SetUrl(url);

            InitTimer();
            timer.Elapsed += new ElapsedEventHandler(WatchThread);
            ThreadView.ItemsSource = threads;
        }

        private void InitTimer()
        {
            timer = new Timer();
            timer.Interval = INTERVAL;
            timer.Enabled = true;
        }

        private void WatchThread(object sender, ElapsedEventArgs e)
        {
            String[] infoString = null;
            try
            {
                infoString = myMonitorServiceClient.GetThreadInfo(pid);
                refresh myrefresh = new refresh(RefreshThread);
                ThreadView.Dispatcher.Invoke(myrefresh, new object[] { infoString });
            }
            catch
            {
                timer.Enabled = false;
                MessageBox.Show("进程已结束");
            }
        }

        private void RefreshThread(String[] infoString)
        {
            int selectedTID = -1;
            ThreadInfo selectedThread = (ThreadInfo)(ThreadView.SelectedItem);
            if (selectedThread != null)
            {
                selectedTID = selectedThread.TID;
            }

            threads.Clear();
            foreach (String info in infoString)
            {
                ThreadInfo ti = new ThreadInfo(info);
                threads.Add(ti);
                if (ti.TID == selectedTID)
                {
                    ThreadView.SelectedItem = ti;
                }
                if (lastCpuTime.ContainsKey(ti.TID))
                {
                    ti.CpuUsage = (ti.CpuTime - lastCpuTime[ti.TID]) / INTERVAL * 1000 / cpuCount * 100;
                    ti.CpuUsage = (int)(ti.CpuUsage * 100) / 100.0;
                    lastCpuTime[ti.TID] = ti.CpuTime;
                }
                else
                {
                    ti.CpuUsage = 0;
                    lastCpuTime.Add(ti.TID, ti.CpuTime);
                }
            }
        }

        private void ComboBox_DropDownOpened(object sender, EventArgs e)
        {
            timer.Enabled = false;
        }

        private void ComboBox_DropDownClosed(object sender, EventArgs e)
        {
            int TID = ((ThreadInfo)(ThreadView.SelectedItem)).TID;
            String Priority = ((ComboBox)sender).Text;
            myMonitorServiceClient.SetThreadPriority(pid, TID, Priority);
            timer.Enabled = true;
        }
    }
}
