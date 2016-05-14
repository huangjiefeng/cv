#ifndef CV_STREAMED_MEDIA_SERVICE_H
#define CV_STREAMED_MEDIA_SERVICE_H

#include <QtCore/QtCore>
#include <QSharedMemory>

#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>
//#include <dbus/msdxclient/common/MsdxClientServiceCommon.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include "util/report/RecordStat.h"
#include "util/report/FilePlayStat.h"
#include "util/BackupManager/BackupManager.h"

#include "msdx/IGraphController.h"

typedef void*  HANDLE;

class CvCoreIf;
class ConferenceRoomIf;
class ChannelDispatcherIf;
class QosReportSender;
class FilePlayCtrlInfo;


class StreamMediaStatus
{
public:
    bool valid;
    //for send and small
    typedef QPair<QString, unsigned short> SendDest;
    QVector<SendDest> video_send_dest;
    QVector<SendDest> audio_send_dest;
    //for recv
    int window_handle;
    int video_width;
    int video_height;
    //for send and recv
    bool enable_video;
    bool enable_audio;

public:
    //friend functions
    friend QDataStream& operator>>(QDataStream& in, StreamMediaStatus& data);
    friend QDataStream& operator<<(QDataStream& out, StreamMediaStatus& data);
};


class CvStreamedMedia:public QObject
{
	Q_OBJECT
public:
	CvStreamedMedia();
	~CvStreamedMedia();

	bool Init( const QString mediaID );

	static void RecordCallback(void * obj, RecStatItem &stat);
  static void FilePlayCallback(void * obj, FilePlayStatItem &stat);


public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Destroy();
    QByteArray GetMediaInfo(int info_type);
    void Pause();
    void Run();
    void SetMediaInfo(int action_id, const QByteArray &input_garray);
    void Stop();
Q_SIGNALS: // SIGNALS
    void QosNofity(const QByteArray &output_garray, int notify_index);
    void RunningStateChanged(const QString &media_id, int current_state);

protected Q_SLOTS:
    void HandleVideoWindowCheckTimerTimeoutSlot();

private:

	//发送RTP统计信息给Coolview控制进程
	static int sendRtpStatInfoToClient(const RtpStatItem& rtpstat);

	/**
	 * @brief	发送RTP统计信息给Coolview控制进程
	 * @param pclass
	 * @param &rtpstat 
	 *@return 0表示发送成功，负数则发送失败
	 */
	static int deliverRtpStatInfo(void * pclass, const RtpStatItem * rtpstat);

  static void initQosReportSender();

	int CreateRecvGraph( const RecvGraphInfo& info );
	int CreateSendGraph( const SendGraphInfo& info );
  int CreateSmallVideoGraph( const RecvGraphInfo& info );
  int CreateFilePlayGraph( const FilePlayInfo& info );

	int AddSendDest(const NetInfo &info);
    int RemoveSendDest(const NetInfo &info);
	void ResetVideoWindow( const int wnd );
    void EnableVideo(bool b);
    void EnableAudio(bool b);
    void SetCurrentState(StreamedMediaState state);
    void ControlPlay(const FilePlayCtrlInfo &info);

    void BackupStatus();
    int RestoreStatus();
    void ResetBackupStatus();

private:

  static QosReportSender *qosReportSenderToCoolview;
  static QosReportSender *qosReportSenderToConfRoom;

	StreamedMediaState		_currentState;
	MediaDirection			_mediaDirection;
	int						_graphID;
	static QString			_mediaID;
	static QString			_userID; //记录userID，在Qos Signal中要用到

	///记录视频宽高，用于调整视频窗口
	int						_videoWidth;
	int						_videoHeight;

	///保留本示例的指针
	static CvStreamedMedia*	_instance;

	ChannelDispatcherIf*	_channelDispatcher;

    //本地播放graph，基于新版msdx增加的，因此处理方式与其他graph不同 ——Liaokz，2015-1
    msdx::IFilePlayGraphController *_file_play_controller;
    //end 本地播放graph

    //持有ChannelDispatcher数据恢复共享内存,这样当ChannelDispatcher崩溃时
    //只要还有一个StreamMedia进程存在,该共享内存就不会被关闭
    //并且，CvStreamMedia也要保存进程的状态信息
    BackupManagerPtr backup_manager_;
    StreamMediaStatus stream_media_status_;

    QTimer video_window_check_timer_; //用于检测recv端视频窗口的大小变更
    int video_window_handle_;
    
};

#endif