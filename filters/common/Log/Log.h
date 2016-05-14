#pragma once

#ifndef LOG_H
#define LOG_H

#include <wchar.h>



class ILogger
{
public:
  enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
  };

public:
  virtual ~ILogger() {};

  virtual void SetModuleName(const char *mod) = 0;

  virtual void LogMsg(LogLevel level, const char *function, int line, const char *fmt, ...) = 0;
  virtual void LogMsg(LogLevel level, const char *function, int line, const wchar_t *fmt, ...) = 0;
  virtual void LogPrint(const char *fmt, ...) = 0;
  virtual void LogPrint(const wchar_t *fmt, ...) = 0;
};

//set the log prefix
void SetLogComponent(const char *name);

//get log object
ILogger *CreateLogger();
ILogger *GetDefaultLogger();


//方便外部使用的宏,带有EX的是可以指定logger对象的
#define LOG_ERROR_EX(_logger_ptr_, _fmt_, ...) \
    if (_logger_ptr_) _logger_ptr_->LogMsg(ILogger::LOG_LEVEL_ERROR, \
            __FUNCTION__, \
            __LINE__,\
            _fmt_, \
            __VA_ARGS__) 

#define LOG_ERROR(_fmt_, ...) LOG_ERROR_EX(GetDefaultLogger(), _fmt_, __VA_ARGS__)


#define LOG_WARN_EX(_logger_ptr_, _fmt_, ...)\
    if (_logger_ptr_) _logger_ptr_->LogMsg(ILogger::LOG_LEVEL_WARN, \
            __FUNCTION__, \
            __LINE__,\
            _fmt_, \
            __VA_ARGS__) 

#define LOG_WARN(_fmt_, ...) LOG_WARN_EX(GetDefaultLogger(), _fmt_, __VA_ARGS__)


#define LOG_INFO_EX(_logger_ptr_, _fmt_, ...)\
    if (_logger_ptr_) _logger_ptr_->LogMsg(ILogger::LOG_LEVEL_INFO, \
            __FUNCTION__, \
            __LINE__,\
            _fmt_, \
            __VA_ARGS__) 

#define LOG_INFO(_fmt_, ...) LOG_INFO_EX(GetDefaultLogger(), _fmt_, __VA_ARGS__)


#ifdef _DEBUG 
#define LOG_DEBUG_EX(_logger_ptr_, _fmt_, ...)\
    if (_logger_ptr_) _logger_ptr_->LogMsg(ILogger::LOG_LEVEL_DEBUG, \
            __FUNCTION__, \
            __LINE__,\
            _fmt_, \
            __VA_ARGS__) 

#define LOG_DEBUG(_fmt_, ...) LOG_DEBUG_EX(GetDefaultLogger(), _fmt_, __VA_ARGS__) 
#else
#define LOG_DEBUG_EX(fmt, ...)
#define LOG_DEBUG(_fmt_, ...)
#endif // _DEBUG

#define LOG_PRINTF_EX(_logger_ptr_, _fmt_, ...)\
    if (_logger_ptr_) _logger_ptr_->LogPrint(_fmt_, \
            __VA_ARGS__) 

#define LOG_PRINTF(_fmt_, ...) LOG_PRINTF_EX(GetDefaultLogger(), _fmt_, __VA_ARGS__)

#endif
