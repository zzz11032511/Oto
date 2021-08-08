#pragma once

enum TOKENCODE {
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

    TcEEq,         // '=='
    TcNEq,         // '!='
    TcLt,          // '<'
    TcGe,          // '>='
    TcLe,          // '<='
    TcGt,          // '>'
    
    TcBegin,     // begin BEGIN
    TcEnd,       // end END
    
    TcIn,        // in IN
    TcOut,       // out OUT
    
    TcImport,    // import IMPORT
    TcDefine,    // define DEFINE
    TcChannel,   // channel CHANNEL
    TcSound,     // sound SOUND
    TcFilter,    // filter FILTER

    TcIf,        // if IF
    TcElsif,     // elsif ELSIF
    TcElse,      // else ELSE
    TcThen,      // then THEN
    TcLoop,      // loop LOOP

    TcAnd,       // and AND
    TcOr,        // or OR
    
    TcNot,       // not NOT

    TcPlay,      // play PLAY
    TcBpm,       // bpm BPM
    TcNote,      // note NOTE
    TcMute,      // mute MUTE
    
    TcPrint,     // print PRINT
    TcBeep,      // beep BEEP
    TcExit       // exit EXIT
};