#include "eol_manager.h"

static EolTestResult_t s_manager_result = {
    .status = EOL_TEST_NOT_RUN,
    .name = "EOL Manager",
    .detail = "Manager initialized"
};

void eol_manager_init(void)
{
    s_manager_result.status = EOL_TEST_NOT_RUN;
    s_manager_result.name = "EOL Manager";
    s_manager_result.detail = "Manager initialized";
}

void eol_manager_run(void)
{
    s_manager_result.status = EOL_TEST_PASS;
    s_manager_result.detail = "Manager skeleton running";
}

const EolTestResult_t *eol_manager_get_result(void)
{
    return &s_manager_result;
}