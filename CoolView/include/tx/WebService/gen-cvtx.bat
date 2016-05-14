@echo gSoapPath:%gSoapDir%
@set path=%gSoapDir%\bin\win32;%path%
mkdir cvtx
soapcpp2.exe -inwx -d./cvtx -I%gSoapDir%/import cvtx.h
@pause