#ifndef ORTP_LOGGER_H
#define ORTP_LOGGER_H

#include "log/Log.h"
#include "ortp/ortp.h"

void OrtpLogHandler(OrtpLogLevel lev, const char *fmt, va_list args);

#endif
