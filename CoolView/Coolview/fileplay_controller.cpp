#include "fileplay_controller.h"

#include "util\MediaManager.h"
#include "msg_manager_interface.h"
#include "vconf_sip_msg_receiver.h"
#include "conference_controller_interface.h"
#include "util/ini/CVIniConfig.h"
#include "util/ini/TxConfig.h"
#include "dbus/channelDispatcher/client/ChannelDispatcherIf.h"
#include "dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"

#include <QApplication>
#include <QFileDialog>
#include <time.h>

const unsigned short base_port = 20000;
const QString sync_id_prefix = "vod_sync_";
const QString fileplay_channel_type = QString(STREAMED_MEDIA_SERVICE_NAME);
const QString media_info_file_ext = ".minf";

//
// Util functions
//
void GenerateRandomSyncId( char *buf, unsigned long len )
{
    const char ch_set[] = "0123456789abcdef";
    const unsigned long ch_set_len = strlen(ch_set);
    const unsigned long rand_len = len;
    srand((unsigned int)time(NULL));
    for (unsigned long i = 0; i < rand_len; ++i) {
        buf[i] = ch_set[rand() % ch_set_len];
    }
    buf[len] = '\0'; // ensure space for tail '\0'
}

//
//members
//
FilePlayController::FilePlayController(QObject *parent /*= nullptr*/)
  : IFilePlayController(parent)
{
  conference_controller_ = nullptr;
  msg_receiver_ = nullptr;
  media_manager_ = nullptr;
  channel_dispatcher_ = nullptr;
  op_id_ = 1;
  total_duration_ = 0;
  current_play_position_ = 0;
  last_seek_op_time_ = 0;
}

FilePlayController::~FilePlayController()
{

}

void FilePlayController::Initialize( 
  IConferenceController *conference_controller,
  MediaManager *media_manager, 
  IMsgManager *msg_manager)
{
  assert(conference_controller != nullptr);
  conference_controller_ = conference_controller;
  connect(conference_controller_, &IConferenceController::NotifyConferenceStateChangedSignal,
    this, &FilePlayController::HandleConferenceStateChangedSlot, Qt::QueuedConnection);

  assert(msg_manager != nullptr);
  IMsgReceiver *receiver = msg_manager->GetMsgReceiver();
  msg_receiver_ = dynamic_cast<VConfSipMsgReceiver*>(receiver);
  if (msg_receiver_) {
    connect(this, &FilePlayController::NotifyVirtualRecvTerminalListSignal,
      msg_receiver_, &VConfSipMsgReceiver::HandleVRecvTerminalListSlot, Qt::QueuedConnection);
  }

  assert(media_manager != nullptr);
  media_manager_ = media_manager;

  channel_dispatcher_ = new ChannelDispatcherIf(
    CHANNEL_DISPATCHER_SERVICE_NAME, 
    CHANNEL_DISPATCHER_OBJECT_PATH,
    QDBusConnection::sessionBus());
}

