一、LocalMonitorWinForm项目：
1.使用了open-hardware-monitor开源项目
svn地址为：
http://open-hardware-monitor.googlecode.com/svn/trunk
在生成OpenHardwareMonitorLib.dll库时，需要修改OpenHardwareMonitor.Hardware.Ring0类
将internal static class Ring0 { --> public static class Ring0 {

2.使用了SharpPcap.dll，因此需要安装WinPcap。

3.在external目录下为第三方的dll

4.在运行时，需要将cfg目录复制到bin目录下。

5.若在运行时，遇到PerformanceCounter字符串格式化错误：
解决方案如下：
(1) 从可以运行的机器中，复制c:\windows\system32\PerfStringBackup.ini覆盖到出错的机器。
(2) 在c:\windows\system32目录下执行命令行：lodctr /R:PerfStringBackup.INI

二、RemoteMonitor项目：作为一个客户端，登陆到http服务器，用于查看所有登陆的终端的实时监控信息。

三、test项目：用于向本地终端发送wcf请求，获取监控信息，返回raw信息（xml格式）。

