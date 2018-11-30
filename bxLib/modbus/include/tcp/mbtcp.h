
#ifndef __MB_TCP_H
#define __MB_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbconfig.h"
#include "mbproto.h"
#include "mbframe.h"
#include "mbcpu.h"

#include "mb.h"

MbErrorCode_t MbsTCPInit(uint16_t ucTCPPort);
void MbsTCPStart(void *dev);
void MbsTCPStop(void *dev);
void MbsTCPClose(void *dev);
MbErrorCode_t MbsTCPReceive(void *dev, uint8_t *pucRcvAddress, uint8_t **pPdu,uint16_t *pusLength);
MbErrorCode_t MbsTCPSend(void *dev, uint8_t _unused, const uint8_t *pPdu,uint16_t usLength );

MbErrorCode_t MbmTCPInit(uint16_t ucTCPPort);
void MbmTCPStart(void *dev);
void MbmTCPStop(void *dev);
void MbmTCPClose(void *dev);
MbErrorCode_t MbmTCPReceive(void *dev, uint8_t *pucRcvAddress, uint8_t **pPdu,uint16_t *pusLength);
MbErrorCode_t MbmTCPSend(void *pdev,const uint8_t *pAdu, uint16_t usLength);


#ifdef __cplusplus
}
#endif
#endif
