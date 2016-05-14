#include <QMetaType>
#include <QDir>
#include <QFile>
#include <QProcess>

#include <String>

#include "windows.h"

#include "util\ini\TxConfig.h"
#include "optimizer.h"
#include "Util.h"
#include "log.h"
#include "FileScanner.h"

Optimizer::Optimizer()
  : BaseLoopJob("Optimizer", 10000, NULL)
{

}

Optimizer::~Optimizer()
{

}

void Optimizer::Init()
{
  BaseLoopJob::Init();

  _recPath = QString::fromLocal8Bit(CTxConfig::getInstance().GetRecPath());
  _uploadPath = QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadPath());
  _cmd = CTxConfig::getInstance().GetOptCmd();
  _param = CTxConfig::getInstance().GetOptParam();
  _optErrorMark = "_optimizing_error";

  _proc.moveToThread(&_thread);
  qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
  QObject::connect(&_proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(OptCmdRunError(QProcess::ProcessError)));
  QObject::connect(&_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(OptCmdStdOutput()));
  QObject::connect(&_proc, SIGNAL(readyReadStandardError()), this, SLOT(OptCmdStdOutput()));

}

void Optimizer::Run()
{
  QString absPath = GetAbsolutePath(_recPath);
  if (!PathExist(absPath.toLocal8Bit().data()))
  {
    return;
  }

  if (_cmd.length() == 0)
  {
    LOG_ERROR_EX(_logger, "No optimizing prog specified");
    Stop();
    return;
  }

  if (!PathExist(qPrintable(_cmd)))
  {
    LOG_ERROR_EX(_logger, "Optimizing prog not exist");
    Stop();
    return;
  }

  if (_param.length() == 0)
  {
    LOG_ERROR_EX(_logger, "No optimizing param specified");
    Stop();
    return;
  }

  QStringList includeMark;
  includeMark << ".mp4";
  QStringList excludeMark;
  excludeMark << ".tmp" << _optErrorMark;

  FileInfoQueue queue;
  FileScanner::ScanFiles(queue, absPath, includeMark, excludeMark);

  while(!queue.empty())
  {
    QFileInfo info = queue.top();
    queue.pop();

    QString path = info.absoluteFilePath();
    LOG_PRINTF_EX(_logger,"Interleaving %s\n", path.toLocal8Bit().data());
    Interleave(path);
  }
}

void Optimizer::Interleave( QString file )
{
  file.replace("/", "\\");
  // Handle the capture file for fast pseudo-streaming
  QStringList params = (QStringList() << _param << file);

  //TODO: 通过第三方工具将MP4调整为ISO标准推荐的结构。
  //优化文件时造成大量资源耗费，以后尝试新的方式。
  //而且点播服务器会重分割文件，在此优化显得多余

  //_proc.start(_cmd, params);
  //if (!_proc.waitForFinished(-1))
  //{
  //  // error occur
  //  return;
  //}
  //
  //int ret = _proc.exitCode();
  //if (0 != ret)
  //{
  //  LOG_ERROR_EX(_logger, "Optimizing mp4 file error.");
  //  QString newName = file;
  //  newName.replace(".mp4", _optErrorMark + ".mp4");
  //  QFile::rename(file, newName);
  //  return;
  //}

  // Move to upload dir
  QString strNewFile = file;
  strNewFile.replace(_recPath, _uploadPath);
  LOG_PRINTF_EX(_logger,"\nMoving %s to %s.\n", qPrintable(file), qPrintable(strNewFile));
  MakeParentDir(qPrintable(strNewFile));
  MoveFile(qPrintable(file), qPrintable(strNewFile));

  // ------------------------Win32 Impl------------------------------
  /*
  strPostHandleProgramPath = GetAbsolutePath(strPostHandleProgramPath);

  std::string param = CTxConfig::getInstance().GetOptParam();
  if (param.empty())
  {
    printf_s("Optimizing prog param not exist\n");
    return;
  }

  // print cmd line template
  char szCmdLineTemp[1024] = {0};
  sprintf_s(szCmdLineTemp, sizeof(szCmdLineTemp)-1, "\"%s\" %s"
    , strPostHandleProgramPath.c_str()
    , strParam.c_str());

  // print cmd line param
  char szCmdLine[1024] = {0};
  sprintf_s(szCmdLine, sizeof(szCmdLine)-1, szCmdLineTemp, qPrintable(file));

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  int ret = CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

  if (!ret)
  {
    printf_s("Optimizing prog running error\n");
    return;
  }
  
  WaitForSingleObject( pi.hProcess, INFINITE );
  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );
  */

}

void Optimizer::OptCmdRunError( QProcess::ProcessError error )
{
  LOG_ERROR_EX(_logger, "Optimizer prog run error.(Code=%d)", error);
}

void Optimizer::OptCmdStdOutput()
{
  QString output = _proc.readAllStandardOutput();
  if (output.length() > 0)
  {
    LOG_ERROR_EX(_logger, "Optimizer prog std output: \n%s", output.toLocal8Bit().data());
  }
  output = _proc.readAllStandardError();
  if (output.length() > 0)
  {
    LOG_ERROR_EX(_logger, "Optimizer prog std error: \n%s", output.toLocal8Bit().data());
  }
}
