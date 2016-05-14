#ifndef _TX_CLIENT_CONTROLLER_INTERFACE_H_
#define _TX_CLIENT_CONTROLLER_INTERFACE_H_

#include <QObject>

#include "msg_manager_interface.h"
#include "conference_controller_interface.h"
#include "util/report/RecordStat.h"


class ITXClientController : public QObject {
  Q_OBJECT

 public:
  ITXClientController(QObject *parent) : QObject(parent) {}
  virtual ~ITXClientController() {}

  virtual QString GenerateLocalRecordFilePath(int virtual_index) = 0;

 Q_SIGNALS:
  //远程控制相关
  void SendRecordControlRequestSignal(const RecordControlParam &param); //已废弃

  //本地录像状态
  void ReceiveUDPRecReportSignal(const RecStatItem &item);

 public Q_SLOTS: 
   //
   // 控制终端本地录像的方法
   //
   // 本地视频采集状态改变时，根据需要开启自动录制
   virtual void HandleSendMediaStateChangedNotifySlot(const QString &media_id, int type) = 0;
   // 处理本地视频录制分割请求
   virtual void HandleSegmentLocalRecordVideoRequestSlot() = 0;
   // 会议状态变更
   virtual void HandleConferenceStateChangedSlot(
     const QString &conference_uri, 
     IConferenceController::ConferenceState state) = 0;

   //
   // 远程录像控制相关方法
   //
   virtual void HandleOneVideoRecordRequestSlot(const QString &user_id, int type) = 0;
   virtual void HandleWholeVideoRecordRequestSlot(const QString &user_id, int type, QByteArray param) = 0;
   virtual void HandleRecvRecordControlAckNotifySlot(const RecordControlAckMsg &info) = 0;
};

#endif
