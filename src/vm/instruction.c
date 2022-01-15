#include <acl.h>
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
        } else if (var->type == TY_STRING) {
            printf("%s\n", ((String *)var->value.p)->str);
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
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        sound = (Sound *)var->value.p;
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
    } else {
        oto_error(OTO_ARGUMENTS_TYPE_ERROR);
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
    
    // freqが0だとエラーになるので補正
    if (freq == 0) {
        freq = 1;
    }

    Playdata data;
    data.freq[0] = freq;
    data.length  = status->sampling_rate * duration;
    data.sound   = sound;
    data.volume  = (int8_t)volume;

    write_out_data(data, false);

    printf("[Play] ");
    if (sound != NULL) {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : %d\n", 
               freq, duration, (int64_t)volume, sound->oscillator->wave);
    } else {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : NULL\n", 
               freq, duration, (int64_t)volume);
    }

    set_stream_active_flag(true);
    while (is_stream_active()) {
        usleep(1);
    }
}

static AInt16a transform_tdata(float data, AInt32a height) {
    if (data > 1) {
        data = 1;
    } else if (data < -1) {
        data = -1;
    }
    return (AInt16a)((height / 2) - (data * (height / 2) * 0.8));
}

static void print_wave_sub(Status *status, Playdata *data) {
    AInt16a width  = 1000;
    AInt16a height = 200;
    AWindow *w = aOpenWin(width, height * 2, "wave", 1);

    AInt32a color = aRgb8(0x2e, 0x2e, 0x2e);
    for (AInt16a y = 0; y < height * 2; y++) {
        for (AInt16a x = 0; x < width; x++) {
            aSetPix0(w, x, y, color);
        }
    }

    // 全体の波形
    color = aRgb8(0, 0xc3, 0xff);
    int32_t beforex = 0;
    int32_t beforey = transform_tdata(databuf[0], height);
    int32_t a = data->length / width;
    for (int32_t i = 0; i < width; i++) {
        AInt16a y = transform_tdata(databuf[i * a], height);
        aDrawLine(w,     beforex,     beforey,     i,     y, color);
        aDrawLine(w,     beforex, beforey + 1,     i, y + 1, color);
        aDrawLine(w, beforex + 1,     beforey, i + 1,     y, color);
        aDrawLine(w, beforex + 1, beforey + 1, i + 1, y + 1, color);
        beforex = i;
        beforey = y;
        // aSetPix0(w, i, y, color);
    }

    // 拡大した波形
    color = aRgb8(0, 0xc3, 0xff);
    beforex = 0;
    beforey = transform_tdata(databuf[data->length / 2], height) + height;
    for (int32_t i = 1; i < width; i++) {
        AInt16a y = transform_tdata(databuf[i + data->length / 2], height) + height;
        aDrawLine(w,     beforex,     beforey,     i,     y, color);
        aDrawLine(w,     beforex, beforey + 1,     i, y + 1, color);
        aDrawLine(w, beforex + 1,     beforey, i + 1,     y, color);
        aDrawLine(w, beforex + 1, beforey + 1, i + 1, y + 1, color);
        beforex = i;
        beforey = y;
        // aSetPix0(w, i, y, color);
    }

    // 何等かのキーが押されるまで待機
    aInkeyWait(w, 1);
    free(w);
}

void oto_instr_printwav(Status *status) {
    Sound *sound = NULL;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popp();
        if (var->type != TY_SOUND) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        sound = (Sound *)var->value.p;
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
    } else {
        oto_error(OTO_ARGUMENTS_TYPE_ERROR);
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

    if (freq == 0) {
        freq = 1;
    }

    Playdata data;
    data.freq[0] = freq;
    data.length  = status->sampling_rate * duration;
    data.sound   = sound;
    data.volume  = (int8_t)volume;

    if (IS_NOT_NULL(databuf)) {
        free(databuf);
    }
    databuf = MYMALLOC(data.length, float);
    if (IS_NULL(databuf)) {
        oto_error(OTO_INTERNAL_ERROR);
    }
   
    write_out_data(data, true);

    printf("[Play] ");
    if (sound != NULL) {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : %d\n", 
               freq, duration, (int64_t)volume, sound->oscillator->wave);
    } else {
        printf("frequency : %8.3f, length : %2.2f, velocity : %I64d, wave : NULL\n", 
               freq, duration, (int64_t)volume);
    }

    set_stream_active_flag(true);
    while (is_stream_active()) {
        usleep(1);
    }

    print_wave_sub(status, &data);
}

void oto_instr_printvar(VectorPTR *var_list, Status *status) {
    if (var_list->length <= TC_EXIT + 1) {
        // 変数が1つも定義されていない
        printf("No variables have been declared\n");
        return;
    }

    if (status->language == LANG_JPN_KANJI) {
        printf("- 変数リスト -\n");
    } else if (status->language == LANG_JPN_HIRAGANA) {
        printf("- へんすうリスト -\n");
    } else if (status->language == LANG_ENG) {
        printf("- Variable list -\n");
    }
    
    uint64_t i = TC_EXIT + 1;
    do {
        Var *var = ((Var *)var_list->data[i]);
        i++;

        vartype_t type = var->type;
        if (type == TY_CONST) {
            continue;
        } else if (type == TY_FLOAT) {
            printf("%8s", var->token->str);
            printf("(float) : ");
            printf("%f", var->value.f);
        } else if (type == TY_ARRAY) {
            continue;
        } else if (type == TY_STRING) {
            if (var->token->type == TK_TY_STRING) {
                // 文字列リテラルならスルー
                continue;
            }
            printf("%8s", var->token->str);
            printf("(string) : ");
            printf("%s\n", ((String *)var->value.p)->str);
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
        oto_error(OTO_FILTER_ERROR);
    }

    int64_t param = def_filters[fc].param;
    for (int64_t i = param - 1; i >= 0; i--) {
        if (vmstack_typecheck() == VM_TY_VARPTR) {
            filter->args[i] = vmstack_popp();
        } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
            filter->args[i] = vmstack_popp();
        } else if (vmstack_typecheck() == VM_TY_INITVAL) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
    }
            
    vector_ptr_append(sound->filters, (void *)filter);
}