CC=clang
BIN_NAME=minesweeper
VERSION=1

output: main.o game.o
	$(CC) -o ./build/output/minesweeper ./build/main.o ./build/game.o -lncurses

main.o:
	$(CC) -o ./build/main.o ./src/main.c -c

game.o:
	$(CC) -o ./build/game.o ./src/game.c -c
