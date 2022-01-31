#include <oto/oto_gui.h>
#include "vm.h"

static void print_array(double *data, int64_t len) {
    printf("[");
    for (int64_t i = 0; i < len; i++) {
        printf("%f", data[i]);
        if (i != len - 1) {
            printf(", ");
        }
    }
    printf("]");
}

void oto_instr_print() {
    DEBUG_IPRINT(vmstack_typecheck());
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popv();
        if (var->type == TY_FLOAT || var->type == TY_CONST) {
            printf("%f\n", var->value.f);
        } else if (var->type == TY_FILTER) {
            printf("[filter]\n");
        } else if (var->type == TY_SOUND) {
            printf("[sound]\n");
        } else if (var->type == TY_OSCIL) {
            printf("[oscillator]\n");
        } else if (var->type == TY_STRING) {
            printf("%s\n", ((String *)var->value.p)->str);
        } else if (var->type == TY_ARRAY) {
            Array *array = (Array *)var->value.p;
            print_array(array->data, array->len);
            printf("\n");
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
        Var *var = vmstack_popp();
        if (var->type != TY_FLOAT && var->type != TY_CONST) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        duration = var->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        // 初期値だとしてもpopしないといけない
        vmstack_popf();
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popp();
        if (var->type != TY_FLOAT && var->type != TY_CONST) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        freq = var->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        freq = 500.0;
    }

    Beep(freq, duration * 1000);
}

static void play_sub(Status *status, Playdata *data) {
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
        Var *var = vmstack_popp();
        if (var->type != TY_FLOAT && var->type != TY_CONST) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        volume = var->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        volume = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        volume = 80;
    }

    if (!(0 <= volume && volume <= 100)) {
        volume = 80;
    }

    // if (status->safety_flag && volume > 35) {
    //     volume = 35;
    // }

    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popp();
        if (var->type != TY_FLOAT && var->type != TY_CONST) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
        duration = var->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        duration = 1;
    }

    double sound_num = 1;
    double freq[MAX_POLYPHONIC] = {0};
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popp();
        if (var->type == TY_ARRAY) {
            Array *array = (Array *)var->value.p;
            sound_num = array->len;
            for (int64_t i = 0; i < sound_num; i++) {
                if (i >= MAX_POLYPHONIC) break;
                freq[i] = array->data[i];
                
                // freqが0だとエラーになるので補正
                if (freq[i] <= 0) {
                    freq[i] = 1;
                }
            }

        } else if (var->type == TY_FLOAT || var->type == TY_CONST) {
            freq[0] = var->value.f;
            if (freq[0] == 0) {
                freq[0] = 1;
            }
        
        } else {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }

    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq[0] = vmstack_popf();
        if (freq[0] <= 0) {
            freq[0] = 1;
        }
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        freq[0] = 500.0;
    }
    
    data->sound_num = sound_num;
    for (int64_t i = 0; i < sound_num; i++) {
        data->freq[i] = freq[i];
    }
    data->length  = status->sampling_rate * duration;
    data->sound   = sound;
    data->volume  = (int8_t)volume;

    printf("[Play] frequency : ");
    print_array(freq, sound_num);
    if (sound != NULL) {
        printf(", duration : %2.2f, volume : %I64d, wave : %d\n", 
               duration, (int64_t)volume, sound->oscillator->wave);
    } else {
        printf(", duration : %2.2f, volume : %I64d, wave : NULL\n", 
               duration, (int64_t)volume);
    }
}

void oto_instr_play(Status *status) {
    Playdata data;

    play_sub(status, &data);
    write_out_data(data, false, true);

    set_stream_active_flag(true);
    while (is_stream_active()) {
        usleep(1);
    }
}

static AInt16a transform_tdata(float data) {
    if (data > 1) {
        data = 1;
    } else if (data < -1) {
        data = -1;
    }
    return (AInt16a)((PRINTWAV_WIN_HEIGHT / 2) - (data * (PRINTWAV_WIN_HEIGHT / 2) * 0.8));
}

