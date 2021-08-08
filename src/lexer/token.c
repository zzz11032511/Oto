#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "tokencode.h"
#include "../error/error.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* 予約語一覧 */
struct rsvword_t {
    str_t name;
    str_t upper_name;
    int tc;
};

static const struct rsvword_t rsvwords[] = {
    {"begin",   "BEGIN",   TcBegin   },
    {"end",     "END",     TcEnd     },
    {"in",      "IN",      TcIn      },
    {"out",     "OUT",     TcOut     },
    {"import",  "IMPORT",  TcImport  },
    {"define",  "DEFINE",  TcDefine  },
    {"channel", "CHANNEL", TcChannel },
    {"sound",   "SOUND",   TcSound   },
    {"filter",  "FILTER",  TcFilter  },
    {"if",      "IF",      TcIf      },
    {"elsif",   "ELSIF",   TcElsif   },
    {"else",    "ELSE",    TcElse    },
    {"then",    "THEN",    TcThen    },
    {"loop",    "LOOP",    TcLoop    }, 
    {"and",     "AND",     TcAnd     },
    {"or",      "OR",      TcOr      },
    {"not",     "NOT",     TcNot     },
    {"play",    "PLAY",    TcPlay    }, 
    {"bpm",     "BPM",     TcBpm     },
    {"note",    "NOTE",    TcNote    },
    {"mute",    "MUTE",    TcMute    },
    {"print",   "PRINT",   TcPrint   },
    {"beep",    "BEEP",    TcBeep    },
    {"exit",    "EXIT",    TcExit    },
};

/** 
 * 指定した文字列が予約語であればトークンコードを返す 
 * そうでなければ0を返す
 */
uint32_t get_rsvword_tc(str_t ts, uint32_t len) {
    bool_t is_rw = 0;    // "rw" is reserve word
    uint32_t rw_list_num = GET_ARRAY_LENGTH(rsvwords);

    for (int32_t i = 0; i < rw_list_num; i++) {
        uint32_t rw_len = strlen(rsvwords[i].name);
        if (len != rw_len) continue;

        is_rw = strncmp(rsvwords[i].name, ts, rw_len);
        if (is_rw == 0) return rsvwords[i].tc;

        is_rw = strncmp(rsvwords[i].upper_name, ts, rw_len);
        if (is_rw == 0) return rsvwords[i].tc;
    }
    return 0;
}

bool_t is_rsvword_tc(tokenbuf_t *tcbuf, uint32_t tc) {
    bool_t is_rw = get_rsvword_tc(tcbuf->conv_tokens[tc]->ts,
                                  tcbuf->conv_tokens[tc]->tl);
    if (is_rw) return true;
    return false;
}

tokenbuf_t *new_tokenbuf() {
    tokenbuf_t *buf = (tokenbuf_t *)malloc(sizeof(tokenbuf_t));
    if (buf == NULL) {
        call_error(FAILURE_MAKE_TOKENBUFFER_ERROR);
    }
    return buf;
}

void free_tokenbuf(tokenbuf_t *tcbuf) {
    for (int32_t i = tcbuf->tcs; i > 0; i--) {
        free(tcbuf->conv_tokens[i]);
        tcbuf->conv_tokens[i] = NULL;
    }
    free(tcbuf);
    tcbuf = NULL;
}

/* トークンを新しく作る */
struct token *new_token(uint32_t tc, uint32_t len, str_t s) {
    struct token *t = (struct token *)malloc(sizeof(struct token));
    if (t == NULL) {
        call_error(FAILURE_MAKE_TOKEN_ERROR);
    }
    t->tc = tc;
    t->tl = len;
    t->ts = s;
    return t;
}

/**
 * トークンの内容(文字列)を記憶するための領域
 * 文字列の実体はこのバッファに保存されている
 */
static char tcsbuf[(MAX_TC + 1) * 10];

void put_tc(tokenbuf_t *tcbuf, uint32_t tc, str_t s, uint32_t len) {
    if (tcbuf->tcs >= MAX_TC) {
        call_error(TOO_MANY_TOKEN_ERROR);
    }

    strncpy(&tcsbuf[tcbuf->tcb], (char *)s, len);  // tcsBufにsをコピー
    tcsbuf[tcbuf->tcb + len] = 0;  // tcsBufの終わりに終端コードを付ける
    tcbuf->conv_tokens[tc] = new_token(tc, len, s);
    tcbuf->tcb += len + 1;
    tcbuf->tcs++;
}


static int32_t init_done = 0;

uint32_t get_tc(tokenbuf_t *tcbuf, var_t *var_list, str_t s, uint32_t len, uint32_t type) {
    if (init_done) {
        uint32_t tc = get_rsvword_tc(s, len);
        if (tc != 0) return tc;
    }

    uint32_t i;
    for (i = 0; i < tcbuf->tcs; i++) {  // 登録済みの中から探す
        if (len == tcbuf->conv_tokens[i]->tl &&
            strncmp(s, tcbuf->conv_tokens[i]->ts, len) == 0) {
            break;
        }
    }

    if (i == tcbuf->tcs) {  // 新規作成時の処理
        put_tc(tcbuf, i, s, len);

        // 定数だった場合に型を設定し、初期値を設定する
        var_list[i].type = type;
        var_list[i].tc = i;

        switch (type) {
        case TyConst:
            var_list[i].value.fVal = strtod((char *)(tcbuf->conv_tokens[i]->ts), 0);
            break;
        default:
            var_list[i].value.iVal = 0;
            break;
        }
    }

    return i;
}

/* 最初にlexerしておく文字列 */
static const str_t symbols = "\n , : [ ] ( ) <- -> = + - * / % == != < >= <= > begin end in out import define channel sound filter if elsif else then loop and or not play bpm note mute print beep exit\0";

void tc_init(tokenbuf_t *tcbuf, var_t *var_list) {
    uint32_t size = count_string_size(symbols, '\0');
    lexer(symbols, size, tcbuf, var_list);
    init_done = 1;
}