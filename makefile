SRCSLIST := main.c run.c debug.c \
		    lexer/lexer.c lexer/token.c \
			utils/util.c utils/iStack.c \
			variable/vStack.c \
			parser/compile.c parser/control.c parser/expr.c \
			vm/exec.c  vm/alu.c

PROGRAM  := oto
SRCDIR   := src
TESTDIR  := test
OUTDIR   := build
TARGET   := $(OUTDIR)/$(PROGRAM)

SRCS := $(addprefix $(SRCDIR)/,$(SRCSLIST))
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
$(warning $(OBJS))

DEL := rmdir

CC = gcc
CFLAGS = -Wall -O2

.PHONY: 
	all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

# runするときに実行するファイル名
TESTSRCPATH = aaa.oto

debug: $(TARGET)
	$(TARGET) ${TESTSRCPATH}

run: $(TARGET)
	$(TARGET) ${TESTSRCPATH}

clean:
	rm -rf $(OUTDIR)