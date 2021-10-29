#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "oto_util.h"

// Tokencodes
enum {
    TC_LF = 0,    // \n
    TC_COMMA,     // ,
    TC_COLON,     // :
    TC_SQBROPN,   // [
    TC_SQBRCLS,   // ]
    TC_BROPN,     // (
    TC_BRCLS,     // )
    TC_LARROW,    // <-
    TC_RARROW,    // ->
    TC_EQU,       // =
    TC_PLUS,      // +
    TC_MINUS,     // -
    TC_ASTER,     // *
    TC_SLASH,     // /
    TC_PERCE,     // %
    TC_EEQ,       // '=='
    TC_NEQ,       // '!='
    TC_GT,        // '>'
    TC_LT,        // '<'
    TC_GE,        // '>='
    TC_LE,        // '<='
    
    TC_BEGIN,     // begin BEGIN
    TC_END,       // end END
    TC_DEFINE,    // define DEFINE
    TC_IF,        // if IF
    TC_ELSIF,     // elsif ELSIF
    TC_ELSE,      // else ELSE
    TC_THEN,      // then THEN
    TC_LOOP,      // loop LOOP
    TC_AND,       // and AND
    TC_OR,        // or OR
    TC_NOT,       // not NOT
    TC_FUNC,      // func FUNC
    TC_TRACK,     // track TRACK
    TC_FILTER,    // filter FILTER
    TC_OSCIL,     // oscil OSCIL
    TC_SOUND,     // sound SOUND

    TC_PRINT,     // print PRINT
    TC_BEEP,      // beep BEEP
    TC_PLAY,      // play PLAY
    TC_NOTE,      // note NOTE
    TC_MUTE,      // mute MUTE
    TC_BPM,       // bpm BPM
    TC_PRINTWAV,  // printwav PRINTWAV
    TC_EXPORTWAV, // exportwav EXPORTWAV
    TC_IMPORTWAV, // importwav IMPORTWAV
    TC_DEFSE,     // defse DEFSE
    TC_SPECTRUM,  // spectrum SPECTRUM
    TC_SETFS,     // setfs SETFS
    TC_MIDIIN,    // midiin MIDIIN
    TC_MIDIOUT,   // midiout MIDIOUT

    TC_EXIT       // exit EXIT
};
typedef uint64_t tokencode_t;

// All Oto reserve words and operators

// typedef struct {
//     int8_t *s;
//     tokencode_t tc;
// } Rsvwords;
// extern const Rsvwords rsvwords[];

// Operation codes

enum {
    OP_NOP = 0,    // 何もしない

    /**
     * 変数への単純代入
     * 
     * example:
     *   CPYD Var1 Var2
     *   Var1にVar2の内容をコピーして代入する
     */
    OP_CPYD,

    /**
     * スタックからポップしたものを変数に代入
     * 
     * example:
     *   CPYP Var1
     *   Var1にスタックからポップしてきたものを代入する
     */
    OP_CPYP,

    /* スタックにプッシュする */
    OP_PUSH,
    
    /**
     * スタックに定数をプッシュする
     * 
     * example:
     *   PUSHC Type Value
     *   Typeで指定した型として, スタックにValueという定数をプッシュする
     */
    OP_PUSHC,

    /**
     * 算術演算命令
     * スタックの上から2つをポップして演算し, プッシュする
     * 
     * note:
     *   先にプッシュしたほうが初めの項となる
     */
    OP_ADD,        // 加算
    OP_SUB,        // 引算
    OP_MUL,        // 掛算
    OP_DIV,        // 割算
    OP_MOD,        // 余り
    OP_AND,        // 論理積
    OP_OR,         // 論理和
    OP_EQ,         // == 
    OP_NEQ,        // !=
    OP_LTCMP,      // <
    OP_LTEQCMP,    // <=
    OP_RICMP,      // >
    OP_RIEQCMP,    // >=
    
    /**
     * 二項演算命令
     * 引数に指定した変数同士を演算をする. 算術演算命令より速い.
     * 
     * example:
     *   ADD Var1 Var2 Var3
     *   Var1に, Var2とVar3を加算したものを代入する
     */
    OP_ADD2,       // 二項加算
    OP_SUB2,       // 二項引算
    OP_MUL2,       // 二項掛算
    OP_DIV2,       // 二項割算
    OP_MOD2,       // 二項余り

    /**
     * ループ命令
     * 指定した回数ループを行う
     * 
     * example:
     *   LOOP Addr Loop_Cnt Loop_Var
     *   Addrをループ場所として, Loop_Cnt回分ループする.
     *   Loop_Varは制御変数である.
     */
    OP_LOOP,

    /**
     * 分岐命令
     * 
     * 条件が成立していれば, 指定した場所へジャンプする
     */
    OP_JMP,        // 無条件ジャンプ
    OP_JZ,         // スタックの上が0ならジャンプ
    OP_JNZ,        // スタックの上が0でないならジャンプ
};
typedef uint64_t opcode_t;

// Types

enum {
    TY_VOID = 0,
    TY_RSVWORD,  // 変数として使用不可能なもの
    TY_CONST,
    TY_FLOAT,
    TY_ARRAY,
    TY_STRING,
    TY_INITVAL,  // 引数が初期値であるもの
    TY_OSCIL,
    TY_SOUND,
    TY_FILTER,
    TY_FUNC
};
typedef uint64_t type_t;

// run.c

void oto_run(const int8_t *filename);
void oto_quit();

void set_timecount_flag(bool flag);

// token.c

typedef struct {
    tokencode_t tc;
    char *str; 
    size_t len;
} Token;

tokencode_t allocate_tc(VectorPTR *tc_list, char *str, size_t len, type_t type);
bool is_rsvword(char *str, size_t len);

// lexer.c
void tokenize(char *s, VectorUI64 *tc_list);
VectorUI64 *lexer(char *s);