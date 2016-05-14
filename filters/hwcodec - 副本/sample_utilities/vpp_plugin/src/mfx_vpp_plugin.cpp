/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2010-2011 Intel Corporation. All Rights Reserved.

\* ****************************************************************************** */

#include <stdio.h>
#include <tchar.h>
#include <memory>
#include "mfx_vpp_plugin.h"

#define MY_WAIT(__Milliseconds)  Sleep(__Milliseconds)

MFXVideoVPPPlugin::MFXVideoVPPPlugin(mfxSession session) 
    : MFXVideoMultiVPP(session)    
{    
    m_PluginModule = NULL;
    m_pPlugin = NULL;

    memset(&m_pmfxPluginParam, 0, sizeof(m_pmfxPluginParam));
    m_pAuxParam = NULL;
    m_AuxParamSize = 0;

    memset(&m_FrameAllocator, 0, sizeof(m_FrameAllocator));
    memset(&m_pmfxVPP1Param, 0, sizeof(m_pmfxVPP1Param));
    memset(&m_pmfxVPP2Param, 0, sizeof(m_pmfxVPP2Param));
    m_pVPP1 = NULL;
    m_pVPP2 = NULL;

    m_session2 = NULL;
    m_mfxHDL   = NULL;

    m_allocResponses[0].reset();
    m_allocResponses[1].reset();
}

mfxStatus MFXVideoVPPPlugin::CreatePlugin(mfxCreatePluginFunc pCreateFunc)
{   
    MSDK_SAFE_DELETE(m_pPlugin);
    m_pPlugin = (*pCreateFunc)();
    MSDK_CHECK_POINTER(m_pPlugin, MFX_ERR_NULL_PTR);    

    return MFX_ERR_NONE;
}

mfxStatus MFXVideoVPPPlugin::LoadDLL(TCHAR *dll_path)
{
    MSDK_CHECK_POINTER(dll_path, MFX_ERR_NULL_PTR);

    // Load plugin DLL
    HMODULE m_PluginModule = LoadLibrary(dll_path);
    MSDK_CHECK_POINTER(m_PluginModule, MFX_ERR_NOT_FOUND);

    // Load Create function
    mfxCreatePluginFunc pCreateFunc = (mfxCreatePluginFunc)GetProcAddress(m_PluginModule, "CreatePlugin");
    MSDK_CHECK_POINTER(pCreateFunc, MFX_ERR_NOT_FOUND);

    // Create plugin object
    m_pPlugin = (*pCreateFunc)();
    MSDK_CHECK_POINTER(m_pPlugin, MFX_ERR_NOT_FOUND);
    
    return MFX_ERR_NONE;
}

mfxStatus MFXVideoVPPPlugin::SetAuxParam(void* auxParam, int auxParamSize)
{
    MSDK_CHECK_POINTER(auxParam, MFX_ERR_MEMORY_ALLOC);
    MSDK_CHECK_ERROR(0, auxParamSize, MFX_ERR_NOT_FOUND);

    m_pAuxParam = auxParam;
    m_AuxParamSize = auxParamSize;

    return MFX_ERR_NONE;
}


mfxStatus MFXVideoVPPPlugin::SetFrameAllocator(mfxFrameAllocator *allocator)
{
    MSDK_CHECK_POINTER(allocator, MFX_ERR_NULL_PTR);

    m_FrameAllocator = *allocator;

    return m_pPlugin->SetAllocator(allocator);
}

mfxStatus MFXVideoVPPPlugin::SetHandle(mfxHandleType type, mfxHDL hdl)
{
    if (type == MFX_HANDLE_D3D9_DEVICE_MANAGER)
    {
        m_mfxHDL = hdl;
    }
    return m_pPlugin->SetHandle(type, hdl);
}

MFXVideoVPPPlugin::~MFXVideoVPPPlugin(void)
{
    Close();
}

