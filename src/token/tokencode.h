#pragma once

typedef enum {
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
    TcFunc,      // func FUNC
    TcChannel,   // channel CHANNEL
    TcFilter,    // filter FILTER

    TcIf,        // if IF
    TcElsif,     // elsif ELSIF
    TcElse,      // else ELSE
    TcThen,      // then THEN
    TcLoop,      // loop LOOP

    TcAnd,       // and AND
    TcOr,        // or OR
    TcNot,       // not NOT

    TcSound,     // sound SOUND
    TcBpm,       // bpm BPM
    TcNote,      // note NOTE
    TcMute,      // mute MUTE

    TcPrint,     // print PRINT
    TcBeep,      // beep BEEP
    TcPlay,      // play PLAY
    TcPrintwav,  // printwav PRINTWAV

    TcExit       // exit EXIT
} tokencode_t;