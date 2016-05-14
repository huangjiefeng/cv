@echo off
if not defined DSHOW_BASECLASSES_PATH (
if exist "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v7.1\Samples\Multimedia\DirectShow\BaseClasses" (
setx DSHOW_BASECLASSES_PATH "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v7.1\Samples\Multimedia\DirectShow\BaseClasses" -m
goto :EOF
)
if exist "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v7.0\Samples\Multimedia\DirectShow\BaseClasses" (
setx DSHOW_BASECLASSES_PATH "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v7.0\Samples\Multimedia\DirectShow\BaseClasses" -m
goto :EOF
)
if exist "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v6.1\Samples\Multimedia\DirectShow\BaseClasses" (
setx DSHOW_BASECLASSES_PATH "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v6.1\Samples\Multimedia\DirectShow\BaseClasses" -m
goto :EOF
)
if exist "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v6.0\Samples\Multimedia\DirectShow\BaseClasses" (
setx DSHOW_BASECLASSES_PATH "%SystemDrive%\Program Files\Microsoft SDKs\Windows\v6.0\Samples\Multimedia\DirectShow\BaseClasses" -m
))