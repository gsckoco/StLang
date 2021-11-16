CC=gcc
CFLAGS=-std=gnu99 -m64 -I.

objects := $(patsubst %.c,%.o,$(wildcard *.c))

stcompiler: $(objects)
	$(CC) -o st $(objects)