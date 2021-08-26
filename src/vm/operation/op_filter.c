#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#include "../opcode.h"
#include "../../error/error.h"
#include "../../debug/debug.h"
#include "../../variable/variable.h"
#include "../../variable/var_stack.h"
#include "../../sound/sound.h"
#include "../../sound/filter/filter.h"

void op_filter(struct var_stack *stack, var_t *sound, var_t *filter) {
    if (sound->type != TySound || filter->type != TyFilter) {
        call_exception(EXCEPTION);
    }

    struct var_stack sub_stack;
    sub_stack.sp = 0;

    int32_t num_of_params = 1;
    
    // 現在つながっているフィルター
    var_t *connected = ((SOUND)sound->value.pVal)->filters; 
    // 現在つながっているフィルターの数
    int32_t ptr = ((SOUND)sound->value.pVal)->filter_ptr;

    connected[ptr++] = *filter;

    // 引数の順番が逆なので最初に一時的なスタックに移してから接続する
    for (int32_t i = 0; i < num_of_params; i++) {
        vpush(&sub_stack, vpop(stack));
    }

    for (int32_t i = 0; i < num_of_params; i++) {
        connected[ptr++] = vpop(&sub_stack);
    }

    ((SOUND)sound->value.pVal)->filter_ptr = ptr;
}