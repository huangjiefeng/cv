#pragma once

#include <QString>
#include <QByteArray>
#include <QMap>
#include <QList>
#include <QSharedMemory>
#include <QSharedPointer>
#include <QReadWriteLock>


class BackupManager;
typedef QSharedPointer<BackupManager> BackupManagerPtr;

class BackupManager
{
public:
    static BackupManagerPtr Instance();
    ~BackupManager(void);

    int GetCount() const; //获取备份的数据总数

    typedef QList<QString> IDList;
    IDList GetMediaIDList() const;

    int Backup(QString id, QString type, int index, const QByteArray &data);
    int FetchType(QString id, QString &type);
    int FetchData(QString id, int index, QByteArray &data);

private:
    BackupManager(void);

    void Init();

    static BackupManagerPtr instance_;

    typedef QMap<QString, int> DataIndex;
    DataIndex index_table_; //key:id, value:备份区在共享内存中的索引

    QReadWriteLock lock_; //使用读写锁以弥补共享内存自带锁的不足
    QSharedMemory shared_mem_;
    int * count_;
    struct BlockData * data_;
};

