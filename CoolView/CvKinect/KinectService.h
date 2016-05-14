#ifndef _KINECT_SERVICE_H_
#define _KINECT_SERVICE_H_

#include <Qtcore/QtCore >
#include <dbus/kinect/common/KinectServiceCommon.h>
class CKinect;
class KinectService : public QObject
{
	Q_OBJECT
public:
	static KinectService*	getInstance()
	{
		static KinectService service;
		return &service;
	}

	/**
	 * @brief 发送信号给指定的进程
	 * @param to 进程名称，如果所有进程都能接收，那么填写ALL或者为空字符串""
	 * @param type 手势类型，参看KinectGesture枚举类型
	 */
	void EmitGestureSignal( const QString&to , const KinectGesture type );

	/**
	 * @brief 发送Kinect提示消息
	 * @param type 消息类型，参看KinectMessageType枚举类型
	 * @param message 消息内容
	 */
	void EmitKinectMessage( const KinectMessageType type , const QString&message );

private:
	KinectService();
	~KinectService();  
	CKinect * kinect;
public: // PROPERTIES
	public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
		void GestureOccur(const QString &to, int type);
		void KinectMessage(int type, const QString &message);
};
#endif