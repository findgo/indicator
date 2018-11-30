
#include "mbfunc.h"
#include "modbus.h"

#if MB_MASTER_ENABLED > 0
#include "mbmbuf.h"

/* ok */
MbReqResult_t MbmReqRdCoils(Mbmhandle_t dev, uint8_t slaveaddr, uint16_t RegStartAddr, uint16_t Coilcnt, uint16_t scanrate )
{
    uint8_t *pAdu;
    uint16_t len;
    MbmReq_t *req;
    MbmNode_t *node = NULL;
    MbReqResult_t result;
    
    /* check slave address valid */
    if(slaveaddr > MB_ADDRESS_MAX) 
        return MBR_EINNODEADDR;
    /* check request count range( 1 - 2000 ) */
    if(Coilcnt < MB_READBITS_CNT_MIN || Coilcnt > MB_READBITS_CNT_MAX )
        return MBR_EINVAL;
    /* if slave address not a broadcast address, search in the host?*/
    if(slaveaddr != MB_ADDRESS_BROADCAST){
        /* check node in host list */
        node = MbmSearchNode(dev, slaveaddr);
        if(node == NULL)
            return MBR_ENODENOSETUP;
        
        /* check register addres in range*/
        if((RegStartAddr < node->regs.reg_coils_addr_start)
            || ((RegStartAddr + Coilcnt) > (node->regs.reg_coils_addr_start + node->regs.reg_coils_num)))
            return MBR_ENOREG;
    }
    
    req = MbmReqMsgNew((( MbmDev_t *)dev)->mode, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE);
    if(req == NULL)
        return MBR_ENOMEM;

    pAdu = &(req->adu[0]);
    // set header and get head size
    len = MbmBuildHead((( MbmDev_t *)dev)->mode, 0, slaveaddr, pAdu, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE);

    pAdu[len + MB_PDU_FUNCODE_OFF]               = MB_FUNC_READ_COILS;
    pAdu[len + MB_PDU_FUNC_READ_ADDR_OFF]        = RegStartAddr >> 8;
    pAdu[len + MB_PDU_FUNC_READ_ADDR_OFF + 1]    = RegStartAddr;
    pAdu[len + MB_PDU_FUNC_READ_BITSCNT_OFF]     = Coilcnt >> 8;
    pAdu[len + MB_PDU_FUNC_READ_BITSCNT_OFF + 1] = Coilcnt;
    
    req->adulength = len + MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE;

    req->node      = node;
    req->errcnt    = 0;
    req->slaveaddr = slaveaddr;
    req->funcode   = MB_FUNC_READ_COILS;
    req->regaddr   = RegStartAddr;
    req->regcnt    = Coilcnt;
    req->scanrate  = ((scanrate < MBM_SCANRATE_MAX) ? scanrate : MBM_SCANRATE_MAX);
    req->scancnt   = 0;
    
    result = MbmSend(dev, req);
    if(result != MBR_ENOERR)
        MbmReqMsgDelete(req);
    
    return result;
}
/* ok */
MbReqResult_t MbmReqWrCoil(Mbmhandle_t dev, uint8_t slaveaddr, uint16_t RegAddr, uint16_t val)
{
    uint8_t *pAdu;
    uint16_t len;
    MbmReq_t *req;
    MbmNode_t *node = NULL;
    MbReqResult_t result;

    /* check slave address valid */
    if(slaveaddr > MB_ADDRESS_MAX) 
        return MBR_EINNODEADDR;
    /* if slave address not a broadcast address, search in the host?*/
    if(slaveaddr != MB_ADDRESS_BROADCAST){
        /* check node in host list */
        node = MbmSearchNode(dev,slaveaddr);
        if(node == NULL)
            return MBR_ENODENOSETUP;
        /* check register addres in range*/
        if((RegAddr < node->regs.reg_coils_addr_start)
            || ((RegAddr + 1) > (node->regs.reg_coils_addr_start + node->regs.reg_coils_num)))
            return MBR_ENOREG;
    }
    
    req = MbmReqMsgNew((( MbmDev_t *)dev)->mode, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_WRITE_SIZE);
    if(req == NULL)
        return MBR_ENOMEM;
    
    pAdu = &(req->adu[0]);
    // set header and get head size
    len = MbmBuildHead((( MbmDev_t *)dev)->mode, 0, slaveaddr, pAdu, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_WRITE_SIZE);

    val = (val > 0) ? 0xFF00 : 0x0000;
    pAdu[len + MB_PDU_FUNCODE_OFF]              = MB_FUNC_WRITE_SINGLE_COIL;
    pAdu[len + MB_PDU_FUNC_WRITE_ADDR_OFF]      = RegAddr >> 8;
    pAdu[len + MB_PDU_FUNC_WRITE_ADDR_OFF + 1]  = RegAddr;
    pAdu[len + MB_PDU_FUNC_WRITE_VALUE_OFF]     = val >> 8;
    pAdu[len + MB_PDU_FUNC_WRITE_VALUE_OFF + 1] = val;

    req->adulength = len + MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_WRITE_SIZE;

    req->node      = node;
    req->errcnt    = 0;
    req->slaveaddr = slaveaddr;
    req->funcode   = MB_FUNC_WRITE_SINGLE_COIL;
    req->regaddr   = RegAddr;
    req->regcnt    = 1;
    req->scanrate  = 0;
    req->scancnt   = 0;
    
    result = MbmSend(dev,req);
    if(result != MBR_ENOERR)
        MbmReqMsgDelete(req);

    return result;
}
/* ok */
MbReqResult_t MbmReqWrMulCoils( Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Coilcnt,
                                        uint8_t *valbuf, uint16_t valcnt)
{
    uint8_t *pAdu;
    uint16_t pdulengh,len;
    MbmReq_t *req;
    MbmNode_t *node = NULL;
    MbReqResult_t result;
    uint8_t ucByteCount;
    
    /* check slave address valid */
    if(slaveaddr > MB_ADDRESS_MAX) 
        return MBR_EINNODEADDR;
    /* check request count range( 1 - 2000 ) */    
    if( (Coilcnt < MB_WRITEBITS_CNT_MIN ) || (Coilcnt > MB_WRITEBITS_CNT_MAX ))
        return MBR_EINVAL;
    
    /* Compute the number of expected bytes in the request. */
    ucByteCount = Coilcnt / 8 + (((Coilcnt & 0x0007) > 0) ? 1 : 0);

    if(ucByteCount != valcnt)
        return MBR_EINVAL;
    /* if slave address not a broadcast address, search in the host?*/
    if(slaveaddr != MB_ADDRESS_BROADCAST){
        /* check node in host list */
        node = MbmSearchNode(dev,slaveaddr);
        if(node == NULL)
            return MBR_ENODENOSETUP;
        /* check register addres in range*/
        if((RegStartAddr < node->regs.reg_coils_addr_start)
            || ((RegStartAddr + Coilcnt) > (node->regs.reg_coils_addr_start + node->regs.reg_coils_num)))
            return MBR_ENOREG;
    }
    
    /* slaveaddr +((PDU)funccode + startaddr + coilcnt + bytenum + value_list)  */
    pdulengh =  MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + ucByteCount;
    req = MbmReqMsgNew((( MbmDev_t *)dev)->mode, pdulengh);
    if(req == NULL)
        return MBR_ENOMEM;
    
    pAdu = &(req->adu[0]);
    // get head size
    len = MbmBuildHead((( MbmDev_t *)dev)->mode, 0, slaveaddr, pAdu, pdulengh);    

    pAdu[len + MB_PDU_FUNCODE_OFF]                      = MB_FUNC_WRITE_MULTIPLE_COILS;
    pAdu[len + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF]          = RegStartAddr >> 8;
    pAdu[len + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]      = RegStartAddr;
    pAdu[len + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF]       = Coilcnt >> 8;
    pAdu[len + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1]   = Coilcnt;
    pAdu[len + MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF]       = ucByteCount;

    ucByteCount = 0;
    while(valcnt--)
    {
        pAdu[len + MB_PDU_FUNC_WRITE_MUL_VALUES_OFF + ucByteCount] = *valbuf;
        ucByteCount++;
        valbuf++;
    }
    req->adulength = len + pdulengh;

    req->node      = node;
    req->errcnt    = 0;
    req->slaveaddr = slaveaddr;
    req->funcode   = MB_FUNC_WRITE_MULTIPLE_COILS;
    req->regaddr   = RegStartAddr;
    req->regcnt    = Coilcnt;
    req->scanrate  = 0;
    req->scancnt   = 0;
    
    result = MbmSend(dev,req);
    if(result != MBR_ENOERR)
        MbmReqMsgDelete(req);

    return result;
}