void FilePlayController::HandleAddPlayListSlot(const QStringList &playlist)
{
  // generate sync_id
  char sync_id_suffix[6];
  GenerateRandomSyncId(sync_id_suffix, sizeof(sync_id_suffix)-1);
  sync_id_ = sync_id_prefix + sync_id_suffix;

  // reset variables
  op_id_ = 1;
  earliest_start_time_ = -1;
  total_duration_ = 0;
  current_play_position_ = 0;
  last_seek_op_time_ = 0;

  //按需求，同一个终端视频归为一个终端下的虚拟终端目前本地播放的解决办法是：
  //(1)约定文件名以下划线_分割不同部分;(2)第一部分为终端名;(3)virtual_index为_1_的形式;
  //(4)约定时间戳为ts开始的部分

  //先确定有几个虚拟终端，每个终端有几路视频，每终端最多不超过4路
  QHash<QString, QMap<int, QString>> terminal_to_file_table;
  for (unsigned int i = 0; i < playlist.size(); ++i)
  {
    //修正文件名
    QString file_path = playlist[i];
    file_path.replace("/", "\\");
    //获取文件名
    int name_begin_pos = file_path.lastIndexOf("\\") + 1;
    int name_end_pos = file_path.lastIndexOf(".");
    { 
      //文件名中含有下划线分割部分，尝试取得第一部分作为虚拟终端名称
      int d = file_path.indexOf("_", name_begin_pos);
      if (d != -1 && d < name_end_pos) name_end_pos = d;
    }
    QString name = file_path.mid(name_begin_pos, name_end_pos - name_begin_pos);
    //获取virtual_index，如果不是1-4，则都为0；如果有重复，则保存时被忽略
    int virtual_index = 0;
    if (file_path.indexOf("_1_", name_end_pos) != -1) virtual_index = 0;
    else if (file_path.indexOf("_2_", name_end_pos) != -1) virtual_index = 1;
    else if (file_path.indexOf("_3_", name_end_pos) != -1) virtual_index = 2;
    else if (file_path.indexOf("_4_", name_end_pos) != -1) virtual_index = 3;
    //保存信息
    auto &index_to_file_map = terminal_to_file_table[name];
    if (!index_to_file_map.contains(virtual_index)) {
      index_to_file_map[virtual_index] = file_path;
    }
  }

  int port_count = 0;
  int terminal_count = 0;
  //先打开所有文件，并找出文件最早的开始时间
  for (auto m = terminal_to_file_table.begin(); 
       m != terminal_to_file_table.end(); ++m)
  {
    for (auto f = m.value().begin(); f != m.value().end(); ++f) 
    {
      VirtualSenderInfo sender_info;
      sender_info.uri = QString("vod") + QString::number(terminal_count) + "@local";
      sender_info.virtual_count = m->count();
      sender_info.virtual_index = f.key(); //当前都按照每个终端只有一个摄像头模拟
      sender_info.name = QString("%1(%2)").arg(m.key()).arg(sender_info.virtual_index+1);
      sender_info.media_id = getFilePlayMediaID(sender_info.virtual_index, 
        TerminalHelper::GetUsername(sender_info.uri));

      sender_info.file = *f;
      sender_info.video_port = base_port + port_count * 4; // 每组RTP与RTCP占用2个端口，音视频占掉4个端口
      sender_info.audio_port = sender_info.video_port + 2;
      sender_info.start_time = GetFileStartTime(sender_info.file);
      sender_info.duration = 0;
      virtual_sender_dict_[sender_info.media_id] = sender_info;

      if (earliest_start_time_ == -1 || earliest_start_time_ > sender_info.start_time) {
        earliest_start_time_ = sender_info.start_time;
      }
      port_count += 4;
    }
    ++terminal_count;
  }

  //然后计算每个视频的播放延迟，建立媒体流进程，
  //等待其报告媒体格式后再创建虚拟终端，计算总时长，见MediaFormatInfoReceived
  for (auto &sender_info : virtual_sender_dict_)
  {
    sender_info.start_delay = sender_info.start_time - earliest_start_time_;

    FilePlayInfo info;
    info.vuser_id = sender_info.media_id;
    info.vuser_name = sender_info.name;
    info.sync_id = sync_id_;
    info.initial_delay = sender_info.start_delay;
    info.file_list.push_back(sender_info.file); //用Vector是为了方便以后传入连续播放的文件列表
    info.net_info.ip_addr = "127.0.0.1";
    info.net_info.video_port = sender_info.video_port;
    info.net_info.audio_port = sender_info.audio_port;

    QByteArray bytes;
    QDataStream out( &bytes , QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_4_4 );
    out << info;
    channel_dispatcher_->CreateChannel(sender_info.media_id, fileplay_channel_type, bytes);
  }
}

void FilePlayController::HandleClearPlayListsSlot()
{
  for (auto sender_info : virtual_sender_dict_) {
    channel_dispatcher_->ReleaseChannel(sender_info.media_id, fileplay_channel_type);
  }
  virtual_sender_dict_.clear();
}

void FilePlayController::HandlePlaySlot()
{
  FilePlayCtrlInfo info;
  info.ctrl_type = FilePlayCtrlInfo::kCtrlResume;
  info.op_id = op_id_;

  QByteArray bytes;
  QDataStream out( &bytes , QIODevice::WriteOnly );
  out.setVersion( QDataStream::Qt_4_4 );
  out << info;

  for (auto sender_info : virtual_sender_dict_) {
    channel_dispatcher_->ModifyChannel(sender_info.media_id, fileplay_channel_type,
      Action_PlayControl, bytes);
  }
  ++op_id_;
}

