cd /d %~dp0 
@echo off
regsvr32 AudioAECFilter.dll
regsvr32 encoder_aac.dll
regsvr32 ..\..\..\..\CvSetup\Release\filter\mmaacd.dll
regsvr32 ..\..\..\..\CvSetup\Release\filter\mp4demux.dll
regsvr32 mp4mux.dll
regsvr32 MultiSmartTee.dll
regsvr32 RGB24toYUV2.dll
regsvr32 SCUT_H264_Decoder.dll
regsvr32 SCUT_H264_Encoder.dll
regsvr32 SpeexDec.dll
regsvr32 SpeexEnc.dll
regsvr32 FilePlayCtrlFilter.dll
regsvr32 RtpWare3.dll
@pause