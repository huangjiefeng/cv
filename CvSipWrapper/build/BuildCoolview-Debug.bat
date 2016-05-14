@echo off
echo Building coolview-2008.sln, please wait a minute...
"%windir%\Microsoft.NET\Framework\v3.5\MsBuild.exe" coolview-2008.sln   /p:Configuration=Debug /t:Rebuild   
pause