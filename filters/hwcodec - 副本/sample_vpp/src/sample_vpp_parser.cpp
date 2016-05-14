//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 - 2011 Intel Corporation. All Rights Reserved.
//

#include "sample_vpp_utils.h"

void vppPrintHelp(TCHAR *strAppName, TCHAR *strErrorMessage)
{
    
    _tprintf(_T("Intel(R) Media SDK VPP Sample Version %s\n\n"), MSDK_SAMPLE_VERSION);
    if (strErrorMessage)
    {
        _tprintf(_T("Error: %s\n"), strErrorMessage);
    }          

    _tprintf(_T("Usage1: %s [Options] -i InputFile -o OutputFile\n"), strAppName);

    _tprintf(_T("Options: \n"));
     _tprintf(_T("   [-lib  type]    - type of used library. sw, hw (def: sw)\n\n"));

    _tprintf(_T("   [-sw  width]     - width  of src video (def: 352)\n"));
    _tprintf(_T("   [-sh  height]    - height of src video (def: 288)\n"));
    _tprintf(_T("   [-scrX  x]       - cropX  of src video (def: 0)\n"));
    _tprintf(_T("   [-scrY  y]       - cropY  of src video (def: 0)\n"));
    _tprintf(_T("   [-scrW  w]       - cropW  of src video (def: width)\n"));
    _tprintf(_T("   [-scrH  h]       - cropH  of src video (def: height)\n"));
    _tprintf(_T("   [-sf  frameRate] - frame rate of src video (def: 30.0)\n"));
    _tprintf(_T("   [-scc format]    - format (FourCC) of src video (def: nv12. support nv12|yv12|yuy2|rgb3|rgb4)\n"));

    _tprintf(_T("   [-spic value]    - picture structure of src video\n")); 
    _tprintf(_T("                       0 = interlaced top    field first\n"));
    _tprintf(_T("                       2 = interlaced bottom field first\n")); 
    _tprintf(_T("                       1 = progressive (default)\n\n"));


    _tprintf(_T("   [-dw  width]     - width  of dst video (def: 352)\n"));
    _tprintf(_T("   [-dh  height]    - height of dst video (def: 288)\n"));
    _tprintf(_T("   [-dcrX  x]       - cropX  of dst video (def: 0)\n"));
    _tprintf(_T("   [-dcrY  y]       - cropY  of dst video (def: 0)\n"));
    _tprintf(_T("   [-dcrW  w]       - cropW  of dst video (def: width)\n"));
    _tprintf(_T("   [-dcrH  h]       - cropH  of dst video (def: height)\n"));
    _tprintf(_T("   [-df  frameRate] - frame rate of dst video (def: 30.0)\n"));
    _tprintf(_T("   [-dcc format]    - format (FourCC) of dst video (def: nv12. support nv12|yuy2)\n"));

    _tprintf(_T("   [-dpic value]    - picture structure of dst video\n")); 
    _tprintf(_T("                       0 = interlaced top    field first\n"));
    _tprintf(_T("                       2 = interlaced bottom field first\n")); 
    _tprintf(_T("                       1 = progressive (default)\n\n"));


    _tprintf(_T("   Video Enhancement Algorithms\n"));
    _tprintf(_T("   [-vanalysis]        - enable video analysis algorithm \n"));
    _tprintf(_T("   [-denoise (level)]  - enable denoise algorithm. Level is optional \n"));
    _tprintf(_T("                         range of  noise level is [0, 100]\n"));
    _tprintf(_T("   [-detail  (level)]  - enable detail enhancement algorithm. Level is optional \n"));
    _tprintf(_T("                         range of detail level is [0, 100]\n\n"));
    _tprintf(_T("   [-pa_hue  hue]        - procamp hue property.         range [-180.0, 180.0] (def: 0.0)\n"));
    _tprintf(_T("   [-pa_sat  saturation] - procamp satursation property. range [   0.0,  10.0] (def: 1.0)\n"));
    _tprintf(_T("   [-pa_con  contrast]   - procamp contrast property.    range [   0.0,  10.0] (def: 1.0)\n"));
    _tprintf(_T("   [-pa_bri  brightness] - procamp brightness property.  range [-100.0, 100.0] (def: 0.0)\n\n"));

    _tprintf(_T("   [-n frames]           - number of frames to VPP process\n\n"));
#ifdef D3D_SURFACES_SUPPORT
    _tprintf(_T("\n   [-d3d]                - work with d3d surfaces\n"));
#endif
    _tprintf(_T("\n"));

    _tprintf(_T("Usage2: %s -lib sw -sw 352 -sh 144 -scc rgb3 -dw 320 -dh 240 -dcc nv12 -denoise 32 -vanalysis -i in.rgb -o out.yuv\n"), strAppName);

    _tprintf(_T("\n"));

} // void vppPrintHelp(TCHAR *strAppName, TCHAR *strErrorMessage)

