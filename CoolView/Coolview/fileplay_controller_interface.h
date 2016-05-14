#ifndef FILEPLAY_INTERFACE_H
#define FILEPLAY_INTERFACE_H

#include <QObject>

#include "conference_controller_interface.h"
#include "terminal_description.h"
#include "util/report/FilePlayStat.h"

class IFilePlayController : public QObject {
  Q_OBJECT

 public:
  IFilePlayController(QObject *parent) : QObject(parent) {}
  virtual ~IFilePlayController() {}

  static void RegisterMetaType() {
    qRegisterMetaType<FilePlayStatItem>("FilePlayStatItem");
  }

 Q_SIGNALS:
  //通知播放进度,单位秒
  void NotifyPlayProgressSignal(unsigned long pos, unsigned long duration);

  //通知文件处理异常
  void NotifyFileErrorSignal(const QString &msg);

  //虚拟接收终端列表信号，用于添加播放列表对应的虚拟终端
  void NotifyVirtualRecvTerminalListSignal(const TerminalList&);

 public Q_SLOTS:
  //添加一个播放列表，一个文件对应生成一个终端哦
  virtual void HandleAddPlayListSlot(const QStringList&) = 0;

  //清空所有播放列表
  virtual void HandleClearPlayListsSlot() = 0;

  //播放控制
  virtual void HandlePlaySlot() = 0;
  virtual void HandlePauseSlot() = 0;
  virtual void HandleStopSlot() = 0;
  virtual void HandleSeekSlot(unsigned long sec) = 0;

  //处理会议状态变更
  virtual void HandleConferenceStateChangedSlot(
    const QString &conference_uri, 
    IConferenceController::ConferenceState state) = 0;

  //处理播放状态UDP报告
  virtual void HandleReceiveUDPFilePlayReportSlot(const FilePlayStatItem &) = 0;

};

#endif // end FILEPLAY_INTERFACE_H
