#include <stdlib.h>
#include "demo_funcs.h"
#include "../include/logman/logman.h"

static void func_2(void)
{
    log_warning("warning message");
    ext_func_1();
}

static void func_1(void)
{
    log_info("info message: %s %d", "info message", 94);
    func_2();
}

void mylog_error_callback(void)
{
    printf("%s", log_get_internal_error());
}

int main(void)
{
    if (log_init_default() != LOGERR_NOERR) {
        printf("logman initialization error\n");
        return EXIT_FAILURE;
    }
    log_debug("debug message: %s %lu", "stderr message", 12535);
    func_1();
    log_destruct();
    
    logman_settings settings= {
        .type = LOGTYPE_PRODUCT,
        .out_type = LOGOUT_FILE,
        .output.file_name = "log.log",
        .error_callback = mylog_error_callback,
    };
    if (log_init(&settings) != LOGERR_NOERR) {
        printf("logman initialization error\n");
        return EXIT_FAILURE;
    }
    log_debug("debug message");
    func_1();
    log_info("message");
}
