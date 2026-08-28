#include "eol_manager.h"

#include "eol_node.h"
#include "eol_gateway.h"
#include "eol_registry.h"
#include "eol_ui.h"

static EolTestResult_t s_manager_result = {
    .status = EOL_TEST_NOT_RUN,
    .name = "EOL Manager",
    .detail = "Manager not initialized",
};

static bool s_ui_initialized = false;
static bool s_test_running = false;

static EolTestResult_t run_registered_test(
    EolTestId_t test_id)
{
    const EolTestDefinition_t *definition =
        eol_registry_get(test_id);

    if (definition == NULL)
    {
        EolTestResult_t result = {
            .status = EOL_TEST_NOT_FOUND,
            .name = "Unknown test",
            .detail = "Test ID not present in registry",
        };

        return result;
    }

    switch (definition->group)
    {
        case EOL_GROUP_NODE:
            return eol_node_run_test(test_id);

        case EOL_GROUP_GATEWAY:
            return eol_gateway_run_test(test_id);

        case EOL_GROUP_SYSTEM:
        default:
            return (EolTestResult_t) {
                .status = EOL_TEST_NOT_APPLICABLE,
                .name = definition->name,
                .detail = "System EOL test backend not implemented",
            };
    }
}

void eol_manager_init(void)
{
    EolNodeConfig_t node_config = {
        .node_id = 1,
        .timeout_ms = 5000,
    };

    s_manager_result.status = EOL_TEST_NOT_RUN;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "Manager initialized";

    s_test_running = false;

    s_ui_initialized = eol_ui_init();

    if (!s_ui_initialized)
    {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.detail = "EOL UI initialization failed";
        return;
    }

    if (!eol_node_init(&node_config))
    {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.detail = "Node initialization failed";
    }
}

void eol_manager_run(void)
{
    if (!s_ui_initialized)
    {
        return;
    }

    /*
     * RESET always has priority.
     *
     * The fixture can return to READY state at any time
     * before the next EOL cycle begins.
     */
    if (eol_ui_button_pressed(EOL_UI_BUTTON_RESET))
    {
        s_manager_result.status = EOL_TEST_NOT_RUN;
        s_manager_result.name = "EOL Manager";
        s_manager_result.detail = "EOL reset - ready";

        s_test_running = false;

        /*
         * Wait for RESET release.
         */
        while (eol_ui_button_pressed(EOL_UI_BUTTON_RESET))
        {
            /* Wait for release. */
        }

        return;
    }

    /*
     * Do not start another sequence while one is running.
     */
    if (s_test_running)
    {
        return;
    }

    /*
     * Wait for START.
     */
    if (!eol_ui_button_pressed(EOL_UI_BUTTON_START))
    {
        return;
    }

    /*
     * Current validated development sequence:
     *
     * N01 -> G01 -> G10
     *
     * N01:
     * Node boot / basic health.
     *
     * G01:
     * Gateway shared SPI.
     *
     * G10:
     * Gateway Radio 0 RX/TX control.
     *
     * Remaining registry tests are not executed yet.
     */
    s_test_running = true;

    s_manager_result.status = EOL_TEST_RUNNING;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "EOL sequence running";

    /*
     * Wait for START release so one press cannot retrigger
     * the sequence.
     */
    while (eol_ui_button_pressed(EOL_UI_BUTTON_START))
    {
        /* Wait for release. */
    }

    /*
     * Allow operator abort before starting the sequence.
     */
    if (eol_ui_button_pressed(EOL_UI_BUTTON_FAIL_STOP))
    {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.name = "EOL Manager";
        s_manager_result.detail = "Operator aborted EOL sequence";

        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /* ====================================================================== */
    /* N01 - Node boot / basic health                                         */
    /* ====================================================================== */

    s_manager_result =
        run_registered_test(EOL_N01_BOOT_HEALTH);

    if (s_manager_result.status != EOL_TEST_PASS)
    {
        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /*
     * Check operator abort between tests.
     */
    if (eol_ui_button_pressed(EOL_UI_BUTTON_FAIL_STOP))
    {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.name = "EOL Manager";
        s_manager_result.detail = "Operator aborted EOL sequence";

        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /* ====================================================================== */
    /* G01 - Gateway shared SPI                                               */
    /* ====================================================================== */

    s_manager_result =
        run_registered_test(EOL_G01_SHARED_SPI);

    if (s_manager_result.status != EOL_TEST_PASS)
    {
        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /*
     * Check operator abort between tests.
     */
    if (eol_ui_button_pressed(EOL_UI_BUTTON_FAIL_STOP))
    {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.name = "EOL Manager";
        s_manager_result.detail = "Operator aborted EOL sequence";

        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /* ====================================================================== */
    /* G10 - Gateway Radio 0 RX/TX control                                    */
    /* ====================================================================== */

    s_manager_result =
        run_registered_test(EOL_G10_RADIO0_RXTX_CONTROL);

    if (s_manager_result.status != EOL_TEST_PASS)
    {
        eol_ui_signal_fail();

        s_test_running = false;
        return;
    }

    /*
     * Current validated development sequence completed.
     *
     * N01 -> G01 -> G10
     */
    s_manager_result.status = EOL_TEST_PASS;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "N01, G01 and G10 passed";

    eol_ui_signal_pass();

    s_test_running = false;
}

const EolTestResult_t *eol_manager_get_result(void)
{
    return &s_manager_result;
}