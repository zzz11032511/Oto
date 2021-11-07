#include <oto.h>

#include "vm.h"

#define VAR(tc)  ((Var *)(ic_list->data[tc]))

void exec(VectorPTR *ic_list) {
    int64_t i = 0;
    int64_t end = ic_list->length;

    double tmp1 = 0;

    while (i < end) {
        switch ((opcode_t)ic_list->data[i]) {
        case OP_CPYD:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f;
            break;

        case OP_CPYP:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = tmp1;
            break;

        case OP_PUSH:
            vmstack_pushv(VAR(i + 1));
            break;

        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
        case OP_AND:
        case OP_OR:
        case OP_EQ:
        case OP_NEQ:
        case OP_LTCMP:
        case OP_LTEQCMP:
        case OP_RICMP:
        case OP_RIEQCMP:
            alu((opcode_t)ic_list->data[i]);
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
            oto_error_exit(OTO_UNKNOWN_ERROR);
        }

        i += 5;
    }
}
