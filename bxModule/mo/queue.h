
/**
  ******************************************************************************
  * @file 
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention 	20180920     v1.1   	jgb		
  * @attention 	20181213     v1.2   	jgb		 提供释放队列句柄
  ******************************************************************************
  */

/* 队列,操作和接收都会引起数据拷贝,*/


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "common_type.h"
#include "common_def.h"
#include "memalloc.h"

#ifdef __cplusplus
extern "C" {
#endif

// 句柄
typedef void * QueueHandle_t;


//静态结构体,用于屏蔽用户对结构体的可见
typedef struct QueueStatic_s
{
    void *pvDummy0[ 4 ];
    uint32_t uxDummy1[ 3 ];
} QueueStatic_t;


#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
/**
 * @brief   动态分配一个队列
 * @param   uxQueueItemCap - 队列装条目的能力
 * @param   uxItemSize - 条目大小
 * @return  句柄
 */
QueueHandle_t queueNew( const uint32_t uxQueueItemCap, const uint32_t uxItemSize);
/**
 * @brief   释放动态分配队列句柄
 * @param   xQueue - 句柄
 * @return  句柄
 */
void queueFree( const QueueHandle_t xQueue);
#endif
/**
 * @brief   静态分配一个队列
 * @param   pxStaticQueue - 静态队列缓冲区
 * @param   uxQueueItemCap - 队列装条目的能力
 * @param   uxItemSize - 条目大小
 * @return  句柄
 */
QueueHandle_t queueAssign( QueueStatic_t *const pxStaticQueue , const uint32_t uxQueueItemCap, const uint32_t uxItemSize, uint8_t *const pucQueueStorage );
/**
 * @brief   清空队列
 * @param   xQueue - 句柄
 * @return  句柄
 */
uint8_t queueReset(const QueueHandle_t xQueue);
/**
 * @brief   向队列尾(头)放入一个条目数据, 
 * @param   xQueue - 句柄
 * @param   pvItemToQueue - 条目指针
 * @return  TRUE: SUCCESS, FALSE: other
 */
#define queuePut( xQueue, pvItemToQueue ) xQueueGenericPut( xQueue, pvItemToQueue, FALSE )
#define queuePutBack( xQueue, pvItemToQueue ) xQueueGenericPut( xQueue, pvItemToQueue, FALSE )
#define queuePutFront( xQueue, pvItemToQueue ) xQueueGenericPut( xQueue, pvItemToQueue, TRUE )
/**
 * @brief   从队列头弹出一个条目数据 - 查看队列头的一个条目数据,
 * @param   xQueue - 句柄
 * @param   pvItemToQueue - 条目指针
 * @return  TRUE: SUCCESS, FALSE: other
 */
#define queuePop( xQueue, pvBuffer) xQueueGenericPop( xQueue, pvBuffer, FALSE )
#define queuePeek( xQueue, pvBuffer) xQueueGenericPop( xQueue, pvBuffer, TRUE )

/**
 * @brief   队列有效条目数
 * @param   xQueue - 句柄
 * @return  count
 */
uint32_t queueItemAvailableValid(const QueueHandle_t xQueue );
/**
 * @brief   队列空闲条目空间数
 * @param   xQueue - 句柄
 * @return  count
 */
uint32_t queueItemAvailableIdle( const QueueHandle_t xQueue );
/**
 * @brief   队列空
 * @param   xQueue - 句柄
 * @return  TRUE: empty , FALSE : not empty
 */
uint8_t queueIsEmpty( const QueueHandle_t xQueue );
/**
 * @brief   队列满
 * @param   xQueue - 句柄
 * @return  TRUE: full , FALSE : not full
 */
uint8_t queueIsFull( const QueueHandle_t xQueue );

/****************以下两个函数没有到非常了解,不得使用**************************************************************************************/
//此API并没有引起拷贝数据,它只是先占好了放数据的位置,并返回指占位的指针,均是实际位置
/* 使用方法,调用queueOnAlloc,然后直接转换,直接写数据,注意不得超过 uxItemSize 大小*/
#define queueOnAlloc( xQueue ) xQueueOnAlloc( xQueue, FALSE )
#define queueOnAllocBack( xQueue ) xQueueOnAlloc( xQueue, FALSE )
#define queueOnAllocFront( xQueue ) xQueueOnAlloc( xQueue, TRUE )
//此API并没有读回数据,而只是获取当前要出队的数据指针,这些操作均在实际数据上操作
/* 使用方法,调用queueOnPeek,然后直接转换,直接读数据,注意不要超过uxItemSize大小,使用完后,的确需要释放,调用queuePop并指定第二参数为NULL */
void *queueOnPeek( QueueHandle_t xQueue );
/******************************************************************************************************************************************/

// internal used
uint8_t xQueueGenericPut(const QueueHandle_t xQueue, const void * const pvItemToQueue , const uint8_t isFront );
uint8_t xQueueGenericPop(const QueueHandle_t xQueue, void * const pvBuffer, const uint8_t isJustPeeking );
void *xQueueOnAlloc(const QueueHandle_t xQueue , const uint8_t isFront );

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */

