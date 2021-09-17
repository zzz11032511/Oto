#pragma once

#include <stdio.h>

#include "../../opcode.h"
#include "../../variable/variable.h"
#include "../../variable/var_stack.h"

/* alu.c */
var_t calculation(var_t v1, var_t v2, opcode_t opcode);
int32_t compare(var_t v1, var_t v2, opcode_t opcode);

/* print.c */
void print_var(struct var_stack *stack);

/* sound_instructions.c */

/**
 * BEEP FREQUENCY, LENGTH
 * ビープ音を鳴らす
 * 
 * params:
 *      FREQUENCY 鳴らす音の周波数[Hz](引数を省略した場合500Hz)
 *      LENGTH 鳴らす音の長さ[sec](引数を省略した場合1s)
 */
void beep(struct var_stack *stack);