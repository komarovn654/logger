#pragma "once"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <execinfo.h>

#if (defined(UTEST_BUILD) && UTEST_BUILD == 1)
    #define log_static
#else
    #define log_static static
#endif

#define LOG_BACKTRACE_BUF_SIZE 128

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_debug(...)   __log_log(LOGLEVEL_DEBUG,   __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_info(...)    __log_log(LOGLEVEL_INFO,    __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_warning(...) __log_log(LOGLEVEL_WARNING, __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_error(...)   __log_log(LOGLEVEL_ERROR,   __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_panic(message) {\
    __log_log(message, LOGLEVEL_PANIC, __FILENAME__, __func__, __LINE__); \
    void* buffer = __log_refresh_backtrace_buf();\
    int size = backtrace((void **)buffer, LOG_BACKTRACE_BUF_SIZE);\
    __log_backtrace(true, buffer, size);\
}

typedef enum {
    LOGTYPE_UNKNOWN = 0,
    LOGTYPE_DEBUG,
    LOGTYPE_PRODUCT,
} logger_type;

typedef enum {
    LOGLEVEL_DEBUG = 0,
    LOGLEVEL_INFO,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_PANIC,
    
    LOGLEVEL_COUNT,
} logger_level;

typedef enum {
    LOGOUT_UNKNOWN = 0,
    LOGOUT_STREAM,
    LOGOUT_FILE,
} logger_output;

typedef enum {
    LOGERR_NOERR = 0,
    LOGERR_LOGFILECREATE,
    LOGERR_LOGBUFFINIT,
    LOGERR_LOGUNKNOWNTYPE,
    LOGERR_LOGUNKNOWNOUTTYPE,
} logger_error;

typedef struct {
    logger_type type;
    logger_output out_type;
    union {
        FILE* out_stream;
        const char* file_name;
    } output;
    void (*error_callback)(void);
} logger_settings;

logger_error log_init_default(void);
logger_error log_init(logger_settings* settings);
void log_destruct(void);
char* log_get_internal_error(void);

void* __log_refresh_backtrace_buf(void);
void __log_log(logger_level level, const char* file, const char* func, const int line, const char* mes, ...);
void __log_backtrace(bool is_panic, void* buffer, int size);
