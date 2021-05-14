extern int *ic[10000];    // 内部コード
extern int **icq;         // ic[]への書き込みカウンタ

enum {
    OpCpy = 0,
    OpCeq,
    OpCne,
    OpClt,
    OpCge,
    OpCle,
    OpCgt, 
    OpAdd,
    OpAdd1, 
    OpSub,
    OpSub1,
    OpMul, 
    OpDiv, 
    OpMod, 
    OpAnd, 
    OpNeg,
    OpGoto,
    OpJeq,
    OpJne,
    OpJlt,
    OpJge,
    OpJle,
    OpJgt,
    OpLop,
    OpPrint,
    OpTime, 
    OpEnd
};

/* 内部コードに書き込むための便利関数 */
void putIc(int op, int *p1, int *p2, int *p3, int *p4);

/* トークンコード列を内部コードに変換する関数 */
int compile(unsigned char *s, int *tc);


int phrCmp_tc[32 * 100];    // フレーズのlexer()結果を保存する
int ppc1;                   // 一致したフレーズの次のトークンをさす
int wpc[9];                 // ワイルドカードのトークンの場所をさす
int wpc1[9];                // ワイルドカードのトークンの場所をさす

/* tc[pc]からのトークンコード列がphrで指定されたトークン列と一致するかどうか調べる関数 */
int phrCmp(int pid, const char *phr, int pc);