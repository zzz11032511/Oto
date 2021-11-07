#include <oto.h>

#include "vm.h"

#define VAR(tc)  ((Var *)(ic_list->data[tc]))

void exec(VectorPTR *ic_list) {
    int64_t i = 0;
    int64_t end = ic_list->length;

    double tmp1 = 0;
    double tmp2 = 0;

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
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }

            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp2 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp2 = vmstack_popf();
            }
            vmstack_pushf(tmp1 + tmp2);
            break;

        case OP_SUB:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }

            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp2 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp2 = vmstack_popf();
            }
            vmstack_pushf(tmp2 - tmp1);
            break;

        case OP_MUL:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }

            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp2 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp2 = vmstack_popf();
            }
            vmstack_pushf(tmp1 * tmp2);
            break;

        case OP_DIV:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }

            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp2 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp2 = vmstack_popf();
            }
            vmstack_pushf(tmp2 / tmp1);
            break;

        case OP_MOD:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp1 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp1 = vmstack_popf();
            }

            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmp2 = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmp2 = vmstack_popf();
            }
            vmstack_pushf((int64_t)tmp2 % (int64_t)tmp1);
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
            print_error(OTO_UNKNOWN_ERROR);
            exit(EXIT_FAILURE);
        }

        i += 5;
    }
}
