1、oRTP项目是2010的版本，师兄修改过，不好升级了；
2、oRTP项目配置已被修改，编译结果输出到filter的目录，更新oRTP库记得对照修改，否则会因找不到ortp.lib而报连接错误！
3、mediastreamer2项目来自wengophone
4、项目依赖于openssl库，DLL在.\3rdparty\openssl\bin中。如果输出目录..\_build\Win32\Debug(Release)或System32目录中不存在相关dll，filter注册会失败，需要手动复制过去。
	——Liaokz，2014-12