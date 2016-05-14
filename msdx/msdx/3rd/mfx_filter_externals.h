/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2008-2011 Intel Corporation. All Rights Reserved.
//
//
*/

#pragma once

#pragma warning(disable : 4995)

#include <objbase.h>
#include <initguid.h>

// {6BABAF70-864B-486b-B471-CC4E9AFF931B}
static const GUID IID_IConfigureVideoEncoder =
{ 0x6babaf70, 0x864b, 0x486b, { 0xb4, 0x71, 0xcc, 0x4e, 0x9a, 0xff, 0x93, 0x1b } };

// {8C787D42-E7C5-47e0-BFAC-7305EB54EBBA}
static const GUID IID_IConfigureVideoDecoder = 
{ 0x8c787d42, 0xe7c5, 0x47e0, { 0xbf, 0xac, 0x73, 0x5, 0xeb, 0x54, 0xeb, 0xba } };

// {F1DB8643-AB53-494a-B8D1-7D8DA5A32C2A}
static const GUID IID_IExposeMfxMemTypeFlags = 
{ 0xf1db8643, 0xab53, 0x494a, { 0xb8, 0xd1, 0x7d, 0x8d, 0xa5, 0xa3, 0x2c, 0x2a } };

// {66F887BE-2764-4825-8EA9-E9DF3497B019}
static const GUID IID_IShareMfxSession = 
{ 0x66f887be, 0x2764, 0x4825, { 0x8e, 0xa9, 0xe9, 0xdf, 0x34, 0x97, 0xb0, 0x19 } };

// {A4912AE3-4AD1-4c7f-B9C6-AE1566F8DE5D}
//static const GUID IID_IHWVideoEncoder = 
//{ 0xA4912AE3, 0x4AD1, 0x4c7f, { 0xB9, 0xC6, 0xAE, 0x15, 0x66, 0xF8, 0xDE, 0x5D } };

//MIDL_INTERFACE("A4912AE3-4AD1-4c7f-B9C6-AE1566F8DE5D")
//IHWVideoEncoder : public IUnknown
//{
//	STDMETHOD (setEncoderState) ( bool work ){UNREFERENCED_PARAMETER(work); return E_NOTIMPL;};
//};

interface IConfigureVideoDecoder : public IUnknown
{
    struct Statistics
    {
        DWORD struct_size;          // size of the Statistics structure
        DWORD width;                // frame width
        DWORD height;               // frame height
        DWORD frame_rate;           // frame rate
        struct
        {
            DWORD horizontal;       // horizontal pixel aspect ratio
            DWORD vertical;         // vertical pixel aspect ratio
        } aspect_ratio;             // aspect ratio
        DWORD decode_time;          // decode time in milliseconds
        DWORD frames_decoded;       // number of frames decoded
    };

    // Obtain Decoding Status
    STDMETHOD(GetRunTimeStatistics) (Statistics *statistics) { UNREFERENCED_PARAMETER(statistics); return E_NOTIMPL; }
};

