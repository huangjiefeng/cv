#ifndef ITYPE_HANDLER_H
#define ITYPE_HANDLER_H

#include <streams.h>

class ITypeHandler
{
public:
    virtual ~ITypeHandler() {}

    virtual HRESULT CheckMediaType(const CMediaType &) = 0;
    virtual HRESULT CalcSampleSize(const CMediaType &, long &) = 0;
    virtual bool IsVideo() = 0;
    virtual bool IsAudio() = 0;
};

#endif
