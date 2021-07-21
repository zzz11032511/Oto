SRCSLIST := main.c run.c debug.c errorHandle.c \
		    lexer/lexer.c lexer/token.c \
			utils/util.c utils/iStack.c \
			variable/vStack.c \
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

DEL := rmdir

CC = gcc
CFLAGS = -Wall -O2

.PHONY: 
	all clean

all: $(TARGET)

# 本番用
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

# デバッグ用
$(DEBUGTARGET): $(DEBUGOBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%_debug.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -D DEBUG -o $@ -c $<

# runするときに実行するファイル名
TESTSRCPATH = aaa.oto
run: $(TARGET)
	$(TARGET) $(TESTSRCPATH)

debug: $(DEBUGTARGET)
	$(DEBUGTARGET) $(TESTSRCPATH)

clean:
	rm -rf $(OUTDIR)