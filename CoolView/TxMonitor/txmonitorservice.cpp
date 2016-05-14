#include "txmonitorservice.h"

#include <QtDBus/QDBusConnection>

#include <string>

#include "util\ini\CVIniConfig.h"
#include "util\ini\TxConfig.h"
#include "dbus\txMonitor\service\TxMonitorAdaptor.h"
#include "record_db_controller.h"
#include "Util.h"
#include "Ini.h"
#include "Log.h"


TxMonitorService::TxMonitorService()
  : config_(nullptr)
{
  new TxMonitorAdaptor(this);
}

TxMonitorService::~TxMonitorService()
{
}


void TxMonitorService::Initialize(
  std::shared_ptr<RecordDBController> db_controller)
{
  db_controller_ = db_controller;
  logger_.reset(CreateLogger());
  logger_->SetModuleName("dbus");

  // dbus
  QDBusConnection connection = QDBusConnection::sessionBus();
  bool ret = connection.registerService(TX_MONITOR_SERVICE_NAME);
  if (!ret) {
    LOG_ERROR_EX(logger_, "register service failed: %s", 
      qPrintable(connection.lastError().message()));
  }
  ret = connection.registerObject(TX_MONITOR_SERVICE_OBJECT_PATH ,this);
  if (!ret) {
    LOG_ERROR_EX(logger_, "register object failed: %s", 
      qPrintable(connection.lastError().message()));
  }

  //config
  config_ = &(CVIniConfig::getInstance());

  rec_path_ = QString::fromLocal8Bit(CTxConfig::getInstance().GetRecPath());
  upload_path_ = QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadPath());
}


void TxMonitorService::ReportRecStat( int stat_type, const QByteArray &data_garray )
{
  if (!config_->IsModelTX() || !db_controller_) {
    return;
  }

  if (stat_type == RecStat_FileClose) {
    RecordCompleteParam param;
    param = DBusParamFromByteArray<RecordCompleteParam>(data_garray);

    //将录制完的文件移动到待上传目录
    //同一分区内文件移动只是修改磁盘索引，并不移动文件数据，故几乎不会影响IO
    QString new_path = param.view.file;
    new_path.replace(rec_path_, upload_path_);
    MakeParentDir(qPrintable(new_path)); //创建文件夹，如果路径不存在下面移动文件会失败
    if (MoveFile(qPrintable(param.view.file), qPrintable(new_path))) {
      //修正路径（只存储相对路径，这样如果以后改录像根目录，也不影响数据库）
      param.view.file = GetRelativePath(upload_path_, new_path);
    } else {
      //移动失败时不修改当前的绝对路径，方便定位文件
      LOG_ERROR_EX(logger_, "move file failed:\n\tfrom: %s\n\tto: %s",
        qPrintable(param.view.file), qPrintable(new_path));
    }
    db_controller_->HandleRecordComplete(param);
  }
}

