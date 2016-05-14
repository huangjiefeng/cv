@echo off
set CvPath=..\CoolView

copy /Y Win32\Debug_DLL\WeblibUploader.dll %CvPath%\Debug
copy /Y Win32\Debug_DLL\WeblibUploader.lib %CvPath%\Debug
copy /Y Win32\Debug_DLL\WeblibUploader.pdb %CvPath%\Debug

copy /Y Win32\Release_DLL\WeblibUploader.dll %CvPath%\Release
copy /Y Win32\Release_DLL\WeblibUploader.lib %CvPath%\Release

pause