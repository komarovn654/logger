#pragma "once"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_debug(...)      __log_log(LOGLEVEL_DEBUG,   __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_info(...)       __log_log(LOGLEVEL_INFO,    __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_warning(...)    __log_log(LOGLEVEL_WARNING, __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define log_error(...)      __log_log(LOGLEVEL_ERROR,   __FILENAME__, __func__, __LINE__, __VA_ARGS__)

typedef enum {
    LOGTYPE_UNKNOWN = 0,
    LOGTYPE_DEBUG,
    LOGTYPE_PRODUCT,
} logman_type;

typedef enum {
    LOGLEVEL_DEBUG = 0,
    LOGLEVEL_INFO,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    
    LOGLEVEL_COUNT,
} logman_level;

typedef enum {
    LOGOUT_UNKNOWN = 0,
    LOGOUT_STREAM,
    LOGOUT_FILE,
} logman_output;

typedef enum {
    LOGERR_NOERR = 0,
    LOGERR_LOGFILECREATE,
    LOGERR_LOGBUFFINIT,
    LOGERR_LOGUNKNOWNTYPE,
    LOGERR_LOGUNKNOWNOUTTYPE,
    LOGERR_LOGBUFOVERFLOW,
} logman_error;

typedef struct {
    logman_type type;
    logman_output out_type;
    union {
        FILE* out_stream;
        const char* file_name;
    } output;
    void (*error_callback)(void);
} logman_settings;

logman_error log_init_default(void);
logman_error log_init(logman_settings* settings);
void log_destruct(void);
char* log_get_internal_error(void);

void __log_log(logman_level level, const char* file, const char* func, const int line, const char* mes, ...);
