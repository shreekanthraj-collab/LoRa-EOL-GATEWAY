#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint32_t timeout_ms;
} EolLoraTransportConfig_t;

bool eol_lora_transport_init(
    const EolLoraTransportConfig_t *config);

bool eol_lora_transport_send(
    const uint8_t *data,
    size_t length);

bool eol_lora_transport_receive(
    uint8_t *data,
    size_t capacity,
    size_t *length);

bool eol_lora_transport_start_receive(void);

bool eol_lora_transport_stop_receive(void);