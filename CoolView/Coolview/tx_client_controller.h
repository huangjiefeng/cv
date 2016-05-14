#ifndef TX_CLIENT_CONTROLLER_H
#define TX_CLIENT_CONTROLLER_H

#include "tx_client_controller_interface.h"

#include <QTimer>
#include <QThread>

class RunningProfile;
class RunningConfig;
class IConferenceController;
class IMsgManager;
class MediaManager;
class UDPReportSender;
namespace tx_soap {
class CvTxProxy;
}

class TXClientController : public ITXClientController {
  Q_OBJECT

 public:
  TXClientController(QObject *parent = nullptr,
                     RunningProfile *profile = nullptr,
                     RunningConfig *config = nullptr);
  ~TXClientController();

  void Initialize(
    IMsgManager *msg_manager,
    IConferenceController *conference_controller,
    MediaManager *media_manager);

  QString GenerateLocalRecordFilePath(int virtual_index) override;

 public Q_SLOTS: 
  //
  // 控制终端本地录像的方法
  //
  // 本地视频采集状态改变时，根据需要开启自动录制
  void HandleSendMediaStateChangedNotifySlot(const QString &media_id, int type) override;
  // 处理本地视频录制分割请求
  void HandleSegmentLocalRecordVideoRequestSlot() override;
  // 处理会议状态变更
  void HandleConferenceStateChangedSlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state) override;

  //
  // 远程录像控制相关方法
  //
  void HandleOneVideoRecordRequestSlot(const QString &user_id, int type) override; //connect in CoreService2
  void HandleWholeVideoRecordRequestSlot(const QString &user_id, int type, QByteArray param) override; //connect in CoreService2
  void HandleRecvRecordControlAckNotifySlot(const RecordControlAckMsg &info) override;
  // 定时获取录像状态
  void HandleGetTxRecordStatusTimerTimeoutSlot();

 private:
  // 向TX发送录制控制指令
  void SendRecordControlRequest(const QString &uri, int type, QByteArray param);
  // 创建TX远程调用代理
  void CreateTxSoapProxy();
  // 设置ConfRoom中某终端所有虚拟终端视图的录制状态
  void ChangeRecordState(const QString &uri, int state);
  // 将vuri转换成录制报告的user_name，如果是本终端，则转换成本地回显id
  inline void GetRecUDPReportUserNameFromVirtualUri(char *buf, int len, const QString &vuri);
  inline bool IsRecordCtrlAllow();
  inline bool IsSelfRecordCtrlAllow();
  inline bool IsOnlySelfRecordCtrlAllow();

 private:
  RunningProfile *profile_;
  RunningConfig *running_config_;
  IConferenceController *conference_controller_;
  TerminalManager *terminal_manager_; //指向conference_controller_中的对象
  MediaManager *media_manager_;

  QThread *thread_; //远程调用时为了不阻塞主线程，本对象要在单独线程中运行

  QString recording_path_; //录像文件存储根目录
  QTimer *send_video_auto_cut_timer_; //本地录像的自动分段计时器

  tx_soap::CvTxProxy *tx_soap_proxy_;
  QTimer *get_tx_record_status_timer_; //定时获取TX录制状态的计时器
  UDPReportSender *rec_stat_sender_to_confroom_;
};

#endif
