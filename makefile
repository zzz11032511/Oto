FILE = main
BIN  = main

all:
	$(BIN).exe

$(BIN).exe: $(FILE).c
	gcc -o $(BIN) $(FILE).c

run: $(BIN).exe
	./$(BIN).exe

buildO2: $(FILE).c
	gcc -o $(BIN) -O2 $(FILE).c