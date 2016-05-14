此处是GSOAP的公共文件，为了能在同一程序中使用多个WebService，参阅https://www.cs.fsu.edu/~engelen/soapdoc2.html中19.36.1  C++ Clients Example，以及19.35  How to Build a Client or Server in a C++ Code Namespace，作如下处理：

1、common中是gsoap的基础文件，在common\stdsoap2.cpp加入宏定义#define WITH_NONAMESPACES；

2、定义空的env.h，并用脚本gen-env.bat生成SOAP Header and Fault serialization codes；

3、定义并生成每个WebService自己的文件，可参考..\tx\WebService