// per-component methods currently not implemented, you can add your own implementation if needed
mfxStatus MFXVideoVPPPlugin::Query(mfxVideoParam *in, mfxVideoParam *out, mfxU8 component_idx)
{
    in;out;component_idx;
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus MFXVideoVPPPlugin::GetVideoParam(mfxVideoParam *par, mfxU8 component_idx)
{    
    par;component_idx;
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus MFXVideoVPPPlugin::GetVPPStat(mfxVPPStat *stat, mfxU8 component_idx)
{
    stat;component_idx;
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus MFXVideoVPPPlugin::AllocateFrames(mfxVideoParam *par, mfxVideoParam *par1, mfxVideoParam *par2)
{   
    mfxStatus sts = MFX_ERR_NONE;

    mfxFrameAllocResponse allocResponse = {0};
    mfxFrameAllocRequest plgAllocRequest[2] = {0};
    mfxFrameAllocRequest vpp1Request[2] = {0};
    mfxFrameAllocRequest vpp2Request[2] = {0};

    if (m_pPlugin)
    {
        m_pmfxPluginParam = par;
        
        sts = m_pPlugin->QueryIOSurf(m_pmfxPluginParam, plgAllocRequest);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);        
    } 
    else
    {
        return MFX_ERR_NOT_INITIALIZED;
    } 
    
    if (par1 && m_pVPP1)
    {
        m_pmfxVPP1Param = par1;

        sts = m_pVPP1->QueryIOSurf(m_pmfxVPP1Param, vpp1Request);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);        
        
        plgAllocRequest[0].Type = (mfxU16) ((vpp1Request[1].Type & ~MFX_MEMTYPE_EXTERNAL_FRAME) | MFX_MEMTYPE_INTERNAL_FRAME);
        plgAllocRequest[0].NumFrameMin = plgAllocRequest[0].NumFrameMin + vpp1Request[1].NumFrameMin;
        plgAllocRequest[0].NumFrameSuggested = plgAllocRequest[0].NumFrameSuggested + vpp1Request[1].NumFrameSuggested;

        // check if opaque memory was requested for this pipeline by the application
        bool bIsOpaque = par->IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY ? true : false;

        // allocate actual memory only if not opaque
        if (!bIsOpaque)
        {
            sts = m_FrameAllocator.Alloc(m_FrameAllocator.pthis, &plgAllocRequest[0], &allocResponse);
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        }
        else
        {
            allocResponse.NumFrameActual = plgAllocRequest[0].NumFrameSuggested;
        }        

        if (m_allocResponses[0].get())
        {
            m_FrameAllocator.Free(m_FrameAllocator.pthis, m_allocResponses[0].get());            
        }
        m_allocResponses[0].reset(new mfxFrameAllocResponse(allocResponse));
        //create pool of mfxFrameSurface1 structures
        sts = m_SurfacePool1.Alloc(allocResponse, plgAllocRequest[0].Info, bIsOpaque);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        // fill mfxExtOpaqueAlloc buffer of vpp and plugin in case of opaque memory
        if (bIsOpaque)
        {
            mfxExtOpaqueSurfaceAlloc* vpp1OpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)GetExtBuffer
                (par1->ExtParam, par1->NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);

            MSDK_CHECK_POINTER(vpp1OpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);
            
            vpp1OpaqueAlloc->Out.Surfaces = m_SurfacePool1.m_ppSurfacesPool;
            vpp1OpaqueAlloc->Out.NumSurface = (mfxU16)m_SurfacePool1.m_nPoolSize;

            vpp1OpaqueAlloc->Out.Type = plgAllocRequest[0].Type;

            mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)GetExtBuffer
                (par->ExtParam, par->NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);

            MSDK_CHECK_POINTER(pluginOpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);
            pluginOpaqueAlloc->In = vpp1OpaqueAlloc->Out;
        }
    }
    else if (par1 || m_pVPP1) // configuration differs from the one provided in QueryIOSurf
    {
        return MFX_ERR_UNSUPPORTED;
    }

    if (par2 && par1)
    {
        //setting the same allocator in second session
        MSDK_CHECK_RESULT(sts = MFXVideoCORE_SetFrameAllocator(m_session2, &m_FrameAllocator), MFX_ERR_NONE, sts);
        if (NULL != m_mfxHDL)
        {
            MSDK_CHECK_RESULT(sts = MFXVideoCORE_SetHandle(m_session2,
                MFX_HANDLE_D3D9_DEVICE_MANAGER, m_mfxHDL), MFX_ERR_NONE, sts);
        }       
    }

    if (par2 && m_pVPP2)
    {
        m_pmfxVPP2Param = par2;

        sts = m_pVPP2->QueryIOSurf(m_pmfxVPP2Param, vpp2Request);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        plgAllocRequest[1].Type = (mfxU16) ((vpp2Request[1].Type & ~MFX_MEMTYPE_EXTERNAL_FRAME) | MFX_MEMTYPE_INTERNAL_FRAME);
        plgAllocRequest[1].NumFrameMin = plgAllocRequest[1].NumFrameMin + vpp2Request[0].NumFrameMin;
        plgAllocRequest[1].NumFrameSuggested = plgAllocRequest[1].NumFrameSuggested + vpp2Request[0].NumFrameSuggested;

        // check if opaque memory was requested for this pipeline by the application
        bool bIsOpaque = par->IOPattern & MFX_IOPATTERN_OUT_OPAQUE_MEMORY ? true : false;

        // check that IOPattern of VPP is consistent
        if (bIsOpaque && par1 && !(par1->IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY))
            return MFX_ERR_UNSUPPORTED;

        // allocate actual memory only if not opaque
        if (!bIsOpaque)
        {
            sts = m_FrameAllocator.Alloc(m_FrameAllocator.pthis, &plgAllocRequest[1], &allocResponse);
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        }
        else
        {
            allocResponse.NumFrameActual = plgAllocRequest[1].NumFrameSuggested;
        }        

        if (m_allocResponses[1].get())
        {
            m_FrameAllocator.Free(m_FrameAllocator.pthis, m_allocResponses[1].get());            
        }
        m_allocResponses[1].reset(new mfxFrameAllocResponse(allocResponse));
        //create pool of mfxFrameSurface1 structures
        sts = m_SurfacePool2.Alloc(allocResponse, plgAllocRequest[1].Info, bIsOpaque);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        // fill mfxExtOpaqueAlloc buffer of vpp and plugin in case of opaque memory
        if (bIsOpaque)
        {
            mfxExtOpaqueSurfaceAlloc* vpp2OpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)GetExtBuffer
                (par2->ExtParam, par2->NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
            MSDK_CHECK_POINTER(vpp2OpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);

            vpp2OpaqueAlloc->In.Surfaces = m_SurfacePool2.m_ppSurfacesPool;
            vpp2OpaqueAlloc->In.NumSurface = (mfxU16)m_SurfacePool2.m_nPoolSize;
            vpp2OpaqueAlloc->In.Type = plgAllocRequest[1].Type;

            mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)
                GetExtBuffer(par->ExtParam, par->NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
            MSDK_CHECK_POINTER(pluginOpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);

            pluginOpaqueAlloc->Out = vpp2OpaqueAlloc->In;
        }        
    }
    else if (par2 || m_pVPP2)// configuration differs from the one provided in QueryIOSurf
    {
        return MFX_ERR_UNSUPPORTED;
    }

    return sts;
}

