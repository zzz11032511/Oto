#pragma once

#include <oto.h>

enum {
    VM_TY_IMMEDIATE,
    VM_TY_VARPTR,
    VM_TY_INITVAL
};
typedef int64_t vmvaltype_t;

vmvaltype_t vmstack_typecheck();

void vmstack_pushi(int64_t i);
void vmstack_push_initval();
void vmstack_pushf(double f);
void vmstack_pushv(Var *v);
void vmstack_pushp(void *p);
int64_t vmstack_popi();
double vmstack_popf();
Var *vmstack_popv();
void *vmstack_popp();

void alu(opcode_t op);
