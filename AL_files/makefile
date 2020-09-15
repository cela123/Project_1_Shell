shell: parser.o shell.o path_search.o built_ins.o piping_and_redirection.o
	gcc -o shell -std=c99 parser.o shell.o path_search.o built_ins.o piping_and_redirection.o

debug: parser.o shell.o path_search.o
	gcc -o shell -std=c99 parser.o shell.o path_search.o -g

path_search.o: path_search.c path_search.h
	gcc -c -std=c99 path_search.c

built_ins.o: built_ins.c built_ins.h
	gcc -c -std=c99 built_ins.c

parser.o: parser.c parser.h
	gcc -c -std=c99 parser.c

piping_and_redirection.o: piping_and_redirection.c piping_and_redirection.h
	gcc -c -std=c99 piping_and_redirection.c

shell.o: shell.c
	gcc -c -std=c99 shell.c

clean:
	rm *.o shell
