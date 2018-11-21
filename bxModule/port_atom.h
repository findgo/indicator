/**
  ******************************************************************************
  * @file 
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention 	20151110     v1.1   	jgb		
  * @attention 	20180920     v1.2   	jgb		
  ******************************************************************************
  */

#ifndef __PORT_ATOM_H__
#define __PORT_ATOM_H__

#include "common_platform.h"
#include "common_type.h"

typedef uint8_t intSaveStatus_t;

#define portENABLE_INTERRUPTS()         do{ __set_PRIMASK(0); } while(0)
#define portDISABLE_INTERRUPTS()        do{ __set_PRIMASK(1); } while(0)

#define portEnterCriticial()        portDISABLE_INTERRUPTS()
#define portExitCriticial()         portENABLE_INTERRUPTS()

#define porINTERRUPT_FROM_ISR()     ((0 ==  __get_PRIMASK()) ? TRUE : FALSE)

//! \brief Exit from the safe atom operations
#define taskENTER_CRITICAL() enterAtomCritical()
//! \brief Exit from the safe atom operations
#define taskEXIT_CRITICAL() exitAtomCritical()


#define isrSaveCriticial_status_Variable    intSaveStatus_t bintstatus

#define isrENTER_CRITICAL() \
            bintstatus = porINTERRUPT_FROM_ISR();\
            portEnterCriticial();

//! \brief Exit from the safe atom operations
#define isrEXIT_CRITICAL()           \
                if (bintstatus) {    \
                    portExitCriticial();\
                }

// internal used
void enterAtomCritical( void );
void exitAtomCritical( void );

#endif
