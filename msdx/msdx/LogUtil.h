#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include "log\Log.h"


//若fail_cond条件成立，则返回ret_value
#define CHK_FAIL_RETURN(fail_cond, ret_value) \
    if ((fail_cond)) return (ret_value);

//若fail_cond条件成立，则返回ret_value，同时按照msg_fmt格式输出错误信息
#define CHK_FAIL_MSG_RETURN(fail_cond, ret_value, msg_fmt, ...) \
    if ((fail_cond)) { \
        LOG_ERROR((msg_fmt), __VA_ARGS__); \
        return (ret_value); \
    }


#endif