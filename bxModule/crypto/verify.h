

#ifndef __VERIFY_H_
#define __VERIFY_H_

#include <stdint.h>

uint16_t mCRC16( uint8_t * pucFrame, uint16_t usLen );
uint8_t mXOR(uint8_t *src , uint8_t len);


#endif