mfxStatus MFXVideoVPPPlugin::QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2], mfxVideoParam *par1, mfxVideoParam *par2)
{
    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);
    
    mfxStatus sts = MFX_ERR_NONE;
    mfxFrameAllocRequest plgAllocRequest[2] = {0};
    mfxFrameAllocRequest vpp1Request[2] = {0};
    mfxFrameAllocRequest vpp2Request[2] = {0};

    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);

    // create and init Plugin
    if (m_pPlugin)
    {
        m_pmfxPluginParam = par;
        
        sts = m_pPlugin->QueryIOSurf(m_pmfxPluginParam, plgAllocRequest);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        request[0] = plgAllocRequest[0];
        request[1] = plgAllocRequest[1];
    } 
    else
    {
        return MFX_ERR_NOT_INITIALIZED;
    }    

    // create VPP and allocate surfaces to build VPP->Plugin pipeline
    if (par1)
    {
        m_pmfxVPP1Param = par1;
        m_pVPP1 = new MFXVideoVPP(m_session);
        MSDK_CHECK_POINTER(m_pVPP1, MFX_ERR_MEMORY_ALLOC);
                
        sts = m_pVPP1->QueryIOSurf(m_pmfxVPP1Param, vpp1Request);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);        
        
        plgAllocRequest[0].Type = (mfxU16) ((vpp1Request[1].Type & ~MFX_MEMTYPE_EXTERNAL_FRAME) | MFX_MEMTYPE_INTERNAL_FRAME);
        plgAllocRequest[0].NumFrameMin = plgAllocRequest[0].NumFrameMin + vpp1Request[1].NumFrameMin;
        plgAllocRequest[0].NumFrameSuggested = plgAllocRequest[0].NumFrameSuggested + vpp1Request[1].NumFrameSuggested;

        // check if opaque memory was requested for this pipeline by the application
        bool bIsOpaque = par->IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY ? true : false;

        // check that IOPattern of VPP is consistent
        if (bIsOpaque && !(par1->IOPattern & MFX_IOPATTERN_OUT_OPAQUE_MEMORY))
            return MFX_ERR_UNSUPPORTED;

        // exposed to application
        request[0] = vpp1Request[0];
    }

    //need to create a new session if both vpp are used
    //second vpp will be in a separate joined session
    mfxSession session2 = m_session;
    if (par2 && par1)
    {
        MSDK_CHECK_RESULT(sts = MFXCloneSession(m_session, &m_session2), MFX_ERR_NONE, sts);
        MSDK_CHECK_RESULT(sts = MFXJoinSession(m_session, m_session2), MFX_ERR_NONE, sts);

        session2 = m_session2;
    }
    
    // create VPP and allocate surfaces to build Plugin->VPP pipeline
    if (par2)
    {
        m_pmfxVPP2Param = par2;
        m_pVPP2 = new MFXVideoVPP(session2);
        MSDK_CHECK_POINTER(m_pVPP2, MFX_ERR_MEMORY_ALLOC);
        
        sts = m_pVPP2->QueryIOSurf(m_pmfxVPP2Param, vpp2Request);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        plgAllocRequest[1].Type = (mfxU16) ((vpp2Request[1].Type & ~MFX_MEMTYPE_EXTERNAL_FRAME) | MFX_MEMTYPE_INTERNAL_FRAME);
        plgAllocRequest[1].NumFrameMin = plgAllocRequest[1].NumFrameMin + vpp2Request[0].NumFrameMin;
        plgAllocRequest[1].NumFrameSuggested = plgAllocRequest[1].NumFrameSuggested + vpp2Request[0].NumFrameSuggested;

        // check if opaque memory was requested for this pipeline by the application
        bool bIsOpaque = par->IOPattern & MFX_IOPATTERN_OUT_OPAQUE_MEMORY ? true : false;

        // check that IOPattern of VPP is consistent
        if (bIsOpaque && !(par2->IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY))
            return MFX_ERR_UNSUPPORTED;

        // exposed to application
        request[1] = vpp2Request[1];
    }

    return MFX_ERR_NONE;    
}

