#include "QoSReportReceiverThread.h"
#include "QtConfrenceVideoWindow.h"
#include <util/udp/UdpReceiver.h>
#include <util/report/ReporterCommon.h>
#include "MediaWindowManager.h"
#include <log/Log.h>

QoSReportReceiverThread::QoSReportReceiverThread()
{
	_terminate = false;
	_udpServer = nullptr;	
}

QoSReportReceiverThread::~QoSReportReceiverThread( void )
{
	delete _udpServer;
	_udpServer = NULL;
}

void QoSReportReceiverThread::Initialize(const QString&bindIP , const int port)
{
  _terminate = false;

  _udpServer = new UdpReceiver();
  bool bind = false;
  short tryTimes = 5;
  do 
  {
    _snprintf(__global_msg , sizeof(__global_msg) , "Try to bind udp server %s:%d", bindIP.toStdString().c_str() , port );
    CONFROOM_LOG_DEBUG(__global_msg );

    bind = _udpServer->bind( bindIP.toStdString().c_str() , port );

    if( !bind )
    {
      _snprintf(__global_msg , sizeof(__global_msg), "Udp server bind failed with code:%x", GetLastError() );
      CONFROOM_LOG_ERROR(__global_msg );
    }else
    {
      CONFROOM_LOG_ERROR("Udp server bind success." );
    }
    tryTimes--;
  } while (!bind && tryTimes>0 );

  qRegisterMetaType<RtpStatItem>("RtpStatItem");
  qRegisterMetaType<RecStatItem>("RecStatItem");

  connect(this, &QoSReportReceiverThread::StartSignal,
    this, &QoSReportReceiverThread::run);
}

void QoSReportReceiverThread::run()
{
	while( !_terminate && _udpServer->isBinded() )
	{
		char data[1024] = {0};
		int size = _udpServer->receive( &data , sizeof(data) );

        const UDPReportHeader * header = 
            reinterpret_cast<const UDPReportHeader *>(data);

        if (size - sizeof(UDPReportHeader) != header->size) {
            return; // TODO: 此时可能是数据包太大被截断，应记录错误日志
        }

        switch (header->type) {
        case UDPReportType::UdpReportType_RTP:
            if (header->size == sizeof(RtpStatItem)) {
                const RtpStatItem * item =
                    reinterpret_cast<const RtpStatItem *>(data + sizeof(UDPReportHeader));
                //多线程环境不可直接调用！ --By Liaokz
                //MediaWindowManager::getInstance()->UpdateRtcpMessage( (const RtpStatItem &)*item );
                emit SendUDPQoSReportSignal(*item);
            }
            break;
        case UDPReportType::UdpReportType_Record:
            if (header->size == sizeof(RecStatItem)) {
                const RecStatItem * item =
                    reinterpret_cast<const RecStatItem *>(data + sizeof(UDPReportHeader));
                emit SendUDPRecReportSignal(*item);
            }
            break;
        default:
            break;
        }
	}
}

void QoSReportReceiverThread::Start()
{
  emit StartSignal();
}

