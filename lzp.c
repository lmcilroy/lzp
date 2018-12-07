#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define putchar		putchar_unlocked
#define getchar		getchar_unlocked
#define fwrite		fwrite_unlocked
#define fread		fread_unlocked

#define HASH_ORDER	16
#define HASH_SIZE	(1 << HASH_ORDER)
#define HASH(h, x)	(h = (h << 4) ^ x)

unsigned char table[HASH_SIZE];

void
encode(void)
{
	unsigned char buf[9];
	unsigned short hash;
	int mask, i, j, c;

	buf[0] = 'L';
	buf[1] = 'Z';
	buf[2] = 'P';
	buf[3] = '1';
	i = fwrite(buf, 1, 4, stdout);
	if (i != 4) {
		fprintf(stderr, "fwrite failed\n");
		exit(1);
	}

	hash = 0;
	for (;;) {
		j = 1;
		mask = 0;
		for (i = 0; i < 8; i++) {
			c = getchar();
			if (c == EOF)
				break;
			if (c == table[hash]) {
				mask |= 1 << i;
			} else {
				table[hash] = c;
				buf[j++] = c;
			}
			HASH(hash, c);
		}
		if (i > 0) {
			buf[0] = mask;
			i = fwrite(buf, 1, j, stdout);
			if (i != j) {
				fprintf(stderr, "fwrite failed\n");
				exit(1);
			}
		}
		if (c == EOF)
			break;
	}
}

void
decode(void)
{
	unsigned char buf[8];
	unsigned short hash;
	int mask, i, j, c;

	i = fread(buf, 1, 4, stdin);
	if (i != 4) {
		fprintf(stderr, "fread failed\n");
		exit(1);
	}

	if (buf[0] != 'L' || buf[1] != 'Z' ||
	    buf[2] != 'P' || buf[3] != '1') {
		fprintf(stderr, "not compressed stream\n");
		exit(1);
	}

	hash = 0;
	for (;;) {
		j = 0;
		mask = getchar();
		if (mask == EOF)
			return;
		for (i = 0; i < 8; i++) {
			if ((mask & (1 << i)) != 0) {
				c = table[hash];
			} else {
				c = getchar();
				if (c == EOF)
					break;
				table[hash] = c;
			}
			buf[j++] = c;
			HASH(hash, c);
		}
		if (j > 0) {
			i = fwrite(buf, 1, j, stdout);
			if (i != j) {
				fprintf(stderr, "fwrite failed\n");
				exit(1);
			}
		}
	}
}

void
usage(void)
{
	fprintf(stderr, "lzp <-c|-d>\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	if (strcmp(argv[1], "-c") == 0)
		encode();
	else if (strcmp(argv[1], "-d") == 0)
		decode();
	else
		usage();
}
