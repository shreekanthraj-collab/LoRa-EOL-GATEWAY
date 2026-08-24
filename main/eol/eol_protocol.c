#include "eol_protocol.h"

uint8_t eol_crc8(
    const uint8_t *data,
    size_t length)
{
    uint8_t crc = 0x00;

    if (data == NULL)
    {
        return 0U;
    }

    for (size_t i = 0U; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x80U) != 0U)
            {
                crc = (uint8_t)((crc << 1U) ^ 0x31U);
            }
            else
            {
                crc <<= 1U;
            }
        }
    }

    return crc;
}

bool eol_validate_packet(
    const uint8_t *data,
    size_t length,
    uint8_t expected_type)
{
    size_t expected_length;

    if (data == NULL)
    {
        return false;
    }

    switch (expected_type)
    {
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

        case EOL_PKT_TEST_REQUEST:
            expected_length = EOL_TEST_REQUEST_SIZE;
            break;

        default:
            return false;
    }

    if (length != expected_length)
    {
        return false;
    }

    if (data[1] != expected_type)
    {
        return false;
    }

    uint8_t received_crc = data[length - 1U];

    uint8_t calculated_crc = eol_crc8(
        data,
        length - 1U);

    return received_crc == calculated_crc;
}