MbReqResult_t MbmReqRdDiscreteInputs(Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Discnt, uint16_t scanrate)
{
    uint8_t *pAdu;
    uint16_t len;
    MbmReq_t *req;
    MbmNode_t *node = NULL;
    MbReqResult_t result;
    
    /* check slave address valid */
    if(slaveaddr > MB_ADDRESS_MAX) 
        return MBR_EINNODEADDR;
    /* check request count range( 1 - 2000 ) */
    if(Discnt < MB_READBITS_CNT_MIN || Discnt > MB_READBITS_CNT_MAX)
        return MBR_EINVAL;
    /* if slave address not a broadcast address, search in the host?*/
    if(slaveaddr != MB_ADDRESS_BROADCAST){
        /* check node in host list */
        node = MbmSearchNode(dev,slaveaddr);
        if(node == NULL)
            return MBR_ENODENOSETUP;
        /* check register addres in range*/
        if((RegStartAddr < node->regs.reg_discrete_addr_start)
            || ((RegStartAddr + Discnt) > (node->regs.reg_discrete_addr_start + node->regs.reg_discrete_num)))
            return MBR_ENOREG;
    }
    req = MbmReqMsgNew((( MbmDev_t *)dev)->mode, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE);
    if(req == NULL)
        return MBR_ENOMEM;
    
    pAdu = &(req->adu[0]);
    // set header and get head size
    len = MbmBuildHead((( MbmDev_t *)dev)->mode, 0, slaveaddr, pAdu, MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE);
    
    pAdu[len + MB_PDU_FUNCODE_OFF]               = MB_FUNC_READ_DISCRETE_INPUTS;
    pAdu[len + MB_PDU_FUNC_READ_ADDR_OFF]        = RegStartAddr >> 8;
    pAdu[len + MB_PDU_FUNC_READ_ADDR_OFF + 1]    = RegStartAddr;
    pAdu[len + MB_PDU_FUNC_READ_BITSCNT_OFF]     = Discnt >> 8;
    pAdu[len + MB_PDU_FUNC_READ_BITSCNT_OFF + 1] = Discnt;
    
    req->adulength = len + MB_PDU_SIZE_FUNCODE + MB_PDU_FUNC_READ_SIZE;

    req->node      = node;
    req->errcnt    = 0;
    req->slaveaddr = slaveaddr;
    req->funcode   = MB_FUNC_READ_DISCRETE_INPUTS;
    req->regaddr   = RegStartAddr;
    req->regcnt    = Discnt;
    req->scanrate  = ((scanrate < MBM_SCANRATE_MAX) ? scanrate : MBM_SCANRATE_MAX);
    req->scancnt   = 0;

    result = MbmSend(dev, req);
    if(result != MBR_ENOERR)
        MbmReqMsgDelete(pAdu);

    return result;
}


