

#ifndef __MAX31855_H_
#define __MAX31855_H_



//all default value is 0
#define MAX31855_FAULT  ((uint8_t)1 << 3)   /*when scv,scg,oc is fault*/
#define MAX31855_SCV_FAULT ((uint8_t)1 << 2) /*This bit is a 1 when the thermocouple is short-circuited to VCC*/
#define MAX31855_SCG_FAULT ((uint8_t)1 << 1) /*This bit is a 1 when the thermocouple is short-circuited to GND.*/
#define MAX31855_OC_FAULT ((uint8_t)1 << 0) /*This bit is a 1 when the thermocouple is open (no connections).*/


void Max31855Init(void);
void Max31855Update(void);
uint32_t Max31855RawValue(void);
/*放大4倍*/
int16_t Max31855ThermocoupleTemp(void);
/*放大16倍*/
int16_t Max31855InternalTemp(void);
uint8_t Max31855Fault(void);



#endif



