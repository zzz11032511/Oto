#pragma once

#include <stdio.h>

#include "../../opcode.h"
#include "../../variable/variable.h"
#include "../../variable/var_stack.h"

/* alu.c */
var_t calculation(var_t v1, var_t v2, opcode_t opcode);
int32_t compare(var_t v1, var_t v2, opcode_t opcode);


/* print.c */

/**
 * PRINT VAR
 * 変数の内容を出力する
 * 
 * params:
 *      VAR 出力する変数または値
 */
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

/**
 * PLAY FREQUENCY, LENGTH, VELOCITY, SOUND
 * 指定したSOUNDを出力する
 * 
 * params:
 *      FREQUENCY 鳴らす音の周波数[Hz](引数を省略した場合500Hz)
 *      LENGTH 鳴らす音の長さ[sec](引数を省略した場合1s)
 *      VELOCITY 音量(0 ~ 100)(引数を省略した場合100)
 *      SOUND 出力する音源情報(引数を省略した場合正弦波)
 */
void play(struct var_stack *stack, uint64_t samples_per_sec);

void define_sound(var_t *v1, var_t *v2);