#pragma once

#include "eol_registry.h"
#include "eol_types.h"

/**
 * Gateway EOL test backend.
 *
 * Uses only the public Gateway diagnostic APIs.
 * Does not access Gateway GPIO/SPI internals.
 */
EolTestResult_t eol_gateway_run_test(
    EolTestId_t test_id);