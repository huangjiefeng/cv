using System;
using System.Collections.Generic;
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
using System.Data;

using System.Collections;
using System.ComponentModel;
using System.Collections.Specialized;
using System.Timers;

using RemoteMonitor.MonitorServiceNS;

namespace RemoteMonitor
{
    public partial class ProcessForm : Window
    {
        public delegate void refresh(string[] infoString, long[] network);

        private Dictionary<int, double> lastCpuTime = new Dictionary<int, double>(64);
        private Dictionary<int, ProcessInfo> processDictionary = new Dictionary<int, ProcessInfo>(64);
        private const int INTERVAL = 2000;
        private Timer timer;

        private String targetURL = "";
        private int cpuCount = System.Environment.ProcessorCount;

        private MonitorServiceClient myMonitorServiceClient;

        private ProcessInfoCollection processes = new ProcessInfoCollection();

        public ProcessForm(String url)
        {
            InitializeComponent();

            targetURL = url;
            myMonitorServiceClient = new MonitorServiceClient();
            myMonitorServiceClient.Endpoint.Address = new System.ServiceModel.EndpointAddress(url);
            //myMonitorServiceClient.SetUrl(url);

            InitTimer();
            timer.Elapsed += new ElapsedEventHandler(WatchProcess);
            ProcessView.ItemsSource = processes;

            ProcessView.MouseDoubleClick += new MouseButtonEventHandler(ShowThreadInfoForm);
        }

        private void ShowThreadInfoForm(object sender, MouseButtonEventArgs e)
        {
            if (ProcessView.SelectedIndex == -1)
            {
                return;
            }

            ProcessInfo p = (ProcessInfo)(ProcessView.SelectedItem);
            if (p.Priority.Equals(""))
            {
                return;
            }
            int pid = Convert.ToInt32(p.PID);
            ThreadForm tf = new ThreadForm(targetURL, pid);
            tf.ShowDialog();
        }

        private void WatchProcess(object sender, ElapsedEventArgs e)
        {
            /*long[] networkInfo = myMonitorServiceClient.GetNetworkInfo();
            String[] infoString = myMonitorServiceClient.GetProcessInfo();
            
            ProcessView.Dispatcher.Invoke(new refresh(RefreshView), new object[]{infoString, networkInfo});*/
        }

        private void InitTimer()
        {
            timer = new Timer();
            timer.Interval = INTERVAL;
            timer.Enabled = true;
        }

        private void RefreshView(String[] infoString, long[] network)
        {
            int selectedPID = -1;
            ProcessInfo selectedProcess = (ProcessInfo)(ProcessView.SelectedItem);
            if (selectedProcess != null)
            {
                selectedPID = selectedProcess.PID;
            }

            processes.Clear();
            foreach (String info in infoString)
            {
                ProcessInfo pi = new ProcessInfo(info);
                processes.Add(pi);
                if (pi.PID == selectedPID)
                {
                    ProcessView.SelectedItem = pi;
                }
                if (lastCpuTime.ContainsKey(pi.PID))
                {
                    pi.CpuUsage = (pi.CpuTime - lastCpuTime[pi.PID]) / INTERVAL * 1000 / cpuCount * 100; 
                    pi.CpuUsage = (int)(pi.CpuUsage * 100) / 100.0;
                    lastCpuTime[pi.PID] = pi.CpuTime;
                }
                else
                {
                    pi.CpuUsage = 0;
                    lastCpuTime.Add(pi.PID, pi.CpuTime);
                }
            }

            ReceivedByte.Text = network[0].ToString();
            SentByte.Text = network[1].ToString();
        }

        private void KillProcess_Click(object sender, RoutedEventArgs e)
        {
            int PID = ((ProcessInfo)(ProcessView.SelectedValue)).PID;
            myMonitorServiceClient.KillProcess(PID);
        }

        private void ComboBox_DropDownOpened(object sender, EventArgs e)
        {
            timer.Enabled = false;
        }

        private void ComboBox_DropDownClosed(object sender, EventArgs e)
        {
            int PID = ((ProcessInfo)(ProcessView.SelectedItem)).PID;
            String Priority = ((ComboBox)sender).Text;
            myMonitorServiceClient.SetProcessPriority(PID, Priority);
            timer.Enabled = true;
        }
    }
}
