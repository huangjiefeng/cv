#include "ortp_logger.h"

void OrtpLogHandler( OrtpLogLevel lev, const char *fmt, va_list args )
{
  static ILogger *ortp_logger = nullptr;
  if (!ortp_logger) {
    ortp_logger = CreateLogger();
    ortp_logger->SetModuleName("ortp");
  }

  char msg[250]; //每行日志不可超过200
  sprintf_s(msg, sizeof(msg), fmt, args);

  switch(lev){
  case ORTP_DEBUG:
    LOG_DEBUG_EX(ortp_logger, "[tid%d] %s", 
      GetCurrentThreadId(), msg);
    break;
  case ORTP_MESSAGE:
    LOG_INFO_EX(ortp_logger, "[tid%d] %s", 
      GetCurrentThreadId(), msg);
    break;
  case ORTP_WARNING:
    LOG_WARN_EX(ortp_logger, "[tid%d] %s", 
      GetCurrentThreadId(), msg);
    break;
  case ORTP_ERROR:
  case ORTP_FATAL:
    LOG_ERROR_EX(ortp_logger, "[tid%d] %s", 
      GetCurrentThreadId(), msg);
    break;
  default:
    ;//ortp_fatal("Bad level !");
  }
}
