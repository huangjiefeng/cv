/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011 Intel Corporation. All Rights Reserved.

\* ****************************************************************************** */

#ifndef __MFX_MULTI_VPP_H
#define __MFX_MULTI_VPP_H

// An interface for a pipeline consisting of multiple (maximum 3) VPP-like components. Base implementation - for single VPP. 
// The application should use this interface to be able to seamlessly switch from MFXVideoVPP to MFXVideoVPPPlugin in the pipeline.
class MFXVideoMultiVPP
{
public:

    MFXVideoMultiVPP(mfxSession session) { m_session = session; }
    virtual ~MFXVideoMultiVPP(void) { Close(); }    

    // topology methods
    virtual mfxStatus QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2], mfxVideoParam *par1 = NULL, mfxVideoParam *par2 = NULL) 
    { par1; par2; return MFXVideoVPP_QueryIOSurf(m_session, par, request); }

    virtual mfxStatus Init(mfxVideoParam *par, mfxVideoParam *par1 = NULL, mfxVideoParam *par2 = NULL) 
    { par1; par2; return MFXVideoVPP_Init(m_session, par); }

    virtual mfxStatus Reset(mfxVideoParam *par, mfxVideoParam *par1 = NULL, mfxVideoParam *par2 = NULL) 
    { par1; par2; return MFXVideoVPP_Reset(m_session, par); }

    virtual mfxStatus RunFrameVPPAsync(mfxFrameSurface1 *in, mfxFrameSurface1 *out, mfxExtVppAuxData *aux, mfxSyncPoint *syncp) 
    { return MFXVideoVPP_RunFrameVPPAsync(m_session, in, out, aux, syncp); }

    virtual mfxStatus Close(void) { return MFXVideoVPP_Close(m_session); }

    // per-component methods
    virtual mfxStatus Query(mfxVideoParam *in, mfxVideoParam *out, mfxU8 component_idx = 0) 
    { component_idx; return MFXVideoVPP_Query(m_session, in, out); }

    virtual mfxStatus GetVideoParam(mfxVideoParam *par, mfxU8 component_idx = 0) 
    { component_idx; return MFXVideoVPP_GetVideoParam(m_session, par); }

    virtual mfxStatus GetVPPStat(mfxVPPStat *stat, mfxU8 component_idx = 0) 
    { component_idx; return MFXVideoVPP_GetVPPStat(m_session, stat); }

protected:

    mfxSession m_session;                                       // (mfxSession) handle to the owning session
};

#endif//__MFX_MULTI_VPP_H