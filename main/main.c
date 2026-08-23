#include <stdio.h>

#include "eol_manager.h"
#include "protocol_test.h"

void app_main(void)
{
    eol_protocol_run_tests();

    eol_manager_init();
    eol_manager_run();

    while (1) {
        printf("LoRa EOL Gateway alive\n");
    }
}