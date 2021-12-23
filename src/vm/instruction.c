#include "vm.h"

void oto_instr_print() {
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        printf("%f\n", vmstack_popv()->value.f);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        printf("%f\n", vmstack_popf());
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }
}

void oto_instr_beep() {
    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        duration = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        // 初期値だとしてもpopしないといけない
        vmstack_popf();
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        freq = 500.0;
    }

    Beep(freq, duration * 1000);
}

void oto_instr_play(Status *status) {
    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        duration = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        freq = 500.0;
    }

    Playdata data;
    data.freq[0] = freq;
    data.length  = status->sampling_rate * duration;
    data.sound   = NULL;
    data.volume  = 100;

    write_out_data(data);
    printf("[Play] ");
    printf("frequency : %8.3f, length : %2.2f, velocity : %d, wave : %d\n", 
           freq, duration, 100, 1);
    
    set_stream_active_flag(true);
    while (is_stream_active()) {
        // usleep(10);
    }
}

void oto_instr_printvar(VectorPTR *var_list) {
    if (var_list->length <= TC_EXIT + 1) {
        // 変数が1つも定義されていない
        printf("No variables have been declared\n");
        return;
    }

    printf("- Variable list -\n");

    uint64_t i = TC_EXIT + 1;
    do {
        Var *var = ((Var *)var_list->data[i]);
        i++;

        vartype_t type = var->type;
        // printf("%s\n", var->token->str);
        if (type == TY_CONST) {
            continue;
        } else if (type == TY_FLOAT) {
            printf("%15s", var->token->str);
            printf("(float) : ");
            printf("%f", var->value.f);
        } else if (type == TY_ARRAY) {
            continue;
        } else if (type == TY_STRING) {
            continue;
        } else if (type == TY_OSCIL) {
            continue;
        } else if (type == TY_SOUND) {
            continue;
        } else if (type == TY_FILTER) {
            continue;
        } else if (type == TY_FUNC) {
            continue;
        }

        printf("\n");

    } while (i < var_list->length);
    printf("\n");
}

void oto_instr_sleep() {
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Sleep((int64_t)(vmstack_popv()->value.f) * 1000);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        Sleep((int64_t)(vmstack_popf()) * 1000);
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }
}