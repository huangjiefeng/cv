
#include <QtCore/QCoreApplication>

#include <muduo/base/Logging.h>

#include "BfcpClientService.h"

int main(int argc, char *argv[])
{
  muduo::Logger::setLogLevel(muduo::Logger::kTRACE);
  QCoreApplication a(argc, argv);
  BfcpClientService service(nullptr);
  service.Initialize();
  return a.exec();
}
