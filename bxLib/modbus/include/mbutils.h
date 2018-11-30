

#ifndef __MB_UTILS_H
#define __MB_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "mbproto.h"
#include "mbconfig.h"
#include "mbcpu.h"

#include "mb.h"

void MbSetBits( uint8_t *ucByteBuf, uint16_t usBitOffset, uint8_t ucNBits, uint8_t ucValue );
uint8_t MbGetBits( uint8_t * ucByteBuf, uint16_t usBitOffset, uint8_t ucNBits );

uint16_t MbCRC16(uint8_t *pucFrame, uint16_t usLen);

uint8_t MbChar2Bin(uint8_t ucCharacter);
uint8_t MbBin2Char(uint8_t ucByte);
uint8_t MbLRC(uint8_t *pucFrame, uint16_t usLen);

MbException_t MbError2Exception(MbErrorCode_t eErrorCode);
const char *MbError2Str(MbException_t excode);


#ifdef __cplusplus
}
#endif

#endif

