#ifndef BCAS_YAKISOBA_H
#define BCAS_YAKISOBA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief Decrypt an ECM payload
 * 
 * @param[in]  Payload  Pointer to the head of the (encrypted) ECM payload.
 * @param[in]  Size     Length of the payload, including MAC field.
 * @param[out] Keys     Decrypted scramble keys (odd, even), must be >= u8[2][8].
 * @param[out] VarPart  Decrypted variable part in the ECM body if they exit.
 *                      NULL or ( @a Payload + 26 ) is allowed.
 *                      if !NULL, must be >= (@a Size - 30).
 * @return              0 on success, < 0 on error.
 * @retval -EINVAL      @a Paylod is NULL, or @a Size too small
 * @retval -ENOKEY      Missing the work key for BroadcasterGroupD and WorkKeyID
 * @retval -EILSEQ      MAC mismatch
 */
int bcas_decodeECM(const uint8_t *Payload, uint32_t Size, uint8_t *Keys, uint8_t *VarPart);


/**
 * \brief Decrypt an EMM payload (in-place)
 * 
 * @param[in]  Payload  Pointer to the head of the EMM payload.
 * @param[in]  Size     Length of the payload.
 * @param[out] Out      Decrypted payload, or NULL.
 *                      @a Payload is allowed. if !NULL, must be >= @a Size.
 * @param[in] Indiviaul flag to indicate if @a Payload is an EMM individual message
 * @return              0 on success, < 0 on error.
 * @retval -EINVAL      @a Paylod is NULL, or @a Size too small
 * @retval -EILSEQ      MAC mismatch
 */
int bcas_decodeEMM(const uint8_t *Payload, uint32_t Size, uint8_t *Out, int Individual);

#ifdef __cplusplus
}
#endif

#endif
