#include <stdio.h>
#include <string.h>

#include "eol_protocol.h"

static int tests_passed;
static int tests_failed;

static void check(const char *name, bool condition)
{
    if (condition) {
        printf("[PASS] %s\n", name);
        tests_passed++;
    } else {
        printf("[FAIL] %s\n", name);
        tests_failed++;
    }
}

void eol_protocol_run_tests(void)
{
    tests_passed = 0;
    tests_failed = 0;

    printf("\n=== EOL Protocol Self-Test ===\n");

    LoRaEOLStart start = {
        .node = 1,
        .type = EOL_PKT_START,
        .seq = 1,
        .total_tests = EOL_TOTAL_TESTS,
        .fw_major = 1,
        .fw_minor = 0,
        .gwid = FACTORY_GW_ID,
        .crc8 = 0
    };

    start.crc8 = eol_crc8((const uint8_t *)&start,
                          sizeof(start) - 1);

    check("Valid START",
          eol_validate_packet((const uint8_t *)&start,
                              sizeof(start),
                              EOL_PKT_START));

    LoRaEOLResult result = {
        .node = 1,
        .type = EOL_PKT_RESULT,
        .seq = 2,
        .test_idx = 0,
        .pass = 1,
        .name = "BOOT",
        .gwid = FACTORY_GW_ID,
        .crc8 = 0
    };

    result.crc8 = eol_crc8((const uint8_t *)&result,
                           sizeof(result) - 1);

    check("Valid RESULT",
          eol_validate_packet((const uint8_t *)&result,
                              sizeof(result),
                              EOL_PKT_RESULT));

    LoRaEOLSummary summary = {
        .node = 1,
        .type = EOL_PKT_SUMMARY,
        .seq = 3,
        .overall = 1,
        .pass_count = 16,
        .fail_count = 0,
        .total = 16,
        .gwid = FACTORY_GW_ID,
        .crc8 = 0
    };

    summary.crc8 = eol_crc8((const uint8_t *)&summary,
                            sizeof(summary) - 1);

    check("Valid SUMMARY",
          eol_validate_packet((const uint8_t *)&summary,
                              sizeof(summary),
                              EOL_PKT_SUMMARY));

    LoRaEOLAck ack = {
        .node = 1,
        .type = EOL_PKT_ACK,
        .seq = 4,
        .result = EOL_ACK_OK,
        .gwid = FACTORY_GW_ID,
        .crc8 = 0
    };

    ack.crc8 = eol_crc8((const uint8_t *)&ack,
                        sizeof(ack) - 1);

    check("Valid ACK",
          eol_validate_packet((const uint8_t *)&ack,
                              sizeof(ack),
                              EOL_PKT_ACK));

    uint8_t bad_crc[EOL_START_SIZE];

    memcpy(bad_crc, &start, sizeof(start));
    bad_crc[EOL_START_SIZE - 1] ^= 0xFF;

    check("Bad CRC rejected",
          !eol_validate_packet(bad_crc,
                               sizeof(bad_crc),
                               EOL_PKT_START));

    check("Wrong packet type rejected",
          !eol_validate_packet((const uint8_t *)&start,
                               sizeof(start),
                               EOL_PKT_RESULT));

    check("Short packet rejected",
          !eol_validate_packet((const uint8_t *)&start,
                               sizeof(start) - 1,
                               EOL_PKT_START));

    uint8_t extra_byte[EOL_START_SIZE + 1];

    memcpy(extra_byte, &start, sizeof(start));
    extra_byte[EOL_START_SIZE] = 0x00;

    check("Extra-byte packet rejected",
          !eol_validate_packet(extra_byte,
                               sizeof(extra_byte),
                               EOL_PKT_START));

    check("Unsupported CANCEL rejected",
          !eol_validate_packet((const uint8_t *)&start,
                               sizeof(start),
                               EOL_PKT_CANCEL));

    printf("=== EOL Protocol Self-Test Complete ===\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n\n", tests_failed);
}

int eol_protocol_tests_failed(void)
{
    return tests_failed;
}