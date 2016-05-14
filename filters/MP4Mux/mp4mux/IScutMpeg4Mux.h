/************************************************************************/
/* Defined by Liaokz. 2013-5-5                                          */
/************************************************************************/

#ifndef ISCUT_MPEG4_MUX_H
#define ISCUT_MPEG4_MUX_H

#include "RecordStat.h"


// {DE58BFED-53EB-494a-85F4-4C0020B92D86}
DEFINE_GUID(IID_ISCUTMpeg4Mux, 
			0xde58bfed, 0x53eb, 0x494a, 0x85, 0xf4, 0x4c, 0x0, 0x20, 0xb9, 0x2d, 0x86);


MIDL_INTERFACE("DE58BFED-53EB-494a-85F4-4C0020B92D86")
ISCUTMpeg4Mux : public IUnknown
{
	virtual HRESULT SetCallback(__in void * obj, __in RecordStatCallback func);

    virtual HRESULT SetInitialJobId(__in long nJobId) = 0;

    // NO more than one %d could be used for episode num. e.g. _T("Video%03d.mp4")
	virtual HRESULT SetFileName(__in LPCTSTR szName) = 0;

    // Set szName to NULL to retrieve the string size required
	virtual HRESULT GetFileName(__out_opt LPTSTR szName, __inout UINT &nLength) = 0;

    // when called, file will be cut at once and new file referred by szFileName created
    // nJobId should never repeat!
	virtual HRESULT CutMovieFile(__in long nJobId, __in_opt LPCTSTR szNewFileName) = 0; 
};

#endif
