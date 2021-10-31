#include "oto.h"

static VectorPTR *token_list = NULL;

void init_token_list() {
    if (IS_NOT_NULL(token_list)) {
        free_item_vector_ptr(token_list);
        free(token_list);
    }

    token_list = new_vector_ptr(DEFAULT_MAX_TC);
    if (IS_NULL(token_list)) {
        // エラー処理
        return;
    }
}

static Token *new_token(tokencode_t tc,
                        char *str, size_t len, tokentype_t type) {
    Token *token = MYMALLOC1(Token);
    if (IS_NULL(token)) {
        return NULL;
    }

    token->tc   = tc;
    token->len  = len;
    token->type = type;

    char *s = MYMALLOC(len, char);
    if (IS_NULL(s)) {
        return NULL;
    }

    token->str = s;
    strncpy(token->str, str, len);

    return token;
}

static void add_new_token(tokencode_t tc,
                          char *str, size_t len, tokentype_t type) {
    Token *token = new_token(tc, str, len, type);
    if (IS_NULL(token)) {
        // エラー処理
        return;
    }

    TEST_EQ_NOT_PRINT(tc, token_list->length);

    if (IS_NOT_NULL(token_list->data[tc])) {
        free(token_list->data[tc]);
    }

    set_vector_ptr(token_list, tc, (void *)token);
}

static void append_new_token(char *str, size_t len, tokentype_t type) {
    add_new_token(token_list->length, str, len, type);
}

static struct rsvword {
    int8_t *str;
    tokencode_t tc;
} rsvwords[] = {
    {"begin",     TC_BEGIN     }, {"end",       TC_END       },
    {"define",    TC_DEFINE    }, {"if",        TC_IF        },
    {"elsif",     TC_ELSIF     }, {"else",      TC_ELSE      },
    {"then",      TC_THEN      }, {"loop",      TC_LOOP      }, 
    {"and",       TC_AND       }, {"or",        TC_OR        },
    {"not",       TC_NOT       }, {"func",      TC_FUNC      },
    {"track",     TC_TRACK     }, {"filter",    TC_FILTER    },
    {"oscil",     TC_OSCIL     }, {"sound",     TC_SOUND     },
    {"print",     TC_PRINT     }, {"beep",      TC_BEEP      },
    {"play",      TC_PLAY      }, {"note",      TC_NOTE      },
    {"mute",      TC_MUTE      }, {"bpm",       TC_BPM       },
    {"printwav",  TC_PRINTWAV  }, {"exportwav", TC_EXPORTWAV },
    {"importwav", TC_IMPORTWAV }, {"defse",     TC_DEFSE     },
    {"spectrum",  TC_SPECTRUM  }, {"setfs",     TC_SETFS     },
    {"midiin",    TC_MIDIIN    }, {"midiout",   TC_MIDIOUT   },
    {"exit",      TC_EXIT      },
    {NULL,        0            },
};

tokencode_t get_rsvword_tc(char *str, size_t len) {
    tokencode_t tc = 0;
    while (rsvwords[tc].str != NULL) {
        if (strcmp_cs(rsvwords[tc].str, str) == 0) {
            return tc;
        }
        tc++;
    }
    return 0;
}

tokencode_t allocate_tc(char *str, size_t len, tokentype_t type) {
    tokencode_t tc = get_rsvword_tc(str, len);
    if (tc != 0) {
        // トークンが予約語だった
        return tc;
    }

    size_t tcs = token_list->length;

    for (tc = 0; tc < tcs; tc++) {
        size_t tl = ((Token *)token_list->data[tc])->len;
        char *ts  = ((Token *)token_list->data[tc])->str;
        if (len == tl && (strncmp(str, ts, len) == 0)) {
            break;
        }
    }

    if (tc == tcs) {
        // 新規作成時の処理
        append_new_token(str, len, type);
    }

    return tc;
}


static Var *new_variable(Token *token, tokentype_t type) {
    Var *var = MYMALLOC1(Var);
    if (IS_NULL(var)) {
        return NULL;
    }

    var->token   = token;
    var->type    = type;
    var->value.i = 0;

    return var;
}

VectorPTR *make_var_list() {
    VectorPTR *var_list = new_vector_ptr(token_list->length);
    if (IS_NULL(var_list)) {
        return NULL;
    }

    Token *now_token = NULL;    
    Var *new_var = NULL;
    for (uint64_t i = 0; i < token_list->length; i++) {
        now_token = ((Token *)token_list->data[i]);

        switch (now_token->type) {
        case TK_TY_SYMBOL:
        case TK_TY_RSVWORD:
            new_var = new_variable(now_token, TY_UNABLE);
            break;
        
        case TK_TY_VARIABLE:
            new_var = new_variable(now_token, TY_VOID);
            break;

        case TK_TY_LITERAL:
            new_var = new_variable(now_token, TY_CONST);
            break;

        default:
            return NULL;
        }
        if (IS_NULL(new_var)) {
            free(var_list);
            return NULL;
        }

        if (now_token->type == TK_TY_LITERAL) {
            new_var->value.f = strtod(now_token->str, 0);
        }

        TEST_EQ_NOT_PRINT(i, var_list->length);

        append_vector_ptr(var_list, (void *)new_var);
    }

    return var_list;
}

void print_var(VectorPTR *var_list) {
    uint64_t i = 0;
    do {
        Var *var = ((Var *)var_list->data[i]);

        printf("var_list[%d] : ", i);
        printf("tc : %d,",  var->token->tc);
        printf("str : %s ", var->token->str);
        printf("strlen : %d,",  var->token->len);
        printf("var_type : %d", var->type);
        printf("value : %f", var->value.f);
        printf("\n");
        i++;

    } while (i < var_list->length);
}