:rem  ‰»Î±‡“Îƒ£ Ω
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
set CvDir=..\CoolView\%CompileMode%
set SrcDir=%CompileMode%

copy /Y %SrcDir%\CvChannelDispatcher.exe %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\CvChannelDispatcher.pdb %CvDir%

copy /Y %SrcDir%\CvStreamedMedia.exe %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\CvStreamedMedia.pdb %CvDir%

copy /Y %SrcDir%\CvScreenMediaSend.exe %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\CvScreenMediaSend.pdb %CvDir%

copy /Y %SrcDir%\CvScreenMediaRecv.exe %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\CvScreenMediaRecv.pdb %CvDir%

copy /Y %SrcDir%\CvTestMedia.exe %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\CvTestMedia.pdb %CvDir%

copy /Y %SrcDir%\msdx.dll %CvDir%
if %CompileMode% equ Debug copy /Y %SrcDir%\msdx.pdb %CvDir%
@pause

