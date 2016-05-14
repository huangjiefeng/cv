#include "udp_report_receiver.h"

#include <assert.h>

#include <util/udp/UdpReceiver.h>
#include <util/report/ReporterCommon.h>
#include <log/Log.h>

UDPReportReceiver::UDPReportReceiver(QObject *parent)
    : QObject(parent) {
  udp_receiver_ = nullptr;
  terminate_ = false;
}

UDPReportReceiver::~UDPReportReceiver() {
  if (udp_receiver_) {
    delete udp_receiver_;
    udp_receiver_ = nullptr;
  }
}

bool UDPReportReceiver::Bind( const QString &ip_addr, int port ) {
  terminate_ = false;
  udp_receiver_ = new UdpReceiver();
  bool bind = false;
  int try_times = 5;
  do {
    QString info = QString("Try to bind udp receive server %1:%2")
      .arg(ip_addr).arg(port);
    CONFROOM_LOG_DEBUG(qPrintable(info));

    bind = udp_receiver_->bind(ip_addr.toStdString().c_str(), port);

    if(!bind) {
      info = QString("UDP receiver bind failed with code:%1")
        .arg(GetLastError(), 0, 16);
      CONFROOM_LOG_ERROR(qPrintable(info));
    } else {
      CONFROOM_LOG_ERROR("UDP receiver bind success.");
    };
    --try_times;
  } while (!bind && try_times > 0 );
  return bind;
}

void UDPReportReceiver::StartSlot() {
  while(!terminate_ && udp_receiver_->isBinded()) {
    char data[1024] = {0};
    int size = udp_receiver_->receive(&data, sizeof(data));

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
            emit ReceiveUDPQoSReportSignal(reinterpret_cast<const RtpStatItem&>(*item));
        }
        break;
    case UDPReportType::UdpReportType_Record:
        if (header->size == sizeof(RecStatItem)) {
            const RecStatItem * item =
                reinterpret_cast<const RecStatItem *>(data + sizeof(UDPReportHeader));
            emit ReceiveUDPRecReportSignal(reinterpret_cast<const RecStatItem&>(*item));
        }
        break;
    case UDPReportType::UdpReportType_FilePlay:
        if (header->size == sizeof(FilePlayStatItem)) {
            const FilePlayStatItem * item =
                reinterpret_cast<const FilePlayStatItem *>(data + sizeof(UDPReportHeader));
            emit ReceiveUDPFilePlayReportSignal(reinterpret_cast<const FilePlayStatItem&>(*item));
        }
      break;
    default:
        break;
    }
  }
}

void UDPReportReceiver::StopSlot() {
  terminate_ = true;
}


