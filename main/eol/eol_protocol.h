#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EOL_PKT_START       0x60
#define EOL_PKT_RESULT      0x61
#define EOL_PKT_SUMMARY     0x62
#define EOL_PKT_ACK         0x63
#define EOL_PKT_CANCEL      0x64

#define EOL_NAME_LEN        12
#define EOL_TOTAL_TESTS     16
#define FACTORY_GW_ID       0x00

#define EOL_ACK_OK          0x01
#define EOL_ACK_RETRY       0x02
#define EOL_ACK_CANCEL      0x03

#define EOL_START_SIZE      8
#define EOL_RESULT_SIZE     19
#define EOL_SUMMARY_SIZE    9
#define EOL_ACK_SIZE        6

#pragma pack(push, 1)

typedef struct {
    uint8_t node;
    uint8_t type;
    uint8_t seq;
    uint8_t total_tests;
    uint8_t fw_major;
    uint8_t fw_minor;
    uint8_t gwid;
    uint8_t crc8;
} LoRaEOLStart;

typedef struct {
    uint8_t node;
    uint8_t type;
    uint8_t seq;
    uint8_t test_idx;
    uint8_t pass;
    char    name[EOL_NAME_LEN];
    uint8_t gwid;
    uint8_t crc8;
} LoRaEOLResult;

typedef struct {
    uint8_t node;
    uint8_t type;
    uint8_t seq;
    uint8_t overall;
    uint8_t pass_count;
    uint8_t fail_count;
    uint8_t total;
    uint8_t gwid;
    uint8_t crc8;
} LoRaEOLSummary;

typedef struct {
    uint8_t node;
    uint8_t type;
    uint8_t seq;
    uint8_t result;
    uint8_t gwid;
    uint8_t crc8;
} LoRaEOLAck;

#pragma pack(pop)

_Static_assert(sizeof(LoRaEOLStart) == EOL_START_SIZE,
               "LoRaEOLStart size mismatch");

_Static_assert(sizeof(LoRaEOLResult) == EOL_RESULT_SIZE,
               "LoRaEOLResult size mismatch");

_Static_assert(sizeof(LoRaEOLSummary) == EOL_SUMMARY_SIZE,
               "LoRaEOLSummary size mismatch");

_Static_assert(sizeof(LoRaEOLAck) == EOL_ACK_SIZE,
               "LoRaEOLAck size mismatch");

uint8_t eol_crc8(const uint8_t *data, size_t length);

bool eol_validate_packet(const uint8_t *data,
                         size_t length,
                         uint8_t expected_type);
