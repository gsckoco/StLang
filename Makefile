CC=gcc
CFLAGS=-std=gnu99 -m64 -I.

stcompiler: St.o StLex.o StParser.o StVM.o
	$(CC) -o st St.o StLex.o StParser.o StVM.o