#ifndef FILEPLAY_CONTROLLER_H
#define FILEPLAY_CONTROLLER_H

#include "fileplay_controller_interface.h"

#include "conference_controller_interface.h"

class MediaManager;
class IMsgManager;
class VConfSipMsgReceiver;
class IConferenceController;
class ChannelDispatcherIf;
typedef struct _FilePlayStatItem FilePlayStatItem; // in FilePlayStat.h

class FilePlayController : public IFilePlayController {
  Q_OBJECT

 public:
  FilePlayController(QObject *parent = nullptr);
  ~FilePlayController();

  void Initialize(
    IConferenceController *conference_controller,
    MediaManager *media_manager,
    IMsgManager *msg_manager);

 public Q_SLOTS:
  virtual void HandleAddPlayListSlot(const QStringList &playlist);
  virtual void HandleClearPlayListsSlot();

  virtual void HandlePlaySlot();
  virtual void HandlePauseSlot();
  virtual void HandleStopSlot(); // not support so far
  virtual void HandleSeekSlot(unsigned long sec);

  virtual void HandleConferenceStateChangedSlot( 
    const QString &conference_uri, 
    IConferenceController::ConferenceState state);
  virtual void HandleReceiveUDPFilePlayReportSlot(const FilePlayStatItem &item);

 private:
  struct VirtualSenderInfo
  {
    QString uri; // 构造出的虚拟用户uri
    int virtual_index;
    int virtual_count;
    QString name; //方便阅读的终端名称
    QString media_id; // 底层媒体流进程标识
    QString file;
    long long start_time; //视频开始时的自然时间，1900-1-1以来的毫秒数
    long long start_delay; //视频起始延迟，ms
    long long duration; //ms
    unsigned short video_port;
    unsigned short audio_port;
    QString video_codec;
    long video_width;
    long video_height;
    long video_fps;
    QString audio_codec;
    unsigned long audio_sample_rate;
    unsigned long audio_channel;
    unsigned long audio_bit;
  };

  void MediaFormatInfoReceived(const FilePlayStatItem &);
  void ProgressInfoReceived(const FilePlayStatItem &);
  void CreateVirtualTerminal(const VirtualSenderInfo &);
  long long GetFileStartTime(const QString &file_path);

 private:
  IConferenceController *conference_controller_;
  VConfSipMsgReceiver *msg_receiver_;
  MediaManager *media_manager_;

  ChannelDispatcherIf *channel_dispatcher_;

  QString sync_id_;
  long long earliest_start_time_; // in ms
  long long total_duration_; //in ms
  unsigned long current_play_position_; //播放进度，ms
  qint64 last_seek_op_time_; // 最后一次定位操作的时间，ms
  unsigned int op_id_;

  typedef QHash<QString, VirtualSenderInfo> VirtualSenderInfoDict;
  VirtualSenderInfoDict virtual_sender_dict_;
};

#endif
