#include "oto.h"

#define VAR(tc)  ((Var *)(ic_list->data[tc]))

void exec(VectorPTR *ic_list) {
    int64_t i = 0;
    int64_t end = ic_list->length;

    while (i < end) {
        switch ((opcode_t)ic_list->data[i]) {
        case OP_CPYD:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f;
            break;
        
        case OP_ADD2:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f + VAR(i + 3)->value.f;
            break;

        case OP_SUB2:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f - VAR(i + 3)->value.f;
            break;

        case OP_MUL2:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f * VAR(i + 3)->value.f;
            break;

        case OP_DIV2:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f / VAR(i + 3)->value.f;
            break;

        case OP_MOD2:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = 
                (int64_t)VAR(i + 2)->value.f % (int64_t)VAR(i + 3)->value.f;
            break;

        case OP_PRINT:
            printf("%f\n", VAR(i + 1)->value.f);
            break;

        case OP_EXIT:
            return;

        default:
            break;
        }

        i += 5;
    }
}
