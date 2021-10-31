#include "oto.h"

const Token symbols[] = {
    {TC_LF,     "\n",  1, 0},
    {TC_COMMA,   ",",  1, 0}, {TC_COLON,   ":",  1, 0},
    {TC_SQBROPN, "[",  1, 0}, {TC_SQBRCLS, "]",  1, 0},
    {TC_BROPN,   "(",  1, 0}, {TC_BRCLS,   ")",  1, 0},
    {TC_LARROW,  "<-", 2, 0}, {TC_RARROW,  "->", 2, 0},
    {TC_EQU,     "=",  1, 0}, {TC_PLUS,    "+",  1, 0},
    {TC_MINUS,   "-",  1, 0}, {TC_ASTER,   "*",  1, 0},
    {TC_SLASH,   "/",  1, 0}, {TC_PERCE,   "%",  1, 0},
    {TC_EEQ,     "==", 2, 0}, {TC_NEQ,     "!=", 2, 0},
    {TC_GT,      ">",  1, 0}, {TC_LT,      "<",  1, 0},
    {TC_GE,      ">=", 2, 0}, {TC_LE,      "<=", 2, 0},
    {0,          NULL, 0, 0}
};

const Token rsvwords[] = {
    {TC_BEGIN,     "begin",     5, 1}, {TC_END,       "end",       3, 1},
    {TC_DEFINE,    "define",    6, 1}, {TC_IF,        "if",        2, 1},
    {TC_ELSIF,     "elsif",     5, 1}, {TC_ELSE,      "else",      4, 1},
    {TC_THEN,      "then",      4, 1}, {TC_LOOP,      "loop",      4, 1}, 
    {TC_AND,       "and",       3, 1}, {TC_OR,        "or",        2, 1},
    {TC_NOT,       "not",       3, 1}, {TC_FUNC,      "func",      4, 1},
    {TC_TRACK,     "track",     5, 1}, {TC_FILTER,    "filter",    6, 1},
    {TC_OSCIL,     "oscil",     5, 1}, {TC_SOUND,     "sound",     5, 1},
    {TC_PRINT,     "print",     5, 1}, {TC_BEEP,      "beep",      4, 1},
    {TC_PLAY,      "play",      4, 1}, {TC_NOTE,      "note",      4, 1},
    {TC_MUTE,      "mute",      4, 1}, {TC_BPM,       "bpm",       3, 1},
    {TC_PRINTWAV,  "printwav",  8, 1}, {TC_EXPORTWAV, "exportwav", 9, 1},
    {TC_IMPORTWAV, "importwav", 9, 1}, {TC_DEFSE,     "defse",     5, 1},
    {TC_SPECTRUM,  "spectrum",  8, 1}, {TC_SETFS,     "setfs",     5, 1},
    {TC_MIDIIN,    "midiin",    4, 1}, {TC_MIDIOUT,   "midiout",   7, 1},
    {TC_EXIT,      "exit",      4, 1},
    {0,            NULL,        0, 1},
};

static VectorPTR *token_list = NULL;

void init_token_list() {
    if (IS_NOT_NULL(token_list)) {
        free_token_list();
        token_list = NULL;
    }

    token_list = new_vector_ptr(DEFAULT_MAX_TC);
    if (IS_NULL(token_list)) {
        // エラー処理
        return;
    }
}

void free_token_list() {
    uint64_t i = 0;
    while (i < token_list->length) {
        free(((Token *)(token_list->data[i]))->str);
        free(token_list->data[i]);
        token_list->data[i] = NULL;
        i++;
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

    if (IS_NOT_NULL(token_list->data[tc])) {
        free(token_list->data[tc]);
    }

    set_vector_ptr(token_list, tc, (void *)token);
}

static void append_new_token(char *str, size_t len, tokentype_t type) {
    add_new_token(token_list->length, str, len, type);
}

void init_rsvword() {
    if (IS_NULL(token_list)) {
        return;
    }

    uint64_t i = 0;
    while (symbols[i].str != NULL) {
        append_new_token(symbols[i].str, symbols[i].len, TK_TY_SYMBOL);
        // TEST_EQ(((Token *)token_list->data[i])->tc, symbols[i].tc);
        i++;
    }

    i = 0;
    while (rsvwords[i].str != NULL) {
        append_new_token(rsvwords[i].str, rsvwords[i].len, TK_TY_RSVWORD);
        // TEST_EQ(((Token *)token_list->data[i])->tc, rsvwords[i].tc);
        i++;
    }
}

tokencode_t get_rsvword_tc(char *str, size_t len) {
    tokencode_t tc = TC_BEGIN;
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

#define IS_HEAP_TYPE(type) \
    (type == TY_ARRAY || type == TY_STRING || type == TY_OSCIL || \
     type == TY_SOUND || type == TY_FILTER)

void free_var_list(VectorPTR *var_list) {
    // free_item_vector_ptr(var_list);
    uint64_t i = 0;
    while (i < var_list->length) {
        Var *var = ((Var *)var_list->data[i]);

        if (IS_HEAP_TYPE(var->type)) {
            free(var->value.p);
        }
        free(var);
        var = NULL;
        i++;
    }
    free(var_list);
}

void print_var(VectorPTR *var_list) {
    uint64_t i = 0;
    do {
        Var *var = ((Var *)var_list->data[i]);

        printf("var_list[%d] : ", i);
        printf("tc : %d, ",  var->token->tc);
        printf("str : %s, ", var->token->str);
        printf("strlen : %d, ",  var->token->len);
        printf("var_type : %d, ", var->type);
        printf("value : %f", var->value.f);
        printf("\n");
        i++;

    } while (i < var_list->length);
}
