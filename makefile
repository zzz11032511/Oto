SRCSLIST := main.c run.c \
			debug/debug.c \
			error/error.c \
		    lexer/lexer.c lexer/token.c \
			utils/util.c utils/int_stack.c \
			variable/var_stack.c \
			parser/compile.c parser/control.c parser/expr.c \
			vm/exec.c  vm/alu.c

PROGRAM       := oto
DEBUGPROGRAM  := debug
SRCDIR        := src
TESTDIR       := test
OUTDIR        := build
TARGET        := $(OUTDIR)/$(PROGRAM)
DEBUGTARGET   := $(OUTDIR)/$(DEBUGPROGRAM)

SRCS      := $(addprefix $(SRCDIR)/,$(SRCSLIST))
OBJS      := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
DEBUGOBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%_debug.o,$(SRCS)))
# $(warning $(OBJS))

CC = gcc
CFLAGS = -Wall -O2

# ----------------------------------------------

.PHONY: 
	all clean

all: $(TARGET)

# ----------------------------------------------

# 本番用
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

# ----------------------------------------------

# デバッグ用
$(DEBUGTARGET): $(DEBUGOBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%_debug.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -D DEBUG -o $@ -c $<

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
clean:
	rm -rf $(OUTDIR)

# windows
# powershellがmakefileだと上手く使えなかったのでベタ書き(いつか改善する)
# WIN_BUILD_DIR := $(OUTDIR)/$(SRCDIR)
# builddir:
# 	mkdir "./$(WIN_BUILD_DIR)/lexer"
# 	mkdir "./$(WIN_BUILD_DIR)/parser"
# 	mkdir "./$(WIN_BUILD_DIR)/utils"
# 	mkdir "./$(WIN_BUILD_DIR)/variable"
# 	mkdir "./$(WIN_BUILD_DIR)/vm"
# 	mkdir "./$(WIN_BUILD_DIR)/debug"
# 	mkdir "./$(WIN_BUILD_DIR)/error"

# clean:
# 	del /s /q %cd%\build\*
