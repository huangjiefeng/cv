using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using test.TestNS;
using System.Diagnostics;

namespace test
{
    public partial class Form1 : Form
    {
        MonitorServiceClient client;

        Timer timer = new Timer();

        Button lastButton = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            client = new MonitorServiceClient();
            client.Open();

            /*timer.Interval = 2000;
            timer.Enabled = true;
            timer.Tick += new EventHandler(timer_Tick);*/
        }

        void timer_Tick(object sender, EventArgs e)
        {
            String value = "";
            if (lastButton == btnGetProcessInfo)
            {
                String result = client.GetProcessInfo("");
                value = result;
            }
            else if (lastButton == btnGetTerminalInfo)
            {
                //value = client.GetTerminalInfo();
            }
            textBox1.Text = value;
        }

        private void btnGetProcessInfo_Click(object sender, EventArgs e)
        {
            textBox1.Text = client.GetProcessInfo("QQ");
        }

        private void btnGetTerminalInfo_Click(object sender, EventArgs e)
        {
            textBox1.Text = client.GetTerminalInfo();
        }

        private void btnGetHardwareInfo_Click(object sender, EventArgs e)
        {
            textBox1.Text = client.GetHardwareInfo();
        }

        private void btnShutDown_Click(object sender, EventArgs e)
        {
            client.ShutDown();
            textBox1.Text = "ShutDown";
        }
    }
}
