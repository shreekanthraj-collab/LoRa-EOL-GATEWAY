#include "eol_lora_transport.h"
#include "eol_gateway_link.h"

static EolGatewayLinkConfig_t s_config;
static bool s_initialized;

bool eol_gateway_link_init(
    const EolGatewayLinkConfig_t *config)
{
    if (config == NULL) {
        return false;
    }

    s_config = *config;
    s_initialized = true;

    return true;
}

bool eol_gateway_link_send(
    const uint8_t *data,
    size_t length)
{
    if (!s_initialized || data == NULL || length == 0U) {
        return false;
    }

    return false;
}

bool eol_gateway_link_receive(
    uint8_t *data,
    size_t capacity,
    size_t *length)
{
    if (!s_initialized ||
        data == NULL ||
        capacity == 0U ||
        length == NULL) {
        return false;
    }

    *length = 0U;

    return false;
}

bool eol_gateway_link_start_receive(void)
{
    if (!s_initialized) {
        return false;
    }

    return false;
}

bool eol_gateway_link_stop_receive(void)
{
    if (!s_initialized) {
        return false;
    }

    return false;
}