#ifndef _TX_CONTROLLER_INTERFACE_H_
#define _TX_CONTROLLER_INTERFACE_H_

#include <QObject>

#include "session_controller_interface.h"
#include "conference_controller_interface.h"
#include "terminal_description.h"
#include "msg_manager_interface.h"
#include "util/report/RecordStat.h"
#include "util/report/RtpStat.h"

struct RecordControlMsg;

class ITXController : public QObject {
  Q_OBJECT

 public:
  ITXController(QObject *parent) : QObject(parent) {}
  virtual ~ITXController() {}
  
  static void RegisterMetaType() {
    qRegisterMetaType<RecStatItem>("RecStatItem");
  }

  virtual QString GenerateRecordFilePath(TerminalPointer terminal) = 0;
  virtual int GetRecordInitialJobId(const QString &vuri) = 0;
  //是否有正在录制的作业
  virtual bool HasRecording() = 0;

 Q_SIGNALS:
  void StartReceiveVideoRequestSignal(const QString &vuri);
  void StopReceiveVideoRequestSignal(const QString &vuri);

  //通知视频录制的发起人信息变更，一般用于UI变更
  void NotifyRecordRequesterSignal(const QString &vuri, 
    const QString &requester_uri);
  //通知收到录制状态报告
  void NotifyRecStatReceivedSignal(const RecStatItem &rec_state);
  //远程控制相关
  void SendRecordControlRequestAckSignal(const RecordControlAckParam &param);
  //通知所有录制都已结束
  void NotifyAllRecordJobFinishSignal();

 public Q_SLOTS: 
  // 处理录制状态报告
  virtual void HandleReceiveUDPRecReportSlot(
    const RecStatItem& recstat) = 0;
  // 处理QoS报告——临时的，用于取得发送端IP
  virtual void HandleReceiveUDPQoSReportSlot(
    const RtpStatItem& rtp_stat) = 0;
  // 处理终端状态变更
  virtual void HandleTerminalStatusChangedSlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri) = 0;
  // 处理终端重登陆
  virtual void HandleTerminalReloginSlot(
    ConstTerminalPointer terminal,
    const QString &conference_uri) = 0;
  // 分段请求
  virtual void HandleSegmentMediaRequestSlot(const QString &vuri) = 0;
  // 登陆状态变更时作出响应
  virtual void HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state) = 0;
  // 会议状态变更
  virtual void HandleConferenceStateChangedSlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state) = 0;

  // 远程录像控制相关方法
  virtual void HandleRecvRecordControlSlot(const RecordControlMsg &info) = 0;
};

#endif // _TX_CONTROLLER_INTERFACE_H_