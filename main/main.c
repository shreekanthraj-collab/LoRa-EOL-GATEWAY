#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    while (1) {
        printf("LoRa EOL Gateway boot OK\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}