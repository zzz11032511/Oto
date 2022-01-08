#include "vm.h"

#define VAR(tc)  ((Var *)(ic_list->data[tc]))

void exec(VectorPTR *ic_list, VectorPTR *var_list, Status *status) {
    int64_t i = 0;
    int64_t end = ic_list->length;

    double  tmpf  = 0;
    int64_t tmpi1 = 0;
    int64_t tmpi2 = 0;

    while (i < end) {
        switch ((opcode_t)ic_list->data[i]) {
        case OP_CPYD:
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = VAR(i + 2)->value.f;
            break;

        case OP_CPYP:
            if (vmstack_typecheck() == VM_TY_VARPTR) {
                tmpf = vmstack_popv()->value.f;
            } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
                tmpf = vmstack_popf();
            }
            VAR(i + 1)->type    = TY_FLOAT;
            VAR(i + 1)->value.f = tmpf;
            break;

        case OP_PUSH:
            vmstack_pushv(VAR(i + 1));
            break;

        case OP_PUSH_INITVAL:
            vmstack_push_initval();
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

        case OP_LOOP:
            tmpi1 = (int64_t)ic_list->data[i + 3];
            tmpi1++;
            ic_list->data[i + 3] = (void *)tmpi1;

            tmpi2 = (int64_t)VAR(i + 2)->value.f;

            if (tmpi1 > tmpi2) {
                // ループカウンタを初期化する
                ic_list->data[i + 3] = 0;

                i = (int64_t)VAR(i + 1);
                continue;
            }
            break;

        case OP_JMP:
            i = (int64_t)VAR(i + 1);
            continue;

        case OP_JZ:
            tmpi1 = vmstack_popi();
            if (tmpi1 == 0) {
                i = (int64_t)VAR(i + 1);
                continue;
            }
            break;

        case OP_JNZ:
            tmpi1 = vmstack_popi();
            if (tmpi1 != 0) {
                i = (int64_t)VAR(i + 1);
                continue;
            }
            break;

        case OP_OSCILDEF:
            VAR(i + 1)->type = TY_OSCIL;
            if (VAR(i + 3) == NULL) {
                VAR(i + 1)->value.p = (void *)new_oscil(
                    (int64_t)VAR(i + 2)->value.f, 0, 0
                );
            } else {
                if ((VAR(i + 2)->type == TY_FLOAT || VAR(i + 2)->type == TY_CONST)
                 || (VAR(i + 3)->type == TY_FLOAT || VAR(i + 3)->type == TY_CONST)
                 || (VAR(i + 4)->type == TY_FLOAT || VAR(i + 4)->type == TY_CONST)) {
                    VAR(i + 1)->value.p = (void *)new_oscil(
                        (int64_t)VAR(i + 2)->value.f,
                        (int64_t)VAR(i + 3)->value.f,
                        (int64_t)VAR(i + 4)->value.f
                    );
                } else {
                    oto_error(OTO_UNKNOWN_ERROR);
                }
            }
            break;

        case OP_SOUNDDEF:
            VAR(i + 1)->type = TY_SOUND;
            if (VAR(i + 2)->type == TY_OSCIL) {
                VAR(i + 1)->value.p = (void *)new_sound((Oscillator *)(VAR(i + 2)->value.p));
            } else {
                oto_error(OTO_UNKNOWN_ERROR);
            }
            break;

        case OP_CPYS:
            if (VAR(i + 2)->type != TY_SOUND) {
                oto_error(OTO_UNKNOWN_ERROR);
            
            } else if (VAR(i + 1)->type == TY_SOUND) {
                // ((Sound *)VAR(i + 1)->value.p)->oscillator = ((Sound *)VAR(i + 2)->value.p)->oscillator;
                // free_items_vector_ptr(((Sound *)VAR(i + 1)->value.p)->filters);
                // free_vector_ptr(((Sound *)VAR(i + 1)->value.p)->filters);
                // new_vector_ptr()
                oto_error(OTO_EXIST_SOUND_OBJECT_ERROR);

            } else if (VAR(i + 1)->type != TY_SOUND) {
                if (VAR(i + 1)->type == TY_FILTER) {
                    oto_error(OTO_NAME_ERROR);
                } else if (VAR(i + 1)->type == TY_OSCIL) {
                    oto_error(OTO_NAME_ERROR);
                } else if (VAR(i + 1)->type == TY_STRING) {
                    oto_error(OTO_NAME_ERROR);
                } else if (VAR(i + 1)->type == TY_ARRAY) {
                    oto_error(OTO_NAME_ERROR);
                }

                VAR(i + 1)->type = TY_SOUND;
                VAR(i + 1)->value.p = (void *)new_sound(((Sound *)VAR(i + 2)->value.p)->oscillator);
            }
            break;

        case OP_CONNFILTER:
            oto_connect_filter(((Sound *)(VAR(i + 1)->value.p)), (filtercode_t)VAR(i + 2), status);
            break;

        case OP_PRINT:
            oto_instr_print();
            break;

        case OP_BEEP:
            oto_instr_beep();
            break;

        case OP_PLAY:
            oto_instr_play(status);
            break;

        case OP_PRINTWAV:
            oto_instr_printwav(status);
            break;

        case OP_PRINTVAR:
            oto_instr_printvar(var_list);
            break;
        
        case OP_SLEEP:
            oto_instr_sleep();
            break;

        case OP_EXIT:
            return;

        case OP_NOP:
            break;

        default:
            oto_error(OTO_UNKNOWN_ERROR);
        }

        i += 5;
    }
}
