cd /d %~dp0 
@echo off
regsvr32 /u AudioAECFilter.dll
regsvr32 /u encoder_aac.dll
regsvr32 /u ..\..\..\..\CvSetup\Release\filter\mmaacd.dll
regsvr32 /u ..\..\..\..\CvSetup\Release\filter\mp4demux.dll
regsvr32 /u mp4mux.dll
regsvr32 /u MultiSmartTee.dll
regsvr32 /u RGB24toYUV2.dll
regsvr32 /u SCUT_H264_Decoder.dll
regsvr32 /u SCUT_H264_Encoder.dll
regsvr32 /u SpeexDec.dll
regsvr32 /u SpeexEnc.dll
regsvr32 /u FilePlayCtrlFilter.dll
regsvr32 /u RtpWare3.dll
@pause