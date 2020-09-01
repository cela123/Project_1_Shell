shell: parser.o shell.o path_search.o
	gcc -o shell.x -std=c99 parser.o shell.o path_search.o

parser.o: parser.c parser.h
	gcc -c -std=c99 parser.c

path_search.o: path_search.c path_search.h
	gcc -c -std=c99 path_search.c

shell.o: shell.c
	gcc -c -std=c99 shell.c

clean:
	rm *.o shell.x
