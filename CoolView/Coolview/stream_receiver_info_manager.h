#ifndef STREAM_RECEIVER_INFO_MANAGER_H
#define STREAM_RECEIVER_INFO_MANAGER_H

#include <QString>
#include <QList>
#include <QHash>

struct StreamReceiverInfo {
  StreamReceiverInfo()
      : video_port(0),
        audio_port(0),
        screen_port(0),
        is_small_video(false),
        local_virtual_index(0) {
  }

  QString recv_uri; // 接收者的uri
  QString recv_ip;  // 接收者的ip
  int video_port;   // 视频数据传输端口
  int audio_port;   // 音频数据传输端口
  bool is_small_video; // 是否小流
  int local_virtual_index; // 本地发送进程的虚拟终端索引
  int screen_port;  // 屏幕共享端口
};

class StreamReceiverInfoManager {

public:
  StreamReceiverInfoManager() {}
  ~StreamReceiverInfoManager() { Clear(); }

  void AddRemoteReceiver(const StreamReceiverInfo &info);

  void RemoveReceiverByURI(const QString &recv_uri, int local_virtual_index);

  void RemoveReceiverByIP(const QString &recv_ip, int local_virtual_index);

  StreamReceiverInfo* FindRemoteReceiverByURI(
    const QString &recv_uri, 
    int local_virtual_index);

  StreamReceiverInfo* FindRemoteReceiverByIP(
    const QString &ip,
    int local_virtual_index);

  int GetReceiverCount() const;

  void Clear();

  typedef QList<StreamReceiverInfo> ReceiverList;
  ReceiverList ToList() const;

private:
  typedef QHash<int, StreamReceiverInfo*> VIndexReceiversDict;
  typedef QHash<QString, VIndexReceiversDict> URIReceiversDict;

  URIReceiversDict dict_;
};

#endif // STREAM_RECEIVER_INFO_MANAGER_H
