#include "util_bits.h"
/*
 * @ 关于下面两个宏的正向和反向，
 * @ 正向，按位数域定向为正向，BITSTART 从右往左 数 __LENGTH 长      即按正常位节
 * @ 反向，按位数域定向反向为反向，BITSTART 从左往右数 __LENGTH 长 即按反向位节
 */

#define WRITEBITS_UINTx( __VALUE, __BITSTART, __LENGTH, __DATA, __TYPE) \
{                                                                       \
    __TYPE bytes;                                                       \
    __TYPE mask;                                                        \
                                                                        \
    bytes = __VALUE;                                                    \
    mask = ((1 << __LENGTH) - 1) << __BITSTART;                         \
    __DATA <<= __BITSTART; /* shift data into correct position*/                \
    __DATA &= mask; /* zero all non-important bits in data*/                    \
    bytes &= ~(mask); /* zero all important bits in existing byte*/         \
    bytes |= __DATA; /* combine data with existing byte */                  \
                                                                        \
    __VALUE = bytes;                                                    \
}
#define WRITEBITS_INVUINTx( __VALUE, __BITSTART, __LENGTH, __DATA, __TYPE)  \
{                                                                       \
    __TYPE bytes;                                                       \
    __TYPE mask;                                                        \
                                                                        \
    bytes = __VALUE;                                                    \
    mask = ((1 << __LENGTH) - 1) << (__BITSTART - __LENGTH + 1);            \
    __DATA <<= (__BITSTART - __LENGTH + 1); /* shift data into correct position*/ \
    __DATA &= mask; /* zero all non-important bits in data*/                    \
    bytes &= ~(mask); /* zero all important bits in existing byte*/         \
    bytes |= __DATA; /* combine data with existing byte */                  \
                                                                        \
    __VALUE = bytes;                                                    \
}
#define READBITS_UINTx( __VALUE, __BITSTART, __LENGTH, __TYPE)          \
{                                                                       \
    __TYPE bytes;                                                       \
    __TYPE mask;                                                        \
                                                                        \
    bytes = __VALUE;                                                    \
    mask = ((1 << __LENGTH) - 1);                                       \
    bytes >>= __BITSTART;                                               \
    bytes &= mask; /* zero all non-important bits in data*/                 \
                                                                        \
    return bytes;                                                       \
}
#define READBITS_INVUINTx( __VALUE, __BITSTART, __LENGTH, __TYPE)       \
{                                                                       \
    __TYPE bytes;                                                       \
    __TYPE mask;                                                        \
                                                                        \
    bytes = __VALUE;                                                    \
    mask = ((1 << __LENGTH) - 1);                                       \
    bytes >>= (__BITSTART - __LENGTH + 1);                              \
    bytes &= mask; /* zero all non-important bits in data*/                 \
                                                                        \
    return bytes;                                                       \
}


/**
  * @brief  将value内存的内容 bitstart位数起长度length的位数，改为数据data 
  * @param  value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note   方向: 正向       如果data数据超出，则会被截断
  * @retval none
  */
void writebits_U8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data)
{
    WRITEBITS_UINTx(*value, bitStart, length, data, uint8_t)
}
/**
  * @brief  
  * @param   value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note    方向: 正向
  * @retval 
  */
void writebits_U16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data)
{
    WRITEBITS_UINTx(*value, bitStart, length, data, uint16_t)
}
/**
  * @brief  
  * @param   value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note    方向: 正向
  * @retval 
  */
void writebits_U32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data)
{
    WRITEBITS_UINTx(*value, bitStart, length, data, uint32_t)
}

/**
  * @brief  将value内存的内容 bitstart位数起长度length的位数，改为数据data 
  * @param  value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note    方向: 反向       如果data数据超出，则会被截断
  * @retval none
  */
void writebits_invU8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data)
{
    WRITEBITS_INVUINTx(*value, bitStart, length, data, uint8_t)
}
/**
  * @brief  
  * @param   value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note    方向: 反向
  * @retval 
  */
void writebits_invU16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data)
{
    WRITEBITS_INVUINTx(*value, bitStart, length, data, uint16_t)
}

/**
  * @brief  
  * @param   value 要改的内容 bitStart 起始位 length 长度位 data 数据
  * @note    方向: 反向
  * @retval 
  */
void writebits_invU32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data)
{
    WRITEBITS_INVUINTx(*value, bitStart, length, data, uint32_t)
}

/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 正向
  * @retval  值
  */
uint8_t readbits_U8(uint8_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_UINTx( value, bitStart, length, uint8_t)
}
/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 正向
  * @retval  值
  */
uint16_t readbits_U16(uint16_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_UINTx( value, bitStart, length, uint16_t)
}

/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 正向
  * @retval  值
  */
uint32_t readbits_U32(uint32_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_UINTx( value, bitStart, length, uint32_t)
}
/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 反向
  * @retval  值
  */
uint8_t readbits_invU8(uint8_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_INVUINTx( value, bitStart, length, uint8_t)
}
/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 反向
  * @retval  值
  */
uint16_t readbits_invU16(uint16_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_INVUINTx( value, bitStart, length, uint16_t)
}

/**
  * @brief  
  * @param   value 要读的内容 bitStart 起始位 length 长度位
  * @note    方向: 反向
  * @retval  值
  */
uint32_t readbits_invU32(uint32_t value, uint8_t bitStart, uint8_t length)
{
    READBITS_INVUINTx( value, bitStart, length, uint32_t)
}

