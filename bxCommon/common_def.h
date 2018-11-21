
/**
  ******************************************************************************
  * @file   commond_def.h
  * @author  
  * @version 
  * @date    
  * @brief    通用性定义,用于数据处理
  ******************************************************************************
  * @attention  20151110     v1.1       jgb     �ع�
  * @attention  20180906     v1.2       jgb     脱离平台限制
  ******************************************************************************
  */



#ifndef __COMMON_DEF_H_
#define __COMMON_DEF_H_

#include "common_type.h"
#include "port_atom.h"
#include "porting_def.h"
/* ------------------------------------------------------------------------------------------------
 *                                             Macros
 * ------------------------------------------------------------------------------------------------
 */
 //转换成字符串宏
#ifndef Mostr
#define Mostr(s) #s
#endif
//连接a和b宏
#ifndef Moconn
#define Moconn(a ,b)  (a##b)
#endif

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef BF
#define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#ifndef MIN
#define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
#define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif

#ifndef UBOUND
#define UBOUND(__ARRAY) (sizeof(__ARRAY)/sizeof(__ARRAY[0]))
#endif

//!< 
#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32_t)((uint32_t)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((uint32_t)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define BUILD_UINT8(loByte, hiByte) ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

/* takes a byte out of a uint32_t : var - uint32_t,  ByteNum - byte to take out (0 - 3) */
#define BREAK_UINT32( var, ByteNum ) (uint8_t)((uint32_t)(((var) >>((ByteNum) * 8)) & 0x00FF))
/* takes a byte out of a uint64_t : var - uint64_t,  ByteNum - byte to take out (0 - 7) */
#define BREAK_UINT64( var, ByteNum ) (uint8_t)((uint64_t)(((var) >>((ByteNum) * 8)) & 0x00FF))

//!< 
#define SWAP8_U16(v)                    ((((uint16_t)(v) & 0xFF00) >> 8) | \
                                        (((uint16_t)(v) & 0x00FF) << 8))
#define SWAP8_U24(v)                    ((((uint32_t)(v) & 0x00FF0000) >> 16) | \
                                        (((uint32_t)(v) & 0x0000FF00) << 0) | \
                                        (((uint32_t)(v) & 0x000000FF) << 16))
#define SWAP8_U32(v)                    ((((uint32_t)(v) & 0xFF000000) >> 24) | \
                                        (((uint32_t)(v) & 0x00FF0000) >> 8) | \
                                        (((uint32_t)(v) & 0x0000FF00) << 8) | \
                                        (((uint32_t)(v) & 0x000000FF) << 24))


//! \name binary compatibility  
#define USE_SET_BIT(__x, BitNum)    ((__x) |= (BV(BitNum)))
#define USE_CLR_BIT(__x, BitNum)    ((__x) &= ~(BV(BitNum)))
#define USE_CPL_BIT(__x ,BitNum)    ((__x) ^= (BV(BitNum)))
#define USE_GET_BIT(__x, BitNum)    (((__x) >> (BitNum)) & 0x01)

//��һ����ĸת��Ϊ��д
#define  UPCASE( c )    (((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
    
//�ж��ַ��ǲ���10��ֵ������
#define  DECCHK( c )    ((c) >= '0' && (c) <= '9')
//�ж��ַ��ǲ���16��ֵ������
#define  HEXCHK( c )    ( ((c) >= '0' && (c) <= '9') ||\
                        ((c) >= 'A' && (c) <= 'F') ||\
                        ((c) >= 'a' && (c) <= 'f') )

#define  MEM_B( x )  ( *( (uint8_t *) (x) ) )//�õ�ָ����ַ�ϵ�һ��byte
#define  MEM_W( x )  ( *( (uint16_t *) (x) ) )//�õ�ָ����ַ�ϵ�����byte

// the offset of the MEMBER in the structure
// 获得MEMBER在结构体的偏移量
#define offsetofStruct(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/* 通过结构体成员MEMBER的地址,得到TYPE的首地址
 * container_from - cast a member of a structure out to the containing structure 通过容器成员MEMBER的地址,得到容器的地址
 * @ptr:    the pointer to the member.  结构体成员MEMBER的地址
 * @TYPE:   the type of the container struct this is embedded in. 结构体类型
 * @MEMBER: the name of the member within the struct. 结构体成员的名称
 */
#define container_from(ptr, TYPE, MEMBER) ((TYPE *)( (char *)ptr - offsetofStruct( TYPE, MEMBER) ))
    
// the MEMBER of size in the structure
// 获得结构体中那个 MEMBER成员的占空间的大小
#define FSIZE( TYPE, MEMBER )  sizeof( ((TYPE *) 0)->MEMBER )





//调试信息配置

#define GLOBAL_DEBUG
#define configASSERT( x )




#endif




