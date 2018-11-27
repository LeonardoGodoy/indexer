all: file_reader.o indexer

counter.o: counter.h
	cc -c counter.c

file_reader.o: file_reader.h
	cc -c file_reader.c

indexer: file_reader.o counter.o
	cc counter.o file_reader.o indexer.c -o indexer -lm

clean:
	rm *.o
