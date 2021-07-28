#pragma once

enum {
    OpNop = 0,    // 何もしない
    OpCpyS,       // 変数への単純代入
    OpCpyP,       // スタックからポップしたものを代入する
    OpPush,       // スタックからプッシュする
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpMod,        // 余り
    OpAnd,        // 論理積
    OpOr,         // 論理和
    OpAdd1,       // 1加算
    OpSub1,       // 1減算

    OpEq,         // == 
    OpNEq,        // !=
    OpLtCmp,      // <
    OpLtEqCmp,    // <=
    OpRiCmp,      // >
    OpRiEqCmp,    // >=

    OpJmp,        // 無条件ジャンプ
    OpJz,         // スタックの上が0ならジャンプ
    OpJnz,        // スタックの上が0でないならジャンプ

    OpPrint,      // 変数の出力
    
    OpExit         // 終了
};