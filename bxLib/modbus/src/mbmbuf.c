
#include "mbproto.h"
#include "mbmbuf.h"

#if MB_MASTER_ENABLED > 0
#include "msglink.h"

MbmReq_t *MbmReqMsgNew(MbMode_t mode, uint16_t Pdusize)
{
    uint16_t Adusize;
    MbmReq_t *req;
    
    if(mode == MB_RTU)
        Adusize = MB_SER_ADU_SIZE_ADDR + MB_SER_ADU_SIZE_CRC + Pdusize;
    else if(mode == MB_ASCII)
        Adusize = MB_SER_ADU_SIZE_ADDR + MB_SER_ADU_SIZE_LRC + Pdusize;
    else
        Adusize = MB_TCP_ADU_SIZE_MBAP + Pdusize;
    
    req = (MbmReq_t *)msgalloc(sizeof(MbmReq_t) + Adusize);
    if(req == NULL){
        return NULL;
    }
        
    memset(req, 0, sizeof(MbmReq_t) + Adusize);

    return req;
}

void MbmReqMsgDelete(void *msg_ptr)
{
    msgdealloc(msg_ptr);
}

/* set head and return head length */
uint8_t MbmBuildHead(MbMode_t mode, uint16_t tid, uint8_t slaveaddr, uint8_t *pAdu, uint16_t pdulength)
{
    if(mode == MB_RTU || mode == MB_ASCII){
        pAdu[MB_SER_ADU_ADDR_OFFSET] = slaveaddr;

        /* rtu ascii header size */
        return MB_SER_ADU_SIZE_ADDR;
    }
    else{
        pAdu[MB_TCP_ADU_TID_OFFSET]     = tid >> 8;
        pAdu[MB_TCP_ADU_TID_OFFSET + 1] = tid;
        pAdu[MB_TCP_ADU_PID_OFFSET]     = MB_TCP_PROTOCOL_ID >> 8;
        pAdu[MB_TCP_ADU_PID_OFFSET + 1] = MB_TCP_PROTOCOL_ID ;
        pAdu[MB_TCP_ADU_LEN_OFFSET]     = (pdulength + 1) >> 8;
        pAdu[MB_TCP_ADU_LEN_OFFSET + 1] = (pdulength + 1) & 0xff;
        pAdu[MB_TCP_ADU_UID_OFFSET]     = slaveaddr;

        //  tcp MBAP header size
        return MB_TCP_ADU_SIZE_MBAP;
    }
}
#endif
