 #pragma once

#include <stdbool.h>
#include <stdint.h>

#include "eol_types.h"
#include "eol_registry.h"

typedef struct
{
	uint8_t node_id;
	uint32_t timeout_ms;
} EolNodeConfig_t;

bool eol_node_init(const EolNodeConfig_t *config);

EolTestResult_t eol_node_run_test(
	EolTestId_t test_id
);
