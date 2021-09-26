#pragma once

#include <stdint.h>

#include "../opcode.h"
#include "../variable/variable.h"

void put_ic(uint32_t *icp, opcode_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4);