#include "oto.h"

void print_src_tokens(VectorI64 *src_tokens) {
    printf("- Source tokens -\n");
    for (uint64_t i = 0; i < src_tokens->length; i++) {
        printf("%I64d ", src_tokens->data[i]);
    }
    printf("\n\n");
}

void print_var(VectorPTR *var_list) {
    printf("- Variable list -\n");

    uint64_t i = 0;
    do {
        Var *var = ((Var *)var_list->data[i]);

        // printf("var_list[%5I64d] : ", i);
        printf("tc : %5I64d, ",  var->token->tc);

        if (i == TC_LF) {
            printf("str : %15s, ", "\\n");
        } else {
            printf("str : %15s, ", var->token->str);
        }

        printf("strlen : %I64d, ",  var->token->len);
        printf("var_type : %2I64d, ", var->type);
        printf("value : %f", var->value.f);
        printf("\n");
        i++;

    } while (i < var_list->length);
    printf("\n");
}

static struct operation {
    char *str;
    opcode_t opcode;
} operations[] = {
    {"NOP",      OP_NOP      },
    {"CPYD",     OP_CPYD     }, {"PUSH",     OP_PUSH     },
    {"PUSHC",    OP_PUSHC    }, {"POP",      OP_POP      },
    {"ADD",      OP_ADD      }, {"SUB",      OP_SUB      },
    {"MUL",      OP_MUL      }, {"DIV",      OP_DIV      },
    {"MOD",      OP_MOD      }, {"AND",      OP_AND      },
    {"OR",       OP_OR       }, {"EQ",       OP_EQ       },
    {"NEQ",      OP_NEQ      }, {"LTCMP",    OP_LTCMP    },
    {"LTEQCMP",  OP_LTEQCMP  }, {"RICMP",    OP_RICMP    },
    {"RIEQCMP",  OP_RIEQCMP  }, {"ADD2",     OP_ADD2     },
    {"SUB2",     OP_SUB2     }, {"MUL2",     OP_MUL2     },
    {"DIV2",     OP_DIV2     }, {"MOD2",     OP_MOD2     },
    {"LOOP",     OP_LOOP     }, {"JMP",      OP_JMP      },
    {"JZ",       OP_JZ       }, {"JNZ",      OP_JNZ      },
    {"PRINT",    OP_PRINT    },
    {"EXIT",     OP_EXIT     }
};

void print_ic_list(VectorPTR *ic_list) {
    printf("- Internal code -\n");

    opcode_t op = 0;
    Var *v1 = NULL;
    Var *v2 = NULL;
    Var *v3 = NULL;
    Var *v4 = NULL;
    
    uint64_t i = 0;
    while (i < ic_list->length) {
        op = (opcode_t)ic_list->data[i++];
        v1 = (Var *)ic_list->data[i++];
        v2 = (Var *)ic_list->data[i++];
        v3 = (Var *)ic_list->data[i++];
        v4 = (Var *)ic_list->data[i++];

        printf("%5I64d : %10s ", i, operations[op].str);
        printf("%10s ", v1->token->str);
        
        if (IS_NULL(v2)) {
            printf("\n");
            continue;
        }
        printf("%10s ", v2->token->str);

        if (IS_NULL(v3)) {
            printf("\n");
            continue;
        }
        printf("%10s ", v3->token->str);

        if (IS_NULL(v4)) {
            printf("\n");
            continue;
        }
        printf("%10s ", v4->token->str);
        printf("\n");
    }
    printf("\n");
}
