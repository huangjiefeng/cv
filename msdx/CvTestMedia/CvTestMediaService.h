#ifndef CV_STREAMED_MEDIA_SERVICE_H
#define CV_STREAMED_MEDIA_SERVICE_H

#include <QtCore/QtCore>
#include <dbus/channel/type/testMedia/common/testMediaServiceCommon.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>

typedef void*  HANDLE;

class CvCoreIf;
class ConferenceRoomIf;
class ChannelDispatcherIf;

class CvTestMedia:public QObject
{
	Q_OBJECT
public:
	CvTestMedia( const QString mediaID );
	~CvTestMedia();


public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Destroy();
    QByteArray GetMediaInfo(int info_type);
    void Pause();
    void Run();
    void SetMediaInfo(int action_id, const QByteArray &input_garray);
    void Stop();
Q_SIGNALS: // SIGNALS
    void RunningStateChanged(const QString &media_id, int current_state);

private:

	int CreateAudioTestGraph( const TestMediaInfo& info );
	int CreateVideoTestGraph( const VideoPreviewInfo &info);


private:
	TestMediaState		_currentState;
	QString			_mediaID;


	///保留本示例的指针
	CvTestMedia*	_instance;

	ChannelDispatcherIf*	_channelDispatcher;
};

#endif