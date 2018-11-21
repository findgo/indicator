

#include "tsm.h"

//an Invoke ID = 0 is an unused spot in the table 
// bit mask: 0 -> used  1 -> idle
static  uint32_t tsm_invokeID_bitsTbl[8] = {0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
static uint8_t tsm_current_invokeID = 1;

/*********************************************************************
 * @brief   set corresponding invoke id mask in used
 *
 * @param   invokeID - invoke id
 *
 * @return 
 */
static void __invokeID_set_inused(uint8_t invokeID)
{
    uint8_t idx = invokeID >> 5; // invokeID / 32
    uint8_t mask = 1 << (invokeID & 0x1f); // invokeID % 32

    tsm_invokeID_bitsTbl[idx] &= ~mask;
}
/*********************************************************************
 * @brief   check corresponding invoke id is free?
 *
 * @param   invokeID - invoke id
 *
 * @return  TRUE : the invoke id is idle, FALSE : in used
 */
uint8_t tsm_invokeID_isidle(uint8_t invokeID)
{
    uint8_t idx = invokeID >> 5; // invokeID / 32
    uint8_t mask = 1 << (invokeID & 0x1f); // invokeID % 32

    if( tsm_invokeID_bitsTbl[idx] & mask)
        return TRUE;

    return FALSE;
}
/*********************************************************************
 * @brief   alloc a invoke id
 *
 * @return  0 : no invoke id can alloc, other : invoke id
 */
uint8_t tsm_invokeID_alloc(void)
{
    uint8_t invokeid = tsm_current_invokeID;

    do{
        if(tsm_invokeID_Isidle(tsm_current_invokeID)){
            invokeid = tsm_current_invokeID++;
            __invokeID_set_inused(invokeid);
        
            return invokeid;
        }
        //check next is idle?
        tsm_current_invokeID++;
    }while(invokeid != tsm_current_invokeID);

    // no free transmission  sequence number
    return 0;
}
/*********************************************************************
 * @brief   free a invoke id
 *
 * @param   invokeID - invoke id
 *
 * @return  
 */
void tsm_invokeID_free(uint8_t invokeID)
{
    uint8_t idx = invokeID >> 5; // invokeID / 32
    uint8_t mask = 1 << (invokeID & 0x1f); // invokeID % 32

    tsm_invokeID_bitsTbl[idx] |= mask;
}