static mfxU32 Str2FourCC( TCHAR* strInput )
{
    mfxU32 fourcc = MFX_FOURCC_YV12;//default

    if ( 0 == _tcscmp(strInput, _T("yv12")) ) 
    {
        fourcc = MFX_FOURCC_YV12;
    } 
    else if ( 0 == _tcscmp(strInput, _T("rgb3")) ) 
    {
        fourcc = MFX_FOURCC_RGB3;
    } 
    else if ( 0 == _tcscmp(strInput, _T("rgb4")) ) 
    {
        fourcc = MFX_FOURCC_RGB4;
    } 
    else if ( 0 == _tcscmp(strInput, _T("yuy2")) )
    {
        fourcc = MFX_FOURCC_YUY2;
    } 
    else if ( 0 == _tcscmp(strInput, _T("nv12")) ) 
    {
        fourcc = MFX_FOURCC_NV12;
    } 

    return fourcc;

} // mfxU32 Str2FourCC( TCHAR* strInput )

static 
mfxU8 GetPicStruct( mfxU8 picStruct )
{  
    if ( 0 == picStruct ) 
    {
        return MFX_PICSTRUCT_FIELD_TFF;
    } 
    else if( 2 == picStruct )
    {
        return MFX_PICSTRUCT_FIELD_BFF;
    } 
    else 
    {
        return MFX_PICSTRUCT_PROGRESSIVE;
    }

} // mfxU8 GetPicStruct( mfxU8 picStruct ) 


mfxStatus vppParseInputString(TCHAR* strInput[], mfxU8 nArgNum, sInputParams* pParams)
{
    MSDK_CHECK_POINTER(pParams,  MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(strInput, MFX_ERR_NULL_PTR);

    mfxU32 readData;
    mfxU32 ioStatus;

    if (nArgNum < 4)
    {
        vppPrintHelp(strInput[0], _T("Please, specify all necessary parameters"));

        return MFX_ERR_MORE_DATA;
    }

    pParams->frameInfo[VPP_IN].CropX = 0;
    pParams->frameInfo[VPP_IN].CropY = 0;
    pParams->frameInfo[VPP_IN].CropW = NOT_INIT_VALUE;
    pParams->frameInfo[VPP_IN].CropH = NOT_INIT_VALUE;
    // zeroize destination crops
    pParams->frameInfo[VPP_OUT].CropX = 0;
    pParams->frameInfo[VPP_OUT].CropY = 0;
    pParams->frameInfo[VPP_OUT].CropW = NOT_INIT_VALUE;
    pParams->frameInfo[VPP_OUT].CropH = NOT_INIT_VALUE;
    for (mfxU8 i = 1; i < nArgNum; i++ ) 
    {
        MSDK_CHECK_POINTER(strInput[i], MFX_ERR_NULL_PTR);
        {      
            if ( 0 == _tcscmp(strInput[i], _T("-sw")) ) 
            {          
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].nWidth);
            }
            else if (0 == _tcscmp(strInput[i], _T("-sh")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].nHeight);
            }
            else if (0 == _tcscmp(strInput[i], _T("-scrX")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].CropX);
            }
            else if (0 == _tcscmp(strInput[i], _T("-scrY")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].CropY);
            }
            else if (0 == _tcscmp(strInput[i], _T("-scrW")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].CropW);
            }
            else if (0 == _tcscmp(strInput[i], _T("-scrH")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].CropH);
            } 
            else if(0 == _tcscmp(strInput[i], _T("-spic")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_IN].PicStruct);
                pParams->frameInfo[VPP_IN].PicStruct = GetPicStruct(pParams->frameInfo[VPP_IN].PicStruct);
            }
            else if(0 == _tcscmp(strInput[i], _T("-sf")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->frameInfo[VPP_IN].dFrameRate);          
            }
            else if (0 == _tcscmp(strInput[i], _T("-dw")) ) 
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].nWidth);         
            } 
            else if (0 == _tcscmp(strInput[i], _T("-dh")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].nHeight);
            }
            else if (0 == _tcscmp(strInput[i], _T("-dcrX")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].CropX);
            }
            else if (0 == _tcscmp(strInput[i], _T("-dcrY")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].CropY);
            }
            else if (0 == _tcscmp(strInput[i], _T("-dcrW")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].CropW);
            }
            else if (0 == _tcscmp(strInput[i], _T("-dcrH")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].CropH);
            }
            else if(0 == _tcscmp(strInput[i], _T("-dpic")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%hd"), &pParams->frameInfo[VPP_OUT].PicStruct);
                pParams->frameInfo[VPP_OUT].PicStruct = GetPicStruct(pParams->frameInfo[VPP_OUT].PicStruct);
            }
            else if(0 == _tcscmp(strInput[i], _T("-df")))
            {
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->frameInfo[VPP_OUT].dFrameRate);          
            }
            //-----------------------------------------------------------------------------------
            //                   Video Enhancement Algorithms
            //-----------------------------------------------------------------------------------
            else if (0 == _tcscmp(strInput[i], _T("-denoise")))
            {
                pParams->denoiseParam.mode = VPP_FILTER_ENABLED_DEFAULT;

                if( i+1 < nArgNum )
                {                
                    ioStatus = _stscanf_s(strInput[i+1], _T("%hd"), &readData);                
                    if ( ioStatus > 0 )
                    {
                        pParams->denoiseParam.factor = (mfxU16)readData;
                        pParams->denoiseParam.mode   = VPP_FILTER_ENABLED_CONFIGURED;
                        i++;
                    }
                }
            }
            else if (0 == _tcscmp(strInput[i], _T("-detail")))
            {
                pParams->detailParam.mode = VPP_FILTER_ENABLED_DEFAULT;

                if( i+1 < nArgNum )
                {
                    ioStatus = _stscanf_s(strInput[i+1], _T("%hd"), &readData);
                    if ( ioStatus > 0 )
                    {
                        pParams->detailParam.factor = (mfxU16)readData;
                        pParams->detailParam.mode   = VPP_FILTER_ENABLED_CONFIGURED;
                        i++;
                    }
                }
            }
            else if (0 == _tcscmp(strInput[i], _T("-vanalysis")))
            {
                pParams->vaParam.mode = VPP_FILTER_ENABLED_DEFAULT;                        
            }
            else if (0 == _tcscmp(strInput[i], _T("-pa_hue")))
            {
                pParams->procampParam.mode = VPP_FILTER_ENABLED_CONFIGURED;
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->procampParam.hue);    
            }
            else if (0 == _tcscmp(strInput[i], _T("-pa_bri")))
            {
                pParams->procampParam.mode = VPP_FILTER_ENABLED_CONFIGURED;
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->procampParam.brightness);    
            }
            else if (0 == _tcscmp(strInput[i], _T("-pa_con")))
            {
                pParams->procampParam.mode = VPP_FILTER_ENABLED_CONFIGURED;
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->procampParam.contrast);    
            }
            else if (0 == _tcscmp(strInput[i], _T("-pa_sat")))
            {
                pParams->procampParam.mode = VPP_FILTER_ENABLED_CONFIGURED;
                i++;
                _stscanf_s(strInput[i], _T("%lf"), &pParams->procampParam.saturation);    
            }
            //-----------------------------------------------------------------------------------
            else if (0 == _tcscmp(strInput[i], _T("-i")))
            {
                i++;
                _tcscpy_s(pParams->strSrcFile, strInput[i]);
            }        
            else if (0 == _tcscmp(strInput[i], _T("-o")))
            {
                i++;
                _tcscpy_s(pParams->strDstFile, strInput[i]);        
            }
            else if (0 == _tcscmp(strInput[i], _T("-scc")))
            {
                i++;
                pParams->frameInfo[VPP_IN].FourCC = Str2FourCC( strInput[i] );
            }
            else if (0 == _tcscmp(strInput[i], _T("-dcc")))
            {
                i++;
                pParams->frameInfo[VPP_OUT].FourCC = Str2FourCC( strInput[i] );
            }
            else if (0 == _tcscmp(strInput[i], _T("-n")) )
            {
                i++;
                ioStatus = _stscanf_s(strInput[i], _T("%d"), &pParams->requestedFramesCount);
            }
            else if (0 == _tcscmp(strInput[i], _T("-lib")) )
            {                
                i++;
                if (0 == _tcscmp(strInput[i], _T("sw")) )
                {
                    pParams->impLib = MFX_IMPL_SOFTWARE;
                }
                else if (0 == _tcscmp(strInput[i], _T("hw")) )
                {
                    pParams->impLib = MFX_IMPL_HARDWARE;
                }

            }
