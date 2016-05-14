#include "baseloopjob.h"

#include "Log.h"

BaseLoopJob::BaseLoopJob(const char * name, const int msec, QObject *parent)
  : QObject(parent)
  , _jobName(name)
  , _interval(msec)
  , _logger(nullptr)
{
}

BaseLoopJob::~BaseLoopJob()
{

}

void BaseLoopJob::Timeout()
{
  LOG_PRINTF_EX(_logger, "Running %s in thread %d...", 
    _jobName, QThread::currentThreadId());
  Run();
}

void BaseLoopJob::Init()
{
  moveToThread(&_thread);

  _logger = CreateLogger();
  _logger->SetModuleName(_jobName);

  _timer.setInterval(_interval);
  _timer.moveToThread(&_thread);

  QObject::connect(&_timer, &QTimer::timeout, 
    this, &BaseLoopJob::Timeout);
  QObject::connect(&_thread, SIGNAL(started()), 
    &_timer, SLOT(start())); // 必须通过信号槽启动计时器，因为在主线程中调用Start，计时器工作在内部线程
  QObject::connect(&_thread, SIGNAL(finished()), 
    &_timer, SLOT(stop()));
}

void BaseLoopJob::Start()
{
  _thread.start(); // 启动工作线程
  emit StartJobSignal();
}

void BaseLoopJob::Stop()
{
  emit StopJobSignal();
  _thread.quit();
}
