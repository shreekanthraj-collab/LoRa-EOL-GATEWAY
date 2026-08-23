#include "eol_node.h"

static EolNodeConfig_t s_config;

bool eol_node_init(const EolNodeConfig_t *config)
{
    if (config == NULL) {
        return false;
    }

    s_config = *config;
    return true;
}

EolTestResult_t eol_node_run_test(EolTestId_t test_id)
{
    EolTestResult_t result = {
        .status = EOL_TEST_NOT_FOUND,
        .name = NULL,
        .detail = "Node EOL test backend not implemented"
    };

    const EolTestDefinition_t *definition = eol_registry_get(test_id);

    if (definition != NULL) {
        result.name = definition->name;
    }

    return result;
}