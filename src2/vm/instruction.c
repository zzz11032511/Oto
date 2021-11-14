#include <oto.h>
#include <oto_sound.h>

#include "vm.h"

void oto_instr_beep() {
    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        duration = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        freq = 500.0;
    }

    Beep(freq, duration * 1000);
}

void oto_instr_print() {
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        printf("%f\n", vmstack_popv()->value.f);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        printf("%f\n", vmstack_popf());
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error_exit(OTO_MISSING_ARGUMENTS_ERROR);
    }
}