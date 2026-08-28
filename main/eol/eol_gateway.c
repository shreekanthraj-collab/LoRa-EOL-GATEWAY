#include "eol_gateway.h"

#include <stdbool.h>
#include <stdint.h>

#include "eol_types.h"
#include "eol_registry.h"
#include "gw_radio.h"

/* ========================================================================== */
/* Local helpers                                                              */
/* ========================================================================== */

static EolTestResult_t result_make(
    EolTestStatus_t status,
    const char *name,
    const char *detail)
{
    EolTestResult_t result = {
        .status = status,
        .name = name,
        .detail = detail,
    };

    return result;
}

/* ========================================================================== */
/* G10 - Radio 0 RX/TX control                                                */
/* ========================================================================== */

static EolTestResult_t run_g10_radio0_rxtx_control(void)
{
    GwResult_t status;

    if (!gwRadioIsInitialized(GW_RADIO_0))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G10 Radio 0 RX/TX control",
            "Radio 0 is not initialized");
    }

    /*
     * RX enabled, TX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_0,
        true,
        false);

    if (status != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G10 Radio 0 RX/TX control",
            "Failed to enable RX");
    }

    /*
     * TX enabled, RX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_0,
        false,
        true);

    if (status != GW_RESULT_OK)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_0,
            false,
            false);

        return result_make(
            EOL_TEST_FAIL,
            "G10 Radio 0 RX/TX control",
            "Failed to enable TX");
    }

    /*
     * Return RF switch to idle.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_0,
        false,
        false);

    if (status != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G10 Radio 0 RX/TX control",
            "Failed to disable RX/TX");
    }

    return result_make(
        EOL_TEST_PASS,
        "G10 Radio 0 RX/TX control",
        "Radio 0 RX/TX control passed");
}

/* ========================================================================== */
/* G11 - Radio 1 RX/TX control                                                */
/* ========================================================================== */

static EolTestResult_t run_g11_radio1_rxtx_control(void)
{
    GwResult_t status;

    if (!gwRadioIsInitialized(GW_RADIO_1))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G11 Radio 1 RX/TX control",
            "Radio 1 is not initialized");
    }

    /*
     * RX enabled, TX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_1,
        true,
        false);

    if (status != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G11 Radio 1 RX/TX control",
            "Failed to enable RX");
    }

    /*
     * TX enabled, RX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_1,
        false,
        true);

    if (status != GW_RESULT_OK)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_1,
            false,
            false);

        return result_make(
            EOL_TEST_FAIL,
            "G11 Radio 1 RX/TX control",
            "Failed to enable TX");
    }

    /*
     * Return RF switch to idle.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_1,
        false,
        false);

    if (status != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G11 Radio 1 RX/TX control",
            "Failed to disable RX/TX");
    }

    return result_make(
        EOL_TEST_PASS,
        "G11 Radio 1 RX/TX control",
        "Radio 1 RX/TX control passed");
}

/* ========================================================================== */
/* Gateway EOL dispatcher                                                     */
/* ========================================================================== */

EolTestResult_t eol_gateway_run_test(
    EolTestId_t test_id)
{
    const EolTestDefinition_t *definition;

    definition = eol_registry_get(test_id);

    if (definition == NULL)
    {
        return result_make(
            EOL_TEST_NOT_FOUND,
            "Unknown Gateway test",
            "Test ID not present in registry");
    }

    if (definition->group != EOL_GROUP_GATEWAY)
    {
        return result_make(
            EOL_TEST_NOT_APPLICABLE,
            definition->name,
            "Test does not belong to Gateway group");
    }

    switch (test_id)
    {
        case EOL_G10_RADIO0_RXTX_CONTROL:
            return run_g10_radio0_rxtx_control();

        case EOL_G11_RADIO1_RXTX_CONTROL:
            return run_g11_radio1_rxtx_control();

        default:
            return result_make(
                EOL_TEST_NOT_APPLICABLE,
                definition->name,
                "Gateway EOL backend not implemented");
    }
}