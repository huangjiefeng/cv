#include "component_manager.h"

#include "txmonitorservice.h"
#include "tx_web_service_controller.h"
#include "record_db_controller.h"
#include "optimizer.h"
#include "cleaner.h"
#include "scheduler.h"

#include "util\ini\CVIniConfig.h"
#include "util\ini\TxConfig.h"


ComponentManager::ComponentManager(QObject *parent)
  : QObject(parent)
{

}

ComponentManager::~ComponentManager()
{

}

void ComponentManager::Initialize()
{
  bool is_model_tx = CVIniConfig::getInstance().IsModelTX();

  //db
  if (is_model_tx) {
    db_controller_.reset(new RecordDBController);
  }

  //dbus service
  tx_monitor_service_.reset(new TxMonitorService);
  tx_monitor_service_->Initialize(db_controller_);

  //考虑到TX性能问题，不再做文件结构优化了
  /*Optimizer *optimizer = new Optimizer();
  optimizer->Init();
  optimizer->Start();
  _optimizer = optimizer;*/

  //初始化磁盘维护程序
  Cleaner *cleaner = new Cleaner();
  cleaner->Init();
  cleaner->Start();
  cleaner_.reset(cleaner);

  if (!is_model_tx) {
    //非TX后面的组件不创建
    return;
  }

  //初始化云存储上传网速调度器
  Scheduler *scheduler = new Scheduler();
  scheduler->Init();
  scheduler->Start();
  scheduler_.reset(scheduler);

  //vod webservice组件
  vod_controller_.reset(new TxWebServiceController);
  vod_controller_->Initialize();
}
