#pragma once

enum OP_CODE {
    OpNop = 0,    // 何もしない
    OpCpyD,       // 変数への単純代入
    OpCpyP,       // スタックからポップしたものを代入する
    OpPush,       // スタックからプッシュする
    OpPushC,      // 定数をプッシュする
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpMod,        // 余り
    OpAnd,        // 論理積
    OpOr,         // 論理和

    OpAdd2,       // 二項加算
    OpSub2,       // 二項減算
    OpMul2,       // 二項掛算
    OpDiv2,       // 二項割算
    OpMod2,       // 二項余り

    OpEq,         // == 
    OpNEq,        // !=
    OpLtCmp,      // <
    OpLtEqCmp,    // <=
    OpRiCmp,      // >
    OpRiEqCmp,    // >=

    OpLoop,       // ループ
    OpJmp,        // 無条件ジャンプ
    OpJz,         // スタックの上が0ならジャンプ
    OpJnz,        // スタックの上が0でないならジャンプ

    OpPrint,      // 変数の出力
    
    OpExit         // 終了
};
