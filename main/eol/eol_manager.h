#pragma once

#include "eol_types.h"

void eol_manager_init(void);
void eol_manager_run(void);
const EolTestResult_t *eol_manager_get_result(void);