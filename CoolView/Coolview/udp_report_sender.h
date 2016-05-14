#ifndef UDP_REPORT_SENDER_H
#define UDP_REPORT_SENDER_H

#include <QObject>

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"
#include <util/report/ReporterCommon.h>

class UdpSender;

class UDPReportSender : public QObject
{
  Q_OBJECT

public:
  UDPReportSender();
  ~UDPReportSender();

  bool Connect(const QString &ip_addr, int port);
  void Close();

  bool SendUDPQoSReport(const RtpStatItem &item);
  bool SendUDPRecReport(const RecStatItem &item);

private:
  bool SendPackage( const UDPReportHeader &header, const char * data);

private:
  UdpSender*	udp_sender_;
    
};

#endif // UDP_REPORT_SENDER_H
