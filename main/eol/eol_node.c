#include "eol_node.h"

#include <string.h>

#include "eol_gateway_link.h"
#include "eol_protocol.h"

#define EOL_NODE_RX_BUFFER_SIZE    (EOL_RESULT_SIZE)
#define EOL_NODE_DEFAULT_GW_ID     FACTORY_GW_ID

static EolNodeConfig_t s_config;
static bool s_initialized;
static uint8_t s_sequence;

static EolTestResult_t eol_node_make_result(
    EolTestStatus_t status,
    const char *name,
    const char *detail)
{
    EolTestResult_t result = {
        .status = status,
        .name = name,
        .detail = detail
    };

    return result;
}

bool eol_node_init(
    const EolNodeConfig_t *config)
{
    EolGatewayLinkConfig_t link_config;

    if (config == NULL)
    {
        return false;
    }

    link_config.node_id = config->node_id;
    link_config.timeout_ms = config->timeout_ms;

    if (!eol_gateway_link_init(&link_config))
    {
        s_initialized = false;
        return false;
    }

    s_config = *config;
    s_sequence = 0U;
    s_initialized = true;

    return true;
}

EolTestResult_t eol_node_run_test(
    EolTestId_t test_id)
{
    const EolTestDefinition_t *definition;
    LoRaEOLTestRequest request;
    uint8_t response[EOL_NODE_RX_BUFFER_SIZE];
    size_t response_length = 0U;

    if (!s_initialized)
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            NULL,
            "Node EOL backend not initialized");
    }

    definition = eol_registry_get(test_id);

    if (definition == NULL)
    {
        return eol_node_make_result(
            EOL_TEST_NOT_FOUND,
            NULL,
            "Node EOL test not found");
    }

    if (definition->group != EOL_GROUP_NODE)
    {
        return eol_node_make_result(
            EOL_TEST_NOT_APPLICABLE,
            definition->name,
            "Test does not belong to Node group");
    }

    request = (LoRaEOLTestRequest) {
        .node = s_config.node_id,
        .type = EOL_PKT_TEST_REQUEST,
        .seq = s_sequence++,
        .test_idx = (uint8_t)test_id,
        .gwid = EOL_NODE_DEFAULT_GW_ID,
        .crc8 = 0U
    };

    request.crc8 = eol_crc8(
        (const uint8_t *)&request,
        sizeof(request) - 1U);

    if (!eol_gateway_link_start_receive())
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Failed to start Node receive");
    }

    if (!eol_gateway_link_send(
            (const uint8_t *)&request,
            sizeof(request)))
    {
        (void)eol_gateway_link_stop_receive();

        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Failed to send Node test request");
    }

    if (!eol_gateway_link_receive(
            response,
            sizeof(response),
            &response_length))
    {
        (void)eol_gateway_link_stop_receive();

        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "No valid Node test response");
    }

    (void)eol_gateway_link_stop_receive();

    if (!eol_validate_packet(
            response,
            response_length,
            EOL_PKT_RESULT))
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Invalid Node result packet");
    }

    const LoRaEOLResult *node_result =
        (const LoRaEOLResult *)response;

    if (node_result->node != s_config.node_id)
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Node ID mismatch");
    }

    if (node_result->test_idx != (uint8_t)test_id)
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Test ID mismatch");
    }

    if (node_result->seq != request.seq)
    {
        return eol_node_make_result(
            EOL_TEST_FAIL,
            definition->name,
            "Sequence mismatch");
    }

    if (node_result->pass != 0U)
    {
        return eol_node_make_result(
            EOL_TEST_PASS,
            definition->name,
            "Node test passed");
    }

    return eol_node_make_result(
        EOL_TEST_FAIL,
        definition->name,
        "Node test failed");
}