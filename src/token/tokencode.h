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
    TcTrack,     // track TRACK
    TcFilter,    // filter FILTER

    TcIf,        // if IF
    TcElsif,     // elsif ELSIF
    TcElse,      // else ELSE
    TcThen,      // then THEN
    TcLoop,      // loop LOOP

    TcAnd,       // and AND
    TcOr,        // or OR
    TcNot,       // not NOT

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
} tokencode_t;