CC = g++
CFLAGS = -g -Wall -std=c++11

all: Data_processor

Data_processor: Data_processor.cpp Sptrsort.h Person.h Person.cpp
	$(CC) $(CFLAGS) -o $@ Data_processor.cpp Person.cpp

clean:
	rm -f data_processor
