cd /d %~dp0 
@echo off
regsvr32 h264_dec_filter
regsvr32 h264_enc_filter
@pause