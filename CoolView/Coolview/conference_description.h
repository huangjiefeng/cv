#ifndef CONFERENCE_DESCRIPTION_H
#define CONFERENCE_DESCRIPTION_H

#include <memory>

#include <QString>
#include <QHash>
#include <QList>

struct ConferenceDescription {
  QString cid;         //会议的id，全局唯一
  QString uri;
  QString title;
  QString description;
  QString host_uri;		//主持人URI
  QString control_mode;	//会议模式：discuss或report
  QString join_mode;

  // TODO: change QString to char[]
  QString join_password;

  QString start_time;
  QString duration;

  // 如果会议是组播会议，则其为组播地址，否则为0.0.0.0  
  // 由于当前控制服务器发给终端的会议列表没有该地址信息，
  // 所以multicast_address字段可能为空，使用时需注意。
  QString multicast_address;	
  QString level; //会议级别
  QString chairman;	//主席账号
  int ppt_port;	//用于屏幕共享

  bool use_FCS; // 是否使用发言权控制
};

typedef std::shared_ptr<ConferenceDescription> ConferencePointer;
typedef QHash<QString, ConferencePointer> ConferenceDictionary;
typedef QList<ConferencePointer> ConferenceList;

typedef std::shared_ptr<const ConferenceDescription> ConstConferencePointer;
typedef QList<ConstConferencePointer> ConstConferenceList;
typedef QHash<QString, ConstConferencePointer> ConstConferenceDictionary;

class ConferenceHelper {
public:
  static void RegisterMetaType() {
    qRegisterMetaType<ConferenceDescription>("ConferenceDescription");
    qRegisterMetaType<ConferencePointer>("ConferencePointer");
    qRegisterMetaType<ConstConferencePointer>("ConstConferencePointer");
    qRegisterMetaType<ConferenceList>("ConferenceList");
    qRegisterMetaType<ConstConferenceList>("ConstConferenceList");
  }

  static ConferencePointer GetVodPlayVirtualConference() {
    ConferencePointer vod_play_virtual_conference = ConferencePointer(new ConferenceDescription);
    vod_play_virtual_conference->cid = "2147483647"; //2^31-1，这个总该不会和服务器数据库中的值冲突了
    vod_play_virtual_conference->uri = "vod0@local"; //避免使用focus0，以免和真实会议冲突
    vod_play_virtual_conference->title = QString::fromLocal8Bit("文件点播");
    vod_play_virtual_conference->description = QString::fromLocal8Bit("虚拟会议，用于回看已录制的视频");
    vod_play_virtual_conference->use_FCS = false;
    return vod_play_virtual_conference;
  }

  static bool IsVirtualConference(const QString &uri) {
      return !uri.startsWith("focus");
  }

  static bool IsVodVirtualConference(const QString &uri) {
      return uri.startsWith("vod");
  }
};

#endif // CONFERENCE_DESCRIPTION_H