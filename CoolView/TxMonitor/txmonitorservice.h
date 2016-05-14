#ifndef TXMONITORSERVICE_H
#define TXMONITORSERVICE_H

#include <memory>

#include <QObject>

#include "dbus/txMonitor/common/TxMonitorServiceCommon.h"

class CVIniConfig;
class RecordDBController;
class ILogger;

class TxMonitorService : public QObject
{
  Q_OBJECT

public:
  TxMonitorService();
  ~TxMonitorService();

  void Initialize(std::shared_ptr<RecordDBController> db_controller);

// Adapter impl
public: // PROPERTIES
public Q_SLOTS: // METHODS
  void ReportRecStat(int stat_type, const QByteArray &data_garray);

Q_SIGNALS: // SIGNALS

private:
  CVIniConfig * config_;

  std::shared_ptr<RecordDBController> db_controller_;
  std::shared_ptr<ILogger> logger_;

  QString rec_path_;
  QString upload_path_;
};

#endif // TXMONITORSERVICE_H
