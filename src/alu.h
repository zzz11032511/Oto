#pragma once

#include "variable.h"

var_t add2(var_t v1, var_t v2);
var_t sub2(var_t v1, var_t v2);
var_t mul2(var_t v1, var_t v2);
var_t div2(var_t v1, var_t v2);
var_t mod2(var_t v1, var_t v2);

int32_t compare(var_t v1, var_t v2, int32_t opcode);