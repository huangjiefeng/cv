@echo gSoapPath:%gSoapDir%
@set path=%gSoapDir%\bin\win32;%path%
mkdir monitor
soapcpp2.exe -1inwx -Qmonitor_soap -d./monitor -C -I%gSoapDir%/import monitor.h
@pause