* Monogram AAC Encoder

1、原版无法在VS2008中编译Release版本，原因及解决方法如下：

We have a DLL which contains a number of DirectShow filters which inherit from the DirectShow base classes.  The DLL also uses classes from MFC.  So the DLL links against both MFC and the DirectShow base classes (strmbase.lib).  Everything compiles and works fine in VS 2005.
 
After upgrading to VS 2008, I get the following linker errors in a Release build, but not in a Debug build:
mfcs90u.lib(oleexp.obj) : error LNK2005: _DllGetClassObject@12 already defined in strmbase.lib(dllentry.obj)
mfcs90u.lib(oleexp.obj) : error LNK2005: _DllCanUnloadNow@0 already defined in strmbase.lib(dllentry.obj)
 
After some searching, I found this related thread, suggesting a conflict between MFC and COM exports from a DLL.  I tried the suggested workaround, and got past the linker error with...
1. Change DllGetClassObject to MyDllGetClassObject in dllentry.cpp from the DS base classes source.
2. Change DllCanUnloadNow to MyDllCanUnloadNow in dllentry.cpp from the DS base classes source.
3. Rebuild the DirectShow base class library as strmbase.lib.
4. Modify the .def file in our DLL project by changing it as follows:
	; Filters.def : Declares the module parameters for the DLL.
	EXPORTS
	  DllMain                                    PRIVATE
	  DllGetClassObject   = MyDllGetClassObject  PRIVATE
	  DllCanUnloadNow     = MyDllCanUnloadNow    PRIVATE
	  DllRegisterServer                          PRIVATE
	  DllUnregisterServer                        PRIVATE

(经过修正的strmbase静态库已经包含在mmaac项目的3rd\lib中，编译时优先使用这些库，而非DShow路径下的库文件)
另注：迁移到VS2012后无此问题，请保持现有项目配置，使用DirectShow的原始静态库。（Liaokz, 2013.9.9）

2、原版Debug版无法注册，因为faac库的编译参数问题。已使用VC2012重新编译lib库并放在mmaac\3rd中。一般可直接编译mmaac编码器。如果特殊情况需要重编译faac时可以使用faac-1.28\libfaac中的VC2012工程重新编译lib库。

3、mmaac\binaries中是官方编译的文件，似乎有bug，会出错。

