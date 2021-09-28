#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#if (defined(AARCH_X86))
#define AINTBIT 32
typedef signed char AInt8;
typedef int AInt16;
typedef int AInt32;
typedef long long AInt64;
typedef unsigned char AUInt8;
typedef int AUInt16;
typedef unsigned int AUInt32;
typedef unsigned long long AUInt64;
#elif (defined(AARCH_X64))
#define AINTBIT 64
typedef signed char AInt8;
typedef int AInt16;
typedef int AInt32;
typedef long long AInt64;
typedef unsigned char AUInt8;
typedef int AUInt16;
typedef unsigned int AUInt32;
typedef unsigned long long AUInt64;
#elif (defined(AARCH_ARM32))
#define AINTBIT 32
typedef signed char AInt8;
typedef int AInt16;
typedef int AInt32;
typedef long long AInt64;
typedef unsigned char AUInt8;
typedef int AUInt16;
typedef unsigned int AUInt32;
typedef unsigned long long AUInt64;
#elif (defined(AARCH_AARCH64))
#define AINTBIT 64
typedef signed char AInt8;
typedef int AInt16;
typedef int AInt32;
typedef long long AInt64;
typedef unsigned char AUInt8;
typedef int AUInt16;
typedef unsigned int AUInt32;
typedef unsigned long long AUInt64;
#endif

typedef int8_t AInt8a;
typedef int16_t AInt16a;
typedef int32_t AInt32a;
typedef int64_t AInt64a;
typedef uint8_t AUInt8a;
typedef uint16_t AUInt16a;
typedef uint32_t AUInt32a;
typedef uint64_t AUInt64a;
typedef intptr_t AInt;
typedef uintptr_t AUInt;
typedef FILE AFile;

#define aStdin stdin
#define aStdout stdout
#define aStderr stderr

#define avfPrintf vfprintf
#define afPrintf fprintf
#define aExitInt exit
#define aPrintf printf
#define avsnPrintf vsnprintf

#define aSizeof (AInt)sizeof

#define aDbgErrExit(...)

#define AMODE_SET 0
#define AMODE_OR 1
#define AMODE_AND 2
#define AMODE_XOR 3

#define AKEY_ENTER 10
#define AKEY_ESC 27
#define AKEY_BACKSPACE 8
#define AKEY_TAB 9
#define AKEY_PAGEUP 0x1020
#define AKEY_PAGEDWN 0x1021
#define AKEY_END 0x1022
#define AKEY_HOME 0x1023
#define AKEY_LEFT 0x1024
#define AKEY_RIGHT 0x1025
#define AKEY_UP 0x1026
#define AKEY_DOWN 0x1027
#define AKEY_INS 0x1028
#define AKEY_DEL 0x1029

#define AKEY_LOCKS 0x00f00000

#define AKEY_LV0 0x00
#define AKEY_LV1 0x01
#define AMOS_LV0 0x00
#define AMOS_LV1 0x10
#define AMOS_LV2 0x20
#define AMOS_LV3 0x30

#if (!defined(AMAXWINDOWS))
#define AMAXWINDOWS 16
#endif

#if (!defined(AKEYBUFSIZ))
#define AKEYBUFSIZ 1024
#endif
void aErrExit(const char *f, ...);
void *aErrExitP0(void *p, const char *f, ...);
void *aMalloc(AInt sz);
void aFree(void *p, AInt sz);

typedef struct AWindow_ {
    char title[64];
    AInt16a xsiz, ysiz;
    AInt32a *buf;
    int table_i;
#if (AKEYBUFSIZ > 0)
    AInt32a keybuf[AKEYBUFSIZ];
    AInt kbw, kbr;
    AInt32a inkeyPrm[4];
#endif
#if (!defined(ANOUSE_LEAPFLUSH))
    AInt32a lastFlush;
#endif
    AInt8a phase, mode, autoClose, reqClose, keyLv, mosLv;

    HWND win;
    HINSTANCE hInst;
    BITMAPINFO bmi;
} AWindow;

#define DRAWOVALPARAM 1 

static LRESULT CALLBACK aWndProc(HWND hw, unsigned int msg, WPARAM wp,
                                 LPARAM lp);
static int aWinthread(AWindow *w);
void aInitWin0(AWindow *w, int xsiz, int ysiz, const char *title,
               int autoClose);
void aWait0(AInt32a msec);
AWindow *aOpenWin(AInt16a x, AInt16a y, const char *t, AInt8a autoClose);
void aFlushAll0(AWindow *w);
void aFlushAll(AWindow *w);
void aLeapFlushAll0(AWindow *w, AInt32a ms);
void aLeapFlushAll(AWindow *w, AInt32a ms);
void aPutKeybuf(AWindow *w, AInt32a c);
AInt aGetSpcKeybuf(AWindow *w);
AWindow *aGetWinFromWId(HWND hw);
static LRESULT CALLBACK aWndProc(HWND hw, unsigned int msg, WPARAM wp,
                                 LPARAM lp);
int aAliveWindows();

void aSetPix0(AWindow *w, AInt16a x, AInt16a y, AInt32a c);
AInt32a aRgb8(AInt32a r, AInt32a g, AInt32a b);
void aSetMode(AWindow *w, int m);
void aWait(AInt32a ms);
void aSetPix(AWindow *w, AInt32a x, AInt32a y, AInt32a c);
void aFillRect0(AWindow *w, AInt16a sx, AInt16a sy, AInt16a x0, AInt16a y0,
                AInt32a c);
void aFillRect(AWindow *w, AInt32a sx, AInt32a sy, AInt32a x0, AInt32a y0,
               AInt32a c);
void aDrawRect0(AWindow *w, AInt16a sx, AInt16a sy, AInt16a x0, AInt16a y0,
                AInt32a c);
void aDrawRect(AWindow *w, AInt32a sx, AInt32a sy, AInt32a x0, AInt32a y0,
               AInt32a c);
void aDrawLine0(AWindow *w, AInt16a x0, AInt16a y0, AInt16a x1, AInt16a y1,
                AInt32a c);
void aDrawLine(AWindow *w, AInt32a x0, AInt32a y0, AInt32a x1, AInt32a y1,
               AInt32a c);
void aFillOval0(AWindow *w, AInt16a sx, AInt16a sy, AInt16a x0, AInt16a y0,
                AInt16a c);
void aFillOval(AWindow *w, AInt32a sx, AInt32a sy, AInt32a x0, AInt32a y0,
               AInt32a c);

void aDrawOval(AWindow *w, AInt32a sx, AInt32a sy, AInt32a x0, AInt32a y0, AInt32a c);  
void aPutKeybuf(AWindow *w, AInt32a c);
AInt aGetSpcKeybuf(AWindow *w);
AInt32a aInkeySub1(AWindow *w, AInt i);
AInt aInkeySub0(AWindow *w);
AInt32a aInkey(AWindow *w, AInt flg);
void aDrawStr0(AWindow *w, AInt16a x, AInt16a y, AInt32a c, AInt32a b,
               const char *s);
void aDrawStr(AWindow *w, AInt32a x, AInt32a y, AInt32a c, AInt32a b,
              const char *s);
void aClrKeybuf(AWindow *w);
void aSetInkey(AWindow *w, AInt8a mod);
AInt32a aInkeyWait(AWindow *w, AInt flg);
