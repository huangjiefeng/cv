/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011 Intel Corporation. All Rights Reserved.

\* ****************************************************************************** */

#ifndef __MFX_PLUGIN_BASE_H__
#define __MFX_PLUGIN_BASE_H__

#include <mfxplugin++.h>

class MFXPluginBase : public MFXPlugin
{
public:
    virtual ~MFXPluginBase() { };

    /* API between application and plugin */    
    virtual mfxStatus QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2]) = 0;
    virtual mfxStatus SetHandle(mfxHandleType type, mfxHDL handle) = 0;
    virtual mfxStatus SetAllocator(mfxFrameAllocator *pAlloc) = 0;
    virtual mfxStatus Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize) = 0;    
    virtual mfxStatus Close() = 0;
};

typedef MFXPluginBase* (*mfxCreatePluginFunc)();

MFXPluginBase* CreatePlugin();

#endif // __MFX_PLUGIN_BASE_H__
