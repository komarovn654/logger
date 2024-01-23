//
//  logger.c
//  logger
//
//  Created by Николай Комаров on 21.01.2024.
//

#include "logger.h"
#include <execinfo.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <sys/stat.h>

#define DATE_BUF_SIZE 32
#define TIME_BUF_SIZE 32
#define MESSAGE_BUF_SIZE  512
#define INTERR_BUF_SIZE  128
#define BACKTRACE_BUF_SIZE 128

static struct logger_src {
    logger_type type;
    FILE* out_stream;
    
    char* date_buf;
    char* time_buf;
    char* message_buf;
    void* backtrace_buf;
    
    char* int_err;
    void (*error_callback)(void);

    void (*writer)(void);
} log_obj;

static void log_error_callback_default(void) {}

static logger_error log_buffers_init(void)
{
    log_obj.int_err = (char*)malloc(INTERR_BUF_SIZE * sizeof(char));
    if (log_obj.int_err == NULL) {
        return LOGERR_LOGBUFFINIT;
    }

    log_obj.date_buf = (char*)malloc(DATE_BUF_SIZE * sizeof(char));
    if (log_obj.date_buf == NULL) {
        return LOGERR_LOGBUFFINIT;
    }
    
    log_obj.time_buf = (char*)malloc(TIME_BUF_SIZE * sizeof(char));
    if (log_obj.time_buf == NULL) {
        return LOGERR_LOGBUFFINIT;
    }

    log_obj.message_buf = (char*)malloc(MESSAGE_BUF_SIZE * sizeof(char));
    if (log_obj.message_buf == NULL) {
        return LOGERR_LOGBUFFINIT;
    }
    
    log_obj.backtrace_buf = (char*)malloc(MESSAGE_BUF_SIZE * sizeof(char));
    if (log_obj.backtrace_buf == NULL) {
        return LOGERR_LOGBUFFINIT;
    }

    return LOGERR_NOERR;
}

static void log_date_update(void)
{
    if (log_obj.date_buf == NULL) {
        return;
    }
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    memset(log_obj.date_buf, 0, DATE_BUF_SIZE);
    memset(log_obj.time_buf, 0, TIME_BUF_SIZE);
 
    snprintf(log_obj.date_buf, DATE_BUF_SIZE, "%i.%i.%i ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(log_obj.time_buf, TIME_BUF_SIZE, "%i:%i:%i", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

static void log_write_std(void) {
    fprintf(log_obj.out_stream,"%s", log_obj.message_buf);
}

static void log_write_file(void) {
    size_t len = strlen(log_obj.message_buf);
    if (len != fwrite(log_obj.message_buf, len, sizeof(char), log_obj.out_stream)) {
        // TODO
    }
//    if (!fflush(log_obj.out_stream)){
//        // TODO
//    }
}

static logger_error log_set_out_file(const char* file_name)
{
    FILE* f = fopen(file_name, "w");
    if (f == NULL) {
        return LOGERR_LOGFILECREATE;
    }
    
    log_obj.out_stream = f;
    log_obj.writer = log_write_file;
    return LOGERR_NOERR;
}

static void log_form_string(const char* type, const char* file, const char* func, const int line, const char* mes)
{
    memset(log_obj.message_buf, 0, MESSAGE_BUF_SIZE);
    switch (log_obj.type) {
        case LOGTYPE_DEBUG:
            snprintf(log_obj.message_buf,  MESSAGE_BUF_SIZE, "%s%s%s%s::%s::%i::%s\n",
                     log_obj.date_buf, log_obj.time_buf, type, file, func, line, mes);
            break;
        case LOGTYPE_PRODUCT:
            snprintf(log_obj.message_buf,  MESSAGE_BUF_SIZE, "%s%s%s%s\n",
                     log_obj.date_buf, log_obj.time_buf, type, mes);
            break;
        default:
            // TODO
            break;
    }
}

static int log_write_callstack(void* buffer, int size)
{
    char **strings = backtrace_symbols(buffer, size);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    memset(log_obj.message_buf, 0, MESSAGE_BUF_SIZE);
    for (int i = 0, j = 0; i < size; i++) {
        sprintf(&log_obj.message_buf[j], "%s\n", strings[i]);
        j += strlen(strings[i]) + 1;
    }
    
//    for (int i = 0; i < size; ++i){
//        if (strlen(strings[i]) != fwrite(strings[i], 1, strlen(strings[i]), f)){
//            printf("logging_error: Error writing Error to log\n");
//            free(strings);
//            return 1;
//        }
//        fprintf(f, "\n");
//    }
//
//    if (fflush(f))
//        printf("logging_error: Error writing Error to log\n");

    free(strings);
    return 0;
}

void* __log_refresh_backtrace_buf(void)
{
    memset(log_obj.backtrace_buf, 0, BACKTRACE_BUF_SIZE);
    return log_obj.backtrace_buf;
}

logger_error log_init_default(void)
{
    log_obj.type = LOGTYPE_DEBUG;
    log_obj.out_stream = stderr;
    log_obj.writer = log_write_std;
    log_obj.error_callback = log_error_callback_default;
    return log_buffers_init();
}

logger_error log_init(logger_settings* settings)
{
    if (settings == NULL) {
        return log_init_default();
    }
    logger_error err = LOGERR_NOERR;
    
    log_obj.type = (settings->type == LOGTYPE_UNKNOWN) ? LOGTYPE_UNKNOWN : settings->type;
    log_obj.error_callback = (settings->error_callback == NULL) ? log_error_callback_default : settings->error_callback;
    
    switch (settings->out_type) {
        case LOGOUT_STREAM:
            log_obj.out_stream = (settings->output.out_stream != stderr && settings->output.out_stream != stdout) ?
                stderr : settings->output.out_stream;
            log_obj.writer = log_write_std;
            break;
        case LOGOUT_FILE:
            err = log_set_out_file(settings->output.file_name);
            if (err != LOGERR_NOERR) {
                return err;
            }
            break;
        default:
        // TODO
            break;
    }
    
    return log_buffers_init();
}

void log_destruct(void)
{
    free(log_obj.date_buf);
    free(log_obj.time_buf);
    free(log_obj.int_err);
    free(log_obj.message_buf);
}

void __log_debug(const char* file, const char* func, const int line, const char* mes)
{
    if (log_obj.message_buf == NULL) {
        return;
    }

    log_date_update();
    log_form_string("::DEBUG::", file, func, line, mes);
    log_obj.writer();
}

void __log_info(const char* file, const char* func, const int line, const char* mes)
{
    if (log_obj.message_buf == NULL) {
        return;
    }

    log_date_update();
    log_form_string("::INFO::", file, func, line, mes);
    log_obj.writer();
}

void __log_warning(const char* file, const char* func, const int line, const char* mes)
{
    if (log_obj.message_buf == NULL) {
        return;
    }

    log_date_update();
    log_form_string("::WARNING::", file, func, line, mes);
    log_obj.writer();
}

void __log_error(const char* file, const char* func, const int line, const char* mes, void* buffer, int size)
{
    if (log_obj.message_buf == NULL) {
        return;
    }

    log_date_update();
    log_form_string("::ERROR::", file, func, line, mes);
    log_obj.writer();
    
    log_write_callstack(buffer, size);
    log_obj.writer();
}

void __log_panic(const char* file, const char* func, const int line, const char* mes)
{
    
}
