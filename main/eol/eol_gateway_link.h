#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t node_id;
    uint32_t timeout_ms;
} EolGatewayLinkConfig_t;

bool eol_gateway_link_init(
    const EolGatewayLinkConfig_t *config);

bool eol_gateway_link_send(
    const uint8_t *data,
    size_t length);

bool eol_gateway_link_receive(
    uint8_t *data,
    size_t capacity,
    size_t *length);

bool eol_gateway_link_start_receive(void);

bool eol_gateway_link_stop_receive(void);