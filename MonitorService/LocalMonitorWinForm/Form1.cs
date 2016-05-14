using System;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;
using System.ServiceModel;
using System.Timers;

using System.Management;
using System.Diagnostics;


namespace LocalMonitorWinForm
{
    public partial class Form1 : Form
    {
        private ServiceHost host;
        private Monitor monitor;
        private String url;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            String[] args = Environment.GetCommandLineArgs();
            if (args.Length == 3 && args[1].Equals("-update"))
            {

            }
            else
            {
                monitor.StopCapture();
                host.Close();
            }
        }

        private void StartUpHost()
        {
            monitor = new Monitor();
            monitor.StartCapture();
            //TerminalInfo info = Monitor.TerminalInfo;
            host = new ServiceHost((Object)monitor, new Uri("http://" + monitor.TerminalInfo.Ipv4 + ":8010/MonitorService/"));
            try
            {
                host.Open();
            }
            catch(AddressAlreadyInUseException)
            {
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }
        }

        private String GetServiceUrl(String ip)
        {
            return "http://" + ip + ":8010/MonitorService/";
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Visible = false;
            this.FormClosing += new FormClosingEventHandler(Form1_FormClosing);

            String[] args = Environment.GetCommandLineArgs();
            if (args.Length == 3 && args[1].Equals("-update"))
            {
                String myProcessName = Process.GetCurrentProcess().ProcessName;
                int myProcessId = Process.GetCurrentProcess().Id;
                Process[] processes = Process.GetProcessesByName(myProcessName);
                foreach (Process process in processes)
                {
                    if (process.Id != myProcessId)
                    {
                        process.Kill();
                    }
                }
                this.Close();
            }
            else
            {
                StartUpHost();
            }
        }
    }
}
