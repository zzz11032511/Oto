#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "lexer.h"
#include "variable.h"

void *iVarAlloc(int value)
{
    int *addr = malloc(sizeof(int));

    if (addr == NULL) {
        exit(1);
    }

    *addr = value;

    return (void *)addr;
}

void *fVarAlloc(double value)
{
    double *addr = malloc(sizeof(double));

    if (addr == NULL) {
        exit(1);
    }

    *addr = value;

    return (void *)addr;
}

int varDef(var_t *var, int type, ...)
{
    va_list ap;
    va_start(ap, type);

    if (var->type == type) {
        // もし型が同じならただの代入処理
        return 0;
    }

    var->type = type;
    
    int iValue = 0;
    double fValue = 0;

    int errFlag = 0;
    switch (type) {
    case TyInt:
        iValue = va_arg(ap, long);
        var->value = iVarAlloc(iValue);
        break;
        
    case TyConstI:
        iValue = va_arg(ap, long);
        var->value = iVarAlloc(iValue);
        break;
    
    case TyFloat:
        fValue = va_arg(ap, double);
        var->value = fVarAlloc(fValue);
        break;
    
    case TyConstF:
        fValue = va_arg(ap, double);
        var->value = fVarAlloc(fValue);
        break;
    
    default:
        errFlag = 1;
        break;
    }

    va_end(ap);

    return errFlag;
}