using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using System.Configuration;
using System.Windows;

namespace CoolViewManager.Utility
{
    static class CoolViewHttpClient
    {
        private static String cookie = null;
        private static String baseUrl = ConfigurationSettings.AppSettings["baseUrl"];

        public static event Action LoginSucceedEventHandler;

        public static void Login(String username, String password)
        {
            String requestUrl = baseUrl + "/user/login.action?username=" + username + "&password=" + password;
            WebRequest request = HttpWebRequest.Create(requestUrl);
            request.Method = "GET";
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();

            if (response.StatusCode != HttpStatusCode.OK)
                throw new Exception("登录失败");

            StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
            String responseText = reader.ReadToEnd().Trim();
            reader.Close();

            if (responseText.Contains("400"))
            {
                MessageBox.Show("用户名或密码错误！");
                return;
            }

            cookie = response.Headers["Set-Cookie"];

            if (LoginSucceedEventHandler != null)
                LoginSucceedEventHandler();
        }

        public static String Get(String actionUrl, List<KeyValuePair<String, String>> parameters)
        {
            if (cookie == null)
                throw new Exception("请先登录");

            String requestUrl = baseUrl + actionUrl + "?";
            for (int i = 0; i < parameters.Count; i++)
            {
                if(i==0)
                    requestUrl += parameters[0].Key + "=" + parameters[0].Value;
                else
                    requestUrl += "&" + parameters[i].Key + "=" + parameters[i].Value;
            }

            WebRequest request = HttpWebRequest.Create(requestUrl);
            request.Method = "GET";
            request.Headers.Add("Cookie", cookie);
            WebResponse response = request.GetResponse();

            StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
            String responseText = reader.ReadToEnd().Trim();
            reader.Close();

            return responseText;
        }
    }
}
