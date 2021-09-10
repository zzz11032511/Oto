#pragma once

typedef enum {
    TyVoid = 0,
    TyRsvWord,
    TySymbol,
    TyFloat,
    TyConst,
    TySound,
    TyFilter,
    TyFunc
} type_t;