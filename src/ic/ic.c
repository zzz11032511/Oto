#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../opcode.h"
#include "../util/util.h"
#include "../error/error.h"
#include "../token/token.h"
#include "../variable/variable.h"
#include "../variable/type.h"

/* 内部コード列の長さ */
#define IC_LIST_SIZE 20000

Var *ic[IC_LIST_SIZE] = {0};

void put_ic(uint32_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4) {
    ic[(*icp)++] = (Var *)((uint64_t)op);
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}
