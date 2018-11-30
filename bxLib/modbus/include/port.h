
#ifndef _MB_PORT_H
#define _MB_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbcpu.h"
#include "modbus.h"

#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0

/* ------------ Serial port functions ----------------------------*/
uint8_t MbPortSerialInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, MbParity_t eParity);
void MbPortSerialClose(uint8_t port);
void MbPortSerialEnable(uint8_t port, uint8_t xRxEnable, uint8_t xTxEnable );
uint8_t MbPortSerialGetByte(uint8_t port, char * pucByte );
uint8_t MbPortSerialPutByte(uint8_t port, char ucByte);

/* ------------ Timers functions ---------------------------------*/
uint8_t MbPortTimersInit(uint8_t port, uint16_t usTimeOut50us );
void MbPortTimersClose(uint8_t port);
void MbPortTimersEnable(uint8_t port);
void MbPortTimersDisable(uint8_t port);
void MbPortTimersDelay(uint8_t port,uint16_t usTimeOutMS );

uint32_t MbSys_now(void);

#endif
/* ----------------------- TCP port functions -------------------------------*/
#if MB_TCP_ENABLED > 0

uint8_t MbTCPPortInit( uint16_t usTCPPort );
void MbTCPPortClose( void );
void MbTCPPortDisable( void );
uint8_t MbTCPPortGetRequest( uint8_t **ppucMBTCPFrame, uint16_t * usTCPLength );
uint8_t MbTCPPortSendResponse( const uint8_t *pucMBTCPFrame, uint16_t usTCPLength );

#endif

#ifdef __cplusplus
}
#endif
#endif