static void print_wave_sub(Status *status, Playdata *data) {
    AWindow *w = aOpenWin(PRINTWAV_WIN_WIDTH, PRINTWAV_WIN_HEIGHT * 2, "wave", 1);

    aFillRect(w, PRINTWAV_WIN_WIDTH, PRINTWAV_WIN_HEIGHT * 2, 0, 0, PRINTWAV_WIN_BACKGROUND_COLOR);

    // 全体の波形
    AInt32a color = aRgb8(0, 0xc3, 0xff);
    int32_t beforex = 0;
    int32_t beforey = transform_tdata(databuf[0]);
    int32_t a = data->length / PRINTWAV_WIN_WIDTH;
    for (int32_t i = 0; i < PRINTWAV_WIN_WIDTH; i++) {
        AInt16a y = transform_tdata(databuf[i * a]);
        aDrawLine(w,     beforex,     beforey,     i,     y, color);
        aDrawLine(w,     beforex, beforey + 1,     i, y + 1, color);
        aDrawLine(w, beforex + 1,     beforey, i + 1,     y, color);
        aDrawLine(w, beforex + 1, beforey + 1, i + 1, y + 1, color);
        beforex = i;
        beforey = y;
    }

    // 拡大した波形
    color = aRgb8(0, 0xc3, 0xff);
    beforex = 0;
    beforey = transform_tdata(databuf[data->length / 2]) + PRINTWAV_WIN_HEIGHT;
    for (int32_t i = 1; i < PRINTWAV_WIN_WIDTH; i++) {
        AInt16a y = transform_tdata(databuf[i + data->length / 2]) + PRINTWAV_WIN_HEIGHT;
        aDrawLine(w,     beforex,     beforey,     i,     y, color);
        aDrawLine(w,     beforex, beforey + 1,     i, y + 1, color);
        aDrawLine(w, beforex + 1,     beforey, i + 1,     y, color);
        aDrawLine(w, beforex + 1, beforey + 1, i + 1, y + 1, color);
        beforex = i;
        beforey = y;
    }

    // 何等かのキーが押されるまで待機
    aWait(-1);
}

void oto_instr_printwav(Status *status) {
    Playdata data;
    play_sub(status, &data);
    
    if (IS_NOT_NULL(databuf)) {
        free(databuf);
    }
    databuf = MYMALLOC(data.length, float);
    if (IS_NULL(databuf)) {
        oto_error(OTO_INTERNAL_ERROR);
    }
   
    write_out_data(data, true, true);

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
            printf("%8s", var->token->str);
            printf("(array) : ");
            Array *array = (Array *)var->value.p;
            print_array(array->data, array->len);
            printf("\n");
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
    int64_t time = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        time = (int64_t)((vmstack_popv()->value.f) * 1000);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        time = (int64_t)((vmstack_popf()) * 1000);
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }
    printf("[sleep] %I64d[ms]\n", time);
    Sleep(time);
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

void oto_define_array(VectorPTR *var_list, Var *var, int64_t arraysize) {
    Array *array = MYMALLOC1(Array);
    if (IS_NULL(array)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    array->len = arraysize;
    array->data = MYMALLOC(arraysize, double);
    if (IS_NULL(array->data)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    for (int64_t i = 0; i < arraysize; i++) {
        if (vmstack_typecheck() == VM_TY_VARPTR) {
            array->data[arraysize - i - 1] = ((Var *)vmstack_popp())->value.f;
        } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
            array->data[arraysize - i - 1] = vmstack_popf();
        } else if (vmstack_typecheck() == VM_TY_INITVAL) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }

    }

    var->type = TY_ARRAY;
    var->value.p = (void *)array;
}

void oto_instr_setsynth(Status *status) {
    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        freq = 500;
    }
    
    Array *range = NULL;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        Var *var = vmstack_popv();
        if (var->type != TY_ARRAY) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }

        range = (Array *)var->value.p;
        if (range->len < 2) {
            oto_error(OTO_ARGUMENTS_TYPE_ERROR);
        }
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        oto_error(OTO_ARGUMENTS_TYPE_ERROR);
    } else {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }
    
    Var *var = NULL;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        var = vmstack_popv();
        if (var->type != TY_FLOAT) {
            printf("aaaaa\n");
            oto_error(OTO_MISSING_ARGUMENTS_ERROR);
        }
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        oto_error(OTO_ARGUMENTS_TYPE_ERROR);
    } else {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }

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

    int64_t no = -1;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        no = (int64_t)vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        no = (int64_t)vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error(OTO_MISSING_ARGUMENTS_ERROR);
    }

    set_synth(no, sound, var, range->data[0], range->data[1], freq);
}

void oto_instr_setloop() {
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        set_synth_looppoint(vmstack_popv()->value.f);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        set_synth_looppoint(vmstack_popf());
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        vmstack_popf();
        set_synth_looppoint(10);
    }
}
