#pragma once

// 内部コード列の長さ
#define IC_LIST_SIZE 10000

enum OP_CODE {
    OpNop = 0,    // 何もしない
    OpDef,        // 変数定義
    OpCpyS,       // 変数への単純代入
    OpCpyP,       // スタックからポップしたものを代入する
    OpPush,       // スタックからプッシュする
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpMod,        // 余り
    OpAdd1,        // 1加算
    OpSub1,        // 1減算
    OpPrint,      // 変数の出力

    OpEnd         // 終了
};