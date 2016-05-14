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

namespace RemoteMonitor
{
    /// <summary>
    /// LoginForm.xaml 的交互逻辑
    /// </summary>
    public partial class LoginForm : Window
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CoolViewManager.Utility.CoolViewHttpClient.Login(usernameUI.Text, passwordUI.Password);
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            App.LoginForm = this;
            CoolViewManager.Utility.CoolViewHttpClient.LoginSucceedEventHandler += new Action(CoolViewHttpClient_LoginSucceedEventHandler);
        }

        void CoolViewHttpClient_LoginSucceedEventHandler()
        {
            this.Hide();
            App.MainForm = new MainForm();
            App.MainForm.Show();
        }
    }
}
