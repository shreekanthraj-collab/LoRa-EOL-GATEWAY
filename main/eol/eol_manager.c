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

static bool operator_abort_requested(void)
{
    if (!eol_ui_button_pressed(EOL_UI_BUTTON_FAIL_STOP))
    {
        return false;
    }

    s_manager_result.status = EOL_TEST_FAIL;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "Operator aborted EOL sequence";

    eol_ui_signal_fail();

    s_test_running = false;

    return true;
}

static bool run_test_and_check(
    EolTestId_t test_id)
{
    s_manager_result = run_registered_test(test_id);

    if (s_manager_result.status != EOL_TEST_PASS)
    {
        eol_ui_signal_fail();

        s_test_running = false;

        return false;
    }

    return true;
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
     */
    if (eol_ui_button_pressed(EOL_UI_BUTTON_RESET))
    {
        s_manager_result.status = EOL_TEST_NOT_RUN;
        s_manager_result.name = "EOL Manager";
        s_manager_result.detail = "EOL reset - ready";

        s_test_running = false;

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
     * Current EOL Gateway sequence:
     *
     * N01
     * G01
     * G03
     * G04
     * G05
     * G07
     * G08
     * G09
     * G10
     * G11
     *
     * G02/G06 identity remain deferred because the
     * production Gateway API does not expose a radio
     * identity diagnostic function.
     */
    s_test_running = true;

    s_manager_result.status = EOL_TEST_RUNNING;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "EOL sequence running";

    /*
     * Wait for START release.
     */
    while (eol_ui_button_pressed(EOL_UI_BUTTON_START))
    {
        /* Wait for release. */
    }

    /*
     * Allow operator abort before starting.
     */
    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* N01 - Node boot / basic health                                         */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_N01_BOOT_HEALTH))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G01 - Gateway shared SPI                                               */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G01_SHARED_SPI))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G03 - Radio 0 RESET                                                    */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G03_RADIO0_RESET))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G04 - Radio 0 BUSY                                                     */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G04_RADIO0_BUSY))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G05 - Radio 0 DIO1                                                     */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G05_RADIO0_DIO1))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G07 - Radio 1 RESET                                                    */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G07_RADIO1_RESET))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G08 - Radio 1 BUSY                                                     */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G08_RADIO1_BUSY))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G09 - Radio 1 DIO1                                                     */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G09_RADIO1_DIO1))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G10 - Radio 0 RX/TX control                                            */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G10_RADIO0_RXTX_CONTROL))
    {
        return;
    }

    if (operator_abort_requested())
    {
        return;
    }

    /* ====================================================================== */
    /* G11 - Radio 1 RX/TX control                                            */
    /* ====================================================================== */

    if (!run_test_and_check(EOL_G11_RADIO1_RXTX_CONTROL))
    {
        return;
    }

    /*
     * Sequence completed.
     */
    s_manager_result.status = EOL_TEST_PASS;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail =
        "N01, G01, G03, G04, G05, G07, G08, G09, G10 and G11 passed";

    eol_ui_signal_pass();

    s_test_running = false;
}

const EolTestResult_t *eol_manager_get_result(void)
{
    return &s_manager_result;
}