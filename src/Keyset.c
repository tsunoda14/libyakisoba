#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"
#include "Keyset.h"

#define DEFAULT_KEYS_FILE ".bcas_keys"
#if defined(SYSCONF_DIR)
#define DEFAULT_SYS_KEYS_FILE SYSCONF_DIR "/bcas_keys"
#else
#define DEFAULT_SYS_KEYS_FILE "/etc/bcas_keys"
#endif

typedef struct {
	u8 WorkKeyID;
	u8 Key[8];
} Key_t;

static const u8 GroupIDs[] = {
	0x02,	/* WOWOW */
	0x03,	/* STAR CHANNEL */
	0x17,	/* E2 110CS */
	0x1d,	/* (ex-) Safetynet */
	0x1e,	/* NHK */
	0x20,	/* Email */
};

#define NUM_GROUPS (sizeof(GroupIDs)/sizeof(GroupIDs[0]))
#define MAX_KEYS_PER_GROUP 10

static Key_t Keyset[NUM_GROUPS][MAX_KEYS_PER_GROUP];

static u8 CardID[8];
static u8 CardKey[8];

u8 *GetCardId(void)
{
	return CardID;
}

u8 *GetCardKey(void)
{
	return CardKey;
}

s32 Register(u8 BroadcasterGroupID, u8 WorkKeyID, const u8 *Key)
{
	int gidx, kidx;

	if (Key == NULL)
		return -1;

	for (gidx = 0; gidx < NUM_GROUPS; gidx ++)
		if (GroupIDs[gidx] == BroadcasterGroupID)
			break;
	if (gidx == NUM_GROUPS)
		return -1;

	kidx = WorkKeyID % MAX_KEYS_PER_GROUP;
	if (Keyset[gidx][kidx].WorkKeyID != 0xff &&
	    Keyset[gidx][kidx].WorkKeyID >= WorkKeyID)
		return -2;

	Keyset[gidx][kidx].WorkKeyID = WorkKeyID;
	memcpy(Keyset[gidx][kidx].Key, Key, 8);

	return 0;
}

s32 GetKey(u8 BroadcasterGroupID, u8 WorkKeyID, u8 *Key)
{
	int gidx, kidx;

	if (Key == NULL)
		return -1;

	for (gidx = 0; gidx < NUM_GROUPS; gidx ++)
		if (GroupIDs[gidx] == BroadcasterGroupID)
			break;
	if (gidx == NUM_GROUPS)
		return -2;

	kidx = WorkKeyID % MAX_KEYS_PER_GROUP;
	if (Keyset[gidx][kidx].WorkKeyID != WorkKeyID)
		return -3;

	memcpy(Key, Keyset[gidx][kidx].Key, 8);
	return 0;
}

static int load_keys(const char *fname)
{
	FILE *f;
	u8 key[8];
	uint8_t bgroup_id, wk_id;

	int ret;
	char c;
	u8 *k;
	int i, found;

	f = fopen(fname, "rb");
	if (!f)
		return 1;

	found = 0;
	while (1) {
		if (fscanf(f, " %c", &c) != 1)
			break;

		switch (c) {
		case '#':
		case ';':
			break;

		case 'K':
		case 'k':
			ret = fscanf(f, "ey [%hhx ] [%hhx ] %*1[:=]",
					&bgroup_id, &wk_id);
			if (ret == EOF || ret != 2)
				break;

			k = key;
			for (i = 0; i < 8; i++) {
				ret = fscanf(f, " %hhx%*1[,]", k++);
				if (ret == EOF || ret != 1)
					break;
			}
			if (i != 8)
				break;

			found++;
			Register(bgroup_id, wk_id, key);
			break;

		case 'C':
		case 'c':
			ret = fscanf(f, "ard%c%n", &c, &i);
			if (ret == EOF || i  < 4)
				break;
			if (c == 'I' || c == 'i') {
				i = 0;
				ret = fscanf(f, "%*1[dD] %*1[:=]%n", &i);
				if (ret == EOF || i < 2)
					break;
				k = CardID;
			} else if (c == 'K' || c == 'k') {
				i = 0;
				ret = fscanf(f, "ey %*1[:=]%n", &i);
				if (ret == EOF || i < 3)
					break;
				k = CardKey;
			} else
				break;

			for (i = 0; i < 8; i++) {
				ret = fscanf(f, " %hhx%*1[,]", k++);
				if (ret == EOF || ret != 1)
					break;
			}

			break;
		}

		fscanf(f, "%*[^\n]");
	}

	fclose(f);
	return !found;
}

/* (Re)read config file and set work keys */
static void __attribute__ ((constructor)) InitKeys(void)
{
	const char *val;
	char fname[256];
	size_t len;

	memset(Keyset, 0xff, sizeof(Keyset));

	val = getenv("BCAS_KEYS_FILE");
	if (val && !load_keys(val))
		return;

	val = getenv("HOME");
	if (val &&
	    snprintf(fname, sizeof(fname), "%s/%s", val, DEFAULT_KEYS_FILE) > 0 &&
	    !load_keys(fname))
		return;

	strncpy(fname, DEFAULT_SYS_KEYS_FILE, sizeof(fname));
	load_keys(fname);
	return;
} 
