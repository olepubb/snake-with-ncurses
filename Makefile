CC=clang

prog: main.c snake.h
	$(CC) -lncurses main.c -o snake