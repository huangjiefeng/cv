#include "FileScanner.h"

#include <assert.h>

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QQueue>
#include <QStack>

// Definition of FileInfoComparer

bool FileInfoComparer::operator()( QFileInfo a, QFileInfo b )
{
	return a.created() > b.created();
}


// Definiton of FileScanner
FileScanner::FileScanner(void)
{
}

FileScanner::~FileScanner(void)
{
}

void FileScanner::ScanFiles( FileInfoQueue &queue, const QString &path, 
                            QStringList mark, QStringList exclude, bool recursive /*= true*/ )
{
  QStack<QString> pathList;
  pathList.push_back(path);

  while (!pathList.isEmpty())
  {
    QString curDirPath = pathList.back();
    pathList.pop_back();

    QDir dir(curDirPath);
    if (!dir.exists()) continue;

    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
      QFileInfo fileInfo = list.at(i);
      // ignore sys dir and uploaded mark
      /*if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
      {
        continue;
      }*/
      // Sub Dir
      if(fileInfo.isDir())
      {
        if (recursive)
          pathList.push_back(fileInfo.filePath());
      }
      // Files
      else
      {
        QString path = fileInfo.absoluteFilePath();
        path.replace("/", "\\");
        // check mark
        int i = 0;
        for (i = 0; i < mark.size(); ++i)
        {
          if (path.indexOf(mark.at(i)) != -1)
          {
            break;
          }
        }
        if (i == mark.size()) // file do not contain mark, ignore
        {
          continue;
        }
        // check exclude
        for (i = 0; i < exclude.size(); ++i)
        {
          if (exclude.at(i).length() != 0 && path.indexOf(exclude.at(i)) != -1)
          {
            break;
          }
        }
        if (i < exclude.size()) // file contain exclude mark, ignore
        {
          continue;
        }
        // otherwise, put into queue
        queue.push(fileInfo);
      }
    }
  }
}

bool FileScanner::RemoveEmptyDirs(const QString &dir_path, 
                           const QStringList &ignore)
{
  const static QDir::Filters dir_filter = 
    QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot;
  QDir cur_dir(dir_path);
  if (!cur_dir.exists()) return true; //当空的处理

  cur_dir.setFilter(dir_filter);
  QFileInfoList sub_list = cur_dir.entryInfoList();

  bool is_empty = true;
  for (auto &sub_item : sub_list) {
    /*if(sub_item.fileName() == "." || sub_item.fileName() == "..")
      continue;*/
    if (sub_item.isDir()) {
      bool sub_dir_empty = RemoveEmptyDirs(sub_item.filePath(), ignore);
      if (sub_dir_empty) {
        //子目录没有有意义的文件了，清空所有被忽略的文件，并删除文件夹
        QDir sub_dir(sub_item.filePath());
        sub_dir.setFilter(dir_filter);
        QFileInfoList sub_sub_list = sub_dir.entryInfoList();
        for (auto &sub_sub_item : sub_sub_list) {
          //if(sub_sub_item.fileName() != "." && sub_sub_item.fileName() != "..")
          sub_dir.remove(sub_sub_item.fileName());
        }
        cur_dir.rmdir(sub_item.fileName());
      } else {
        is_empty = false;
      }
    }
    else if (sub_item.isFile()) {
      is_empty = is_empty && ignore.contains(sub_item.fileName(), Qt::CaseInsensitive);
    }
  }
  return is_empty;
}
