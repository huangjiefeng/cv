#ifndef UTIL_H
#define UTIL_H

#include "streams.h"

long long GetDShowTimeFromUnixTime(struct timeval *tv);
void GenerateRandomSyncId(char *buf, unsigned long len);

//仿照CAutoLock写的，作用是允许手动控制锁，并能在退出作用域时自动解锁
class CritSecHelper
{
    CritSecHelper(const CritSecHelper &) {}
    CritSecHelper *operator=(const CritSecHelper &) {}
protected:
    CCritSec * m_pLock;
    bool is_locked_;
public:
    CritSecHelper(CCritSec *pLock)
    {
        m_pLock = pLock;
        m_pLock->Lock();
        is_locked_ = true;
    }
    ~CritSecHelper()
    {
        if (is_locked_) m_pLock->Unlock();
    }
    void Lock()
    {
        if (!is_locked_) m_pLock->Lock();
        is_locked_ = true;
    }
    void Unlock()
    {
        if (is_locked_) m_pLock->Unlock();
        is_locked_ = false;
    }
};

#endif
