#pragma once

#include "../variable/variable.h"

var_t calculation(var_t v1, var_t v2, uint32_t opcode);

int32_t compare(var_t v1, var_t v2, uint32_t opcode);