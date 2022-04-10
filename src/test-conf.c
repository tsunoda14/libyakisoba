#include <stdio.h>
#include "yakisoba.h"
#include "Global.h"
#include "Keyset.h"

void print_key(u8 *data)
{
	int i;

	for (i = 0; i < 8; i++)
		printf(" 0x%02hhx", *data++);
	printf("\n");
}
 
void main(void)
{
	int i, j;
	u8 key[8];

	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			if (!GetKey(i, j, key)) {
				printf("Key[%02x][%02x]:", i, j);
				print_key(key);
			}

	printf("CardID: ");
	print_key(GetCardId());

	printf("CardKey:");
	print_key(GetCardKey());
}
