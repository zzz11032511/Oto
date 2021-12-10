#include <oto.h>

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
        compile_args(icp, argtcs, 2);
        put_opcode(icp, OP_PLAY, 0, 0, 0, 0);
        break;

    // case TC_NOTE:
    // case TC_MUTE:
    // case TC_BPM:
    // case TC_PRINTWAV:
    // case TC_EXPORTWAV:
    // case TC_IMPORTWAV:
    // case TC_DEFSE:
    // case TC_SPECTRUM:
    // case TC_SETFS:
    // case TC_MIDIIN:
    // case TC_MIDIOUT:
    default:
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    *idx += argtcs->length + 1;
}