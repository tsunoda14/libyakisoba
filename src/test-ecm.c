#include <stdio.h>
#include <string.h>
#include <yakisoba.h>

unsigned char sample[] = {
  0x44, 0x1e, 0x02, 0xb5, 0x99, 0xe3, 0xb9, 0xe6,
  0xe0, 0xa0, 0x13, 0x00, 0x16, 0x87, 0x7f, 0x4e,
  0x93, 0x5a, 0x3f, 0x49, 0xd4, 0xc0, 0x45, 0x64,
  0x2a, 0xe6, 0xcf, 0x1c, 0x01, 0x7b
};
unsigned int sample_len = 30;

int main(void)
{
	unsigned char keys[16];
	int ret;

	ret = bcas_decodeECM(sample, sample_len, keys, NULL);
	if (ret != 0) {
		fprintf(stderr, "failed to decode the sample ECM: %s\n",
			strerror(-ret));
		return ret;
	}

	if (strncmp(keys, "Oyasuminasai 2ch", 16)) {
		int i;

		fprintf(stderr, "wrong decoded text.\n");
		for (i = 0; i < 8; i++)
			printf("0x%02hhx ", keys[i]);
		printf("\n");
		for (; i < 16; i++)
			printf("0x%02hhx ", keys[i]);
		printf("\n");
		return 1;
	}

	return 0;
}
