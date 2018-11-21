

#ifndef __HEAP_MANGE_H_
#define __HEAP_MANGE_H_

#include "heap_def.h"

// for byte alignment
#if portBYTE_ALIGNMENT == 32
    #define portBYTE_ALIGNMENT_MASK     ( 0x001f )
#endif

#if portBYTE_ALIGNMENT == 16
    #define portBYTE_ALIGNMENT_MASK     ( 0x000f )
#endif

#if portBYTE_ALIGNMENT == 8
    #define portBYTE_ALIGNMENT_MASK     ( 0x0007 )
#endif

#if portBYTE_ALIGNMENT == 4
    #define portBYTE_ALIGNMENT_MASK     ( 0x0003 )
#endif

#if portBYTE_ALIGNMENT == 2
    #define portBYTE_ALIGNMENT_MASK     ( 0x0001 )
#endif

#if portBYTE_ALIGNMENT == 1
    #define portBYTE_ALIGNMENT_MASK     ( 0x0000 )
#endif

#ifndef portBYTE_ALIGNMENT_MASK
    #error "Invalid portBYTE_ALIGNMENT definition"
#endif

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
size_t xPortGetFreeHeapSize( void );

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
extern void vApplicationMallocFailedHook( void );
#endif


#endif