#ifdef D3D_SURFACES_SUPPORT
            else if( 0 == _tcscmp(strInput[i], _T("-d3d")) )
            {
                pParams->bd3dAlloc = true;
            }
#endif
        }
    }
    if (NOT_INIT_VALUE == pParams->frameInfo[VPP_IN].CropW)
    {
        pParams->frameInfo[VPP_IN].CropW = pParams->frameInfo[VPP_IN].nWidth;
    }

    if (NOT_INIT_VALUE == pParams->frameInfo[VPP_IN].CropH)
    {
        pParams->frameInfo[VPP_IN].CropH = pParams->frameInfo[VPP_IN].nHeight;
    }

    if (NOT_INIT_VALUE == pParams->frameInfo[VPP_OUT].CropW)
    {
        pParams->frameInfo[VPP_OUT].CropW = pParams->frameInfo[VPP_OUT].nWidth;
    }

    if (NOT_INIT_VALUE == pParams->frameInfo[VPP_OUT].CropH)
    {
        pParams->frameInfo[VPP_OUT].CropH = pParams->frameInfo[VPP_OUT].nHeight;
    }

    if (0 == _tclen(pParams->strSrcFile)) 
    {
        vppPrintHelp(strInput[0], _T("Source file name not found"));
        return MFX_ERR_UNSUPPORTED;
    };

    if (0 == _tclen(pParams->strDstFile)) 
    {
        vppPrintHelp(strInput[0], _T("Destination file name not found"));
        return MFX_ERR_UNSUPPORTED;
    };

    return MFX_ERR_NONE;

} // mfxStatus vppParseInputString(...)

/* EOF */
