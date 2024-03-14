#pragma once

#include "../include/logman/logman.h"

#if (defined(UTEST_BUILD) && UTEST_BUILD == 1)
    #define log_static
#else
    #define log_static static
#endif

#define DATE_BUF_SIZE      32
#define MESSAGE_BUF_SIZE   512
#define INTERR_BUF_SIZE    128

typedef struct logman_src {
    logman_output out_type;
    FILE* out_stream;
    
    char* date_buf;
    char* message_buf;
    
    char* err_message;
    void (*error_callback)(void);

    void (*writer)(char *buf);
    void (*message_former)(logman_level level, const char* file, const char* func, const int line, 
        const char* message, va_list va);
} logman_src;
