
/* Standard includes. */
#include <stdlib.h>
#include "timers.h"

//使能匿名结构体
#pragma anon_unions

/* The definition of the timers themselves. */
typedef struct tmrTimer_s
{
    ListItem_t              xTimerListItem;     /*<< Standard linked list item as used by all kernel features for event management. */
    void                    *arg;             /*<< An arg for the callback is used. */
    TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
} tmrTimer_t;

typedef struct tmrTimerParameter_s
{
    uint32_t            xTimeoutInTicks;        /*<< An value record time out value */
    uint32_t            xMarkTimeInTicks;       /*<<  mark time when command send*/
    tmrTimer_t          *pxTimer;            /*<< The timer to which the command will be applied. */
} tmrTimerParameter_t;

/* The structure that contains the two message types, along with an identifier
that is used to determine which message type is valid. */
typedef struct tmrTimerQueueMessage
{
    uint32_t          xMessageID;         /*<< The command being sent to the timer service task. */
    tmrTimerParameter_t;
} tmrTimerQueueMessage_t;


/*local funcition */
static void __InitialiseNewTimer( tmrTimer_t *const pxNewTimer , TimerCallbackFunction_t pxCallbackFunction, void *arg );
static void __CheckForValidListAndQueue( void ) ;
static uint8_t __InsertTimerInActiveList( tmrTimer_t * const pxTimer, const uint32_t xTimeNow , 
                                        const uint32_t xTimeoutInTicks, const uint32_t xMarkTimeInTicks);
static uint8_t __GenericCommandReceived(tmrTimerQueueMessage_t *const message);
static void __ProcessReceivedCommands(const uint32_t xTimeNow);

// local variable
/* The list in which active timers are stored.  Timers are referenced in expire
time order, with the nearest expiry time at the front of the list.  Only the
timer service task is allowed to access these lists. */
static List_t xActiveTimerList1;
static List_t xActiveTimerList2;
static List_t *pxCurrentTimerList;
static List_t *pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
static bool bTimerInitialised = FALSE;
static queue_t xTimerQueue;
static uint8_t ucTimerQueueStorageStatic[ configTIMER_QUEUE_LENGTH * sizeof( tmrTimerQueueMessage_t ) ];

static volatile uint32_t xLastTime = ( uint32_t ) 0U; 

/*
 * Called after a tmrTimer_t structure has been allocated either statically or dynamically to fill in the structure's members.
 */
