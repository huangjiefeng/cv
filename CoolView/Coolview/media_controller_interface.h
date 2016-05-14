#ifndef MEDIA_CONTROLLER_INTERFACE_H
#define MEDIA_CONTROLLER_INTERFACE_H

#include <QObject>
#include <QRect>

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

enum MediaControlType;
enum MediaStreamType;

class IMediaController : public QObject {
  Q_OBJECT
 public:
   IMediaController(QObject *parent) : QObject(parent) {}
   virtual ~IMediaController() {}

 Q_SIGNALS:

 public Q_SLOTS:
   // 发送QoS信息给QoS服务器
   virtual void HandleSendRTPStatToQoSServerRequestSlot(
     const RtpStatItem& rtp_stat) = 0;

   // 发送录制信息给服务器 - 暂未实现发送
   virtual void HandleSendRecStatToServerRequestSlot(
     const RecStatItem& recstat) = 0;

   // 向指定用户请求媒体流
   // seat: 视频窗口所在位置，1表示第一个窗口，以此类推
   virtual void HandleAddRecvStreamMediaRequestSlot(
     const QString &remote_vuri,
     int screen_index, 
     int seat,
     bool use_small_video) = 0;

   // 告知远程用户停止发送视频流到本地
   virtual void HandleRemoveRecvStreamMediaRequestSlot(
     const QString &remote_vuri) = 0;

   // 改变视屏显示位置
   // 界面不应直接调用该函数
   virtual void HandleChangeVideoSeatSlot( 
     const QString &remote_uri,
     int screen_index,
     int seat) = 0;

   // 创建本地发送进程
   virtual void HandleCreateSendStreamMediaRequestSlot() = 0;

   // 告知所有参会终端的媒体发送状态，并进行对媒体流进行控制
   // type: 媒体类型，只支持kAudioControlType和kVideoControlType
   // enable: 如果发送媒体流，则为true，否则为false
   virtual void HandleControlSendMediaRequestSlot(
     MediaControlType type,
     bool enable) = 0;

   // 创建发送共享屏幕流
   virtual void HandleCreateSendScreenMediaRequestSlot(const QRect &wnd) = 0;

   // 创建接收共享屏幕流
   virtual void HandleCreateRecvScreenMediaRequestSlot(
     const QString &remote_uri, 
     int screen_index) = 0;

   // 告知远程用户停止发送屏幕流
   virtual void HandleRemoveRecvScreenMediaRequestSlot(
     const QString &remote_uri,
     int screen_index) = 0;

   // 控制本地屏幕共享发送/停止
   // 同时告知其他终端创建/销毁当前终端的屏幕流接收进程
   virtual void HandleScreenShareControlRequestSlot(bool enable) = 0;
   
   // 恢复媒体进程
   virtual void RecoveryMediaProcess(
     MediaStreamType type, 
     bool is_send, 
     const QString &remove_user_id) = 0;
};

#endif // MEDIA_CONTROLLER_INTERFACE_H