#include "scheduler.h"

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <stdlib.h>

#include "log.h"
#include "WeblibUploader/workerservice.h"
#include "util/ini/TxConfig.h"

// Need to link with Iphlpapi.lib
#pragma comment(lib, "iphlpapi.lib")

// Need to link with Ole32.lib to print GUID
#pragma comment(lib, "ole32.lib")

// 本cpp部分代码引用了MSDN的示例，使用了malloc和free等C风格API


const int SCHEDULER_CHECK_INTERVAL = 
  CTxConfig::getInstance().GetSchCheckInterval(); // 检查周期毫秒数


Scheduler::Scheduler()
  : BaseLoopJob("Scheduler", SCHEDULER_CHECK_INTERVAL, NULL)
  , _uploadWorkService(nullptr)
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::Init()
{
  BaseLoopJob::Init();

  //为信号槽机制注册自定义类型
  qRegisterMetaType<NetworkInfo>("NetworkInfo");
  qRegisterMetaType<WeblibSettingInfo>("WeblibSettingInfo");

  RefreshAdapterInfo();
  _uploadWorkService = &WorkerService::getInstance();
  if (_uploadWorkService)
  {
    connect(this, &Scheduler::RequestResetWeblibUploader,
      _uploadWorkService, &WorkerService::slotSetWeblibSetting);
    connect(this, &Scheduler::RequestStartWeblibUploader,
      _uploadWorkService, &WorkerService::slotStartUpload);
    connect(this, &Scheduler::RequestStopWeblibUploader,
      _uploadWorkService, &WorkerService::slotStopUpload);

    // 启动Weblib上传
    emit RequestStartWeblibUploader();
  }
}

void Scheduler::Run()
{
  ScheduleNetwork();
}

void Scheduler::RefreshAdapterInfo()
{
  LOG_PRINTF_EX(_logger,"Refreshing network adapter...");
  // Declare and initialize variables.
  DWORD dwSize = 0;
  DWORD dwRetVal = 0;
  _adapterInfo.adapter_index = -1;
  
  PIP_ADAPTER_INFO pAdapterInfo = NULL;
  ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
  if(pAdapterInfo == NULL)
  {
    LOG_PRINTF_EX(_logger,"Error allocating memory needed to call GetAdaptersinfo");
    return;
  }

  // Make an initial call to GetAdaptersInfo to get
  // the necessary size into the ulOutBufLen variable
  if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
  {
    free(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
    if(pAdapterInfo == NULL)
    {
      LOG_PRINTF_EX(_logger,"Error allocating memory needed to call GetAdaptersinfo");
      return;
    }
  }

  if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
  {
    PIP_ADAPTER_INFO pAdapter = pAdapterInfo;

    //TODO:存在多适配器时，应从配置中读取当前使用的网络适配器
    //由于终端上只安装一块网卡，故目前只取第一个
    while(pAdapter)
    {
      if (-1 == _adapterInfo.adapter_index)
      {
        _adapterInfo.adapter_index = pAdapter->Index; 
        _adapterInfo.adapter_description = 
          QString::fromLocal8Bit(pAdapter->Description);
        GetInterfaceInfo(_adapterInfo); // 获取初始信息
      }
      LOG_PRINTF_EX(_logger,"Adapter found: %s\n", pAdapter->Description);
      pAdapter = pAdapter->Next;
    }
  }

  free(pAdapterInfo);
}

bool Scheduler::GetInterfaceInfo( NetworkInfo &info )
{
  PMIB_IF_ROW2 pIfRow = (PMIB_IF_ROW2)malloc(sizeof(MIB_IF_ROW2));
  if (NULL == pIfRow)
  {
    LOG_ERROR_EX(_logger,"Allocate mem for MIB_IF_ROW2 failed.");
    return false;
  }
  // Make sure the ifRow is zeroed out
  SecureZeroMemory((PVOID)pIfRow, sizeof(MIB_IF_ROW2) );

  pIfRow->InterfaceIndex = info.adapter_index; 
  if (NO_ERROR == GetIfEntry2(pIfRow))
  {
    info.transmit_bandwidth = pIfRow->TransmitLinkSpeed;
    info.receive_bandwidth = pIfRow->ReceiveLinkSpeed;
    //注意计算速度以后再更新总计字节数
    if (-1 == info.adapter_index)
    {
      info.in_speed_in_bytes = 0;
      info.out_speed_in_bytes = 0;
    }
    else
    {
      const unsigned int interval_secs = SCHEDULER_CHECK_INTERVAL / 1000;
      info.in_speed_in_bytes = 
        (pIfRow->InOctets - info.in_bytes) / interval_secs;
      info.out_speed_in_bytes = 
        (pIfRow->OutOctets - info.out_bytes) / interval_secs;
    }
    //总计字节数
    info.in_bytes = pIfRow->InOctets;
    info.out_bytes = pIfRow->OutOctets;
  }
  else
  {
    LOG_ERROR_EX(_logger,"Get interface info of adapter of index %d failed.", 
      info.adapter_index);
    free(pIfRow);
    return false;
  }
  free(pIfRow);
  return true;
}

void Scheduler::ScheduleNetwork()
{
  if (-1 == _adapterInfo.adapter_index)
  {
    RefreshAdapterInfo();
    if (-1 == _adapterInfo.adapter_index)
    {
      LOG_ERROR_EX(_logger,"Invalid adapter index.");
      return;
    }
  }

  if (GetInterfaceInfo(_adapterInfo))
  {
    LOG_PRINTF_EX(_logger,"Adapter: %s\n\tDownload: %8.2f kB/s\n\tUpload  : %8.2f kB/s",
      qPrintable(_adapterInfo.adapter_description),
      _adapterInfo.in_speed_in_bytes/1000.0,
      _adapterInfo.out_speed_in_bytes/1000.0);
    // 控制上传速度
    if (!_uploadWorkService)
    {
      return;
    }

    CTxConfig &tx_config = CTxConfig::getInstance();
    WeblibSettingInfo upload_setting = _uploadWorkService->getWeblibSetting();
    // 计算可使用的最大带宽
    const double trans_util = tx_config.GetSchTransmitUtilization();
    const double upload_util = tx_config.GetSchUploadUtilization();
    const quint64 max_trans_bytes = 
      _adapterInfo.transmit_bandwidth * trans_util / 8;
    const quint64 max_upload_bytes = 
      _adapterInfo.transmit_bandwidth * upload_util / 8;

    // 借鉴TCP的AIMD方式
    // 如果当前上行超过阈值
    if (_adapterInfo.out_speed_in_bytes > max_trans_bytes)
    {
      upload_setting.uploadspeed /= 2;
    }
    else
    {
      upload_setting.uploadspeed += tx_config.GetSchUploadSpeedIncrement();
      if (max_upload_bytes < upload_setting.uploadspeed)
      {
        upload_setting.uploadspeed = max_upload_bytes;
      }
    }
    LOG_PRINTF_EX(_logger,"Weblib upload limit: %I64d kB/s", upload_setting.uploadspeed/1000);
    // 应用设置
    emit RequestResetWeblibUploader(upload_setting); // 跨线程调用
    // 通知界面
    emit NotifyNetworkScheduleSignal(_adapterInfo, upload_setting.uploadspeed);
  }
}
