/************************************************************************
*                                                                       *
*   NuiImageBuffer.h                                                    *
*                                                                       *
*   Copyright (c) Microsoft Corp. All rights reserved.                  *
*                                                                       *
************************************************************************/

#pragma once

// FYI: this extra stuff is to make this object look like a D3D texture, for max
// code-compat with the xbox XDK

typedef struct _KINECT_LOCKED_RECT
{
    INT                 Pitch;
    void *              pBits;
} KINECT_LOCKED_RECT;

typedef struct _KINECT_SURFACE_DESC
{
    UINT                Width;
    UINT                Height;
} KINECT_SURFACE_DESC;

class NuiImageBuffer
{
    friend class BufferPlusInfo; // this is for internal compiler use

    int     m_Width;
    int     m_Height;
    int     m_BytesPerPixel;

    // note: users of the NUI API should obtain the buffer pointer through LockRect( )
    BYTE *  m_pBuffer;

public:

    int BufferLen( )
    {
        return m_Width * m_Height * m_BytesPerPixel;
    }

    int Pitch( )
    {
        return m_Width * m_BytesPerPixel;
    }

    // this method is only to make it look like a D3D texture call
    //
    STDMETHODIMP LockRect( UINT Level,KINECT_LOCKED_RECT* pLockedRect,CONST RECT* pRectUsuallyNull,DWORD Flags)
    {
        UNREFERENCED_PARAMETER(pRectUsuallyNull);
        UNREFERENCED_PARAMETER(Flags);

        if( !pLockedRect ) return E_POINTER;

        if( Level != 0 )
        {
            return E_INVALIDARG;
        }

        pLockedRect->pBits = m_pBuffer;

        if( !m_pBuffer )
        {
            pLockedRect->Pitch = 0;
            return E_OUTOFMEMORY;
        }

        if( m_Height )
        {
            pLockedRect->Pitch = Pitch( );
            return S_OK;
        }
        else
        {
            pLockedRect->Pitch = 0;
            return E_POINTER;
        }
    }

    HRESULT GetLevelDesc( UINT Level, KINECT_SURFACE_DESC *pDesc )
    {
        if( !pDesc ) return E_POINTER;

        if( Level != 0 )
        {
            return E_INVALIDARG;
        }

        pDesc->Width = m_Width;
        pDesc->Height = m_Height;
        return S_OK;
    }

    STDMETHODIMP UnlockRect( UINT Level)
    {
        if( Level != 0 )
        {
            return E_INVALIDARG;
        }

        return S_OK;
    }
};
