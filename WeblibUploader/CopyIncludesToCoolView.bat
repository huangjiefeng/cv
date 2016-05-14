@echo off
set CvPath=..\CoolView\include\WeblibUploader
set CodePath=WeblibUploader

copy /Y %CodePath%\dllexports.h %CvPath%\
copy /Y %CodePath%\WeblibUIComponont.h %CvPath%\
copy /Y %CodePath%\workerservice.h %CvPath%\
copy /Y %CodePath%\commonstruts.h %CvPath%\

pause