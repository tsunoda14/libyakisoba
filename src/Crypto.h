#ifndef BCAS_CRYPTO_H
#define BCAS_CRYPTO_H

void Transform(u8 Protocol, const u8 *Password, const u8 *Input, u32 Size, u8 *Output, int Decryption);
void GenerateMAC(u8 Protocol, const u8 *Password, const u8 *Payload, u32 Size, u8 *MAC);

#endif
