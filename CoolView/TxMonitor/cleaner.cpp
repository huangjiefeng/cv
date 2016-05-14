#include <QCoreApplication>

#include <String>

#include "windows.h"

#include "cleaner.h"
#include "Util.h"
#include "log.h"
#include "FileScanner.h"
#include "record_sqlite_db.h"
#include "util\ini\TxConfig.h"
#include "dbus\txMonitor\common\TxMonitorServiceCommon.h"


Cleaner::Cleaner()
  : BaseLoopJob("Cleaner", 10000, NULL)
{

}

Cleaner::~Cleaner()
{

}

void Cleaner::Init()
{
  BaseLoopJob::Init();

  _lowSpaceThreshold = CTxConfig::getInstance().GetClnLowSpace();
  _highSpaceThreshold = CTxConfig::getInstance().GetClnHighSpace();

  _recPath = GetAbsolutePath(
    QString::fromLocal8Bit(CTxConfig::getInstance().GetRecPath()));
  _uploadPath = GetAbsolutePath(
    QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadPath()));
  _uploadMark = QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadMark());
  _uploadDirMark = QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadDirMark());
  _appPath = QCoreApplication::applicationDirPath();

  MakeDirRecursive(_recPath.toLocal8Bit().constData()); //创建路径，免得获取容量时出错
  MakeDirRecursive(_uploadPath.toLocal8Bit().constData());

  connect(this, &BaseLoopJob::StartJobSignal,
    this, &Cleaner::RunOnceAtStart);

  db_.reset(new RecordSqliteDB);
}

void Cleaner::RunOnceAtStart()
{
  // 清理所有之前可能异常关机留下的临时文件
  LOG_PRINTF_EX(_logger, "Deleting all .tmp files...\n");
  DeleteFiles(_appPath, ".tmp", "", false);
  DeleteEmptyFolders();
}

void Cleaner::Run()
{
  // 清理MP4优化工具产生的临时文件 - 不用了
  /*LOG_PRINTF_EX(_logger, "Deleting .tmp files...\n");
  DeleteFiles(_appPath, ".tmp", "", 0, false);*/

  if (GetPathFreeSpace(_recPath) < _lowSpaceThreshold)
  {
    LOG_INFO("Disk space low. Deleting files...");
    DeleteFiles(_uploadPath, _uploadMark, "", _highSpaceThreshold);

    if (GetPathFreeSpace(_recPath) >= _highSpaceThreshold)
    {
      return;
    }
    DeleteFiles(_uploadPath, "", "", _highSpaceThreshold);

    if (GetPathFreeSpace(_recPath) >= _highSpaceThreshold)
    {
      return;
    }
    DeleteFiles(_recPath, "", ".tmp", _highSpaceThreshold);
  }

}

void Cleaner::DeleteFiles( QString path, QString mark, QString exclude, ULONGLONG size /*= 0*/, bool recursive /*= true*/ )
{
  std::string absPath = GetAbsolutePath(std::string(qPrintable(path)));
  if (!PathExist(absPath))
  {
    return;
  }

  FileInfoQueue queue;

  FileScanner::ScanFiles(queue, absPath.c_str(), mark.split('|'), exclude.split('|'), recursive);

  ULARGE_INTEGER nFreeSpaceAvailable = {0};
  GetDiskFreeSpaceEx(absPath.c_str(), NULL, NULL, &nFreeSpaceAvailable);

  while (!queue.empty() && (size == 0 || nFreeSpaceAvailable.QuadPart < size))
  {
    QFileInfo info = queue.top();
    queue.pop();

    // do not delete dir upload mark
    QString name = info.fileName();
    if (name == _uploadDirMark)
    {
      continue;
    }

    QString path = info.absoluteFilePath();
    LOG_PRINTF_EX(_logger, "Delete file %s\n", path.toLocal8Bit().data());
    if (!QFile::remove(path))
    {
      LOG_ERROR_EX(_logger, "Delete file %s failed.", path.toLocal8Bit().data());
    }
    else
    {
      SetFileDeletedFlag(path);
    }
    GetDiskFreeSpaceEx(absPath.c_str(), NULL, NULL, &nFreeSpaceAvailable);
  }
}

ULONGLONG Cleaner::GetPathFreeSpace( QString path )
{
  std::string absPath = GetAbsolutePath(std::string(qPrintable(path)));
  if (!PathExist(absPath))
  {
    LOG_ERROR_EX(_logger, "GetFreeSpace of invalid path: %s", absPath.c_str());
    return 0;
  }

  ULARGE_INTEGER nFreeSpaceAvailable;
  GetDiskFreeSpaceEx(absPath.c_str(), NULL, NULL, &nFreeSpaceAvailable);

  LOG_PRINTF_EX(_logger, "Free space of %s : %.2fGB\n", 
    absPath.c_str(), nFreeSpaceAvailable.QuadPart/1000000000.0);

  emit NotifyFreeDiskSpaceChangeSignal(nFreeSpaceAvailable.QuadPart);
  return nFreeSpaceAvailable.QuadPart;
}

void Cleaner::SetFileDeletedFlag( QString file )
{
  if (!db_) {
    return;
  }
  db_->UpdateViewSetFlagsByPath(file, RecFileStat_Deleted);
}

void Cleaner::DeleteEmptyFolders()
{
  FileScanner::RemoveEmptyDirs(GetAbsolutePath(_recPath), 
    QStringList() << _uploadDirMark);
  FileScanner::RemoveEmptyDirs(GetAbsolutePath(_uploadPath), 
    QStringList() << _uploadDirMark);
}

