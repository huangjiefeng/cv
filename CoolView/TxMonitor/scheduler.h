#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "BaseLoopJob.h"
#include "scheduler_types.h"


class WorkerService;
struct WeblibSettingInfo;

//调度器，用于检测运行状况，并启停和控制相应工作，确保TX运行正常
//目前只是先了上传调度
class Scheduler : public BaseLoopJob
{
  Q_OBJECT

public:
  Scheduler();
  ~Scheduler();

  virtual void Init();

signals:
  //通知网络调度相关信息
  void NotifyNetworkScheduleSignal(
    const NetworkInfo &info,
    quint64 weblib_speed_limit);

  // TODO:Weblib操作可以用一个代理类封起来
  void RequestResetWeblibUploader(
    const WeblibSettingInfo &setting);
  void RequestStartWeblibUploader();
  void RequestStopWeblibUploader();

protected:
  virtual void Run();

private:
  //刷新适配器信息
  void RefreshAdapterInfo();
  //获取接口网络数据信息，info参数即使输入参数，也是输出参数
  bool GetInterfaceInfo(NetworkInfo &info);
  //网络调度
  void ScheduleNetwork();

private:
  NetworkInfo _adapterInfo;
  WorkerService *_uploadWorkService; // 注意指向的对象工作在其他线程
    // 跨线程调用应该使用信号槽机制
};

#endif // SCHEDULER_H
