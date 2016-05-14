#ifndef UDP_REPORT_RECEIVER_H
#define UDP_REPORT_RECEIVER_H

#include <QObject>

#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"
#include "util/report/FilePlayStat.h"
class UdpReceiver;

class UDPReportReceiver : public QObject {
  Q_OBJECT

public:
  UDPReportReceiver(QObject *parent);
  ~UDPReportReceiver();

  bool Bind(const QString &ip_addr, int port);

Q_SIGNALS:
  void ReceiveUDPQoSReportSignal(const RtpStatItem &item);
  void ReceiveUDPRecReportSignal(const RecStatItem &item);
  void ReceiveUDPFilePlayReportSignal(const FilePlayStatItem &item);

public Q_SLOTS:
  void StartSlot();
  void StopSlot();

private:
  UdpReceiver*	udp_receiver_;
  volatile bool	terminate_;
};

#endif // UDP_REPORT_RECEIVER_H
