
#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct Queue_s
{
    int8_t *pcHead;                 /*< Points to the beginning of the queue storage area. */
    int8_t *pcTail;                 /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t *pcWriteTo;              /*< Points to the free next place in the storage area. */
    int8_t *pcReadFrom;             /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
    
    volatile uint32_t uItemCurCnt;   /*< The number of items currently in the queue. */
    uint32_t uItemCap;                  /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    uint32_t uxItemSize;                /*< The size of each items that the queue will hold. */
} QueueInner_t;

// local function 
static void __CopyDataToQueue( QueueInner_t * const pxQueue, const void *pvItemToQueue, const uint8_t isFront );
static void __CopyDataFromQueue( QueueInner_t * const pxQueue, void *const pvBuffer );
static void __InitialiseNewQueue(QueueInner_t * const pxNewQueue, const uint32_t uxQueueItemCap, const uint32_t uxItemSize, uint8_t *pucQueueStorage);

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
queue_t * queueNew( const uint32_t uxQueueItemCap, const uint32_t uxItemSize)
{
    QueueInner_t * pxNewQueue;
    size_t xQueueSizeInBytes;
    uint8_t *pucQueueStorage;

    configASSERT( uxQueueItemCap > ( uint32_t ) 0 );
    configASSERT( uxItemSize > ( uint32_t ) 0 );

    /* Allocate enough space to hold the maximum number of items that can be in the queue at any time. */
    xQueueSizeInBytes = ( size_t ) ( uxQueueItemCap * uxItemSize );

    pxNewQueue = ( QueueInner_t * ) mo_malloc( sizeof( QueueInner_t ) + xQueueSizeInBytes );

    if( pxNewQueue ){
        /* Jump past the queue structure to find the location of the queue storage area. */
        pucQueueStorage = ( ( uint8_t * ) pxNewQueue ) + sizeof( QueueInner_t );

        __InitialiseNewQueue( pxNewQueue, uxQueueItemCap, uxItemSize, pucQueueStorage);
    }

    return ( queue_t * )pxNewQueue;
}

void queueFree(  queue_t *const xQueue)
{
    mo_free(xQueue);
}

#endif
void queueAssign(queue_t *const pxStaticQueue , const uint32_t uxQueueItemCap, const uint32_t uxItemSize,  uint8_t *const pucQueueStorage )
{
    QueueInner_t *pxNewQueue;

    configASSERT( uxQueueItemCap > ( uint32_t ) 0 );
    configASSERT( uxItemSize > ( uint32_t ) 0 );

    /* The StaticQueue_t structure and the queue storage area must be supplied. */
    configASSERT( pxStaticQueue != NULL );

    /* A queue storage area should be provided */
    configASSERT( pucQueueStorage != NULL );

    /* The address of a statically allocated queue was passed in, use it.
    The address of a statically allocated storage area was also passed in but is already set. */
    pxNewQueue = ( QueueInner_t * ) pxStaticQueue;
    
    if( pxNewQueue ) {
        __InitialiseNewQueue( pxNewQueue, uxQueueItemCap, uxItemSize, pucQueueStorage );
    }
}
uint8_t queueReset( queue_t *const xQueue)
{
    QueueInner_t *const pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );
    
    pxQueue->uItemCurCnt = ( uint32_t ) 0U;
    pxQueue->pcTail = pxQueue->pcHead + ( pxQueue->uItemCap * pxQueue->uxItemSize );
    pxQueue->pcWriteTo = pxQueue->pcHead;
    pxQueue->pcReadFrom = pxQueue->pcHead + ( ( pxQueue->uItemCap - ( uint32_t ) 1U ) * pxQueue->uxItemSize );

    /* A value is returned for calling semantic consistency with previous versions. */
    return TRUE;
}
static void __CopyDataToQueue( QueueInner_t * const pxQueue, const void *pvItemToQueue, const uint8_t isFront )
{
    if( isFront == FALSE ){
        ( void ) memcpy( ( void * ) pxQueue->pcWriteTo, pvItemToQueue, ( size_t ) pxQueue->uxItemSize ); 
        
        pxQueue->pcWriteTo += pxQueue->uxItemSize;
        if( pxQueue->pcWriteTo >= pxQueue->pcTail )  {
            pxQueue->pcWriteTo = pxQueue->pcHead;
        }
    }
    else{
        if(pvItemToQueue){
            ( void ) memcpy( ( void * ) pxQueue->pcReadFrom, pvItemToQueue, ( size_t ) pxQueue->uxItemSize ); 
        }
        
        pxQueue->pcReadFrom -= pxQueue->uxItemSize;
        if( pxQueue->pcReadFrom < pxQueue->pcHead ) {
            pxQueue->pcReadFrom = ( pxQueue->pcTail - pxQueue->uxItemSize );
        }
    }

    pxQueue->uItemCurCnt++;
}
static void __CopyDataFromQueue( QueueInner_t * const pxQueue, void * const pvBuffer )
{
    pxQueue->pcReadFrom += pxQueue->uxItemSize;
    
    if( pxQueue->pcReadFrom >= pxQueue->pcTail ) {
        pxQueue->pcReadFrom = pxQueue->pcHead;
    }
    if(pvBuffer)
        ( void ) memcpy( ( void * ) pvBuffer, ( void * ) pxQueue->pcReadFrom, ( size_t ) pxQueue->uxItemSize ); 
}
static void __InitialiseNewQueue(QueueInner_t *const pxNewQueue, const uint32_t uxQueueItemCap, const uint32_t uxItemSize, uint8_t *pucQueueStorage)
{
    /* Set the head to the start of the queue storage area. */
    pxNewQueue->pcHead = ( int8_t * ) pucQueueStorage;
    pxNewQueue->uItemCap = uxQueueItemCap;
    pxNewQueue->uxItemSize = uxItemSize;
    ( void )queueReset( (queue_t *)pxNewQueue );
}
uint8_t xQueueGenericPut( queue_t *const xQueue, const void * const pvItemToQueue , const uint8_t isFront )
{
    QueueInner_t * const pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );    
    configASSERT( pvItemToQueue );

    /* Is there room on the queue now?  The running task must be the highest priority task wanting to access the queue.  
      If the head item in the queue is to be overwritten then it does not matter if the queue is full. */
    if( pxQueue->uItemCurCnt < pxQueue->uItemCap  ) {
        
        __CopyDataToQueue( pxQueue, pvItemToQueue, isFront );

        return TRUE;
    }

    return FALSE;
}

