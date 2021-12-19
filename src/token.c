#include <oto/oto.h>

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
    {TC_PLUSEQ,  "+=", 2, 0}, {TC_MINUEQ,  "-=", 2, 0},
    {TC_ASTEEQ,  "*=", 2, 0}, {TC_SLASEQ,  "/=", 2, 0},
    {TC_PERCEQ,  "%=", 2, 0},
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
    {TC_PRINTVAR,  "printvar",  8, 1}, {TC_EXIT,      "exit",      4, 1},
    {0,            NULL,        0, 1},
};

static Token *new_token(tokencode_t tc,
                        char *str, size_t len, tokentype_t type) {
    Token *token = MYMALLOC1(Token);
    if (IS_NULL(token)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    token->tc   = tc;
    token->len  = len;
    token->type = type;

    // '\0"の分1足す
    char *s = MYMALLOC(len + 1, char);
    if (IS_NULL(s)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    token->str = s;
    strncpy(token->str, str, len);

    return token;
}

/* もし予約語でない場合は0を返す */
static tokencode_t get_rsvword_tc(char *str, size_t len) {
    int64_t i = 0;
    while (rsvwords[i].str != NULL) {
        // 予約語については大文字小文字を区別しない
        if (is_str_equal_cs(str, len, rsvwords[i].str, rsvwords[i].len)) {
            return rsvwords[i].tc;
        }
        i++;
    }
    return 0;
}

tokencode_t allocate_tc(char *str, size_t len, tokentype_t type, VectorPTR *var_list) {
    tokencode_t tc = get_rsvword_tc(str, len);
    if (tc != 0) {
        // トークンが予約語だった
        return tc;
    }

    size_t tcs = var_list->length;

    for (tc = 0; tc < tcs; tc++) {
        size_t tl = ((Var *)var_list->data[tc])->token->len;
        char *ts  = ((Var *)var_list->data[tc])->token->str;
        if (len == tl && (strncmp(str, ts, len) == 0)) {
            break;
        }
    }

    if (tc == tcs) {
        // 新規作成時の処理
        Token *token = new_token(tc, str, len, type);
        if (IS_NULL(token)) {
            // Error
            oto_error(OTO_INTERNAL_ERROR);
        }
        add_new_variable(var_list, token);
    }

    return tc;
}

static Var *new_variable(Token *token, tokentype_t type) {
    Var *var = MYMALLOC1(Var);
    if (IS_NULL(var)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    var->token   = token;
    var->type    = type;
    var->value.i = 0;

    return var;
}

void add_new_variable(VectorPTR *var_list, Token *new_token) {
    Var *new_var = NULL;

    switch (new_token->type) {
    case TK_TY_SYMBOL:
    case TK_TY_RSVWORD:
        new_var = new_variable(new_token, TY_UNABLE);
        break;
    
    case TK_TY_VARIABLE:
        new_var = new_variable(new_token, TY_VOID);
        break;

    case TK_TY_LITERAL:
        new_var = new_variable(new_token, TY_CONST);
        break;

    default:
        return;
    }

    if (IS_NULL(new_var)) {
        free(var_list);
        oto_error(OTO_INTERNAL_ERROR);
    }

    if (new_token->type == TK_TY_LITERAL) {
        new_var->value.f = strtod(new_token->str, 0);
    }

    vector_ptr_append(var_list, (void *)new_var);
}

void init_var_list(VectorPTR *var_list) {
    if (IS_NULL(var_list)) {
        return;
    }

    int64_t i = 0;
    while (symbols[i].str != NULL) {
        add_new_variable(var_list, &symbols[i]);
        i++;
    }
    i = 0;
    while (rsvwords[i].str != NULL) {
        add_new_variable(var_list, &rsvwords[i]);
        i++;
    }
}

#define IS_HEAP_TYPE(type) \
    (type == TY_ARRAY || type == TY_STRING || type == TY_OSCIL || \
     type == TY_SOUND || type == TY_FILTER)

void free_var_list(VectorPTR *var_list) {
    if (IS_NULL(var_list)) {
        return;
    }

    int64_t i = 0;
    while (i < var_list->length) {
        Var *var = ((Var *)var_list->data[i]);

        if (IS_HEAP_TYPE(var->type)) {
            // 動的に確保するオブジェクトのfree
            free(var->value.p);
        }
        free(var);
        i++;
    }
    free(var_list);
}