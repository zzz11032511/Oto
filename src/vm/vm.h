#pragma once

#include <oto/oto.h>
#include <oto/oto_sound.h>

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

/* instructions */
void oto_instr_print();
void oto_instr_beep();
void oto_instr_play(Status *status);
void oto_instr_printwav(Status *status);
void oto_instr_printvar(VectorPTR *var_list, Status *status);
void oto_instr_sleep();

void oto_connect_filter(Sound *sound, filtercode_t fc, Status *status);
void oto_define_array(VectorPTR *var_list, Var *var, int64_t arraysize);