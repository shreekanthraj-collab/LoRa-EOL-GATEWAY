#include <stdio.h>

#include "eol_manager.h"
#include "protocol_test.h"
#include "nfw_delay.h"

void app_main(void)
{
    eol_protocol_run_tests();

    eol_manager_init();

    while (1)
    {
        eol_manager_run();

        (void)nfwDelayMs(20U);
    }
}