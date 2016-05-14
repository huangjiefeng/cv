#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <QString>
#include <QProcess>

#include "baseloopjob.h"

//优化录制后的文件结构，并将其移动到待上传目录
class Optimizer : public BaseLoopJob
{
  Q_OBJECT

public:
  Optimizer();
  ~Optimizer();

  void Init();

protected:
  void Run();

  void Interleave(QString file);

protected slots:
  void OptCmdRunError(QProcess::ProcessError error);
  void OptCmdStdOutput();

private:
  QString _recPath;
  QString _uploadPath;
  QString _cmd;
  QString _param;
  QString _optErrorMark;

  QProcess _proc;
  
};

#endif // OPTIMIZER_H
