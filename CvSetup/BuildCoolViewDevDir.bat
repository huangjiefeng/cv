@echo off

:rem 判断系统版本
echo %PROCESSOR_ARCHITECTURE% | findstr "64" >nul && if %errorlevel% equ 0 (
    set SYS64=1
    echo 系统版本：64位
) 
if %errorlevel% neq 0 (
    set SYS64=0
    echo 系统版本：32位
)

:rem 输入编译模式
choice /c rd /m "Build Release or Debug"
if errorlevel 255 goto :EOF
if errorlevel 2 (
    set CompileMode=Debug
    goto :BEGIN_COPY
)
if errorlevel 1 (
    set CompileMode=Release
    goto :BEGIN_COPY
)

:BEGIN_COPY
echo Set compile mode to %CompileMode%
echo.

set destDir=..\CoolView\%CompileMode%
echo Build %destDir%
echo.
mkdir %destDir%
mkdir %destDir%\debug

:rem config files
for %%i in (Common\cfg\*) do (
    if not exist "%destDir%\cfg\%%~nxi" (
        xcopy %%i "%destDir%\cfg\"
    ) else (
        echo "%destDir%\cfg\%%~nxi" already exist!
    )
)

:rem image and resources
xcopy "Common\Resources\*" "%destDir%\Resources\" /Y /D

:rem dbus
xcopy "Common\dbus\*" "%destDir%\dbus\" /S /Y /D
if %SYS64% equ 0 (
    xcopy "Common\dbus\bin\dbus-1.dll" "%systemroot%\system32\" /Y /D
) else (
    xcopy "Common\dbus\bin\dbus-1.dll" "%systemroot%\SysWOW64\" /Y /D
)

:rem OpenSSL
if %SYS64% equ 0 (
    xcopy "Common\openssl\libeay32.dll" "%systemroot%\system32\" /Y /D
) else (
    xcopy "Common\openssl\libeay32.dll" "%systemroot%\SysWOW64\" /Y /D
)

:rem tx ultilities
xcopy "Common\tx\tools\*" "%destDir%\tools\" /S /Y /D
::xcopy "Common\tx\txuploader\*" "%destDir%\txuploader\" /S /Y /D

:rem 3rd party lib
xcopy "common\openssl\*" "%destDir%" /Y /D

:rem Compile specific components  

:rem qt and vc runtime
xcopy "%CompileMode%\qt\*" "%destDir%" /S /Y /D

:rem ortp
xcopy "%CompileMode%\ortp\*" "%destDir%" /Y /D

:rem main processes
xcopy "%CompileMode%\CoolView\NetworkMeasurer.*" "%destDir%" /Y /D
xcopy "%CompileMode%\CoolView\WeblibUploader.*" "%destDir%" /Y /D
if "%CompileMode%"=="Debug" (
    xcopy "%CompileMode%\CoolView\cwebpaged.*" "%destDir%" /Y /D
) else (
    xcopy "%CompileMode%\CoolView\cwebpage.*" "%destDir%" /Y /D
)
xcopy "%CompileMode%\CvSipWrapper\*" "%destDir%" /Y /D
xcopy "%CompileMode%\msdx\*" "%destDir%" /Y /D

echo.
echo 更新完成（不包含CoolView项目本身的文件），请编译CoolView项目以生成项目文件。
echo.

pause
