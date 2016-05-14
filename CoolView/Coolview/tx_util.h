#ifndef TX_UTIL_H
#define TX_UTIL_H

#include <QString>

namespace tx_util {

extern const char *PathPlaceholderDate; //文件名日期（yy-MM-dd）占位符
extern const char *PathPlaceholderDateTime; //文件名日期时间（yyMMddhhmmss）占位符
extern const char *PathPlaceholderVInfo; //文件名视频格式占位符
extern const char *PathPlaceholderCamIndex; //文件名视频输入索引号占位符
extern const char *PathPlaceholderConf; //文件名会议名称占位符
extern const char *PathPlaceholderConfDate; //文件名会议开始日期占位符
extern const char *PathPlaceholderTerminal; //文件名终端名称占位符
extern const char *PathPlaceholderTitle; //文件名视频集标题占位符
extern const char *PathPlaceholderParticipant; //文件名与会者占位符
extern const char *PathPlaceholderKeywords; //文件名关键词占位符
extern const char *PathPlaceholderDescription; //文件名描述占位符（不要轻易使用，描述可能很长）


struct FileNameParam {
  int camid; // 实例化PathPlaceholderCamIndex
  QString conf_date;
  QString conf;
  QString terminal;
  QString title;
  QString participant;
  QString keywords;
  QString description;

  FileNameParam();
};
QString GetRecordFileNamePath(const QString &path_with_placeholder, 
                              const FileNameParam &param);

//获得录像文件根目录
QString TXGetRecordRootDir();

//获得绝对不重复的高精度时间戳，返回1970-1-1 00:00 UTC以来的微秒数
long long GetNonrepeatableTimestamp();

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

}

#endif
