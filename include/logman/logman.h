#pragma "once"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* LOGMANAPI is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(_LOGMAN_BUILD_DLL)
 /* We are building LOGMAN as a Win32 DLL */
 #define LOGMANAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(LOGMAN_DLL)
 /* We are calling a LOGMAN Win32 DLL */
 #define LOGMANAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(_LOGMAN_BUILD_DLL)
 /* We are building LOGMAN as a Unix shared library */
 #define LOGMANAPI __attribute__((visibility("default")))
#else
 #define LOGMANAPI
#endif

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

LOGMANAPI logman_error log_init_default(void);
LOGMANAPI logman_error log_init(logman_settings* settings);
LOGMANAPI void log_destruct(void);
LOGMANAPI char* log_get_internal_error(void);

LOGMANAPI void __log_log(logman_level level, const char* file, const char* func, const int line, const char* mes, ...);
