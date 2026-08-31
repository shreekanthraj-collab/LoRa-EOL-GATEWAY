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
/* G03 - Radio 0 RESET                                                        */
/* ========================================================================== */

static EolTestResult_t run_g03_radio0_reset(void)
{
    if (!gwRadioIsInitialized(GW_RADIO_0))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G03 Radio 0 RESET",
            "Radio 0 is not initialized");
    }

    if (gwRadioReset(GW_RADIO_0) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G03 Radio 0 RESET",
            "Radio 0 hardware reset failed");
    }

    return result_make(
        EOL_TEST_PASS,
        "G03 Radio 0 RESET",
        "Radio 0 reset command completed");
}

/* ========================================================================== */
/* G04 - Radio 0 BUSY                                                         */
/* ========================================================================== */

static EolTestResult_t run_g04_radio0_busy(void)
{
    bool busy = false;

    if (!gwRadioIsInitialized(GW_RADIO_0))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G04 Radio 0 BUSY",
            "Radio 0 is not initialized");
    }

    if (gwRadioGetBusy(
            GW_RADIO_0,
            &busy) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G04 Radio 0 BUSY",
            "Failed to read Radio 0 BUSY");
    }

    (void)busy;

    return result_make(
        EOL_TEST_PASS,
        "G04 Radio 0 BUSY",
        "Radio 0 BUSY GPIO read completed");
}

/* ========================================================================== */
/* G05 - Radio 0 DIO1                                                         */
/* ========================================================================== */

static EolTestResult_t run_g05_radio0_dio1(void)
{
    bool dio1 = false;

    if (!gwRadioIsInitialized(GW_RADIO_0))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G05 Radio 0 DIO1",
            "Radio 0 is not initialized");
    }

    if (gwRadioGetDio1(
            GW_RADIO_0,
            &dio1) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G05 Radio 0 DIO1",
            "Failed to read Radio 0 DIO1");
    }

    (void)dio1;

    return result_make(
        EOL_TEST_PASS,
        "G05 Radio 0 DIO1",
        "Radio 0 DIO1 GPIO read completed");
}

/* ========================================================================== */
/* G07 - Radio 1 RESET                                                        */
/* ========================================================================== */

static EolTestResult_t run_g07_radio1_reset(void)
{
    if (!gwRadioIsInitialized(GW_RADIO_1))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G07 Radio 1 RESET",
            "Radio 1 is not initialized");
    }

    if (gwRadioReset(GW_RADIO_1) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G07 Radio 1 RESET",
            "Radio 1 hardware reset failed");
    }

    return result_make(
        EOL_TEST_PASS,
        "G07 Radio 1 RESET",
        "Radio 1 reset command completed");
}

/* ========================================================================== */
/* G08 - Radio 1 BUSY                                                         */
/* ========================================================================== */

static EolTestResult_t run_g08_radio1_busy(void)
{
    bool busy = false;

    if (!gwRadioIsInitialized(GW_RADIO_1))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G08 Radio 1 BUSY",
            "Radio 1 is not initialized");
    }

    if (gwRadioGetBusy(
            GW_RADIO_1,
            &busy) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G08 Radio 1 BUSY",
            "Failed to read Radio 1 BUSY");
    }

    (void)busy;

    return result_make(
        EOL_TEST_PASS,
        "G08 Radio 1 BUSY",
        "Radio 1 BUSY GPIO read completed");
}

/* ========================================================================== */
/* G09 - Radio 1 DIO1                                                         */
/* ========================================================================== */

static EolTestResult_t run_g09_radio1_dio1(void)
{
    bool dio1 = false;

    if (!gwRadioIsInitialized(GW_RADIO_1))
    {
        return result_make(
            EOL_TEST_FAIL,
            "G09 Radio 1 DIO1",
            "Radio 1 is not initialized");
    }

    if (gwRadioGetDio1(
            GW_RADIO_1,
            &dio1) != GW_RESULT_OK)
    {
        return result_make(
            EOL_TEST_FAIL,
            "G09 Radio 1 DIO1",
            "Failed to read Radio 1 DIO1");
    }

    (void)dio1;

    return result_make(
        EOL_TEST_PASS,
        "G09 Radio 1 DIO1",
        "Radio 1 DIO1 GPIO read completed");
}

/* ========================================================================== */
/* Public EOL Gateway interface                                               */
/* ========================================================================== */

EolTestResult_t eol_gateway_run_test(
    EolTestId_t test_id)
{
    switch (test_id)
    {
        case EOL_G03_RADIO0_RESET:
            return run_g03_radio0_reset();

        case EOL_G04_RADIO0_BUSY:
            return run_g04_radio0_busy();

        case EOL_G05_RADIO0_DIO1:
            return run_g05_radio0_dio1();

        case EOL_G07_RADIO1_RESET:
            return run_g07_radio1_reset();

        case EOL_G08_RADIO1_BUSY:
            return run_g08_radio1_busy();

        case EOL_G09_RADIO1_DIO1:
            return run_g09_radio1_dio1();

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