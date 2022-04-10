#include <errno.h>
#include <string.h>
#include "yakisoba.h"
#include "Global.h"
#include "Crypto.h"
#include "Keyset.h"

typedef struct {
	// 0x00
	u8 ProtocolNumber;
	// 0x01
	u8 BroadcasterGroupID;
	// 0x02
	u8 WorkKeyID;
	// 0x03
	u8 ScramblingKeyOdd[8];
	// 0x0b
	u8 ScramblingKeyEven[8];
	// 0x13
	u8 ProgramType;
	// 0x14
	u8 Date[2];
	// 0x16
	u8 Time[3];
	// 0x19
	u8 RecordingControl;
} ECM_t /* __attribute__ ((packed)) */;


int32_t bcas_decodeECM(const uint8_t *Payload, uint32_t Size, uint8_t *Keys, uint8_t *VarPart)
{
	u8 Plaintext[256];
	u8 Key[8];
	u8 MAC[4];
	const ECM_t *ECM = (const ECM_t *)Payload;

	if (Payload == NULL || Size < sizeof(ECM_t) + sizeof(MAC_t) || Size > sizeof(Plaintext))
		return -EINVAL;

	Plaintext[0] = ECM->ProtocolNumber;
	Plaintext[1] = ECM->BroadcasterGroupID;
	Plaintext[2] = ECM->WorkKeyID;

	if (GetKey(ECM->BroadcasterGroupID, ECM->WorkKeyID, Key) < 0)
		return -ENOKEY;

	Transform(ECM->ProtocolNumber, Key, ECM->ScramblingKeyOdd, Size - 3,
			Plaintext + 3, TRUE);

	Size -= sizeof(MAC_t);
	GenerateMAC(ECM->ProtocolNumber, Key, Plaintext, Size, MAC);
	if (memcmp(MAC, Plaintext + Size, sizeof(MAC_t)))
		return -EILSEQ;

	ECM = (const ECM_t *)Plaintext;

	if (Keys != NULL)
		memcpy(Keys, ECM->ScramblingKeyOdd, 16);

	if (VarPart != NULL)
		memcpy(VarPart, ECM + 1, Size - sizeof(ECM_t));

	return 0;
}
