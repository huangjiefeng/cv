baseclasses目录内容来自安装Windows 7 SDK后的 X:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses
X是系统盘符。
* Windows 8 SDK不再提供DirectShow Samples。新版本推荐使用Windows Media Foundation。

要正确编译此目录下的项目
(1) 需要添加环境变量DSHOW_BASECLASSES_PATH，并将其指向baseclasses，如：
	E:\Workspace\filter2012\baseclasses
(2) 然后在VS2012的“用户项目默认配置”中，添加包含路径和库路径指向baseclasses。

项目文件：
每个Filter项目文件位于项目目录下，特殊的：
(1) Intel硬编码Filter位于hwcodec\hwcodec_filters
(2) RtpWare中除了Filter，还包括网络测试相关项目

编译配置：
Intel filters：/MT
Others：/MD

编译输出：
按照OutOfSource原则编译（即中间文件和最终二进制文件与源文件在不同目录），生成位置：
Intel filters：hwcodec\_build
Others：_build
* 编译后项目目录中只有根目录存在VS的数据库文件.suo、.sdf和ipch目录。
* RTPWare未按照上述原则设置，RTPWare：Rtpware\Release 或 Debug

