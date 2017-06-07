
#ifndef _RF_H_
#define _RF_H_


#include "hal_rf.h"

void Rf_Init(void);
void rf_check(void);
void halRfSendPacket(uint8_t *txBuffer, uint8_t size); 
uint8_t halRfReceivePacket(uint8_t *rxBuffer, uint8_t length); 


#endif

