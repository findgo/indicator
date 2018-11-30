
#ifndef _MB_RTU_H
#define _MB_RTU_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mbconfig.h"
#include "mbproto.h"

#include "mbcpu.h"
#include "mb.h"

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RTU_RX_IDLE,              /*!< RTU Receiver is in idle state. */
    STATE_RTU_RX_RCV,               /*!< RTU Frame is beeing received. */
    STATE_RTU_TX_XMIT               /*!< RTU Transmitter is in transfer state. */
} MbRTUSndRcvState;    

#if MB_RTU_ENABLED > 0 
#if MB_MASTER_ENABLED > 0

MbErrorCode_t MbmRTUInit( Mbmhandle_t dev, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity);
void MbmRTUStart(Mbmhandle_t dev);
void MbmRTUStop(Mbmhandle_t dev);
void MbmRTUClose( Mbmhandle_t dev);
MbReqResult_t MbmRTUReceive(Mbmhandle_t dev,MbHeader_t *phead,uint8_t *pfunCode, uint8_t **premain, uint16_t *premainLength);
MbReqResult_t MbmRTUSend(Mbmhandle_t dev,const uint8_t *pAdu, uint16_t usLength);

void MbmRTUReceiveFSM(  Mbmhandle_t dev);
void MbmRTUTransmitFSM(  Mbmhandle_t dev);
void MbmRTUTimerT15Expired(Mbmhandle_t dev);
void MbmRTUTimerT35Expired(  Mbmhandle_t dev);

#endif

#if MB_SLAVE_ENABLED > 0

MbErrorCode_t MbsRTUInit(Mbshandle_t dev, uint8_t ucPort, uint32_t ulBaudRate,MbParity_t eParity);
void MbsRTUStart(Mbshandle_t dev);
void MbsRTUStop(Mbshandle_t dev);
void MbsRTUClose(Mbshandle_t dev);
MbErrorCode_t MbsRTUReceiveParse(Mbshandle_t dev, MbsAduFrame_t *aduFrame);
MbErrorCode_t MbsRTUSend(Mbshandle_t dev,uint8_t ucSlaveAddress, const uint8_t *pPdu, uint16_t usLength);

void MbsRTUReceiveFSM(  Mbshandle_t dev);
void MbsRTUTransmitFSM(Mbshandle_t dev);
void MbsRTUTimerT15Expired(  Mbshandle_t dev);
void MbsRTUTimerT35Expired(Mbshandle_t dev);

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