/* TODO implement modbus master request parse */

/* write local bits register to coils or discrete */
static void __MbmLocalWrRegBits(uint8_t *pRegBits, uint16_t usStartAddress, uint8_t *pucRegBitsVal, uint16_t usNCoils)
{
    int16_t iNCoils = ( int16_t )usNCoils;

    while( iNCoils > 0 )
    {
        MbSetBits(pRegBits, usStartAddress, (uint8_t)(iNCoils > 8 ? 8 : iNCoils), *pucRegBitsVal++);
        iNCoils -= 8;
        usStartAddress += 8;
    }
}
/* ok */
MbReqResult_t MbmParseRspRdCoils(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt, 
                                    uint8_t *premain,uint16_t remainLength)
{
    uint8_t ucByteCount;

    ucByteCount = ReqRegcnt / 8 + (((ReqRegcnt & 0x0007) > 0) ? 1 : 0);
    /* check frame is right length */    
    /* check coilcnt with previous request byteNum */
    if((remainLength  != (1 + ucByteCount)) || (ucByteCount != premain[0]))
        return MBR_EINVAL;
      
    __MbmLocalWrRegBits(regs->pRegCoil, ReqRegAddr - regs->reg_coils_addr_start,(uint8_t *)&premain[1],  ReqRegcnt);

    return MBR_ENOERR;
}
/* ok */
MbReqResult_t MbmParseRspWrCoil(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                    uint8_t *premain, uint16_t remainLength)
{
    uint8_t bitval = 0;

    (void) ReqRegcnt;
    if((remainLength != 4) || ReqRegAddr != ((premain[0] << 8) | premain[1]))
        return MBR_EINVAL;

    
    if( ( premain[3] != 0x00 )
        || !((premain[2] == 0xFF) || (premain[2] == 0x00)) )
        return MBR_EINVAL;
    
    if(premain[2] == 0xFF)
        bitval |= 0x01; 

    __MbmLocalWrRegBits(regs->pRegCoil, ReqRegAddr - regs->reg_coils_addr_start, (uint8_t *)&bitval, 1);

    return MBR_ENOERR;
}
/* ok */
MbReqResult_t MbmParseRspWrMulCoils(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt,
                                    uint8_t *premain, uint16_t remainLength)
{
    if(remainLength != 4)
        return MBR_EINVAL;

    if((ReqRegAddr != ((premain[0] << 8) | premain[1]))
        || (ReqRegcnt != ((premain[2] << 8) | premain[3])))
        return MBR_EINVAL;

    return MBR_ENOERR;
}
/* ok */
MbReqResult_t MbmParseRspRdDiscreteInputs(MbReg_t *regs, 
                                    uint16_t ReqRegAddr, uint16_t ReqRegcnt, 
                                    uint8_t *premain, uint16_t remainLength)
{
    uint8_t ucByteCount;
    
    ucByteCount = ReqRegcnt / 8 + (((ReqRegcnt & 0x0007) > 0) ? 1 : 0);
    /* check frame is right length */
    /* check coilcnt with previous request byteNum */
    if((remainLength  != (1 + ucByteCount)) || (ucByteCount != premain[0]))
        return MBR_EINVAL;
      
    __MbmLocalWrRegBits(regs->pRegDisc, ReqRegAddr - regs->reg_discrete_addr_start, (uint8_t *)&premain[1], ReqRegcnt);

    return MBR_ENOERR;
}

#endif

