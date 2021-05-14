#define TMP_FLAG_LIST_SIZE 10
extern char tmp_flag[TMP_FLAG_LIST_SIZE];    // 一時変数の利用状況

int tmpAlloc();         // 未使用の一時変数の確保
void tmpFree(int i);    // iが一時変数を指すトークンコードであれば解放する