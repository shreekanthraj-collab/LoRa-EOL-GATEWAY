#include "eol_protocol.h"

uint8_t eol_crc8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0x00;

    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (uint8_t)((crc << 1) ^ 0x31);
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

bool eol_validate_packet(const uint8_t *data,
                         size_t length,
                         uint8_t expected_type)
{
    if (data == NULL) {
        return false;
    }

    size_t expected_length;

    switch (expected_type) {
        case EOL_PKT_START:
            expected_length = EOL_START_SIZE;
            break;

        case EOL_PKT_RESULT:
            expected_length = EOL_RESULT_SIZE;
            break;

        case EOL_PKT_SUMMARY:
            expected_length = EOL_SUMMARY_SIZE;
            break;

        case EOL_PKT_ACK:
            expected_length = EOL_ACK_SIZE;
            break;

        default:
            return false;
    }

    if (length != expected_length) {
        return false;
    }

    if (data[1] != expected_type) {
        return false;
    }

    uint8_t received_crc = data[length - 1];
    uint8_t calculated_crc = eol_crc8(data, length - 1);

    return received_crc == calculated_crc;
}