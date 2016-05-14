#ifndef CV_CONFIG_SERVICE_H
#define CV_CONFIG_SERVICE_H
#include <QtCore/QtCore>
class TelecontrollerIf;
class CvConfigService : public QObject
{
	Q_OBJECT

public:
	CvConfigService();
	~CvConfigService();

public: // PROPERTIES
	public Q_SLOTS: // METHODS
		void QueryAllConfig(int subscribe_id);
		void QueryCurrentConfig(int subscribe_id);
		void RefreshConfig();
		void SetCamera(const QString &camera_name, const QString &crossbar_name, const QString &crossbar_type, int video_width, int video_height);
		void SetIP(const QString &ip);
		void SetMic(const QString &mic_name, int mic_volumn);
		void SetSpeaker(const QString &speaker_name, int speaker_volumn);
Q_SIGNALS: // SIGNALS

private:
	TelecontrollerIf*	_teleProxy;
};

#endif