mfxStatus MFXVideoVPPPlugin::Init(mfxVideoParam *par, mfxVideoParam *par1, mfxVideoParam *par2)
{
    mfxStatus sts = MFX_ERR_NONE;    
    
    sts = AllocateFrames(par, par1, par2);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    // create and init Plugin
    if (m_pPlugin)
    {
        m_pmfxPluginParam = par;
        
        // Register plugin, acquire mfxCore interface which will be needed in Init
        std::auto_ptr<MFXPluginAdapter> mfxPlugin(new MFXPluginAdapter(m_pPlugin));
        sts = MFXVideoUSER_Register(m_session, 0, mfxPlugin.get());        
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

        // Init plugin
        sts = m_pPlugin->Init(m_pmfxPluginParam, m_pAuxParam, m_AuxParamSize);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);        
    } else
    {
        return MFX_ERR_NOT_INITIALIZED;
    }

    if (par1 && m_pVPP1)
    {
        m_pmfxVPP1Param = par1;
        sts = m_pVPP1->Init(m_pmfxVPP1Param);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
    }
    else if (par1 || m_pVPP1) // configuration differs from the one provided in QueryIOSurf
    {
        return MFX_ERR_UNSUPPORTED;
    }

    if (par2 && m_pVPP2)
    {
        m_pmfxVPP2Param = par2;
        sts = m_pVPP2->Init(m_pmfxVPP2Param);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
    }
    else if (par2 || m_pVPP2)// configuration differs from the one provided in QueryIOSurf
    {
        return MFX_ERR_UNSUPPORTED;
    }

    return MFX_ERR_NONE;
}

