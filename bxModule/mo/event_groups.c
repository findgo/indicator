
#include "event_groups.h"

typedef struct EventGroup_s
{
    EventBits_t eventBits;
} EventGroup_t;

#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1)
EventGroupHandle_t eventGroupNew( void )
{
    EventGroup_t *pxEventBits;

    /* Allocate the event group. */
    pxEventBits = ( EventGroup_t * ) mo_malloc( sizeof( EventGroup_t ) );

    if( pxEventBits){
        pxEventBits->eventBits = 0;
    }

    return ( EventGroupHandle_t ) pxEventBits;
}
#endif

EventGroupHandle_t eventGroupAssign( EventGroupStatic_t *pxEventGroupBuffer )
{
    EventGroup_t *pxEventBits;

    /* A StaticEventGroup_t object must be provided. */
    configASSERT( pxEventGroupBuffer );

    /* The user has provided a statically allocated event group - use it. */
    pxEventBits = ( EventGroup_t * ) pxEventGroupBuffer; 

    if( pxEventBits ){
        pxEventBits->eventBits = 0;
    }

    return ( EventGroupHandle_t ) pxEventBits;
}

/*-----------------------------------------------------------*/

EventBits_t eventGroupWaitBits( EventGroupHandle_t xEventGroup, const EventBits_t uBitsToProcessFor)
{
    EventGroup_t *pxEventBits = ( EventGroup_t * ) xEventGroup;
    EventBits_t previousEventBits;

    /* Check the user is not attempting to wait on the bits used by the kernel
       itself, and that at least one bit is being requested. */
    configASSERT( xEventGroup );
    
    taskENTER_CRITICAL();
    // wait for all bit
    if( uBitsToProcessFor == ( EventBits_t )0 ){
        previousEventBits = pxEventBits->eventBits;
        pxEventBits->eventBits = 0; // clear all bit
    }
    else {
        previousEventBits = pxEventBits->eventBits & uBitsToProcessFor;
        pxEventBits->eventBits &= ~uBitsToProcessFor;
    
    }
    taskEXIT_CRITICAL();
    
    return previousEventBits;
}

EventBits_t eventGroupClearBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear )
{
    EventBits_t uxReturn;
    isrSaveCriticial_status_Variable;

    /* Check the user is not attempting to clear the bits used by the kernel itself. */
    configASSERT( xEventGroup );

    isrENTER_CRITICAL();
    /* The value returned is the event group value prior to the bits being cleared. */
    uxReturn = ( ( EventGroup_t * ) xEventGroup )->eventBits;

    /* Clear the bits. */
    ( ( EventGroup_t * ) xEventGroup )->eventBits &= ~uxBitsToClear;
    isrEXIT_CRITICAL();
    
    return uxReturn;
}


EventBits_t eventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet )
{
    EventBits_t uxReturn;
    isrSaveCriticial_status_Variable;
    
    /* Check the user is not attempting to set the bits used by the kernel itself. */
    configASSERT( xEventGroup );
    
    isrENTER_CRITICAL();
    uxReturn = ( ( EventGroup_t * ) xEventGroup )->eventBits;
    /* Set the bits. */
    ( ( EventGroup_t * ) xEventGroup )->eventBits |= uxBitsToSet;
    isrEXIT_CRITICAL();

    return uxReturn;
}
