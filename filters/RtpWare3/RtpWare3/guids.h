//------------------------------------------------------------------------------
// File: SampleDumpGUIDs.h
//
// Desc: DirectShow sample code - CLSIDs used by the SampleDump Filters.
//
// Copyright (c) SCUT.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef __SCUT_RTPWARE3_GUIDS_H
#define __SCUT_RTPWARE3_GUIDS_H

// {31158A57-8952-4EBE-85F0-909DFF6B4B24}
DEFINE_GUID(CLSID_SCUTRtpSender, 
            0x31158a57, 0x8952, 0x4ebe, 0x85, 0xf0, 0x90, 0x9d, 0xff, 0x6b, 0x4b, 0x24);

// {92A734E7-0A0D-493B-9196-DFB841EC68B4}
DEFINE_GUID(CLSID_SCUTRtpReceiver, 
            0x92a734e7, 0xa0d, 0x493b, 0x91, 0x96, 0xdf, 0xb8, 0x41, 0xec, 0x68, 0xb4);

// {A0904DEC-71DF-4AF4-8B7D-3FDB2067FDB6}
DEFINE_GUID(IID_ISCUTRtpSender, 
            0xa0904dec, 0x71df, 0x4af4, 0x8b, 0x7d, 0x3f, 0xdb, 0x20, 0x67, 0xfd, 0xb6);

// {BCFA2135-67FA-44AE-B490-BC5182C8AED0}
DEFINE_GUID(IID_ISCUTRtpReceiver, 
            0xbcfa2135, 0x67fa, 0x44ae, 0xb4, 0x90, 0xbc, 0x51, 0x82, 0xc8, 0xae, 0xd0);


//请同SPEEX编解码Filter项目中定义保持一致
//自定义的SPEEX类型GUID，Data1为MAKEFOURCC('S','P','X','R')，SPXR是参照VLC解码器得到的
//其余部分参照MEDIASUBTYPE_PCM
static const CLSID MEDIASUBTYPE_SPEEX = 
{0x52585053, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};

//请同AAC编码器项目保持一致
// {000000FF-0000-0010-8000-00AA00389B71}
static const CLSID MEDIASUBTYPE_AAC = 
{0x000000FF, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};


#endif
