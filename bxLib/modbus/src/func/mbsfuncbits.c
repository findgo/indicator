

#include "mbfunc.h"


#if MB_SLAVE_ENABLED > 0
/**
  * @brief  线圈寄存器处理函数，线圈寄存器可读，可读可写
  * @param  regs          操作寄存器指针
  * @param  pucRegBuffer  读操作---返回数据指针，写操作--返回数据指针
  * @param  usAddress     寄存器起始地址
  * @param  usNRegs       寄存器长度
  * @param  eMode         操作方式，读或者写
  * @return               错误状态
  */
static MbErrorCode_t __MbsRegCoilsCB(MbReg_t *regs, uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNCoils, MbRegisterMode_t eMode )
{
    int16_t iNCoils = ( int16_t )usNCoils;
    uint16_t usBitOffset;

    if( ((int16_t)usAddress >= regs->reg_coils_addr_start) && \
        ((usAddress + usNCoils) <= (regs->reg_coils_addr_start + regs->reg_coils_num))){

        usBitOffset = ( uint16_t )( usAddress - regs->reg_coils_addr_start );
        switch ( eMode ){
        case MB_REG_READ:
            while( iNCoils > 0 )
            {
                *pucRegBuffer++ = MbGetBits( regs->pRegCoil, usBitOffset,( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;

        case MB_REG_WRITE:
            while( iNCoils > 0 )
            {
                MbSetBits(regs->pRegCoil, usBitOffset,( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),*pucRegBuffer++);
                iNCoils -= 8;
                usBitOffset+=8;
            }
            break;
        }
        
        return MB_ENOERR;
    }
        
    return MB_ENOREG;
}
/**
  * @brief  离散输入寄存器处理函数，只可读
  * @param  regs          操作寄存器指针
  * @param  pucRegBuffer  返回数据指针
  * @param  usAddress     寄存器起始地址
  * @param  usNDiscrete       寄存器长度
  * @return               错误状态
  */
static MbErrorCode_t __MbsRegDiscreteCB(MbReg_t *regs, uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNDiscrete )
{
  int16_t iNDiscrete = (int16_t)usNDiscrete;
  uint16_t usBitOffset;

    if(((int16_t)usAddress >= regs->reg_discrete_addr_start) \
        && (usAddress + usNDiscrete <= regs->reg_discrete_addr_start + regs->reg_discrete_num)){

        usBitOffset = ( uint16_t )(usAddress - regs->reg_discrete_addr_start);
        while( iNDiscrete > 0 )
        {
            *pucRegBuffer++ = MbGetBits(regs->pRegDisc, usBitOffset,( uint8_t)(iNDiscrete > 8 ? 8 : iNDiscrete));
            iNDiscrete -= 8;
            usBitOffset += 8;
        }
        
        return MB_ENOERR;
    }

    return MB_ENOREG;
}
 /**
* @brief   function handlers:  read coils register 
* @param   regs - real slave register pointer
* @param   pPdu - pdu frame pointer 
* @param   usLen - usLen pdu frame length pointer
* @return  exception code , see mbproto.h
*/
MbException_t MbsFuncRdCoils(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen )
{
    uint16_t usRegAddress;
    uint16_t usCoilCount;
    uint8_t  ucNBytes;
    uint8_t  *pucFrameCur;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) ){
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF + 1] );

        usCoilCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_BITSCNT_OFF] << 8 );
        usCoilCount |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_BITSCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( (usCoilCount >= MB_READBITS_CNT_MIN) && (usCoilCount < MB_READBITS_CNT_MAX)){
            
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pPdu[MB_PDU_FUNCODE_OFF];
            *usLen = MB_PDU_FUNCODE_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_COILS;
            *usLen += 1;

            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            ucNBytes = usCoilCount / 8 + (((usCoilCount & 0x0007) > 0) ? 1 : 0);

            *pucFrameCur++ = ucNBytes;
            *usLen += 1;

            eRegStatus = __MbsRegCoilsCB(regs, pucFrameCur, usRegAddress, usCoilCount, MB_REG_READ );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                /* The response contains the function code, the starting address
                 * and the quantity of registers. We reuse the old values in the 
                 * buffer because they are still valid. */
                *usLen += ucNBytes;;
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid read coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}
 /**
* @brief   function handlers:  write coils register 
* @param   regs - real slave register pointer
* @param   pPdu - pdu frame pointer 
* @param   usLen - usLen pdu frame length pointer
* @return  exception code , see mbproto.h
*/
MbException_t MbsFuncWrCoil(MbReg_t *regs,uint8_t *pPdu, uint16_t * usLen)
{
    uint16_t usRegAddress;
    uint8_t  ucBuf[2];

    MbException_t    eStatus = MB_EX_NONE;
    MbErrorCode_t    eRegStatus;

    if( *usLen == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN ) ){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );

        if( ( pPdu[MB_PDU_FUNC_WRITE_VALUE_OFF + 1] == 0x00 ) \
            && ((pPdu[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF) \
            || (pPdu[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0x00)) ){
            
            ucBuf[1] = 0;
            if( pPdu[MB_PDU_FUNC_WRITE_VALUE_OFF] == 0xFF ){
                ucBuf[0] = 1;
            }
            else{
                ucBuf[0] = 0;
            }
            eRegStatus = __MbsRegCoilsCB(regs,&ucBuf[0], usRegAddress, 1, MB_REG_WRITE );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid write coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}
 /**
* @brief   function handlers:  write multi coils register 
* @param   regs - real slave register pointer
* @param   pPdu - pdu frame pointer 
* @param   usLen - usLen pdu frame length pointer
* @return  exception code , see mbproto.h
*/
MbException_t MbsFuncWrMulCoils(MbReg_t *regs,uint8_t * pPdu, uint16_t * usLen )
{
    uint16_t usRegAddress;
    uint16_t usCoilCnt;
    uint8_t  ucByteCount;
    uint8_t  ucByteCountVerify;

    MbException_t    eStatus = MB_EX_NONE;
    MbErrorCode_t    eRegStatus;

    if(*usLen > (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN )){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );

        usCoilCnt = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF] << 8 );
        usCoilCnt |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1] );

        ucByteCount = pPdu[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

        /* Compute the number of expected bytes in the request. */
        ucByteCountVerify = usCoilCnt / 8 + (((usCoilCnt & 0x0007) > 0) ? 1 : 0);
        
        if( (usCoilCnt >= MB_WRITEBITS_CNT_MIN ) \
            && (usCoilCnt <= MB_WRITEBITS_CNT_MAX ) \
             && (ucByteCountVerify == ucByteCount ) ){
             
            eRegStatus = __MbsRegCoilsCB(regs,&pPdu[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], usRegAddress, usCoilCnt, MB_REG_WRITE );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                /* The response contains the function code, the starting address
                 * and the quantity of registers. We reuse the old values in the 
                 * buffer because they are still valid. */
                *usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid write coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}
 /**
* @brief   function handlers:  read discrete imput register 
* @param   regs - real slave register pointer
* @param   pPdu - pdu frame pointer 
* @param   usLen - usLen pdu frame length pointer
* @return  exception code , see mbproto.h
*/
MbException_t MbsFuncRdDiscreteInputs(MbReg_t *regs, uint8_t * pPdu, uint16_t *usLen )
{
    uint16_t usRegAddress;
    uint16_t usDiscreteCnt;
    uint8_t  ucNBytes;
    uint8_t  *pucFrameCur;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if(*usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN )){

        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF + 1] );

        usDiscreteCnt = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_BITSCNT_OFF] << 8 );
        usDiscreteCnt |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_BITSCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( (usDiscreteCnt >= MB_READBITS_CNT_MIN) && (usDiscreteCnt < MB_READBITS_CNT_MAX ) ){
            
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pPdu[MB_PDU_FUNCODE_OFF];
            *usLen = MB_PDU_FUNCODE_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_DISCRETE_INPUTS;
            *usLen += 1;

            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            ucNBytes = usDiscreteCnt / 8 + (((usDiscreteCnt & 0x0007) > 0) ? 1 : 0);
            
            *pucFrameCur++ = ucNBytes;
            *usLen += 1;

            eRegStatus = __MbsRegDiscreteCB(regs, pucFrameCur, usRegAddress, usDiscreteCnt );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                /* The response contains the function code, the starting address
                 * and the quantity of registers. We reuse the old values in the 
                 * buffer because they are still valid. */
                *usLen += ucNBytes;;
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid read coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}

#endif


