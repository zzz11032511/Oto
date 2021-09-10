#include <stdio.h>
#include <stdint.h>

#include "../opcode.h"
#include "../lexer/lexer.h"
#include "../token/token.h"
#include "../variable/variable.h"
#include "../ic/ic.h"

void print_converted_source() {
    size_t len = get_conv_source_size();

    printf("Converted source code\n");
    for (uint32_t i = 0; i < len; i++) {
        uint32_t tc = get_tc(i);
        printf("[%d] : ", i);
        print_token_str(tc);
        printf(" (%d)\n", tc);
    }
    printf("\n");
}

void print_all_token() {
    size_t tcs = get_conv_token_size();

    printf("Tokenize result\n");
    for (uint32_t i = 0; i < tcs; i++) {
        printf("[%d] : ", i);
        print_token_str(i);
        printf("\n");
    }
    printf("\n"); 
}

void print_rpn_tc(uint32_t *rpn_tc_list, uint32_t len) {
    printf("rpn : ");
    for (int32_t i = 0; i < len; i++) {
        uint32_t tc = rpn_tc_list[i];
        print_token_str(tc);
        printf(" ");     
    }
    printf("\n");    
}

struct opname {
    int8_t *name;
    opcode_t code;
};

static const struct opname opnames[] = {
    {"Nop",     OpNop     },
    {"CpyD",    OpCpyD    },
    {"CpyP",    OpCpyP    },
    {"Push",    OpPush    },
    {"PushC",   OpPushC   },
    {"Add",     OpAdd     },
    {"Add2",    OpAdd2    },
    {"Sub",     OpSub     },
    {"Sub2",    OpSub2    },
    {"Mul",     OpMul     },
    {"Mul2",    OpMul2    },
    {"Div",     OpDiv     },
    {"Div2",    OpDiv2    },
    {"Mod",     OpMod     },
    {"Mod2",    OpMod2    },
    {"And",     OpAnd     },
    {"Or",      OpOr      },
    {"Eq",      OpEq      },
    {"NEq",     OpNEq     },
    {"LtCmp",   OpLtCmp   },
    {"LtEqCmp", OpLtEqCmp },
    {"RiCmp",   OpRiCmp   },
    {"RiEqCmp", OpRiEqCmp },
    {"Loop",    OpLoop    },
    {"Jmp",     OpJmp     },
    {"Jz",      OpJz      },
    {"Jnz",     OpJnz     },
    {"Print",   OpPrint   },
    {"DefS",    OpDefS    },
    {"CpyS",    OpCpyS    },
    {"Beep",    OpBeep    },
    {"Play",    OpPlay    },
    {"Filter",  OpFilter  },
    {"Exit",    OpExit    },
};

extern var_t *ic[];

void print_opcodes() {
    var_t **p = ic;
    uint64_t ic_num = 0;

    printf("\nExecute code\n");
    while (1) {
        uint64_t op = (uint64_t)p[0];

        printf("%5I64u : %9s    ", ic_num, opnames[op].name);

        for (int32_t i = 1; i <= 4; i++) {
            if ((OpLoop <= op && op <= OpJnz) && i == 1) {
                printf("%14I64u", (uint64_t)p[i]);
            } else if (p[i] == 0) {
                break;
            } else {
                print_token_str(p[i]->tc);
                // 14文字分の幅を空ける(ネストが深い)
                size_t len = get_token_strlen(p[i]->tc);
                for (int32_t i = 1; i < (14 - len); i++) {
                    printf(" ");
                }
            }
            printf("    ");
        }
        printf("\n");
 
        if (op == OpExit) {
            break;
        }

        ic_num += 5;
        p += 5;
    }
    printf("\n");
}