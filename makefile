SRCSLIST := main.c util.c run.c token.c lexer.c \
			preprocess.c compile.c exec.c debug.c

PROGRAM       := oto
DEBUGPROGRAM  := debug
SRCDIR        := src2
OUTDIR        := build
LIBDIR        := lib
INCLUDEDIR    := include
TARGET        := $(OUTDIR)/$(PROGRAM)
DEBUGTARGET   := $(OUTDIR)/$(DEBUGPROGRAM)

SRCS      := $(addprefix $(SRCDIR)/,$(SRCSLIST))
OBJS      := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
DEBUGOBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%_debug.o,$(SRCS)))

CC      = gcc
CFLAGS  = -Wall -O2
LIB     = -lwinmm -lacl -lgdi32 -lportaudio
# ----------------------------------------------

.PHONY: 
	all clean

all: $(TARGET)

# ----------------------------------------------

# 本番用
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -I./$(INCLUDEDIR) -L./$(LIBDIR) $(LIB)

$(OUTDIR)/%.o: %.c
# @if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $< -I./$(INCLUDEDIR) -L./$(LIBDIR) $(LIB)

# ----------------------------------------------

# デバッグ用
$(DEBUGTARGET): $(DEBUGOBJS)
	$(CC) $(CFLAGS) -o $@ $^ -I./$(INCLUDEDIR) -L./$(LIBDIR) $(LIB)

$(OUTDIR)/%_debug.o: %.c
# @if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -D DEBUG -o $@ -c $< -I./$(INCLUDEDIR) -L./$(LIBDIR) $(LIB)

# ----------------------------------------------

# runするときに実行するファイル名
TESTSRCPATH = aaa.oto
run: $(TARGET)
	$(TARGET) $(TESTSRCPATH)

debug: $(DEBUGTARGET)
	$(DEBUGTARGET) $(TESTSRCPATH)

time: $(TARGET)
	$(TARGET) -T $(TESTSRCPATH)

# ----------------------------------------------

# linux
# clean:
# 	rm -rf $(OUTDIR)

# windows
# powershellがmakefileだと上手く使えなかったのでベタ書き(いつか改善する)
WIN_BUILD_DIR := $(OUTDIR)/$(SRCDIR)
builddir:
	mkdir "./$(WIN_BUILD_DIR)/test"

clean:
	del /s /q %cd%\build\*