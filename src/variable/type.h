#pragma once

typedef enum {
    TyVoid = 0,
    TyRsvWord,
    TySymbol,
    TyFloat,
    TyConst,
    TyInitVal,  // 引数が初期値であるもの
    TyOsc,
    TySound,
    TyFilter,
    TyFunc
} type_t;