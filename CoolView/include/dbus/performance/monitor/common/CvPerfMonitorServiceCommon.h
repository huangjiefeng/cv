#pragma  once

#define PERFORMANCE_MONITOR_SERVICE_NAME	"com.dcampus.coolview.performance.monitor"
#define PERFORMANCE_MONITOR_OBJECT_PATH		"/com/dcampus/coolview/performance/monitor"

#include <QtCore/QtCore>

//触发器动作
class MonitorTriggerAction
{
public:
	static const unsigned int NO_ACTION = 0;
	static const unsigned int EMAIL = 0x00000001;				//邮件通知
	static const unsigned int RECOVER_PROCESS = 0x00000002;	//恢复进程
	static const unsigned int RESTART_COMPUTOR = 0x00000004;	//重启电脑
};
//触发器，定义了具体条件以及触发的动作类型
class Trigger
{
public:
	static const unsigned short NO_TRIGGER = 0;
	static const unsigned short CPU_USAGE = 0x0001;			//单位为%
	static const unsigned short MEM_USAGE = 0x0002;			//单位为MB
	static const unsigned short TIME_TO_LIVE = 0x0003;		//通过心跳来保持TTL值的大小
	static const unsigned short PROCESS_ALIVE = 0x0004;		//程序是否在工作

	//高两个字节为监控类型，低两个字节存储上限值
	unsigned int		_monitorValue;					
	unsigned int		_action;

	friend QDataStream& operator>>(QDataStream& in, Trigger& data)
	{
		in >> data._monitorValue >> data._action;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const Trigger& data)
	{
		out << data._monitorValue << data._action;
		return out;
	}
};

//受监控的进程资料
class MonitoredProcess
{
public:
	MonitoredProcess()
	{
		_processID = -1;
		_monitorInterval = -1;
		_monitorThread = NULL;
	}

	///是否要监控该进程
	void SetMonitorMark( const bool monitored ){ _isUnderMonitor = monitored; }
	bool GetMonitorMark( ) { return _isUnderMonitor; }


	//start-----通过序列化直接传递的参数
	int					_processID;			//进程ID
	QString				_processName;		//进程名称
	QString				_processDirPath;	//进程所在目录的绝对路径
	QString				_reportEmail;		//记录错误报告的发送地址
	int					_monitorInterval;	//监控间隔，单位为ms，每过一段间隔则检查触发器
	//end
	MonitoredProcess& operator = ( const MonitoredProcess& rightValue);

	QVector<Trigger>	_triggerVector;		//触发器列表
	QByteArray			_processImage;		//进程映像，用于恢复进程
	QThread*			_monitorThread;
	bool				_isUnderMonitor;	//进程信息是否被监控;

	void AddTrigger( unsigned short monitorType , unsigned short threshhold , const unsigned int actionIndex )
	{
		Trigger trigger;
		trigger._monitorValue = monitorType << 16;
		trigger._monitorValue |= threshhold ;
		trigger._action = actionIndex;
		_triggerVector.push_back( trigger );
	}

	friend QDataStream& operator>>(QDataStream& in, MonitoredProcess& data)
	{
		int triggerSize ;
		in >> data._processID >> data._processName >> data._processDirPath >> data._reportEmail >> data._monitorInterval
			>> triggerSize;
		for( int i=0 ; i<triggerSize ; i++ )
		{
			Trigger trigger;
			in >> trigger;
			data._triggerVector.push_back( trigger);
		}
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const MonitoredProcess& data)
	{
		int triggerSize = data._triggerVector.size();
		out << data._processID << data._processName << data._processDirPath << data._reportEmail << data._monitorInterval
			<< triggerSize;
		for( int i=0 ; i<triggerSize ; i++ )
		{
			const Trigger& trigger = data._triggerVector.at(i);
			out << trigger;
		}
		return out;
	}
};

inline MonitoredProcess& MonitoredProcess::operator = ( const MonitoredProcess& rightValue )
{
	this->_processID = rightValue._processID;
	this->_processName = rightValue._processName;
	this->_reportEmail = rightValue._reportEmail;
	this->_monitorInterval = rightValue._monitorInterval;
	this->_triggerVector = rightValue._triggerVector;
	return *this;
}

//监控进程的媒体类型
enum MeidaMonitorType
{
	STREAM_MEDIA_SEND = 0,
	STREAM_MEDIA_RECV = 1,
	SCREEN_MEIDA_SEND = 2,
	SCREEN_MEIDA_RECV = 3
};


//受监控的媒体进程资料
class MediaMonitoredProcess
{
public:
	QString             userID;             //用户ID
	int					processID;			//进程ID
	QString				processName;		//进程名称
	int                 mediaType     ;     //进程监控的媒体类型
    // TODO: 
    bool                isSmallVideo;       //是否是小流

	friend QDataStream& operator>>(QDataStream& in, MediaMonitoredProcess& data)
	{
		in >> data.userID >> data.processID >>data.processName >> data.mediaType;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const MediaMonitoredProcess& data)
	{
		out << data.userID << data.processID << data.processName << data.mediaType;
		return out;
	}

};