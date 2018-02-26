
CC = g++
CFLAGS = -g -Wall
BIN = Citysim

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN) 
	rm -f cityinfo.txt citydtable.txt citygraph.txt
