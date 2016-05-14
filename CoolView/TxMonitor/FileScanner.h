#pragma once
#include <QFileInfo>
#include <QStringList>

#include <queue>
#include <vector>
#include <functional>

class FileInfoComparer
{
public:
	inline bool operator()(QFileInfo a, QFileInfo b);
};

typedef std::priority_queue<QFileInfo, std::vector<QFileInfo>, FileInfoComparer> FileInfoQueue;


class FileScanner
{
	FileScanner(void);
	~FileScanner(void);

public:
  // @brief 从指定路径中取出符合要求的文件，按时间先后顺序放入优先队列
  // @param queue 返回队列
  // @param path  路径
  // @param mark  过滤器，要提取的文件名或路径应包含这些标记，""表示接受所有文件，条件之间为or关系
  // @param exclude  过滤器，要排除的文件名或路径标记，""表示空过滤器，条件之间为or关系
  // @param recursive 是否递归查找
  // @remark exclude条件优先于mark，及一个文件如果既含有mark标记，同时又含有exclude标记，将被排除
	static void ScanFiles(FileInfoQueue &queue, const QString &path, 
    QStringList mark, QStringList exclude, bool recursive = true);

  static bool RemoveEmptyDirs(const QString &root, const QStringList &ignore);

};
