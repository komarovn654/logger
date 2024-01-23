//
//  main.c
//  logger
//
//  Created by Николай Комаров on 23.01.2024.
//

#include "logger/logger.h"

#include <stdio.h>

static void func2(void)
{
    log_error("Hello");
}

static void func1(void)
{
    func2();
}

int main(int argc, const char * argv[]) {
    logger_settings settings = {
        .type = LOGTYPE_PRODUCT,
        .out_type = LOGOUT_FILE,
        .output.file_name = "/Users/nikolajkomarov/dev/log.txt",
    };
    
    log_init(&settings);
    
    log_error("Hello");
    func1();
    return 0;
}