uint8_t xQueueGenericPop( queue_t *const xQueue, void * const pvBuffer, const uint8_t isJustPeeking )
{
    int8_t *pcOriginalReadPosition;
    QueueInner_t * const pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );

    /* Is there data in the queue now?. */
    if( pxQueue->uItemCurCnt > ( uint32_t ) 0 ) {
        /* Remember the read position in case the queue is only being peeked. */
        pcOriginalReadPosition = pxQueue->pcReadFrom;

        __CopyDataFromQueue( pxQueue, pvBuffer );

        if( isJustPeeking == TRUE ){
            /* The data is not being removed, so reset the read pointer. */
            pxQueue->pcReadFrom = pcOriginalReadPosition;
        }
        else{            
            /* Actually removing data, not just peeking. */
            pxQueue->uItemCurCnt--;
        }

        return TRUE;
    }
    
    return FALSE;
}

void *xQueueOnAlloc( queue_t *const xQueue , const uint8_t isFront )
{
    int8_t *pcPutPosition;
    QueueInner_t * const pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );    

    /* Is there room on the queue now?  The running task must be the highest priority task wanting to access the queue.  
      If the head item in the queue is to be overwritten then it does not matter if the queue is full. */
    if( pxQueue->uItemCurCnt < pxQueue->uItemCap  ) {
        
        if( isFront == FALSE ){
            pcPutPosition =  pxQueue->pcWriteTo;
            
            pxQueue->pcWriteTo += pxQueue->uxItemSize;
            if( pxQueue->pcWriteTo >= pxQueue->pcTail )  {
                pxQueue->pcWriteTo = pxQueue->pcHead;
            }
        }
        else{
             pcPutPosition =   pxQueue->pcReadFrom;

            pxQueue->pcReadFrom -= pxQueue->uxItemSize;
            if( pxQueue->pcReadFrom < pxQueue->pcHead ) {
                pxQueue->pcReadFrom = ( pxQueue->pcTail - pxQueue->uxItemSize );
            }
        }
        
        pxQueue->uItemCurCnt++;

        return (void *)pcPutPosition;
    }

    return (void *)NULL;
}

void *queueOnPeek( queue_t *const xQueue )
{
    int8_t *pcpeekPosition;
    QueueInner_t * const pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );

    /* Is there data in the queue now?. */
    if( pxQueue->uItemCurCnt > ( uint32_t ) 0 ) {
        pcpeekPosition = pxQueue->pcReadFrom + pxQueue->uxItemSize;
        
        if( pcpeekPosition >= pxQueue->pcTail ) {
            pcpeekPosition = pxQueue->pcHead;
        }

        return (void *)pcpeekPosition;
    }
    
    return (void *)NULL;
}


uint32_t queueItemAvailableValid( queue_t *const xQueue )
{
    configASSERT( xQueue );

    return ( ( QueueInner_t * ) xQueue )->uItemCurCnt;
} 

uint32_t queueItemAvailableIdle( queue_t *const xQueue )
{
    QueueInner_t *pxQueue = ( QueueInner_t * ) xQueue;

    configASSERT( pxQueue );

    return (pxQueue->uItemCap - pxQueue->uItemCurCnt);
}

uint8_t queueIsEmpty( queue_t *const xQueue )
{
   return (uint8_t )( ( ( QueueInner_t * )xQueue )->uItemCurCnt == ( uint32_t )  0 );
}

uint8_t queueIsFull( queue_t *const xQueue )
{
        return (uint8_t )(( ( QueueInner_t *)xQueue )->uItemCurCnt == ( ( QueueInner_t *)xQueue )->uItemCap );
}