MIDL_INTERFACE("6BABAF70-864B-486b-B471-CC4E9AFF931B")
IConfigureVideoEncoder : public IUnknown
{
    struct Params
    {
        enum Profile // profile values
        {
            PF_AUTOSELECT           = 0,

            // H.264 values            
            PF_H264_BASELINE             = 66,
            PF_H264_MAIN                 = 77,
            PF_H264_HIGH                 = 100,
            PF_H264_HIGH10               = 110,
            PF_H264_HIGH422              = 122,

            // MPEG2 values            
            PF_MPEG2_SIMPLE              = 80, 
            PF_MPEG2_MAIN                = 64, 
            PF_MPEG2_SNR                 = 3, 
            PF_MPEG2_SPATIAL             = 2, 
            PF_MPEG2_HIGH                = 16
        } profile_idc;

        enum Level // level values
        {
            LL_AUTOSELECT           = 0,

            // H.264 values
            LL_1                    = 10,
            LL_1b                   = 9,
            LL_11                   = 11,
            LL_12                   = 12,
            LL_13                   = 13,
            LL_2                    = 20,
            LL_21                   = 21,
            LL_22                   = 22,
            LL_3                    = 30,
            LL_31                   = 31,
            LL_32                   = 32,
            LL_4                    = 40,
            LL_41                   = 41,
            LL_42                   = 42,
            LL_5                    = 50,
            LL_51                   = 51,

            // MPEG2 values
            LL_LOW          = 10,
            LL_MAIN         = 8,
            LL_HIGH1440     = 6,
            LL_HIGH         = 4
        } level_idc;

        struct PSControl // picture sequence control
        {
            DWORD        GopPicSize;           // I-frame interval in frames
            DWORD        GopRefDist;           // Distance between I- or P- key frames;If GopRefDist = 1, there are no B-frames used
            DWORD        NumSlice;             // Number of slices
            DWORD        BufferSizeInKB;       // vbv buffer size 
        } ps_control;

        enum PCControl // picture coding control
        {
            PC_FRAME        = 1,
            PC_FIELD_TFF    = 2,
            PC_FIELD_BFF    = 4,
        } pc_control;

        struct FrameControl
        {
            DWORD        width;         // output frame width
            DWORD        height;        // output frame height
        } frame_control;
        struct ThrottleControl
        {
            // See Requirement 34
            enum ThrottlePolicy
            {
                TT_NA,              // no throttle handling
                TT_AUTO,            // auto throttling
            } throttle_policy;

            DWORD bitrate_up;       // range of bitrate increase adjustment.
            DWORD bitrate_down;     // range of bitrate decrease adjustment.
            DWORD qp_up;            // range of qp increase adjustment
            DWORD qp_down;          // range of qp decrease adjustment
        } throttle_control;
        
        struct RCControl // rate control
        {
            enum RCMethod
            {
                RC_CBR  = 1,                      // Constant Bitrate
                RC_VBR  = 2,                      // Variable Bitrate
				RC_CQP  = 3,
            } rc_method;

            DWORD       bitrate;               // specify bit rate in bps
        } rc_control;

        DWORD  preset;   
        DWORD  target_usage;

        Params() { memset(this, 0, sizeof(*this)); }
    };

    struct Statistics
    {
        DWORD struct_size;          // size of the Statistics structure
        DWORD width;                // frame width
        DWORD height;               // frame height
        DWORD frame_rate;           // frame rate
        struct
        {
            DWORD horizontal;       // horizontal pixel aspect ratio
            DWORD vertical;         // vertical pixel aspect ratio
        } aspect_ratio;             // aspect ratio
        DWORD real_bitrate;         // average bitrate
        DWORD frames_encoded;       // number of frames encoded
        DWORD requested_bitrate;    // requested bit rate
        DWORD frames_received;      // number of frames received
    };

    // Set parameters to the video encoder. If successful, S_OK is returned.
    // Each codec specific interface must implement this function.
    STDMETHOD(SetParams)(Params *params) { UNREFERENCED_PARAMETER(params); return E_NOTIMPL; }

    // Obtain the parameters currently used by the Encoder.
    // Each codec specific interface must implement this function.
    STDMETHOD(GetParams)(Params *params) { UNREFERENCED_PARAMETER(params); return E_NOTIMPL; }

    // Obtain Encoding Status
    STDMETHOD(GetRunTimeStatistics) (Statistics *statistics) PURE;
};

interface IExposeMfxMemTypeFlags : public IUnknown
{
    STDMETHOD(GetMfxMemTypeFlags) (DWORD *pFlags) PURE;
};

interface IShareMfxSession : public IUnknown
{
    STDMETHOD(GetMfxSession) (void **ppSession) PURE; 
    STDMETHOD(SygnalSessionHasChild) () PURE;
};

#define MFX_THREAD_WAIT_TIME 1
#define MFX_THREAD_WAIT Sleep(MFX_THREAD_WAIT_TIME)
