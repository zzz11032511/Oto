default :
	$(MAKE) build

build :
	gcc -c main.c
	gcc -c lexer.c
	gcc -c compile.c
	gcc -c exec.c
	gcc -c util.c
	gcc -o oto main.o lexer.o compile.o exec.o util.o