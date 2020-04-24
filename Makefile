CFLAGS=-c -g -Wall

run: clean compil
	./subway
	
valgrind: compil
	valgrind --leak-check=full ./subway
	
compil: subway.o readWrite.o graph.o 
	gcc -Wall -o subway subway.o readWrite.o graph.o

subway.o: subway.c graph.h readWrite.h type.h
	gcc $(CFLAGS) subway.c
	
readWrite.o: readWrite.c readWrite.h graph.h type.h
	gcc $(CFLAGS) readWrite.c
	
graph.o: graph.c graph.h readWrite.h type.h
	gcc $(CFLAGS) graph.c
	
indent: 
	indent -linux *.h
	indent -linux *.c
	rm -f *.h~
	rm -f *.c~
	
clean:
	rm -f subway
	rm -f subway.o
	rm -f readWrite.o
	rm -f graph.o
