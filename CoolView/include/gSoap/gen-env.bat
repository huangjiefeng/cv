@echo gSoapPath:%gSoapDir%
@set path=%gSoapDir%\bin\win32;%path%
mkdir env
soapcpp2.exe -d.\env -penv env.h
@pause