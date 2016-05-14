#ifndef CLEANER_H
#define CLEANER_H

#include "baseloopjob.h"

#include <QString>
#include <memory>

#ifndef ULONGLONG
#define ULONGLONG unsigned __int64
#endif

class IRecordDB;

//用于检测剩余磁盘空间，不足时删除旧文件
class Cleaner : public BaseLoopJob
{
  Q_OBJECT

public:
  Cleaner();
  ~Cleaner();

  void Init();

signals:
  void NotifyFreeDiskSpaceChangeSignal(quint64 space);

protected:
  void RunOnceAtStart(); // 启动时执行一次的任务
  void Run();

  ULONGLONG GetPathFreeSpace(QString path);

  // DeleteFiles
  void DeleteFiles(QString path, QString mark, QString exclude, 
    ULONGLONG size = 0, bool recursive = true);
  void SetFileDeletedFlag(QString file);
  void DeleteEmptyFolders();

private:

  ULONGLONG _lowSpaceThreshold;    // 低容量阈值(Byte),低于此值开始删除文件
  ULONGLONG _highSpaceThreshold;    // 自由容量阈值(Byte),高于此值可停止删除文件

  QString _recPath;
  QString _uploadPath;
  QString _uploadMark;
  QString _uploadDirMark;
  QString _appPath;
  
  std::shared_ptr<IRecordDB> db_;
};

#endif // CLEANER_H
