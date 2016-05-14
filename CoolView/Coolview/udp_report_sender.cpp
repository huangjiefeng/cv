#include "udp_report_sender.h"

#include "util/udp/UdpSender.h"
#include <util/report/ReporterCommon.h>
#include <log/Log.h>
#include <WS2tcpip.h>

UDPReportSender::UDPReportSender()
  : QObject(nullptr)
{
  udp_sender_ = nullptr;
}

UDPReportSender::~UDPReportSender()
{
  if (udp_sender_) {
    delete udp_sender_;
    udp_sender_ = nullptr;
  }
}

bool UDPReportSender::Connect( const QString &ip_addr, int port )
{
  if (udp_sender_ == nullptr) {
    udp_sender_ = new UdpSender();
  }
  bool bind = false;
  int try_times = 5;
  do {
    QString info = QString("Try to connect udp send server %1:%2")
      .arg(ip_addr).arg(port);
    CONFROOM_LOG_DEBUG(qPrintable(info));

    bind = udp_sender_->connect(ip_addr.toStdString().c_str(), port);

    if(!bind) {
      info = QString("UDP sender connect failed with code:%1")
        .arg(GetLastError(), 0, 16);
      CONFROOM_LOG_ERROR(qPrintable(info));
    } else {
      CONFROOM_LOG_ERROR("UDP sender connect success.");
    };
    --try_times;
  } while (!bind && try_times > 0 );
  return bind;
}

void UDPReportSender::Close()
{
  if (udp_sender_) {
    udp_sender_->close();
  }
}

bool UDPReportSender::SendUDPQoSReport( const RtpStatItem &item )
{
  if (udp_sender_) {
    UDPReportHeader header = {
      UDPReportType::UdpReportType_RTP, 
      sizeof(RtpStatItem)
    };
    return SendPackage(header, (char *)&item);
  }
  return false;
}

bool UDPReportSender::SendUDPRecReport( const RecStatItem &item )
{
  if (udp_sender_) {
    UDPReportHeader header = {
      UDPReportType::UdpReportType_Record, 
      sizeof(RecStatItem)
    };
    return SendPackage(header, (char *)&item);
  }
  return false;
}

bool UDPReportSender::SendPackage( const UDPReportHeader &header, 
                                  const char * data )
{
  if(udp_sender_)
  {
    const unsigned int msg_size = header.size;
    const unsigned int header_size = sizeof(UDPReportHeader);
    char * msg = new char[header_size + msg_size + 1];
    memcpy_s(msg, header_size, reinterpret_cast<const char *>(&header), header_size);
    memcpy_s(msg + header_size, msg_size, data, msg_size);
    int bytes_send = udp_sender_->send(msg, header_size + msg_size);
    delete [] msg;
    return (bytes_send == header_size + msg_size);
  }
  return false;
}
