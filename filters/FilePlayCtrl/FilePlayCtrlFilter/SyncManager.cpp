#pragma warning(disable : 4995)
#include "SyncManager.h"
#include "stdio.h"


SyncManager::SyncManager(void)
: lock_(NULL)
, shared_mem_handle_(NULL)
, shared_mem_view_(nullptr)
, data_(nullptr)
{
    memset(lock_name_, 0, sizeof(lock_name_));
    memset(shared_mem_name_, 0, sizeof(shared_mem_name_));
}

SyncManager::~SyncManager(void)
{
    Reset();
}

int SyncManager::Init(const char *id)
{
    Reset();

    // Init names
    sprintf_s(lock_name_, sizeof(lock_name_), "%s_lock", id);
    sprintf_s(shared_mem_name_, sizeof(shared_mem_name_), "%s_mem", id);

    // Mutex
    lock_ = CreateMutex(NULL, FALSE, lock_name_);
    if (NULL == lock_) return -1;

    // Shared memory
    shared_mem_handle_ = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, 
        PAGE_READWRITE, 0, sizeof(SharedData), shared_mem_name_);
    if (NULL == shared_mem_handle_) return -1;

    const DWORD dwCreateRs = GetLastError();

    shared_mem_view_ = (BYTE *)MapViewOfFile(shared_mem_handle_, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (NULL == shared_mem_view_) return -1;
    data_ = (SharedData *)shared_mem_view_;

    // Init when first create shared memory
    if (ERROR_ALREADY_EXISTS != dwCreateRs)
    {
        data_->version = -1;
        data_->start_time = 0;
        data_->base_stream_time = 0;
        data_->modifier_id = 0;
    }

    return ERROR_ALREADY_EXISTS != dwCreateRs ? 0 : 1;
}

void SyncManager::Reset()
{
    if (shared_mem_view_)
    {
        UnmapViewOfFile(shared_mem_view_);
        shared_mem_view_ = nullptr;
    }

    if (shared_mem_handle_)
    {
        CloseHandle(shared_mem_handle_);
        shared_mem_handle_ = NULL;
    }

    if (lock_)
    {
        CloseHandle(lock_);
        lock_ = NULL;
    }
}

int SyncManager::Lock()
{
    // Ensure exclusive access to the SharedLockData variables   
    DWORD dwWaitResult;
    dwWaitResult = WaitForSingleObject(lock_, INFINITE);
    if (WAIT_OBJECT_0 != dwWaitResult) return -1;
    return 0;
}

void SyncManager::Unlock()
{
    ReleaseMutex(lock_);
}
