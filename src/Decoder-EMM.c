#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "yakisoba.h"
#include "Global.h"
#include "Crypto.h"
#include "Keyset.h"

typedef struct {
	u8 CardID[6];
	u8 MessageLength[2];
	u8 ProtocolNumber;
	u8 BroadcasterGroupID;
	u8 MessageID;
	u8 MessageControl;
	// 0x0c
	u8 AlterationDetector[2];
	u8 ExpiryDate[2];
	u8 FixedMessageID[2];
	u8 ExtraMessageFormatVer;
	u8 ExtraMessageLength[2];
	u8 ExtraMessageCode[0];
} EMD_Header_t /* __attribute__ ((packed)) */;

typedef struct {
	u8 CardID[6];
	u8 Length;
	u8 ProtocolNumber;
	u8 BroadcasterGroupID;
	u8 UpdateNumber[2];
	u8 ExpiryDate[2];
	u8 Payload[0];
} EMM_Header_t /* __attribute__ ((packed)) */;


int32_t bcas_decodeEMM(const uint8_t *Payload, uint32_t Size, uint8_t *Out, int Individual)
{
	u8 Plaintext[256];
	u8 MAC[4];

	if (Payload == NULL || Size < 6 + sizeof(MAC_t) || Size > sizeof(Plaintext))
		return -EINVAL;

	if (memcmp(Payload, GetCardId(), 6) != 0)
		return -ENOMSG;

	const u8 *Body = NULL;
	const u8 *Verify = NULL;
	u8 *Output = NULL;
	u16 SizeVerify = 0;
	u16 Total = Size;
	u8 Protocol;

	if (Individual) {
		const EMD_Header_t *EMM = (const EMD_Header_t *)Payload;

		if (Size < sizeof(EMD_Header_t) + 4)
			return -EINVAL;
		Protocol = EMM->ProtocolNumber;
		if (Protocol == 0xff) { // not encrypted.
			if (Out)
				memmove(Out, EMM, Size);
			return 0;
		}
		Body = EMM->AlterationDetector;
		Verify = Plaintext + 12;
		Output = Plaintext + 12;
		memcpy(Plaintext, EMM, 12);
		Size -= 12;
		SizeVerify = Size - 4;
	} else {
		const EMM_Header_t *EMM = (const EMM_Header_t *)Payload;

		if (Size < sizeof(EMM_Header_t) + sizeof(MAC_t))
			return -EINVAL;
		Protocol = EMM->ProtocolNumber;
		Body = &EMM->BroadcasterGroupID;
		Output = Plaintext + 8;
		Verify = Plaintext;
		memcpy(Plaintext, EMM, 8);
		Size -= 8;
		SizeVerify = Size + 4;
	}

	Transform(Protocol, GetCardKey(), Body, Size, Output, TRUE);

	GenerateMAC(Protocol, GetCardKey(), Verify, SizeVerify, MAC);

	if (memcmp(MAC, Verify + SizeVerify, 4))
		return -EILSEQ;

	if (Out)
		memcpy(Out, Plaintext, Total);

	return 0;
}
