CC=clang
BIN_NAME=minesweeper
VERSION=1

output: main.o
	$(CC) -o ./build/output/minesweeper ./build/main.o -lncurses

main.o:
	$(CC) -o ./build/main.o ./src/main.c -c
