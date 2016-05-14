#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include "Windows.h"


class SyncManager
{
public:
    SyncManager(void);
    ~SyncManager(void);

    int Init(const char *id);
    void Reset();

    int Lock();
    void Unlock();

private:
    HANDLE lock_;
    char lock_name_[64];

    HANDLE shared_mem_handle_;
    char shared_mem_name_[64];
    BYTE *shared_mem_view_; // Ptr to mapped view of shared memory

public:
    struct SharedData
    {
        long version;
        long long start_time;
        long long base_stream_time;
        unsigned long modifier_id;
    } *data_;

};

#endif
