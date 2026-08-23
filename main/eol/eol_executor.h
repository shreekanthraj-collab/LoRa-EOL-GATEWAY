#pragma once

#include "eol_registry.h"
#include "eol_types.h"

typedef EolTestResult_t (*EolTestFunction_t)(EolTestId_t test_id);
typedef struct
{
	EolTestId_t test_id;
	EolTestFunction_t function;
} EolTestExecutorEntry_t;

EolTestStatus_t eol_executor_run(
	const EolTestExecutorEntry_t *entry,
	EolTestResult_t *result);
