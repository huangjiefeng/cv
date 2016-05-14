#include "BackupManager.h"
#include "log/log.h"

//
//用于备份的共享内存结构为：
// ┌──┬──────┬──────┬──────┬─────
// │int │ BlockData  │ BlockData  │ BlockData  │ ......
// └──┴──────┴──────┴──────┴─────
//其中int表示后面BlockData数量。
//
//每一个BlockData为：
// ┌──┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ id │ consistent │    type    │ data1_size │   data1    │ data2_size │   data2    │
// └──┴──────┴──────┴──────┴──────┴──────┴──────┘
//其中data为备份的数据，解释方式由使用者自行决定，可支持备份两块数据
//

#define MAX_DATA_PER_BLOCK 2

struct BlockData
{
    bool consistent; //表示当前数据是否满足一致性
    char id[32];
    char type[128];
    struct Data
    {
        int size;
        char data[2048];
    } datas[MAX_DATA_PER_BLOCK];
};

#define DATA_BLOCK_SIZE sizeof(BlockData) //为每个Controller准备的数据备份区大小
#define MAX_BLOCK_COUNT 512 //最大支持的备份数
#define BACKUP_SHAREDMEM_NAME "CvChannelBackup" //用于备份的共享内存名称


BackupManagerPtr BackupManager::instance_;


BackupManager::BackupManager(void)
    : count_(0)
    , data_(NULL)
{
}

BackupManager::~BackupManager(void)
{
    shared_mem_.detach();
    count_ = 0;
    data_ = NULL;
}

BackupManagerPtr BackupManager::Instance()
{
    //该方法会在多线程中调用,但必定会在主线程中首先调用,故不必加锁保护
    if (!instance_)
    {
        instance_.reset(new BackupManager());
        instance_->Init();
    }
    return instance_;
}

void BackupManager::Init()
{
    shared_mem_.setKey(BACKUP_SHAREDMEM_NAME);
    if (!shared_mem_.attach(QSharedMemory::ReadWrite))
    {
        //不存在备份共享内存,创建新的
        const int size = DATA_BLOCK_SIZE * MAX_BLOCK_COUNT + sizeof(int); //多分配一块int区域用于存储已备份的数据块数量
        bool ret = shared_mem_.create(size, QSharedMemory::ReadWrite);
        if (!ret)
        {
            LOG_ERROR("Create backup memory failed: %s", 
                shared_mem_.errorString().toLocal8Bit().data());
            return;
        }
        LOG_PRINTF("Create backup memory succeed.");
        char * ptr = static_cast<char *>(shared_mem_.data());
        memset(ptr, 0, size);
        count_ = reinterpret_cast<int *>(shared_mem_.data());
        data_ = reinterpret_cast<BlockData *>(count_ + 1); //跳过第一个整数内存区
    }
    else
    {
        //存在备份数据,重建索引
        LOG_INFO("Found backup memory created before.");
        count_ = reinterpret_cast<int *>(shared_mem_.data());
        data_ = reinterpret_cast<BlockData *>(count_ + 1); //跳过第一个整数内存区
        for (int i = 0; i < *count_; ++i)
        {
            BlockData * cur = data_ + i;
            QString id = cur->id;
            index_table_[id] = i;
        }
    }
}

int BackupManager::GetCount() const
{
    return index_table_.size();
}

BackupManager::IDList BackupManager::GetMediaIDList() const
{
    return index_table_.keys();
}

int BackupManager::Backup( QString id, QString type, int index, const QByteArray &data )
{
    if (!shared_mem_.isAttached() || !data_ || index >= MAX_DATA_PER_BLOCK)
    {
        return -1;
    }
    BlockData * cur = NULL;
    int block_index = 0;
    if (!index_table_.contains(id))
    {
        //由于每个controller在各自线程中串行执行,故其他地方访问各自数据区不必加锁
        //但修改公共的索引表和计数器时必须加锁,否则有可能当多个controller同时创建
        //备份时,拿到相同的索引
        QWriteLocker locker(&lock_); //防止忘记解锁
        //索引中无该项,则建立
        block_index = index_table_.size();
        if (block_index >= MAX_BLOCK_COUNT)
        {
            LOG_ERROR("Cannot backup more than %d blocks", MAX_BLOCK_COUNT);
            return -1;
        }
        //新的数据将插入备份列表末尾
        index_table_[id] = block_index;
        //更新索引
        ++(*count_);
        locker.unlock();

        //记录ID和类型
        cur = data_ + block_index;
        if (id.size() > sizeof(cur->id) - 1)
        {
            LOG_ERROR("Backup failed: id %s is too large!", id.toLocal8Bit().data());
            return -2;
        }
        if (type.size() > sizeof(cur->type) - 1)
        {
            LOG_ERROR("Backup failed: type %s is too large!", type.toLocal8Bit().data());
            return -2;
        }
        strcpy_s(cur->id, id.toLocal8Bit().data());
        strcpy_s(cur->type, type.toLocal8Bit().data());
    }
    else
    {
        //否则取出索引号
        QReadLocker locker(&lock_); //防止忘记解锁
        block_index = index_table_[id];
        cur = data_ + block_index;
    }

    BlockData::Data *d = &(cur->datas[index]);

    if (data.size() > sizeof(d->data) - 1)
    {
        LOG_ERROR("Backup failed: data of %d bytes is too large!", data.size());
        return -2;
    }

    cur->consistent = false; //Inconsistent
    memcpy_s(d->data, sizeof(d->data) - 1, data.data(), data.size());
    d->size = data.size();
    cur->consistent = true; //Consistent

    return 0;
}

int BackupManager::FetchType( QString id, QString &type )
{
    if (!shared_mem_.isAttached() || !data_)
    {
        return -1;
    }

    QReadLocker locker(&lock_); //方式忘记解锁
    if (!index_table_.contains(id))
    {
        //索引中无该项,则建立
        return -1;
    }
    const int index = index_table_[id];
    locker.unlock();

    const BlockData * cur = data_ + index;
    type = cur->type;

    return 0;
}

int BackupManager::FetchData( QString id, int index, QByteArray &data )
{
    if (!shared_mem_.isAttached() || !data_ || index >= MAX_DATA_PER_BLOCK)
    {
        return -1;
    }

    QReadLocker locker(&lock_); //方式忘记解锁
    if (!index_table_.contains(id))
    {
        //索引中无该项
        return -1;
    }
    const int block_index = index_table_[id];
    locker.unlock(); //减少持有锁的时间

    const BlockData * cur = data_ + block_index;
    if (!cur->consistent)
    {
        LOG_ERROR("Backup data of %s is inconsistent.", id.toLocal8Bit().data());
        return -2;
    }

    data = QByteArray(cur->datas[index].data, cur->datas[index].size);

    return 0;
}
