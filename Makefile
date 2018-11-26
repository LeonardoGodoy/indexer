all: file_reader.o indexer

file_reader.o: file_reader.h
	cc -c file_reader.c

indexer: file_reader.o
	cc file_reader.o indexer.c -o indexer -lm

clean:
	rm *.o
