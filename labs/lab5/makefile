
CC = g++
CFLAGS = -g -Wall

BIN = Mazemake Mazesolve Mazeshow

all: $(BIN)

Mazemake: Mazemake.cpp Dset.cpp Support.cpp 
	$(CC) $(CFLAGS) -o Mazemake Mazemake.cpp Dset.cpp Support.cpp

Mazesolve: Mazesolve.cpp Dset.cpp Support.cpp
	$(CC) $(CFLAGS) -o Mazesolve Mazesolve.cpp Dset.cpp Support.cpp

Mazeshow: Mazeshow.cpp 
	$(CC) $(CFLAGS) -o Mazeshow Mazeshow.cpp

.cpp.o: 
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(BIN) $(OBJ) [mp]*.txt [mp]*.ppm [mp]*.png
