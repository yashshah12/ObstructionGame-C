all:
	gcc -Wall -std=c99 -pedantic src/a1.c -Iinclude -o bin/runMe -lncurses
