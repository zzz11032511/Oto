#pragma once

#include <stdio.h>

#include "../../opcode.h"
#include "../../variable/variable.h"

/* alu.c */
var_t calculation(var_t v1, var_t v2, opcode_t opcode);
int32_t compare(var_t v1, var_t v2, opcode_t opcode);

/* print.c */
void print_var(var_t var);