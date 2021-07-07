SRCS      := main.c run.c \
			 lexer/lexer.c lexer/token.c \
			 utils/util.c utils/iStack.c \
			 variable/vStack.c \
			 parser/compile.c parser/control.c parser/expr.c \
			 vm/exec.c  vm/alu.c

OBJS      := main.o run.o \
			 lexer/lexer.o lexer/token.o \
			 utils/util.o utils/iStack.o \
			 variable/vStack.o \
			 parser/compile.o parser/control.o parser/expr.o \
			 vm/exec.o vm/alu.o

TESTFILE  := test_lexer test_stack

SRCPATH   := ./src/
TESTPATH  := ./test/

PROGRAM   := oto
MAKE      := make.exe -r
DEL       := del
MAKEOBJ   := gcc -c
CC        := gcc
CFLAGS    := -o $(PROGRAM) -O2 -Wall

TESTFLAGS := -O2 -Wall

all : $(PROGRAM).exe

%.o : 
	$(MAKEOBJ) $(SRCPATH)$*.c

$(PROGRAM).exe : $(OBJS)
	$(CC) $(CFLAGS) $(notdir $(OBJS))

# テスト用
test_% : $(OBJS)
	$(MAKEOBJ) $(TESTPATH)test_$*.c
	$(CC) $(TESTFLAGS) -o test_$* $(filter-out main.o, $(OBJS)) test_$*.o
	test_$*.exe

clean : 
	-$(DEL) *.o
	-$(DEL) *.exe

run :
	$(PROGRAM) aaa.oto