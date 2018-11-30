
#ifndef __MBM_BUF_H_
#define __MBM_BUF_H_


#include "mbconfig.h"
#include "mbcpu.h"

#include "mb.h"

MbmReq_t *MbmReqMsgNew(MbMode_t mode,uint16_t Pdusize);
void MbmReqMsgDelete(void *msg_ptr);
uint8_t MbmBuildHead(MbMode_t mode, uint16_t tid, uint8_t slaveaddr, uint8_t *pAdu, uint16_t pdulength);


#endif

