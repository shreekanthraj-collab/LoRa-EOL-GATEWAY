#include "eol_lora_transport.h"

#include "lora_transport.h"

static EolLoraTransportConfig_t s_config;
static bool s_initialized;

bool eol_lora_transport_init(
    const EolLoraTransportConfig_t *config)
{
    LoraTransportConfig_t lora_config;
    NfwStatus_t status;

    if (config == NULL) {
        return false;
    }

    lora_config = (LoraTransportConfig_t) {
        .spiHost = 2U,
        .frequencyHz = LORA_TRANSPORT_DEFAULT_FREQUENCY_HZ,
        .bandwidth = LORA_TRANSPORT_DEFAULT_BANDWIDTH,
        .spreadingFactor = LORA_TRANSPORT_DEFAULT_SPREADING_FACTOR,
        .codingRate = LORA_TRANSPORT_DEFAULT_CODING_RATE,
        .txPowerDbm = LORA_TRANSPORT_DEFAULT_TX_POWER_DBM,
    };

    status = loraTransportInit(&lora_config);

    if (status != NFW_STATUS_OK) {
        return false;
    }

    s_config = *config;
    s_initialized = true;

    return true;
}

bool eol_lora_transport_send(
    const uint8_t *data,
    size_t length)
{
    if (!s_initialized ||
        data == NULL ||
        length == 0U) {
        return false;
    }

    return loraTransportTransmit(
               data,
               (uint32_t)length,
               s_config.timeout_ms) == NFW_STATUS_OK;
}

bool eol_lora_transport_receive(
    uint8_t *data,
    size_t capacity,
    size_t *length)
{
    uint32_t received_length = 0U;

    if (!s_initialized ||
        data == NULL ||
        capacity == 0U ||
        length == NULL) {
        return false;
    }

    *length = 0U;

    if (loraTransportReceive(
            data,
            (uint32_t)capacity,
            &received_length,
            s_config.timeout_ms) != NFW_STATUS_OK) {
        return false;
    }

    *length = (size_t)received_length;

    return true;
}

bool eol_lora_transport_start_receive(void)
{
    if (!s_initialized) {
        return false;
    }

    return loraTransportStartReceive() == NFW_STATUS_OK;
}

bool eol_lora_transport_stop_receive(void)
{
    if (!s_initialized) {
        return false;
    }

    return loraTransportStandby() == NFW_STATUS_OK;
}