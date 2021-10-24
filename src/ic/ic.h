#pragma once

#include <stdint.h>

#include "../opcode.h"
#include "../variable/variable.h"

void put_ic(uint32_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4);