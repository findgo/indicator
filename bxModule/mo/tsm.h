


#ifndef __TSM_H__
#include __TSM_H__

#include "common_type.h"
#include "common_def.h"


uint8_t tsm_invokeID_isidle(uint8_t invokeID);
uint8_t tsm_invokeID_alloc(void);
void tsm_invokeID_free(uint8_t invokeID);


#endif

