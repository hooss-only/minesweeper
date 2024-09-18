CC=clang
BIN_NAME=minesweeper
VERSION=1

output: main.o
	$(CC) -o ./build/output/minesweeper main.o

main.o:
	$(CC) -o ./build/main.o ./src/main.c -c
