#pragma once

// 内部コード列の長さ
#define IC_LIST_SIZE 10000

enum {
    OpNop = 0,    // 何もしない
    OpDef,        // 変数定義
    OpCpy,        // 変数への代入
    OpPush,       // スタックからプッシュする
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpPrint,      // 変数の出力
    OpTime,       // 時間計測
    OpEnd         // 終了
};