#include "eol_manager.h"

#include "eol_executor.h"
#include "eol_node.h"
#include "eol_registry.h"

static EolTestResult_t s_manager_result = {
    .status = EOL_TEST_NOT_RUN,
    .name = "EOL Manager",
    .detail = "Manager not initialized",
};

static EolTestExecutorEntry_t s_node_test_entry = {
    .test_id = EOL_N01_BOOT_HEALTH,
    .function = eol_node_run_test,
};

void eol_manager_init(void)
{
    EolNodeConfig_t node_config = {
        .node_id = 1,
        .timeout_ms = 5000,
    };

    s_manager_result.status = EOL_TEST_NOT_RUN;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "Manager initialized";

    if (!eol_node_init(&node_config)) {
        s_manager_result.status = EOL_TEST_FAIL;
        s_manager_result.detail = "Node initialization failed";
    }
}

void eol_manager_run(void)
{
    if (s_manager_result.status == EOL_TEST_FAIL) {
        return;
    }

    (void)eol_executor_run(
        &s_node_test_entry,
        &s_manager_result);
}

const EolTestResult_t *eol_manager_get_result(void)
{
    return &s_manager_result;
}