mfxStatus MFXVideoVPPPlugin::Reset(mfxVideoParam *par, mfxVideoParam *par1, mfxVideoParam *par2)
{
    par;par1;par2;
    return MFX_ERR_UNSUPPORTED;
}

mfxStatus MFXVideoVPPPlugin::Close(void)
{     
    MFXVideoUSER_Unregister(m_session, 0);        
    
    MSDK_SAFE_DELETE(m_pPlugin);

    if (m_PluginModule)
    {
        FreeLibrary(m_PluginModule);
        m_PluginModule = NULL;
    }

    MSDK_SAFE_DELETE(m_pVPP1);
    MSDK_SAFE_DELETE(m_pVPP2);

    if (m_session2)
    {
        //TODO: ensure no active tasks
        MFXDisjoinSession(m_session2);      

        MFXClose(m_session2);        

        m_session2 = NULL;
    }

    return MFX_ERR_NONE;
}

mfxStatus MFXVideoVPPPlugin::RunFrameVPPAsync(mfxFrameSurface1 *in, mfxFrameSurface1 *out, mfxExtVppAuxData * /*aux*/, mfxSyncPoint *syncp)
{
    return RunVPP1(in, out, syncp);
}

mfxStatus MFXVideoVPPPlugin::RunVPP1(mfxFrameSurface1 *in, mfxFrameSurface1 *out, mfxSyncPoint *syncp)
{
    if (!m_pVPP1)
    {
        return RunPlugin(in, out, syncp);
    }

    mfxStatus sts;
    mfxFrameSurface1 *pOutSurface = out;
    mfxSyncPoint *pSyncPoint = syncp;
    mfxSyncPoint local_syncp = NULL;
    if (m_pVPP1)
    {
        pOutSurface = m_SurfacePool1.GetFreeSurface();
        MSDK_CHECK_POINTER(pOutSurface, MFX_ERR_MEMORY_ALLOC);
        pSyncPoint = &local_syncp;
    }

    for(;;)
    {
        sts = m_pVPP1->RunFrameVPPAsync(in, pOutSurface, NULL, &local_syncp); 

        if (MFX_WRN_DEVICE_BUSY == sts)
        {
            MY_WAIT(1); // just wait and then repeat the same call
        }
        else
        {
            break;
        }
    }

    if (MFX_ERR_NONE != sts)
        return sts;

    return RunPlugin(pOutSurface, out, syncp);
}

