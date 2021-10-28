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
    TcLF = 0,    // \n
    TcComma,     // ,
    TcColon,     // :
    TcSqBrOpn,   // [
    TcSqBrCls,   // ]
    TcBrOpn,     // (
    TcBrCls,     // )
    TcLArrow,    // <-
    TcRArrow,    // ->
    TcEqu,       // =
    TcPlus,      // +
    TcMinus,     // -
    TcAster,     // *
    TcSlash,     // /
    TcPerce,     // %
    TcEEq,       // '=='
    TcNEq,       // '!='
    TcGt,        // '>'
    TcLt,        // '<'
    TcGe,        // '>='
    TcLe,        // '<='
    
    TcBegin,     // begin BEGIN
    TcEnd,       // end END
    TcDefine,    // define DEFINE
    TcIf,        // if IF
    TcElsif,     // elsif ELSIF
    TcElse,      // else ELSE
    TcThen,      // then THEN
    TcLoop,      // loop LOOP
    TcAnd,       // and AND
    TcOr,        // or OR
    TcNot,       // not NOT
    TcFunc,      // func FUNC
    TcTrack,     // track TRACK
    TcFilter,    // filter FILTER
    TcOscil,     // oscil OSCIL
    TcSound,     // sound SOUND

    TcPrint,     // print PRINT
    TcBeep,      // beep BEEP
    TcPlay,      // play PLAY
    TcNote,      // note NOTE
    TcMute,      // mute MUTE
    TcBpm,       // bpm BPM
    TcPrintwav,  // printwav PRINTWAV
    TcExportwav, // exportwav EXPORTWAV
    TcImportwav, // importwav IMPORTWAV
    TcDefse,     // defse DEFSE
    TcSpectrum,  // spectrum SPECTRUM
    TcSetfs,     // setfs SETFS
    TcMidiin,    // midiin MIDIIN
    TcMidiout,   // midiout MIDIOUT

    TcExit       // exit EXIT
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
    OpNop = 0,    // 何もしない

    /**
     * 変数への単純代入
     * 
     * example:
     *   CpyD Var1 Var2
     *   Var1にVar2の内容をコピーして代入する
     */
    OpCpyD,

    /**
     * スタックからポップしたものを変数に代入
     * 
     * example:
     *   CpyP Var1
     *   Var1にスタックからポップしてきたものを代入する
     */
    OpCpyP,

    /* スタックにプッシュする */
    OpPush,
    
    /**
     * スタックに定数をプッシュする
     * 
     * example:
     *   PushC Type Value
     *   Typeで指定した型として, スタックにValueという定数をプッシュする
     */
    OpPushC,

    /**
     * 算術演算命令
     * スタックの上から2つをポップして演算し, プッシュする
     * 
     * note:
     *   先にプッシュしたほうが初めの項となる
     */
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpMod,        // 余り
    OpAnd,        // 論理積
    OpOr,         // 論理和
    OpEq,         // == 
    OpNEq,        // !=
    OpLtCmp,      // <
    OpLtEqCmp,    // <=
    OpRiCmp,      // >
    OpRiEqCmp,    // >=
    
    /**
     * 二項演算命令
     * 引数に指定した変数同士を演算をする. 算術演算命令より速い.
     * 
     * example:
     *   Add Var1 Var2 Var3
     *   Var1に, Var2とVar3を加算したものを代入する
     */
    OpAdd2,       // 二項加算
    OpSub2,       // 二項引算
    OpMul2,       // 二項掛算
    OpDiv2,       // 二項割算
    OpMod2,       // 二項余り

    /**
     * ループ命令
     * 指定した回数ループを行う
     * 
     * example:
     *   Loop Addr Loop_Cnt Loop_Var
     *   Addrをループ場所として, Loop_Cnt回分ループする.
     *   Loop_Varは制御変数である.
     */
    OpLoop,

    /**
     * 分岐命令
     * 
     * 条件が成立していれば, 指定した場所へジャンプする
     */
    OpJmp,        // 無条件ジャンプ
    OpJz,         // スタックの上が0ならジャンプ
    OpJnz,        // スタックの上が0でないならジャンプ
};
typedef uint64_t opcode_t;

// Types

enum {
    TyVoid = 0,
    TyRsvWord,  // 変数として使用不可能なもの
    TyConst,
    TyFloat,
    TyArray,
    TyString,
    TyInitVal,  // 引数が初期値であるもの
    TyOsc,
    TySound,
    TyFilter,
    TyFunc
};
typedef uint64_t type_t;

// utils.c and utility macros

#define IS_RSVWORD(tc)        ((TcBegin <= tc) && (tc <= TcExit))
#define IS_AVAILABLE_VAR(tc)  (tc > TcExit)
#define IS_SYMBOL(tc)         ((TcLF <= tc) && (tc < TcBegin))
#define IS_NOT_SYMBOL(tc)     (!IS_SYMBOL(tc))
#define IS_INSTRUCTION(tc)    ((TcPrint <= tc) && (tc < TcExit))
#define IS_ARITH_OPERATOR(tc) ((TcPlus <= tc && tc <= TcGt) || (TcAnd <= tc && tc <= TcOr))

// run.c

void oto_run(const int8_t *filename);
void oto_quit();

void set_timecount_flag(bool flag);