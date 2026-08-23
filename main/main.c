#include <stdio.h>

#include "protocol_test.h"

void app_main(void)
{
    eol_protocol_run_tests();

    while (1) {
        printf("LoRa EOL Gateway alive\n");
    }
}