SRCSLIST := main.c run.c \
			error/error.c \
			debug/debug.c \
			ic/ic.c \
		    lexer/lexer.c lexer/preprocess.c \
			token/token.c \
			parser/parser.c parser/expr.c parser/block.c parser/flow.c \
			parser/args.c parser/instruction.c parser/conn_filter.c \
			util/util.c util/int_stack.c \
			variable/variable.c variable/var_stack.c \
			vm/exec.c vm/operation/print.c vm/operation/alu.c \
			vm/operation/sound_instructions.c \
			sound/sound.c sound/track.c sound/sound_io.c sound/oscillator.c \
			sound/filter/filter.c sound/filter/amp.c sound/filter/fade.c sound/filter/iir_filter.c \
			sound/filter/fm.c

PROGRAM       := oto
DEBUGPROGRAM  := debug
SRCDIR        := src
TESTDIR       := test
OUTDIR        := build
LIBDIR        := lib
TARGET        := $(OUTDIR)/$(PROGRAM)
DEBUGTARGET   := $(OUTDIR)/$(DEBUGPROGRAM)

SRCS      := $(addprefix $(SRCDIR)/,$(SRCSLIST))
OBJS      := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
DEBUGOBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%_debug.o,$(SRCS)))
# $(warning $(OBJS))

CC      = gcc
CFLAGS  = -Wall -O2
LIB     = -lwinmm -lacl -lgdi32
INCLUDE = -I ./include
# ----------------------------------------------

.PHONY: 
	all clean

all: $(TARGET)

# ----------------------------------------------

# 本番用
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE) -L./$(LIBDIR) $(LIB)

$(OUTDIR)/%.o: %.c
# @if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE) -L./$(LIBDIR) $(LIB)

# ----------------------------------------------

# デバッグ用
$(DEBUGTARGET): $(DEBUGOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE) -L./$(LIBDIR) $(LIB)

$(OUTDIR)/%_debug.o: %.c
# @if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -D DEBUG -o $@ -c $< $(INCLUDE) -L./$(LIBDIR) $(LIB)

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
	mkdir "./$(WIN_BUILD_DIR)/lexer"
	mkdir "./$(WIN_BUILD_DIR)/token"
	mkdir "./$(WIN_BUILD_DIR)/parser"
	mkdir "./$(WIN_BUILD_DIR)/util"
	mkdir "./$(WIN_BUILD_DIR)/variable"
	mkdir "./$(WIN_BUILD_DIR)/error"
	mkdir "./$(WIN_BUILD_DIR)/debug"
	mkdir "./$(WIN_BUILD_DIR)/ic"
	mkdir "./$(WIN_BUILD_DIR)/vm"
	mkdir "./$(WIN_BUILD_DIR)/vm/operation"
	mkdir "./$(WIN_BUILD_DIR)/sound"
	mkdir "./$(WIN_BUILD_DIR)/sound/filter"

clean:
	del /s /q %cd%\build\*
