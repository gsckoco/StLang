CC=/usr/bin/gcc
CFLAGS=-g -std=gnu99 -m64 -I. -ggdb
LDFLAGS=-g

objects := $(patsubst %.c,%.o,$(wildcard *.c))

stcompiler: $(objects)
	$(CC) -g -O0 -o stc $(objects) 