

#ifndef TIMERS_H
#define TIMERS_H

#include "common_type.h"
#include "memalloc.h"
#include "list.h"
#include "queue.h"
#include "mclock.h"

#define configTIMER_QUEUE_LENGTH  10

#ifdef __cplusplus
extern "C" {
#endif

/* IDs for commands that can be sent/received on the timer queue.*/
#define tmrCOMMAND_START                        ( ( uint32_t ) 0 )
#define tmrCOMMAND_STOP                         ( ( uint32_t ) 1 )
#define tmrCOMMAND_DELETE                       ( ( uint32_t ) 2 )


//静态结构体,用于屏蔽用户对结构体的可见
typedef struct TimerStatic_s
{
    ListItemStatic_t    xDummy0;
    void                *pvDummy1[2];
} TimerStatic_t;

/* timer handle 时间句柄 */
typedef void * TimerHandle_t;
/* Defines the prototype to which timer callback functions must conform. 回调定义*/
typedef void (*TimerCallbackFunction_t)( void* arg );

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
/**
 * @brief   动态态分配一个时间句柄
 * @param   pxCallbackFunction - 回调函数 
 * @param   arg - 回调函数参数
 * @return  成功返回句柄,否则NULL
 */
TimerHandle_t timerNew( TimerCallbackFunction_t pxCallbackFunction, void * arg) ;
#endif
/**
 * @brief   静态分配一个时间句柄
 * @param   pxTimerBuffer - 静态缓冲区 
 * @param   pxCallbackFunction - 回调函数 
 * @param   arg - 回调函数参数
 * @return  成功返回句柄,否则NULL
 */
TimerHandle_t timerAssign(TimerStatic_t * pxTimerBuffer, TimerCallbackFunction_t pxCallbackFunction, void * arg) ; 
/**
 * @brief   查看时间句柄是否活跃在时间任务队列中
 * @param   xTimer - 时间句柄
 * @return  TRUE: active FALSE: not active
 */
uint8_t timerIsTimerActive( TimerHandle_t xTimer );
/**
 * @brief   获得下一次超时到期时间
 * @param   无
 * @return  超时时间值, 如果队列无等任务,将返回最大值0xffffffff
 */
uint32_t timerGetNextTimeout(void);
/**
 * @brief   任务函数,循环调用,或1ms调用, 需要实现内部API __GetCurTimeTick
 * @param   无
 * @return  无
 */
void timerTask( void );
/**
 * @brief   启动,重启,停止,删除
 * @param   xTimer - 句柄
 * @param   timeout - 超时时间
 * @return  无
 */
#define timerStart( xTimer, timeout)    timerGenericCommandSend( ( xTimer ), tmrCOMMAND_START, timeout)
#define timerRestart( xTimer, timeout ) timerStart( xTimer, timeout)
#define timerStop( xTimer )             timerGenericCommandSend( ( xTimer ), tmrCOMMAND_STOP, 0U )
#define timerDelete( xTimer )           timerGenericCommandSend( ( xTimer ), tmrCOMMAND_DELETE, 0U)

#define timerStartFromISR( xTimer, timeout)    timerStart( xTimer, timeout)
#define timerRestartFromISR( xTimer, timeout ) timerStart( xTimer, timeout)
#define timerStopFromISR( xTimer )             timerStop( xTimer )
#define timerDeleteFromISR( xTimer )           timerDelete( xTimer )

// internal function do not use it
uint8_t timerGenericCommandSend( TimerHandle_t xTimer, const uint32_t xCommandID, const uint32_t xTimeoutInTicks);

#ifdef __cplusplus
}
#endif
#endif /* TIMERS_H */



