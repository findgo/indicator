/**
  ******************************************************************************
  * @file   bits.h
  * @author  
  * @version 
  * @date    
  * @brief       位操作，任意位任意数值
  ******************************************************************************
  * @attention  20161110     v1.1       jgb     已测试20161111
  ******************************************************************************
  */
#ifndef __BITS_H__
#define __BITS_H__

#include "stdint.h"

/*
 * @ 关于下面两个宏的正向和反向，
 * @ 正向，按位数域定向为正向，BITSTART 从右往左 数 __LENGTH 长      即按正常位节
 * @ 反向，按位数域定向反向为反向，BITSTART 从左往右数 __LENGTH 长 即按反向位节
 */

void writebits_U8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data);
void writebits_U16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data);
void writebits_U32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data);
void writebits_invU8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data);
void writebits_invU16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data);
void writebits_invU32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data);

uint8_t readbits_U8(uint8_t value, uint8_t bitStart, uint8_t length);
uint16_t readbits_U16(uint16_t value, uint8_t bitStart, uint8_t length);
uint32_t readbits_U32(uint32_t value, uint8_t bitStart, uint8_t length);
uint8_t readbits_invU8(uint8_t value, uint8_t bitStart, uint8_t length);
uint16_t readbits_invU16(uint16_t value, uint8_t bitStart, uint8_t length);
uint32_t readbits_invU32(uint32_t value, uint8_t bitStart, uint8_t length);


#endif