void FilePlayController::HandlePauseSlot()
{
  FilePlayCtrlInfo info;
  info.ctrl_type = FilePlayCtrlInfo::kCtrlPause;
  info.op_id = op_id_;

  QByteArray bytes;
  QDataStream out( &bytes , QIODevice::WriteOnly );
  out.setVersion( QDataStream::Qt_4_4 );
  out << info;

  for (auto sender_info : virtual_sender_dict_) {
    channel_dispatcher_->ModifyChannel(sender_info.media_id, fileplay_channel_type,
      Action_PlayControl, bytes);
  }
  ++op_id_;
}

void FilePlayController::HandleStopSlot()
{
  
}

void FilePlayController::HandleSeekSlot(unsigned long sec)
{
  FilePlayCtrlInfo info;
  info.ctrl_type = FilePlayCtrlInfo::kCtrlSeek;
  info.op_id = op_id_;

  for (auto sender_info : virtual_sender_dict_) {
    info.seek_time = sec * 1000LL; // sec to ms
    //计算出该段视频的时间。例如seek到总时间的10s，该视频启动延迟3s，则应seek到该视频的7s
    //可以是负数，同上例，如果seek到总时间的2s，则该段视频seek到-1s，表示应延迟1s播放
    info.seek_time -= sender_info.start_delay;

    QByteArray bytes;
    QDataStream out( &bytes , QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_4_4 );
    out << info;
    channel_dispatcher_->ModifyChannel(sender_info.media_id, fileplay_channel_type,
      Action_PlayControl, bytes);
  }
  ++op_id_;

  last_seek_op_time_ = QDateTime::currentMSecsSinceEpoch();
  current_play_position_ = 0; //重置等待底层进度报告更新
}

void FilePlayController::HandleConferenceStateChangedSlot( 
  const QString &conference_uri, 
  IConferenceController::ConferenceState state)
{
  if (state == IConferenceController::kIsInConference) {
    if (CVIniConfig::getInstance().isVirtualConferenceEnable() &&
      ConferenceHelper::IsVodVirtualConference(conference_uri)) {
        //本地点播会议，启动文件选择窗口
        QString app_dir = QApplication::applicationDirPath();
        QStringList files = QFileDialog::getOpenFileNames(nullptr, 
          QString::fromLocal8Bit("选择视频文件"), 
          app_dir + "\\" + CTxConfig::getInstance().GetDownloadPath(),
          "MPEG-4 (*.mp4)", 0, 
          QFileDialog::HideNameFilterDetails);
        HandleAddPlayListSlot(files);
    }
  }
  else if (state == IConferenceController::kIsNotInConference) {
    //退出会议停止本地播放流
    HandleClearPlayListsSlot();
  }
}

void FilePlayController::HandleReceiveUDPFilePlayReportSlot(
  const FilePlayStatItem &item)
{
  if (virtual_sender_dict_.find(item.id) == virtual_sender_dict_.end()) {
    //TODO：播放的文件不存在，记录日志
    return;
  }
  switch (item.type)
  {
  case FILEPLAY_STAT_MEDIAINFO:
    MediaFormatInfoReceived(item);
    break;
  case FILEPLAY_STAT_PROGRESS:
    ProgressInfoReceived(item);
    break;
  default:
    break;
  }
}

void FilePlayController::MediaFormatInfoReceived(const FilePlayStatItem &item)
{
  QString media_id = item.id;
  VirtualSenderInfo &sender_info = virtual_sender_dict_[media_id];

  //保存媒体格式
  sender_info.duration = item.format.file_duration;
  sender_info.video_codec = item.format.video_codec;
  sender_info.video_width = item.format.video_width;
  sender_info.video_height = item.format.video_height;
  sender_info.video_fps = item.format.video_fps;
  sender_info.audio_codec = item.format.audio_codec;
  sender_info.audio_sample_rate = item.format.audio_sample_rate;
  sender_info.audio_channel = item.format.audio_channel;
  sender_info.audio_bit = item.format.audio_bit_count;
  CreateVirtualTerminal(sender_info);

  long long my_duration = sender_info.start_delay + sender_info.duration;
  if (total_duration_ < my_duration) {
    total_duration_ = my_duration;
    emit NotifyPlayProgressSignal(0, (unsigned long)(total_duration_/1000));
  }
}

void FilePlayController::ProgressInfoReceived(const FilePlayStatItem &item)
{
  qint64 now = QDateTime::currentMSecsSinceEpoch();
  if (now - last_seek_op_time_ < 200) {
    //定位操作刚下发后，可能收到定位前的进度报告，此时不应该发出进度信号，
    //因此忽略定位操作后200ms以内的进度报告
    return;
  }

  QString media_id = item.id;
  VirtualSenderInfo &sender_info = virtual_sender_dict_[media_id];
  unsigned long pos = (unsigned long)(item.progress.current_pos + sender_info.start_delay);
  if (pos > current_play_position_) {
    //只有进度增加才更新，免得进度条抖动
    current_play_position_ = pos;
    emit NotifyPlayProgressSignal(current_play_position_/1000, total_duration_/1000);
  }
}

void FilePlayController::CreateVirtualTerminal(const VirtualSenderInfo &sender_info)
{
  //构建虚拟发送端 
  TerminalList terminals;
  TerminalPointer terminal(new TerminalDescription);
  terminal->uri = sender_info.uri;
  terminal->virtual_count = sender_info.virtual_count;
  terminal->virtual_index = sender_info.virtual_index;
  terminal->virtual_uri = TerminalHelper::ConstructDefaultVirtualURI(
    terminal->uri, terminal->virtual_index);
  terminal->name = sender_info.name;

  terminal->video_port = sender_info.video_port;
  terminal->audio_port = sender_info.audio_port;
  //构造SDP，此处构造的是非标准的，只含有CoolView需要的最小参数集
  //格式注意要跟SipWrapper中构造的一致。SipWrapper里面要调用很多方法才能构造SDP，所以这里暂时这样简化了
  QString sdp_info = "v=0\n";
  if (!sender_info.audio_codec.isEmpty()) {
    terminal->has_audio = true;
    QString sdp_audio = QString("m=audio RTP/AVP 103\na=ptime:20\na=rtpmap:103 %1/%2/%3\n").
      arg(sender_info.audio_codec).arg(sender_info.audio_sample_rate).arg(sender_info.audio_channel);
    sdp_info += sdp_audio;
  } else {
    terminal->has_audio = false;
  }
  if (!sender_info.video_codec.isEmpty()) {
    terminal->has_video = true;
    QString sdp_video = QString("m=video RTP/AVP 34\na=fmtp:34 CUSTOM=%1,%2 MaxBR=1960 FPS=%3\n"
      "a=rtpmap:34 H263/90000/1\n").
      arg(sender_info.video_width).arg(sender_info.video_height).arg(sender_info.video_fps);
    sdp_info += sdp_video;
  } else {
    terminal->has_video = false;
  }
  terminal->sdp = sdp_info;

  terminal->is_available = true;
  terminal->is_chairman_terminal = false;
  terminal->is_handup = false;
  terminal->is_main_speaker = false;
  terminal->is_speaking = true;

  terminals.push_back(terminal);
  emit NotifyVirtualRecvTerminalListSignal(terminals);
}

long long FilePlayController::GetFileStartTime( const QString &file_path )
{
  long long start_time = 0;
  /*QString info_file = 
      file_path.mid(0, file_path.lastIndexOf('.')) + media_info_file_ext;
  QByteArray info_file_data = info_file.toLocal8Bit();
  const char *file_name_buf = info_file_data.data();

  FILE *f = nullptr;
  f = fopen(file_name_buf, "r");
  if (f) {
    fscanf_s(f, "%I64d", &start_time);
    fclose(f);
  }*/

  QString file_name = file_path;
  int dash_pos = file_name.lastIndexOf("\\");
  if (dash_pos >= 0) {
    file_name = file_name.mid(dash_pos + 1);
  }
  int dot_pos = file_name.lastIndexOf(".");
  if (dot_pos >= 0) {
    file_name = file_name.left(dot_pos);
  }
  QStringList parts = file_name.split("_");
  for (QString &p : parts) {
    if (p.startsWith("ts")) {
      start_time = p.mid(2).toLongLong();
    }
  }  
  return start_time;
}
