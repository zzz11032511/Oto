OBJS := main.o lexer.o compile.o exec.o util.o

SRCPATH  := ./src/
TESTPATH := ./test/

PROGRAM  := oto
MAKE     := make.exe -r
DEL      := del
MAKEOBJ  := gcc -c
CC       := gcc
CFLAGS   := -o $(PROGRAM) -O2 -Wall

all : $(PROGRAM)

$(OBJS) :
	$(MAKEOBJ) $(SRCPATH)$*.c

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

clean : 
	-$(DEL) *.o
	-$(DEL) $(PROGRAM).exe

run : $(PROGRAM).exe
	$(PROGRAM)