shell: parser.o shell.o path_search.o built_ins.o
	gcc -o shell -std=c99 parser.o shell.o path_search.o built_ins.o

path_search.o: path_search.c path_search.h
	gcc -c -std=c99 path_search.c

built_ins.o: built_ins.c built_ins.h
	gcc -c -std=c99 built_ins.c

parser.o: parser.c parser.h
	gcc -c -std=c99 parser.c

shell.o: shell.c
	gcc -c -std=c99 shell.c

clean:
	rm *.o shell.x

