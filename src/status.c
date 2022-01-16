#include <oto/oto.h>

static Status oto_status = {
    false,  // timecount_flag
    false,  // repl_flag
    true,   // safety_flag
    NULL,   // root_srcpath
    NULL,   // srcfile_table
    LANG_JPN_KANJI,  // language
    44100,  // sampling_rate
    0.05    // fade_range
};

Status *get_oto_status() {
    return &oto_status;
}
