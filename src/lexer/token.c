#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "../errorHandle.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* 予約語一覧 */
struct rsvWord_t {
    str_t name;
    str_t upperName;
    int tc;
};

static struct rsvWord_t rsvWord[] = {
    {"begin",   "BEGIN",   TcBegin   }, {"end",     "END",     TcEnd     },
    {"in",      "IN",      TcIn      }, {"out",     "OUT",     TcOut     },
    {"import",  "IMPORT",  TcImport  }, {"define",  "DEFINE",  TcDefine  },
    {"channel", "CHANNEL", TcChannel }, {"sound",   "SOUND",   TcSound   },
    {"filter",  "FILTER",  TcFilter  }, {"if",      "IF",      TcIf      },
    {"elsif",   "ELSIF",   TcElsif   }, {"else",    "ELSE",    TcElse    },
    {"then",    "THEN",    TcThen    }, {"loop",    "LOOP",    TcLoop    }, 
    {"and",     "AND",     TcAnd     }, {"or",      "OR",      TcOr      },
    {"not",     "NOT",     TcNot     }, {"play",    "PLAY",    TcPlay    }, 
    {"bpm",     "BPM",     TcBpm     }, {"note",    "NOTE",    TcNote    },
    {"mute",    "MUTE",    TcMute    }, {"print",   "PRINT",   TcPrint   },
    {"exit",    "EXIT",    TcExit    },
};

/* 予約語かどうか調べる */
int32_t isRsvWord(tokenBuf_t *tcBuf, int32_t tc) {
    int32_t isRW = 0;
    int32_t rsvWordNum = GET_ARRAY_LENGTH(rsvWord);
    for (int32_t i = 0; i < rsvWordNum; i++) {
        int32_t rsvWordLen = strlen(rsvWord[i].name);
        isRW = strncmp(rsvWord[i].name, tcBuf->tokens[tc]->ts, rsvWordLen);
        if (isRW == 0) return rsvWord[i].tc;

        isRW = strncmp(rsvWord[i].upperName, tcBuf->tokens[tc]->ts, rsvWordLen);
        if (isRW == 0) return rsvWord[i].tc;
    }
    return 0;
}

/* トークン保存領域を新しく作る */
tokenBuf_t *newTokenBuf() {
    tokenBuf_t *buf = (tokenBuf_t *)malloc(sizeof(tokenBuf_t));
    if (buf == NULL) {
        callError(FAILURE_MAKE_TOKENBUFFER_ERROR);
    }
    return buf;
}

/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokenBuf(tokenBuf_t *tcBuf) {
    for (int32_t i = tcBuf->tcs; i > 0; i--) {
        free(tcBuf->tokens[i]);
    }
}

/* トークンを新しく作る */
struct token *newToken(int32_t tc, int32_t len, str_t s) {
    struct token *t = (struct token *)malloc(sizeof(struct token));
    if (t == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(FAILURE_MAKE_TOKEN_ERROR);
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
static char tcsBuf[(MAX_TC + 1) * 6];

/* 新しいトークンを作り, tcBuf->tokensに追加する関数 */
int32_t putTc(int32_t tc, int32_t len, str_t s, tokenBuf_t *tcBuf) {
    if (tcBuf->tcs >= MAX_TC) {
        callError(TOO_MANY_TOKEN_ERROR);
    }

    strncpy(&tcsBuf[tcBuf->tcb], (char *)s, len);  // tcsBufにsをコピー
    tcsBuf[tcBuf->tcb + len] = 0;  // tcsBufの終わりに終端コードを付ける
    tcBuf->tokens[tc]        = newToken(tc, len, s);
    tcBuf->tcb += len + 1;
    tcBuf->tcs++;
    return 0;
}

/* トークンコードを得るための関数 */
int32_t getTc(str_t s, int32_t len, tokenBuf_t *tcBuf, var_t *var, int32_t type) {
    int32_t i;
    for (i = 0; i < tcBuf->tcs; i++) {  // 登録済みの中から探す
        if (len == tcBuf->tokens[i]->tl &&
            strncmp(s, tcBuf->tokens[i]->ts, len) == 0) {
            break;
        } else if (len == tcBuf->tokens[i]->tl && 
            strncmp_ignorecase(s, tcBuf->tokens[i]->ts, len) == 0) {
            break;
        }
    }

    if (i == tcBuf->tcs) {  // 新規作成時の処理
        putTc(i, len, s, tcBuf);

        // 定数だった場合に型を設定し、初期値を設定する
        var[i].type = type;
        var[i].tc = i;

        switch (type) {
            case TyConstF:
                var[i].value.fVal = strtod((char *)(tcBuf->tokens[i]->ts), 0);
                break;
            case TyConstI:
                var[i].value.iVal = strtol((char *)(tcBuf->tokens[i]->ts), 0, 0);
                break;
            default:
                var[i].value.iVal = 0;
                break;
        }
    }

    return i;
}

/* 最初にlexerしておく文字列 */
static const str_t symbols = "\n , : [ ] ( ) <- -> = + - * / % == != < >= <= > begin end in out import define channel sound filter if elsif else then loop and or not play bpm note mute print exit\0";

/* 演算子記号などを最初にlexerしておく関数 */
int32_t tcInit(tokenBuf_t *tcBuf, var_t *var) {
    return lexer(symbols, tcBuf, var);
}