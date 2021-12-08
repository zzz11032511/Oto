#include <oto.h>

void usage(const char *name) {
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, char **argv) {
    uint32_t i = 1;

    load_otoconfig();

    if (argc == 1) {
        printf("\nOto (beta 2021-12-04) REPL ... Hello!!!\n");
        printf("コマンド一覧を見たいときは「HELP」\n");
        printf("終了するときは「EXIT」\n");
        set_repl_flag(true);
        oto_init("");
        repl();
        return 0;
    }


    // 実行時間表示
    if (argc > 1 && strcmp(argv[1], "-T") == 0) {
        set_timecount_flag(true);
        i++;
    }

    // ファイル名が指定されていない場合
    if (argc != i + 1) {
        usage(argv[0]);
        exit(1);
    }

    oto_init(argv[i]);
    oto_run(argv[i]);

    return 0;
}
