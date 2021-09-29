#pragma once

typedef enum {
    OpNop = 0,    // 何もしない
    OpCpyD,       // 変数への単純代入
    OpCpyP,       // スタックからポップしたものを代入する
    OpPush,       // スタックからプッシュする
    OpPushC,      // 定数をプッシュする

    OpAdd,        // 加算
    OpAdd2,       // 二項加算
    OpSub,        // 引算
    OpSub2,       // 二項引算
    OpMul,        // 掛算
    OpMul2,       // 二項掛算
    OpDiv,        // 割算
    OpDiv2,       // 二項割算
    OpMod,        // 余り
    OpMod2,       // 二項余り
    OpAnd,        // 論理積
    OpOr,         // 論理和

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

    // Soundはparseの段階で作ってもいいかも
    OpDefS,       // Soundの定義
    OpCpyS,       // Soundのコピー
    OpFilter,     // FilterをSoundにつなぐ

    OpPrint,      // 変数の出力
    OpBeep,       // ビープ音の出力
    OpPlay,       // 音の出力
    OpPrintWav,   // 波形の出力(音も出力する)
    
    OpExit        // 終了    
} opcode_t;