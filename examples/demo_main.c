#include <stdlib.h>
#include "demo_funcs.h"
#include "logger.h"

static void func_2(void)
{
    log_warning("warning message");
    ext_func_1();
}

static void func_1(void)
{
    log_info("info message");
    func_2();
}

void mylog_error_callback(void)
{
    printf("%s", log_get_internal_error());
}

int main(void)
{
    if (log_init_default() != LOGERR_NOERR) {
        printf("Logger initialization error\n");
        return EXIT_FAILURE;
    }
    log_debug("debug message %u %lu", 12246, 12535);
    func_1();
    log_destruct();
    
    logger_settings settings= {
        .type = LOGTYPE_PRODUCT,
        .out_type = LOGOUT_FILE,
        .output.file_name = "log.log",
        .error_callback = mylog_error_callback,
    };
    if (log_init(&settings) != LOGERR_NOERR) {
        printf("Logger initialization error\n");
        return EXIT_FAILURE;
    }
    log_debug("debug message %d", 123);
    func_1();
    // log_panic("panic message");
    log_info("unreachable message");
}
