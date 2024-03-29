SRCSLIST := main.c run.c token.c debug.c error.c status.c option.c \
			util/util.c util/vector.c util/map.c util/slice.c util/stack.c \
			lexer/lexer.c lexer/preprocess.c \
			compiler/compiler.c compiler/util_compiler.c compiler/expr.c compiler/flow.c \
			compiler/conn_filter.c compiler/instruction.c compiler/array.c \
			vm/exec.c vm/vmstack.c vm/alu.c vm/instruction.c vm/synth.c \
			sound/stream.c sound/sound.c sound/generator.c sound/filter.c \
			gui/slider.c

PROGRAM       := oto
DEBUGPROGRAM  := debug
SRCDIR        := src
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

TESTDIR := $(SRCDIR)/test
TESTSRCSLIST := $(addprefix $(SRCDIR)/, $(filter-out main.c, $(SRCSLIST)))
TESTTARGET := test_lexer test_preprocess test_token test_util
TESTEXE := $(addsuffix .exe, $(TESTTARGET))

# テスト
test_% :
	$(CC) -D DEBUG -o $@ $(TESTSRCSLIST) $(TESTDIR)/$@.c -I./$(INCLUDEDIR)
	@$@
	@echo success

testclean :
	del $(TESTEXE)

# ----------------------------------------------

# runするときに実行するファイル名
TESTSRCPATH = aaa.oto
build: $(TARGET)

run: $(TARGET)
	$(TARGET) $(TESTSRCPATH)

repl: $(TARGET)
	$(TARGET)

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
	mkdir "./$(WIN_BUILD_DIR)/util"
	mkdir "./$(WIN_BUILD_DIR)/lexer"
	mkdir "./$(WIN_BUILD_DIR)/compiler"
	mkdir "./$(WIN_BUILD_DIR)/vm"
	mkdir "./$(WIN_BUILD_DIR)/sound"
	mkdir "./$(WIN_BUILD_DIR)/gui"

clean:
	del /s /q %cd%\build\*