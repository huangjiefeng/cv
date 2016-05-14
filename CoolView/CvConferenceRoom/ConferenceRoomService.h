#ifndef CONFERENCE_ROOM_SERVICE_H
#define CONFERENCE_ROOM_SERVICE_H

#include <QtCore/QtCore>
#include <QThread>
#include <map>
#include <string>

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)   { if( (x) ) {  delete (x); (x)=NULL; } }
#endif

#include <util/desktop/screen_helper.h>

#include "MediaWindowManager.h"

class QoSReportReceiverThread;
class ChannelDispatcherIf;
class CvStreamedMediaIf;
typedef std::map<QString , CvStreamedMediaIf*>   StreamedMediaProxyMap;

class ConferenceRoomService : public QObject
{
	friend class QtMeetingPlaceWidget;
	Q_OBJECT

public: // PROPERTIES

	static ConferenceRoomService* getInstance()
	{
		static ConferenceRoomService service;
		return &service;
	}

	void resizeMediaWindow( const QString& userid , const int wnd );

private:
	ConferenceRoomService();
	~ConferenceRoomService();

public Q_SLOTS: // METHODS
	void AddMediaWindow(bool send, const QByteArray &input_garray, int screenIndex, int seet);
	void ChangeLayout(int screenIndex, int displayModel);
	void CloseMediaWindow(const QString &user_id);
	void CloseWindow();
	void ExitConference();
	void ModifyMediaWindow(const QString &user_id, int actionIdex, const QByteArray &arguments);
	void OpenScreenShareFile(const QString &fileName, int screenIndex);
	void ShowRtcpMsg(int screenIndex, bool show);
	void UpdateRtcpMsg(const QByteArray &input_garray);

    void RecoveryRecvMediaProcess(const QByteArray &input_garray, int screenIndex, int seet);

	//接收遥控器的查询信息
	void TeleInfo(int info_index, int subscribe_id, const QByteArray &input_array);
Q_SIGNALS: // SIGNALS


private:


	ChannelDispatcherIf* _channelDispatcherProxy;

	///保留媒体流与对应的dbus proxy
	StreamedMediaProxyMap _mediaProxyMap;

	///接收QoS报告的线程
	QoSReportReceiverThread*	_qosRportReceiver;
  QThread* _qosRportThread;

  MediaWindowManager * _mediaWindowManager;
};



#endif