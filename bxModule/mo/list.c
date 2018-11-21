
#include <stdlib.h>
#include "list.h"

/*-----------------------------------------------------------
 * PUBLIC LIST API documented in list.h
 *----------------------------------------------------------*/

void listInitialise( List_t * const pxList )
{
    /* The list structure contains a list item which is used to mark the
    end of the list.  To initialise the list the list end is inserted
    as the only list entry. */
    pxList->pxIndex = ( ListItem_t * ) &( pxList->xListEnd );           
    
    /* The list end value is the highest possible value in the list to
    ensure it remains at the end of the list. */
    pxList->xListEnd.xItemValue = LIST_ITEM_VALUE_MAX;

    /* The list end next and previous pointers point to itself so we know
    when the list is empty. */
    pxList->xListEnd.pxNext = ( ListItem_t * ) &( pxList->xListEnd );  
    pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );
    
    pxList->uxNumberOfItems = ( uint32_t ) 0U;
}
/*-----------------------------------------------------------*/

void listInitialiseItem( ListItem_t * const pxItem )
{
    /* Make sure the list item is not recorded as being on a list. */
    pxItem->pvContainer = NULL;
}
/*-----------------------------------------------------------*/

void listInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem )
{
    ListItem_t * const pxIndex = pxList->pxIndex;

    /* Insert a new list item into pxList, but rather than sort the list,
    makes the new list item the last item to be removed by a call to
    listGET_OWNER_OF_NEXT_ENTRY(). */
    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;

    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;

    /* Remember which list the item is in. */
    pxNewListItem->pvContainer = ( void * ) pxList;

    ( pxList->uxNumberOfItems )++;
}
/*-----------------------------------------------------------*/

void listInsert( List_t * const pxList, ListItem_t * const pxNewListItem )
{
    ListItem_t *pxIterator;
    const uint32_t xValueOfInsertion = pxNewListItem->xItemValue;

    /* Insert the new list item into the list, sorted in xItemValue order.

    If the list already contains a list item with the same item value then the
    new list item should be placed after it.  This ensures that TCB's which are
    stored in ready lists (all of which have the same xItemValue value) get a
    share of the CPU.  However, if the xItemValue is the same as the back marker
    the iteration loop below will not end.  Therefore the value is checked
    first, and the algorithm slightly modified if necessary. */
    if( xValueOfInsertion == LIST_ITEM_VALUE_MAX ) {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    else {

        for( pxIterator = ( ListItem_t * ) &( pxList->xListEnd ); 
                pxIterator->pxNext->xItemValue <= xValueOfInsertion; 
                    pxIterator = pxIterator->pxNext ) {
            /* There is nothing to do here, just iterating to the wanted
            insertion position. */
        }
    }

    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;

    /* Remember which list the item is in.  This allows fast removal of the item later. */
    pxNewListItem->pvContainer = ( void * ) pxList;

    ( pxList->uxNumberOfItems )++;
}
/*-----------------------------------------------------------*/

uint32_t listRemove( ListItem_t * const pxItemToRemove )
{
    /* The list item knows which list it is in.  Obtain the list from the list item. */
    List_t * const pxList = ( List_t * ) pxItemToRemove->pvContainer;

    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;

    /* Make sure the index is left pointing to a valid item. */
    if( pxList->pxIndex == pxItemToRemove ){
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }

    pxItemToRemove->pvContainer = NULL;
    ( pxList->uxNumberOfItems )--;

    return pxList->uxNumberOfItems;
}
/*-----------------------------------------------------------*/

