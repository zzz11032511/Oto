#include "vm.h"

void oto_instr_print() {
    DEBUG_IPRINT(vmstack_typecheck());
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popv();
        if (var->type == TY_FLOAT) {
            printf("%f\n", var->value.f);
        } else if (var->type == TY_FILTER) {
            printf("[filter]\n");
        } else if (var->type == TY_SOUND) {
            printf("[sound]\n");
        } else if (var->type == TY_OSCIL) {
            printf("[oscillator]\n");
        }

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
    Sound *sound = NULL;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popp();
        if (var->type != TY_SOUND) {
            oto_error(OTO_MISSING_ARGUMENTS_ERROR);
        }
        sound = (Sound *)var->value.p;
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
    } else {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }

    double volume = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        volume = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        volume = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        volume = 80;
    }

    if (!(0 <= volume && volume <= 100)) {
        volume = 80;
    }

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
    data.sound   = sound;
    data.volume  = (int8_t)volume;

    write_out_data(data);

    printf("[Play] ");
    if (sound != NULL) {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : %d\n", 
               freq, duration, (int64_t)volume, sound->oscillator->wave);
    } else {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : NULL\n", 
               freq, duration, (int64_t)volume);
    }

    // printf("filter length : %I64d\n", sound->filters->length);
    
    set_stream_active_flag(true);
    while (is_stream_active()) {
        // usleep(1);
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

void oto_connect_filter(Sound *sound, filtercode_t fc, Status *status) {
    Filter *filter = new_filter(fc);
    if (IS_NULL(filter)) {
        oto_error(OTO_UNKNOWN_ERROR);
    }

    int64_t param = def_filters[fc].param;
    for (int64_t i = param - 1; i >= 0; i--) {
        if (vmstack_typecheck() == VM_TY_VARPTR) {
            filter->args[i] = vmstack_popp();
        } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
            filter->args[i] = vmstack_popp();
        } else if (vmstack_typecheck() == VM_TY_INITVAL) {
            oto_error(OTO_MISSING_ARGUMENTS_ERROR);
        }
    }
            
    vector_ptr_append(sound->filters, (void *)filter);
}