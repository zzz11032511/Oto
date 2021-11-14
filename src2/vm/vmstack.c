#include <oto.h>
#include "vm.h"

#define VMSTACK_SIZE 200

static struct {
    union vmvalue_u {
        int64_t i;
        double  f;
        Var    *v;
        void   *p;
    } value;
    vmvaltype_t type;
} vmstack[VMSTACK_SIZE];
static int64_t sp = 0;

vmvaltype_t vmstack_typecheck() {
    return vmstack[sp - 1].type;
}

void vmstack_pushi(int64_t i) {
    if (sp >= VMSTACK_SIZE) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    vmstack[sp].value.i = i;
    vmstack[sp].type = VM_TY_IMMEDIATE;
    sp++;
}

void vmstack_push_initval() {
    if (sp >= VMSTACK_SIZE) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    vmstack[sp].value.i = 0;
    vmstack[sp].type = VM_TY_INITVAL;
    sp++;
}

void vmstack_pushf(double f) {
    if (sp >= VMSTACK_SIZE) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    vmstack[sp].value.f = f;
    vmstack[sp].type = VM_TY_IMMEDIATE;
    sp++;
}

void vmstack_pushv(Var *v) {
    if (sp >= VMSTACK_SIZE) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    vmstack[sp].value.v = v;
    vmstack[sp].type = VM_TY_VARPTR;
    sp++;
}

void vmstack_pushp(void *p) {
    if (sp >= VMSTACK_SIZE) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    vmstack[sp].value.p = p;
    vmstack[sp].type = VM_TY_IMMEDIATE;
    sp++;
}

int64_t vmstack_popi() {
    if (sp < 0) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    sp--;
    return vmstack[sp].value.i;
}

double vmstack_popf() {
    if (sp < 0) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    sp--;
    return vmstack[sp].value.f;
}

Var *vmstack_popv() {
    if (sp < 0) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    sp--;
    return vmstack[sp].value.v;
}

void *vmstack_popp() {
    if (sp < 0) {
        oto_error_exit(OTO_STACK_OVERFLOW_ERROR);
    }
    sp--;
    return vmstack[sp].value.p;
}
