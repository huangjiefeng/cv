
#pragma once

#include <string>
#include <windows.h>
#include <QtCore>

std::wstring str2wstr(const std::string& str);
std::string wstr2str(const std::wstring& wstr);

BOOL PathExist(std::string strRawPath);	// 输入相对路径或绝对路径
BOOL ParentDirExist(const std::string& strFilePath);	// 判断起始到最后一个'\\'位置的路径是否存在
BOOL MakeDirRecursive(std::string strDirPath);	// 以'\\'结尾的文件夹相对或绝对路径
BOOL MakeParentDir(const std::string& strFilePath);	// 创建从起始到最后一个'\\'位置的路径
BOOL GetApplicationDirPath(char * applicationDirPath= NULL, char * applicationName= NULL);	// 获得以'\\'结束的当前路径
std::string GetAbsolutePath(const std::string& strRelative);	// 输入相对路径，返回绝对路径
QString GetAbsolutePath(const QString &strRelative);
std::string GetTempFilePath(const std::string& strPath); // 输入文件相对或绝对路径，返回其的临时文件路径
QString GetRelativePath(const QString &prefix, QString path);
QString GetDBFilePath(const QString &path); //输入绝对路径，返回数据库中存储的相对路径格式
time_t SystemTimeToTimet( const SYSTEMTIME &st );

template<typename T>
inline QByteArray DBusParamToByteArray(T &param)
{
  QByteArray bytes;
  QDataStream out(&bytes , QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_4);
  out << param;
  return bytes;
}

template<typename T>
inline T DBusParamFromByteArray(const QByteArray &bytes)
{
  T param;
  QDataStream in(bytes);
  in.setVersion(QDataStream::Qt_4_4);
  in >> param;
  return param;
}
