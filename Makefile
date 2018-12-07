CFLAGS=-Wall -Werror -Wcast-align -Wstrict-overflow -Wstrict-aliasing -Wextra -Wpedantic -Wshadow -O3 -march=native -falign-loops=4

all:	lzp

lzp:	lzp.o

lzp.o:	lzp.c

clean:
	rm -f lzp *.o