static void __InitialiseNewTimer( tmrTimer_t *const pxNewTimer , TimerCallbackFunction_t pxCallbackFunction, void *arg ) 
{
    if( pxNewTimer ){
        /* Ensure the infrastructure used by the timer service task has been created/initialised. */
        __CheckForValidListAndQueue();

        /* Initialise the timer structure members using the function parameters. */
        pxNewTimer->arg = arg;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        listInitialiseItem( &( pxNewTimer->xTimerListItem ) );
    }
}
/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
static void __CheckForValidListAndQueue( void )
{
    /* Check that the list from which active timers are referenced, and the
    queue used to communicate with the timer service, have been initialised. */
    if( bTimerInitialised == FALSE ){
        listInitialise( &xActiveTimerList1 );
        listInitialise( &xActiveTimerList2 );
        pxCurrentTimerList = &xActiveTimerList1;
        pxOverflowTimerList = &xActiveTimerList2;

        // 创建一个消息队列用于发送消息
        queueAssign(&xTimerQueue, configTIMER_QUEUE_LENGTH, sizeof(tmrTimerQueueMessage_t), &( ucTimerQueueStorageStatic[ 0 ]));
        bTimerInitialised = TRUE;
    }
}
/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
static uint8_t __InsertTimerInActiveList( tmrTimer_t * const pxTimer, const uint32_t xTimeNow , 
                                                        const uint32_t xMarkTimeInTicks, const uint32_t xTimeoutInTicks)
{
    uint8_t xProcessTimerNow = FALSE;
    uint32_t xNextExpiryTime = xMarkTimeInTicks + xTimeoutInTicks;
    
    listSET_LIST_ITEM_VALUE( &( pxTimer->xTimerListItem ), xNextExpiryTime);
    listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

    if( xNextExpiryTime <= xTimeNow ){
        /* Has the expiry time elapsed between the command to start/restart a timer was issued, 
            and the time the command was processed? */
        if( ( ( uint32_t ) ( xTimeNow - xMarkTimeInTicks ) ) >= xTimeoutInTicks ) {
            /* (1  mark    next            now
             *      |       |               |    
             *   --------------------------------------------->
             *     next =< now ,( uint32_t )(now - mark) > timeout,  it is time out,   
             */

            /*(2            next           now          mark    
             *               |              |            |      
             *   --------------------------------------------->
             *     next =< now ,( uint32_t ) (now - mark) > timeout,  it is time out,  
             */
            xProcessTimerNow = TRUE;
        }
        else{       
            /*(3            next    mark   now
             *               |       |      |    
             *   --------------------------------------------->
             *     next =< now ,( uint32_t ) (now - mark) < timeout,  it is over flow,   
             */
            listInsert( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
        }
    }
    else {
        if( ( xTimeNow < xMarkTimeInTicks ) && ( xNextExpiryTime >= xMarkTimeInTicks ) ){            
            /*(1            now     mark   next
             *               |       |      |    
             *   --------------------------------------------->
             *     next > now , now < mark, next >  mark ,  it is time out,   
             */
            /* If, since the command was issued, the tick count has overflowed
            but the expiry time has not, then the timer must have already passed
            its expiry time and should be processed immediately. */
            xProcessTimerNow = TRUE;
        }
        else {
            /*(2    mark     now            next
             *       |        |              |    
             *   --------------------------------------------->
             *     next > now , now > mark, next >  mark ,  it is not time out 
             */

            /*(3             now            next    mark
             *                |              |       |   
             *   --------------------------------------------->
             *     next > now , now < mark, next <  mark ,  it is not time out 
             */
            listInsert( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
        }
    }

    return xProcessTimerNow;
}

static uint8_t __GenericCommandReceived( tmrTimerQueueMessage_t *const message)
{
    uint8_t bret;
    
    taskENTER_CRITICAL();
    bret = queuePop( &xTimerQueue, message );
    taskEXIT_CRITICAL();

    return bret;
}

/*
 *
 */
static void __ProcessReceivedCommands(const uint32_t xTimeNow)
{
    tmrTimerQueueMessage_t xMessage;
    tmrTimer_t *pxTimer;

    while( __GenericCommandReceived(&xMessage) == TRUE ) 
    {
        /* Commands that are positive are timer commands rather than pended function calls. */            
        /* The messages uses the xTimerParameters member to work on a software timer. */
        pxTimer = xMessage.pxTimer;

        // before do anything ,make sure timer is not in the list
        if( listIS_CONTAINED_WITHIN( NULL, &( pxTimer->xTimerListItem ) ) == FALSE ){
            /* The timer is in a list, remove it. */
            ( void ) listRemove( &( pxTimer->xTimerListItem ) );
        }
        else { /* The timer is not in a list , do nothing*/ }

        if( xMessage.xMessageID == tmrCOMMAND_START ) {
            /* Start or restart a timer.*/
            if( __InsertTimerInActiveList( pxTimer, xTimeNow, xMessage.xMarkTimeInTicks, xMessage.xTimeoutInTicks) != FALSE ) {
                /* The timer expired before it was added to the active timer list.  Process it now. */
                /* 主要是命令执行的延迟,所以此时已超时,直接执行回调 */
                pxTimer->pxCallbackFunction( pxTimer->arg);
            }
        }
        else if( xMessage.xMessageID == tmrCOMMAND_STOP || xMessage.xMessageID == tmrCOMMAND_DELETE ){
            // 开头已经从链表删除过了,不做任何事
            /* The timer has already been removed from the active list */
        }            
        else { /* Don't expect to get here. */ }
    }
}

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
timer_t * timerNew( TimerCallbackFunction_t pxCallbackFunction, void *arg) 
{
    tmrTimer_t *pxNewTimer;

    pxNewTimer = ( tmrTimer_t * ) mo_malloc( sizeof( tmrTimer_t ) );

    if( pxNewTimer ){
        __InitialiseNewTimer(pxNewTimer, pxCallbackFunction, arg );
    }

    return ( timer_t * )pxNewTimer;
}

void timerFree( timer_t *const xTimer) 
{
    mo_free(xTimer);
}

#endif
// ok
void timerAssign(timer_t *const xTimer, TimerCallbackFunction_t pxCallbackFunction, void *arg)
{
    tmrTimer_t *pxNewTimer = ( tmrTimer_t * ) xTimer; 
    
    if( pxNewTimer ){
        __InitialiseNewTimer(pxNewTimer, pxCallbackFunction, arg );
    }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
// ok
uint8_t timerIsTimerActive( timer_t *const xTimer )
{
    tmrTimer_t *const pxTimer = ( tmrTimer_t * ) xTimer;

    /* Is the timer in the list of active timers? */
    /* Checking to see if it is in the NULL list in effect checks to see if
    it is referenced from either the current or the overflow timer lists in
    one go, but the logic has to be reversed, hence the '!'. */
    return (( uint8_t ) !( listIS_CONTAINED_WITHIN( NULL, &( pxTimer->xTimerListItem ) ) ));
} 
/*-----------------------------------------------------------*/
// ok
uint8_t timerGenericCommandSend( timer_t *const xTimer, const uint32_t xCommandID, const uint32_t xTimeoutInTicks)
{
    uint8_t breturn;
    tmrTimerQueueMessage_t xMessage;
    isrSaveCriticial_status_Variable;
    
    /* Send a message to the timer service task to perform a particular action on a particular timer definition. */
    if( bTimerInitialised == TRUE ){
        /* Send a command to the timer service task to start the xTimer timer. */
        xMessage.xMessageID = xCommandID;
        xMessage.xTimeoutInTicks = xTimeoutInTicks;
        xMessage.pxTimer = ( tmrTimer_t * ) xTimer;

        isrENTER_CRITICAL();
        // get when send this command for mark
        xMessage.xMarkTimeInTicks = __GetCurTimeTick();;
        breturn = queuePutBack( &xTimerQueue, &xMessage);
        isrEXIT_CRITICAL();
    }
    
    return breturn;
}
/*-----------------------------------------------------------*/

void timerTask( void )
{
    
    uint32_t xTimeNow;
    uint8_t xTimerListsNeedSwitch = FALSE;
    tmrTimer_t *pxTimer;
    List_t *pxTemp;

    __CheckForValidListAndQueue();

    xTimeNow = __GetCurTimeTick();
    if( xTimeNow < xLastTime ){ // 发生回绕,时间溢出
        xTimerListsNeedSwitch = TRUE;
    }
    //save the last tick time value
    xLastTime = xTimeNow;    
    /* before do anything must be Empty the command queue. */
    /* 会增加插入队列的时间,但是为了保证及时执行命令,防止误回调执行, 只能抛弃一点效率 */
    __ProcessReceivedCommands(xTimeNow);
    
    if( xTimerListsNeedSwitch ) {// 发生回绕,时间溢出 需要切换列表 并处理所有当前时间列表的回调
        
         /* The tick count has overflowed.  The timer lists must be switched.
         If there are any timers still referenced from the current timer list
         then they must have expired and should be processed before the lists
         are switched. */
         while( listLIST_IS_EMPTY( pxCurrentTimerList ) == FALSE )
         {
             /* Remove the timer from the list. */
             pxTimer = ( tmrTimer_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxCurrentTimerList );
             ( void ) listRemove( &( pxTimer->xTimerListItem ) );
             
             pxTimer->pxCallbackFunction( pxTimer->arg );
         }
         
         pxTemp = pxCurrentTimerList;
         pxCurrentTimerList = pxOverflowTimerList;
         pxOverflowTimerList = pxTemp;
    }

    //wheather over flow or not must be check current time list has timer expired to process?
    while( ( !listLIST_IS_EMPTY( pxCurrentTimerList ) ) && ( listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxCurrentTimerList ) <= xTimeNow ) )
    {
        pxTimer = ( tmrTimer_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  
           A check has already been performed to ensure the list is not empty. */
        ( void ) listRemove( &( pxTimer->xTimerListItem ) );

        /* Call the timer callback. */
        pxTimer->pxCallbackFunction( pxTimer->arg );
    }
}


// 获取下一次超时时间间隔
// 待测试
uint32_t timerGetNextTimeout(void)
{
    uint32_t xTimeNow;
    uint32_t xNextExpiryTime;


    if(!listLIST_IS_EMPTY( pxCurrentTimerList )){
        xNextExpiryTime = listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxCurrentTimerList );
    }
    else if(!listLIST_IS_EMPTY( pxOverflowTimerList )){
        xNextExpiryTime = listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxOverflowTimerList );
    }
    else{
        // 没有任何时间任务在队列上, 返回最大值
        return 0xffffffffu;
    }
    
    xTimeNow = __GetCurTimeTick();
    if( xNextExpiryTime <= xTimeNow ){
        /* Has the expiry time elapsed between the command to start/restart a timer was issued, 
            and the time the command was processed? */
        if( ( ( uint32_t ) ( xTimeNow - xLastTime ) ) >= (( uint32_t )(xNextExpiryTime - xLastTime))){
            /* (1  last    next            now
             *      |       |               |    
             *   --------------------------------------------->
             *     next =< now ,( uint32_t )(now - last) > ( uint32_t )(next - last),  it is time out,   
             */

            /*(2            next           now          last    
             *               |              |            |      
             *   --------------------------------------------->
             *     next =< now ,( uint32_t ) (now - last) > ( uint32_t )(next - last),  it is time out,  
             */
            return 0;
        }
        else{       
            /*(3            next    last   now
             *               |       |      |    
             *   --------------------------------------------->
             *     next =< now ,( uint32_t ) (now - last) < ( uint32_t )(next - last),  it is not time out,   
             */
        }
    }
    else {
        if( ( xTimeNow < xLastTime ) && ( xNextExpiryTime >= xLastTime ) ){            
            /*(1            now     last   next
             *               |       |      |    
             *   --------------------------------------------->
             *     next > now , now < last, next >  last ,  it is time out,   
             */
            return 0;
        }
        else {
            /*(2    last     now            next
             *       |        |              |    
             *   --------------------------------------------->
             *     next > now , now > last, next >  last ,  it is not time out 
             */

            /*(3             now            next    last
             *                |              |       |   
             *   --------------------------------------------->
             *     next > now , now < last, next <  last ,  it is not time out 
             */
        }
    }
    
    return (xNextExpiryTime - xTimeNow) & 0xffffffffu;
}

