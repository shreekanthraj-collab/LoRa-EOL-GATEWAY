#include "eol_manager.h"

#include "eol_node.h"
#include "eol_registry.h"

static EolTestResult_t s_manager_result = {
    .status = EOL_TEST_NOT_RUN,
    .name = "EOL Manager",
    .detail = "Manager not initialized",
};

static EolTestResult_t run_registered_test(EolTestId_t test_id)
{
    const EolTestDefinition_t *definition = eol_registry_get(test_id);

    if (definition == NULL) {
        EolTestResult_t result = {
            .status = EOL_TEST_NOT_FOUND,
            .name = "Unknown test",
            .detail = "Test ID not present in registry",
        };

        return result;
    }

    switch (definition->group) {
        case EOL_GROUP_NODE:
            return eol_node_run_test(test_id);

        case EOL_GROUP_GATEWAY:
        case EOL_GROUP_SYSTEM:
        default:
            return (EolTestResult_t) {
                .status = EOL_TEST_NOT_FOUND,
                .name = definition->name,
                .detail = "Test backend not implemented",
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

    s_manager_result = run_registered_test(EOL_N01_BOOT_HEALTH);
}

const EolTestResult_t *eol_manager_get_result(void)
{
    return &s_manager_result;
}