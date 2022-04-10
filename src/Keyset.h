#ifndef BCAS_KEYSET_H
#define BCAS_KEYSET_H

u8 *GetCardId(void);
u8 *GetCardKey(void);

s32 GetKey(u8 BroadcasterGroupID, u8 WorkKeyID, u8 *Key);

#endif
