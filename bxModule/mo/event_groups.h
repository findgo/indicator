
#ifndef __EVENT_GROUPS_H__
#define __EVENT_GROUPS_H__

#include "common_type.h"
#include "common_def.h"
#include "memalloc.h"
#include "port_atom.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVG_BITS_TO_PROCESS_ALL  (0)

#define EVG_BITS_0  (( uint32_t )1 << 0) 
#define EVG_BITS_1  (( uint32_t )1 << 1) 
#define EVG_BITS_2  (( uint32_t )1 << 2) 
#define EVG_BITS_3  (( uint32_t )1 << 3) 
#define EVG_BITS_4  (( uint32_t )1 << 4) 
#define EVG_BITS_5  (( uint32_t )1 << 5) 
#define EVG_BITS_6  (( uint32_t )1 << 6) 
#define EVG_BITS_7  (( uint32_t )1 << 7) 
#define EVG_BITS_8  (( uint32_t )1 << 8) 
#define EVG_BITS_9  (( uint32_t )1 << 9) 
#define EVG_BITS_10  (( uint32_t )1 << 10) 
#define EVG_BITS_11  (( uint32_t )1 << 11) 
#define EVG_BITS_12  (( uint32_t )1 << 12) 
#define EVG_BITS_13  (( uint32_t )1 << 13) 
#define EVG_BITS_14  (( uint32_t )1 << 14) 
#define EVG_BITS_15  (( uint32_t )1 << 15) 
#define EVG_BITS_16  (( uint32_t )1 << 16) 
#define EVG_BITS_17  (( uint32_t )1 << 17) 
#define EVG_BITS_18  (( uint32_t )1 << 18) 
#define EVG_BITS_19  (( uint32_t )1 << 19) 
#define EVG_BITS_20  (( uint32_t )1 << 20) 
#define EVG_BITS_21  (( uint32_t )1 << 21) 
#define EVG_BITS_22  (( uint32_t )1 << 22) 
#define EVG_BITS_23  (( uint32_t )1 << 23) 
#define EVG_BITS_24  (( uint32_t )1 << 24) 
#define EVG_BITS_25  (( uint32_t )1 << 25) 
#define EVG_BITS_26  (( uint32_t )1 << 26) 
#define EVG_BITS_27  (( uint32_t )1 << 27) 
#define EVG_BITS_28  (( uint32_t )1 << 28) 
#define EVG_BITS_29  (( uint32_t )1 << 29) 
#define EVG_BITS_30  (( uint32_t )1 << 30) 
#define EVG_BITS_31  (( uint32_t )1 << 31) 


// 事件位掩码
typedef uint32_t EventBits_t;
// 句柄
typedef void * EventGroupHandle_t;
//静态结构体,用于屏蔽用户对结构体的可见
typedef struct EventGroupStatic_s
{
    uint32_t xDummy0;
} EventGroupStatic_t;


#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1)
/**
 * @brief   动态分配一个事件组句柄
 * @param   无
 * @return  句柄
 */
EventGroupHandle_t eventGroupNew( void );
#endif
/**
 * @brief   静态分配一个事件组句柄
 * @param   静态事件组缓冲区
 * @return  句柄
 */
EventGroupHandle_t eventGroupAssign( EventGroupStatic_t *pxEventGroupBuffer ) ;
/**
 * @brief   获取一个事件组触发的事件, 此函数会清除所有的事件,所以需要及时处理所有的事
 * @param   xEventGroup: 句柄
 * @param   uBitsToProcessFor: 想要处理的事件掩码, 处理全部用参数 EVG_BITS_TO_PROCESS_ALL;
 * @return  要处理的事件掩码
 */
EventBits_t eventGroupWaitBits( EventGroupHandle_t xEventGroup, const EventBits_t uBitsToProcessFor);
/**
 * @brief   设置,清除事件组的事件位
 * @param   xEventGroup - 句柄
 * @param   uxBitsToClear - 事件位掩码,可多位设置
 * @return  设置前的位掩码
 */
EventBits_t eventGroupClearBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear ) ;
EventBits_t eventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet ) ;
#define eventGroupClearBitsFromISR( xEventGroup, uxBitsToClear )    eventGroupClearBits( xEventGroup, uxBitsToClear )
#define eventGroupSetBitsFromISR(xEventGroup, uxBitsToSet )         eventGroupSetBits(xEventGroup, uxBitsToSet )
/**
 * @brief   设置,清除事件组的事件位
 * @param   xEventGroup - 句柄
 * @return  位掩码
 */
#define eventGroupGetBits( xEventGroup )        eventGroupClearBits( xEventGroup, 0 )
#define eventGroupGetBitsFromISR( xEventGroup ) eventGroupClearBitsFromISR( xEventGroup, 0 )


#ifdef __cplusplus
}
#endif

#endif /* EVENT_GROUPS_H */



