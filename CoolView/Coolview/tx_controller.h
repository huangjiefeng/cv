#ifndef TX_CONTROLLER_H
#define TX_CONTROLLER_H

#include <QTimer>
#include <QHash>
#include <QList>

#include "tx_controller_interface.h"

#include "session_controller_interface.h"
#include "conference_controller_interface.h"

class MediaManager;
class IConferenceController;
class IDisplayController;
class IMsgManager;
class RunningConfig;
class RunningProfile;
class UDPReportSender;
class TxMonitorIf;

class StartRecordParam;
class StopRecordParam;
class RecordTerminalStatus;
struct RecordControlMsg;

class TXController : public ITXController {
  Q_OBJECT

 public:
  TXController(QObject *parent = nullptr,
               RunningProfile *profile = nullptr,
               RunningConfig *config = nullptr);
  ~TXController();

  void Initialize(
    ISessionController *session_controller,
    IMsgManager *msg_manager,
    IConferenceController *conference_controller,
    IDisplayController *display_controller,
    MediaManager *media_manager);

  QString GenerateRecordFilePath(TerminalPointer terminal) override;
  int GetRecordInitialJobId(const QString &vuri) override;
  bool HasRecording() override;
  
 public Q_SLOTS: 
  //
  // 处理UDP报告
  //
  // 处理录制状态报告
  void HandleReceiveUDPRecReportSlot(
    const RecStatItem &rec_stat) override; // connect in CoreService2
  // 处理QoS报告
  void HandleReceiveUDPQoSReportSlot(
    const RtpStatItem &rtp_stat) override; // connect in CoreService2

  //
  // 处理ConferenceController信号
  //
  // 终端状态变更，自动启动录像
  void HandleTerminalStatusChangedSlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri) override;
  // 终端重登陆
  void HandleTerminalReloginSlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri) override;
  // 视频分段
  void HandleSegmentMediaRequestSlot(const QString &uri_or_vuri) override;
  // 登录状态变更时作出响应
  void HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state) override;
  // 会议状态变更
  void HandleConferenceStateChangedSlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state) override;

  //
  // 通过SIP远程控制录像（已废弃）
  //
  void HandleRecvRecordControlSlot(const RecordControlMsg &info) override;

  //
  // 控制录像的DBus接口
  //
  int ControlRecord(int op, const QByteArray &params);
  QByteArray GetRecordStatus(const QString &focus, const QString &uri);

protected slots:
  // 对媒体流状态变更作出响应
  void HandleStreamMediaReceiveStartedSlot(const QString &remote_vuri);
  void HandleStreamMediaReceiveStoppedSlot(const QString &remote_vuri);
  // 延迟启动自动录像
  void HandleAutoRecordTimerTimeoutSlot();

 private:

  struct ViewRecordNode {
    QString vuri;
    RecStatItem record_status; //最近报告的状态
    bool start;
    bool complete;
  };
  typedef std::shared_ptr<ViewRecordNode> ViewRecordPointer;
  typedef QList<ViewRecordPointer> ViewRecordList;

  struct EpisodeUserInfo {
    QString title;
    QString participants;
    QString keywords;
    QString description;
  };
  struct EpisodeSysInfo {
    int job_id; //用于和底层报告的录制信息配对
    long long start_time; //严格增长的高精度时间戳，可作为数据库中的id
    bool segmented; //用于防止重复分段
    ViewRecordList views; //分集下的视角列表
  };
  typedef std::shared_ptr<EpisodeSysInfo> EpisodeSysInfoPointer;
  typedef QList<EpisodeSysInfoPointer> EpisodeList;

  struct TerminalRecordNode {
    QString uri;
    QString name; //保存终端名，为什么这么做见ViewRecordComplete方法
    QString requester_uri; //发起录制的人
    int next_job_id;
    //QString source_ip; //发送方地址
    //int report_port; //发送方接收录制报告的端口
    //std::shared_ptr<UDPReportSender> reporter; //录像报告发送器
    EpisodeUserInfo episode_user_info; //用户输入的录像信息
    EpisodeList episodes;
  };
  typedef std::shared_ptr<TerminalRecordNode> TerminalRecordPointer;
  typedef QHash<QString, TerminalRecordPointer> TerminalRecordDict; //key:uri

  //判断录制视频是否应该分段
  bool ShouldNewEpisodeStart(ViewRecordPointer view);
  //处理远程启动录像请求
  int RemoteStartRecord(const StartRecordParam &info); 
  //处理远程停止录像请求
  int RemoteStopRecord(const StopRecordParam &info); 
  //启动录像
  TerminalRecordPointer StartRecord(
    const StartRecordParam &param, 
    bool manual);
  //视角的一段录像完成
  void ViewRecordComplete(const QString &vuri, const RecStatItem &rec_stat);
  //录像文件无效
  void ViewRecordAbandon(const QString &vuri, const RecStatItem &rec_stat);
  //录像进程已停止
  void RecordProcessExit(const QString &vuri);

  //在dict中插入新的node并返回它;若已存在，失败则返回nullptr
  TerminalRecordPointer AddTerminalNode(const QString uri_or_vuri);
  //在TerminalRecordNode中插入新的episode，失败则返回nullptr
  EpisodeSysInfoPointer AddEpisodeNode(TerminalRecordPointer terminal);
  //在episode中插入新的ViewRecordNode，若已存在则返回现有的，失败则返回nullptr
  ViewRecordPointer AddViewNode(EpisodeSysInfoPointer episode, const QString &vuri);
  //找到并返回Episode的指针，没有则返回nullptr
  EpisodeSysInfoPointer FindEpisodeNode(const QString &uri_or_vuri, int job_id);
  //找到并返回ViewRecordNode的指针，没有则返回nullptr
  ViewRecordPointer FindViewNode(const QString &vuri, int job_id);
  //删除node
  void RemoveFinishedEpisodeNode(const QString &uri);

 private:

  RunningProfile *profile_;
  RunningConfig *running_config_;
  IConferenceController *conference_controller_;
  MediaManager *media_manager_;
  TerminalManager *terminal_manager_; //指向conference_controller_中的对象

  std::shared_ptr<TxMonitorIf> tx_monitor_;

  QString recording_path_; //录像文件存储根目录
  bool is_tx_terminal_; //本终端是否为tx
  __int64 max_file_size_;
  __int64 max_file_duration_;

  //持有当前会议指针的引用，防止退出会议后停止录制时拿不到会议信息
  //必须在每次进入会议时更新，退出会议时不能赋值nullptr！
  ConferencePointer current_conference_;

  //保存一份终端列表（TerminalManager不能查询所有终端）
  typedef QVector<QString> TerminalUriList;
  TerminalUriList terminal_uri_list_;
  
  TerminalRecordDict terminal_record_dict_;
  QHash<QString, QString> username_to_vuri_dict_;

  struct AutoStartPendingInfo {
    QString uri;
    time_t online_time;
  };
  QList<AutoStartPendingInfo> auto_record_pending_list_;
  QTimer *auto_record_timer_;
  int auto_record_delay_; //s

  static const int kAutoRecordCheckTimeOut = 1000; //ms
};

#endif // TX_CONTROLLER_H
