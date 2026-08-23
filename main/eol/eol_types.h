
#pragma once

#include <stdint.h>

typedef enum
{
	EOL_TEST_NOT_RUN = 0,
	EOL_TEST_RUNNING,
	EOL_TEST_PASS,
	EOL_TEST_FAIL,
	EOL_TEST_NOT_FOUND,
	EOL_TEST_NOT_APPLICABLE
} EolTestStatus_t;

typedef struct
{
	EolTestStatus_t status;
	const char *name;
	const char *detail;
} EolTestResult_t;
