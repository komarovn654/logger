#include "demo_funcs.h"
#include "../include/logman/logman.h"

void ext_func_1(void)
{
    log_error("error message %s %d", "error", 43);
}
