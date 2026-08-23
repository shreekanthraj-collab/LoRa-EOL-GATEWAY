#include "eol_executor.h"

EolTestStatus_t eol_executor_run(
	const EolTestExecutorEntry_t *entry,
	EolTestResult_t *result)
{
	if ((entry == NULL) || (result == NULL) || (entry->function == NULL)) {
		return EOL_TEST_FAIL;
	}

	result->status = EOL_TEST_RUNNING;

	result->status = entry->function(entry->test_id);

	return result->status;
}