mfxStatus MFXVideoVPPPlugin::RunPlugin(mfxFrameSurface1 *in, mfxFrameSurface1 *out, mfxSyncPoint *syncp)
{
    if (!m_pPlugin || !in) // plugin doesn't support NULL input argument, need to go to buffering stage of VPP2
    {
        return RunVPP2(in, out, syncp);
    }

    mfxStatus sts;    
    mfxSyncPoint *pSyncPoint = syncp;  
    mfxFrameSurface1 *pOutSurface = out;
    mfxSyncPoint local_syncp = NULL;

    if (m_pVPP2)
    {
        pOutSurface = m_SurfacePool2.GetFreeSurface();
        MSDK_CHECK_POINTER(pOutSurface, MFX_ERR_MEMORY_ALLOC);
        pSyncPoint = &local_syncp;
    }   

    for(;;)
    {
        mfxHDL h1 = in;
        mfxHDL h2 = pOutSurface;
        sts = MFXVideoUSER_ProcessFrameAsync(m_session, &h1, 1, &h2, 1, pSyncPoint); 
        pOutSurface->Info.PicStruct = in->Info.PicStruct; // plugin has to passthrough the picstruct parameter if it is in the transcoding pipeline

        if (MFX_WRN_DEVICE_BUSY == sts)
        {
            MY_WAIT(1); // just wait and then repeat the same call
        }
        else
        {
            break;
        }
    }

    if (!m_pVPP2)
    {
        return sts;
    }
    else
    {
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);        
        
        return RunVPP2(pOutSurface, out, syncp);
    }
}

mfxStatus MFXVideoVPPPlugin::RunVPP2(mfxFrameSurface1 *in, mfxFrameSurface1 *out, mfxSyncPoint *syncp)
{
    mfxStatus sts;

    if (!in)
    {
        return MFX_ERR_MORE_DATA;
    }

    for(;;)
    {
        sts = m_pVPP2->RunFrameVPPAsync(in, out, NULL, syncp);

        if (MFX_WRN_DEVICE_BUSY == sts)
        {
            MY_WAIT(1); // just wait and then repeat the same call
        }
        else
        {
            break;
        }
    }

    return sts;
}

mfxStatus MFXVideoVPPPlugin::SurfacePool::Alloc(mfxFrameAllocResponse &response, mfxFrameInfo &info, bool opaque)
{    
    Free();

    m_ppSurfacesPool = new mfxFrameSurface1* [response.NumFrameActual];
    MSDK_CHECK_POINTER(m_ppSurfacesPool, MFX_ERR_MEMORY_ALLOC);

    for (int i = 0; i < response.NumFrameActual; i++)
    {
        m_ppSurfacesPool[i] = new mfxFrameSurface1;
        MSDK_CHECK_POINTER(m_ppSurfacesPool[i], MFX_ERR_MEMORY_ALLOC);

        memcpy(&m_ppSurfacesPool[i]->Info , &info, sizeof(info));
        memset(&m_ppSurfacesPool[i]->Data, 0, sizeof(mfxFrameData));
        if (!opaque)
        {
            m_ppSurfacesPool[i]->Data.MemId = response.mids[i];
        }        
    }
    m_nPoolSize = response.NumFrameActual;
    return MFX_ERR_NONE;
}

mfxFrameSurface1 *MFXVideoVPPPlugin::SurfacePool::GetFreeSurface()
{
    mfxU32 SleepInterval = 10; // milliseconds    

    for (mfxU32 j = 0; j < MSDK_WAIT_INTERVAL; j += SleepInterval)
    {
        for (mfxU16 i = 0; i < m_nPoolSize; i++)
        {
            if (0 == m_ppSurfacesPool[i]->Data.Locked)
            {       
                return m_ppSurfacesPool[i];
            }
        }

        //wait if there's no free surface
        MY_WAIT(SleepInterval);
    }          

    return NULL;
}
