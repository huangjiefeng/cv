#ifndef TERMINAL_DESCRIPTION_H
#define TERMINAL_DESCRIPTION_H

#include <memory>

#include <QObject>
#include <QString>
#include <QHash>
#include <QList>

#include <cassert>

struct TerminalDescription {

  // 不比较total_terminal_num
  bool IsSameWithoutTotalNumCompared(const TerminalDescription &terminals) const;
  
  // TODO: 将终端总数分离出来
  int total_terminal_num; // 会议的终端总数
  QString dns;  // 域名服务器

  uint16_t id; // 终端id，用于发言权控制
  QString	uri; // 终端uri
  QString  virtual_uri; // 终端存在多摄像头时的虚拟终端号，格式类似v1#{uri}，从v1开始
  int virtual_index; // 数字表示的虚拟终端索引，从0开始
  int virtual_count; // 该终端总的虚拟终端数
  QString name;	// 终端名称
  QString location; // 终端位置
  QString model; // 工作模式
  QString version; // 终端版本
 
  QString multicast_address; // 组播会议的IP地址，单播会议则是0.0.0.0

  QString sdp; // sdp
  bool has_video;
  bool has_audio;
  int video_port; // 视频数据传输的本地端口，默认值为0
  int audio_port; // 音频数据传输的本地端口，默认值为0
  int small_video_port; // 视频小流的传输端口，默认值为0

  bool is_available; // 终端状态，是否进入到会议中
  bool is_speaking; // 是否允许发言
  bool is_handup; // 是否举手
  bool is_main_speaker; // 是否是主讲人
  bool is_chairman_terminal; // 是否是主席终端
};

typedef std::shared_ptr<TerminalDescription> TerminalPointer;
// TODO: 让conference controller的signal传输的是ConstTerminalPointer
typedef std::shared_ptr<const TerminalDescription> ConstTerminalPointer;

// virtual terminal list
typedef QList<TerminalPointer> TerminalList;
// key: vuri, value: terminal
typedef QHash<QString, TerminalPointer> TerminalDict;
// key: uri, value: terminal list
typedef QHash<QString, TerminalList> TerminalListDict;

class TerminalHelper {
public:
  static void RegisterMetaType() {
    qRegisterMetaType<TerminalDescription>("TerminalDescription");
    qRegisterMetaType<TerminalPointer>("TerminalPointer");
    qRegisterMetaType<ConstTerminalPointer>("ConstTerminalPointer");
    qRegisterMetaType<TerminalList>("TerminalList");
  }

  static QString ConstructDefaultVirtualURI(
      const QString &uri, int virtual_index = 0) {
    return QString("v%1#%2").arg(virtual_index + 1).arg(uri);
  }

  static bool IsVirtualURI(const QString uri_or_vuri) {
    return uri_or_vuri.contains('#');
  }

  static QString GetTerminalURI(const QString &uri_or_vuri) {
    return uri_or_vuri.mid(uri_or_vuri.indexOf('#') + 1);
  }

  static QString GetRealm(const QString &uri_or_vuri) {
    int at_index = uri_or_vuri.indexOf('@');
    if (at_index == -1) {
      return QString();
    }
    return uri_or_vuri.mid(at_index + 1);
  }

  static QString GetUsername(const QString &uri_or_vuri) {
    return uri_or_vuri.mid(0, uri_or_vuri.indexOf('@'));
  }

  static int GetVirtualIndex(const QString &vuri_or_user_id) {
    int virtual_index = 0;
    int sharp_pos = vuri_or_user_id.indexOf("#");
    if (sharp_pos > 1) {
      QString index_str = vuri_or_user_id.mid(1, sharp_pos - 1);
      virtual_index = index_str.toInt() - 1;
    }
    return virtual_index;
  }
  
  static void GetMediaInfoFromSDP(
    const QString &sdp, 
    int &video_width,
    int &video_height,
    int &video_fps,
    QString &audio_codec,
    int &audio_rate, 
    int &audio_bits,
    int &audio_channel);

  static bool IsModelHD(const QString &model) {
    return model.contains("HD");
  }

  static bool IsModelTX(const QString &model) {
    return model.contains("TX");
  }
};

#endif // TERMINAL_DESCRIPTION_H