#include "compiler.h"

void compile_instruction(int64_t *icp, SliceI64 *srctcs, int64_t *idx) {
    tokencode_t instr = slice_i64_get(srctcs, *idx);
    SliceI64 *argtcs = make_line_tokencodes(srctcs, *idx + 1);
    
    switch (instr) {
    case TC_PRINT:
        compile_args(icp, argtcs, 1);
        put_opcode(icp, OP_PRINT, 0, 0, 0, 0);
        break;
    case TC_BEEP:
        compile_args(icp, argtcs, 2);
        put_opcode(icp, OP_BEEP, 0, 0, 0, 0);
        break;
    case TC_PLAY:
        compile_args(icp, argtcs, 4);
        put_opcode(icp, OP_PLAY, 0, 0, 0, 0);
        break;
    case TC_PRINTWAV:
        compile_args(icp, argtcs, 4);
        put_opcode(icp, OP_PRINTWAV, 0, 0, 0, 0);
        break;
    case TC_PRINTVAR:
        compile_args(icp, argtcs, 0);
        put_opcode(icp, OP_PRINTVAR, 0, 0, 0, 0);
        break;
    case TC_SLEEP:
        compile_args(icp, argtcs, 1);
        put_opcode(icp, OP_SLEEP, 0, 0, 0, 0);
        break;
    case TC_SETSYNTH:
        compile_args(icp, argtcs, 5);
        put_opcode(icp, OP_SETSYNTH, 0, 0, 0, 0);
        break;
    default:
        oto_error(OTO_INTERNAL_ERROR);
    }

    *idx += argtcs->length + 1;
}
