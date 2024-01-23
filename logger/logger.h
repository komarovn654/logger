//
//  logger.h
//  logger
//
//  Created by Николай Комаров on 21.01.2024.
//

#ifndef logger_h
#define logger_h

#include <stdio.h>
#include <execinfo.h>

#define LOG_BACKTRACE_BUF_SIZE 128

#define log_debug(message)     __log_debug(__FILE__, __func__, __LINE__, message);
#define log_info(message)       __log_info(__FILE__, __func__, __LINE__, message);
#define log_warning(message) __log_warning(__FILE__, __func__, __LINE__, message);
#define log_error(message) {\
    void* buffer = __log_refresh_backtrace_buf();\
    int size = backtrace(buffer, LOG_BACKTRACE_BUF_SIZE);\
    __log_error(__FILE__, __func__, __LINE__, message, buffer, size);\
}
#define log_panic(message)     __log_panic(__FILE__, __func__, __LINE__, message);

typedef enum {
    LOGTYPE_UNKNOWN = 0,
    LOGTYPE_DEBUG,
    LOGTYPE_PRODUCT,
} logger_type;

typedef enum {
    LOGOUT_UNKNOWN = 0,
    LOGOUT_STREAM,
    LOGOUT_FILE,
} logger_output;

typedef enum {
    LOGERR_NOERR = 0,
    LOGERR_LOGFILECREATE,
    LOGERR_LOGBUFFINIT,
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

typedef struct logger_src* logger;

logger_error log_init_default(void);
logger_error log_init(logger_settings* settings);
void log_destruct(void);
void* __log_refresh_backtrace_buf(void);

void   __log_debug(const char* file, const char* func, const int line, const char* mes);
void    __log_info(const char* file, const char* func, const int line, const char* mes);
void __log_warning(const char* file, const char* func, const int line, const char* mes);
void   __log_error(const char* file, const char* func, const int line, const char* mes, void* buffer, int size);
void   __log_panic(const char* file, const char* func, const int line, const char* mes);

#endif /* logger_h */
