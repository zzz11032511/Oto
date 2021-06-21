#ifndef INCLUDED_VARIABLE
#define INCLUDED_VARIABLE 1

enum {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyInt,
    TyFloat
};

typedef struct var {
    int type;       // 型
    void* value;    // 中身
} var_t;

int varDef(var_t *var, int type, ...);

#endif