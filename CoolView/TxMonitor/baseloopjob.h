#ifndef BASELOOPJOB_H
#define BASELOOPJOB_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "log.h"

class BaseLoopJob : public QObject
{
  Q_OBJECT

public:
  BaseLoopJob(const char * name = NULL, const int msec = 10000, QObject *parent = NULL);
  ~BaseLoopJob();

  //重载此方法实现额外初始化工作，必须调用基类此方法
  virtual void Init();
  //开始循环工作，一般不需要重载
  virtual void Start();
  //停止循环工作，一般不需要重载
  virtual void Stop();

protected:
  //重载此方法实现每次循环需要执行的工作
  virtual void Run() = 0;

  // signals and slots
signals:
  void StartJobSignal();
  void StopJobSignal();

protected slots:
  //QTimer通过此方法定时调用Run，一般不需要重载
  virtual void Timeout();


  // members
protected:
  const char * _jobName;
  int _interval;

  QTimer  _timer;
  QThread _thread;

  ILogger *_logger;
  
};

#endif // BASELOOPJOB_H
