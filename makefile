shell: parser.o shell.o
	gcc -o shell.x -std=c99 parser.o shell.o

parser.o:	parser.c parser.h
	gcc -c -std=c99 parser.c

shell.o: shell.c
	gcc -c -std=c99 shell.c

clean:
	rm *.o shell.x
