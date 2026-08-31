#include "eol_gateway.h"

#include <stdbool.h>

#include "gw_radio.h"

/* ========================================================================== */
/* Helpers                                                                    */
/* ========================================================================== */

static EolTestResult_t result_make(
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
     * Start from the safe state.
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

    /*
     * RX enabled, TX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_0,
        true,
        false);

    if (status != GW_RESULT_OK)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_0,
            false,
            false);

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
     * RX and TX must never be enabled simultaneously.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_0,
        true,
        true);

    if (status != GW_RESULT_INVALID_ARG)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_0,
            false,
            false);

        return result_make(
            EOL_TEST_FAIL,
            "G10 Radio 0 RX/TX control",
            "RX+TX simultaneous enable was not rejected");
    }

    /*
     * Always leave the RF switch in the safe state.
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
            "Failed to restore safe RX/TX state");
    }

    return result_make(
        EOL_TEST_PASS,
        "G10 Radio 0 RX/TX control",
        "Radio 0 RX/TX control states passed");
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
     * Start from the safe state.
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

    /*
     * RX enabled, TX disabled.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_1,
        true,
        false);

    if (status != GW_RESULT_OK)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_1,
            false,
            false);

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
     * RX and TX must never be enabled simultaneously.
     */
    status = gwRadioSetRxTx(
        GW_RADIO_1,
        true,
        true);

    if (status != GW_RESULT_INVALID_ARG)
    {
        (void)gwRadioSetRxTx(
            GW_RADIO_1,
            false,
            false);

        return result_make(
            EOL_TEST_FAIL,
            "G11 Radio 1 RX/TX control",
            "RX+TX simultaneous enable was not rejected");
    }

    /*
     * Always leave the RF switch in the safe state.
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
            "Failed to restore safe RX/TX state");
    }

    return result_make(
        EOL_TEST_PASS,
        "G11 Radio 1 RX/TX control",
        "Radio 1 RX/TX control states passed");
}

/* ========================================================================== */
/* Public EOL Gateway interface                                               */
/* ========================================================================== */

EolTestResult_t eol_gateway_run_test(
    EolTestId_t test_id)
{
    switch (test_id)
    {
        case EOL_G10_RADIO0_RXTX_CONTROL:
            return run_g10_radio0_rxtx_control();

        case EOL_G11_RADIO1_RXTX_CONTROL:
            return run_g11_radio1_rxtx_control();

        default:
            return result_make(
                EOL_TEST_NOT_APPLICABLE,
                "Gateway EOL",
                "Gateway test backend not implemented");
    }
}