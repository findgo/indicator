
#include "mbfunc.h"

#if MB_SLAVE_ENABLED > 0

/**
  * @brief  保持寄存器处理函数，保持寄存器可读，可读可写
  * @param  regs          操作寄存器指针
  * @param  pucRegBuffer  读操作时--返回数据指针，写操作时--输入数据指针
  * @param  usAddress     寄存器起始地址
  * @param  usNRegs       寄存器长度
  * @param  eMode         操作方式，读或者写
  * @return              错误状态
  */
static MbErrorCode_t __MbsRegHoldingCB(MbReg_t *regs, uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, MbRegisterMode_t eMode)
{
    int16_t iRegIndex;
  
    if( ((int16_t)usAddress >= regs->reg_holding_addr_start) \
        && ((usAddress + usNRegs) <= (regs->reg_holding_addr_start + regs->reg_holding_num))){
        
        //offset index
        iRegIndex = (int16_t)( usAddress - regs->reg_holding_addr_start);
        switch (eMode){
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                //high byte
                *pucRegBuffer++ = (uint8_t)( regs->pReghold[iRegIndex] >> 8 );
                //low byte
                *pucRegBuffer++ = (uint8_t)( regs->pReghold[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                regs->pReghold[iRegIndex] = *pucRegBuffer++ << 8;
                regs->pReghold[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }

        return MB_ENOERR;
    }
        
    return MB_ENOREG;
}
/**
  * @brief  输入寄存器处理函数，输入寄存器可读，但不可写。
  * @param  regs          操作寄存器指针
  * @param  pucRegBuffer  返回数据指针
  * @param  usAddress     寄存器起始地址
  * @param  usNRegs       寄存器长度
  * @return              错误状态
  */
static MbErrorCode_t __MbsRegInputCB(MbReg_t *regs, uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs)
{
    int16_t iRegIndex;
  
    if(((int16_t) usAddress >= regs->reg_input_addr_start ) \
        && (( usAddress + usNRegs) <= (regs->reg_input_addr_start + regs->reg_input_num)) ){
        
        //offset index
        iRegIndex = ( int16_t )( usAddress - regs->reg_input_addr_start);

        while( usNRegs > 0 )
        {
            //high byte
            *pucRegBuffer++ = ( uint8_t )( regs->pReginput[iRegIndex] >> 8 );
            //low byte
            *pucRegBuffer++ = ( uint8_t )( regs->pReginput[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
        return MB_ENOERR;
    }

    return MB_ENOREG;
}

/**
 * @brief   function handlers:  read holding register 
 * @param   regs - real slave register pointer
 * @param   pPdu - pdu frame pointer 
 * @param   usLen - usLen pdu frame length pointer
 * @return  exception code , see mbproto.h
 */
MbException_t MbsFuncRdHoldingRegister(MbReg_t *regs, uint8_t * pPdu, uint16_t * usLen )
{
    uint16_t usRegAddress;
    uint16_t usRegCount;
    uint8_t *pucFrameCur;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if(*usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN )){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF + 1] );

        usRegCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_REGCNT_OFF] << 8 );
        usRegCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_REGCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if((usRegCount >= MB_READREG_CNT_MIN) && (usRegCount <= MB_READREG_CNT_MAX)){
            
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pPdu[MB_PDU_FUNCODE_OFF];
            *usLen = MB_PDU_FUNCODE_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_HOLDING_REGISTER;
            *usLen += 1;

            /* Second byte in the response contain the number of bytes. */
            *pucFrameCur++ = ( uint8_t ) ( usRegCount * 2 );
            *usLen += 1;

            /* Make callback to fill the buffer. */
            eRegStatus = __MbsRegHoldingCB(regs,pucFrameCur, usRegAddress, usRegCount, MB_REG_READ );
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                *usLen += usRegCount * 2;
            }
        }
        else {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}
/**
 * @brief   function handlers:  write holding register 
 * @param   regs - real slave register pointer
 * @param   pPdu - pdu frame pointer 
 * @param   usLen - usLen pdu frame length pointer
 * @return  exception code , see mbproto.h
 */
MbException_t MbsFuncWrHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen )
{
    uint16_t usRegAddress;
    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if(*usLen == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN )){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );

        /* Make callback to update the value. */
        eRegStatus = __MbsRegHoldingCB(regs,&pPdu[MB_PDU_FUNC_WRITE_VALUE_OFF], usRegAddress, 1, MB_REG_WRITE );

        /* If an error occured convert it into a Modbus exception. */
        if( eRegStatus != MB_ENOERR ){
            eStatus = MbError2Exception( eRegStatus );
        }
    }
    else{
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}
/**
 * @brief   function handlers:  write multi holding register 
 * @param   regs - real slave register pointer
 * @param   pPdu - pdu frame pointer 
 * @param   usLen - usLen pdu frame length pointer
 * @return  exception code , see mbproto.h
 */
MbException_t MbsFuncWrMulHoldingRegister(MbReg_t *regs, uint8_t * pPdu, uint16_t * usLen )
{
    uint16_t usRegAddress;
    uint16_t usRegCount;
    uint8_t  ucRegByteCount;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if( *usLen >= ( MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN ) ){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );

        usRegCount = ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8 );
        usRegCount |= ( uint16_t )( pPdu[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1] );

        ucRegByteCount = pPdu[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];
        if( ( usRegCount >= MB_WRITEREG_CNT_MIN )   \
            && ( usRegCount <= MB_WRITEREG_CNT_MAX )  \
            && ( ucRegByteCount == ( uint8_t ) ( 2 * usRegCount ))){
            
            /* Make callback to update the register values. */
            eRegStatus =
                __MbsRegHoldingCB(regs, &pPdu[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], usRegAddress, usRegCount, MB_REG_WRITE );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                /* The response contains the function code, the starting
                 * address and the quantity of registers. We reuse the
                 * old values in the buffer because they are still valid.
                 */
                *usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}
/**
 * @brief   function handlers:  reand and write multi holding register 
 * @param   regs - real slave register pointer
 * @param   pPdu - pdu frame pointer 
 * @param   usLen - usLen pdu frame length pointer
 * @return  exception code , see mbproto.h
 */
MbException_t MbsFuncRdWrMulHoldingRegister(MbReg_t *regs, uint8_t *pPdu, uint16_t *usLen )
{
    uint16_t usRegReadAddress;
    uint16_t usRegReadCount;
    uint16_t usRegWriteAddress;
    uint16_t usRegWriteCount;
    uint8_t  ucRegWriteByteCount;
    uint8_t *pucFrameCur;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if( *usLen >= ( MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN ) ){
        
        usRegReadAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF] << 8U );
        usRegReadAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1] );

        usRegReadCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF] << 8U );
        usRegReadCount |= ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1] );

        usRegWriteAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF] << 8U );
        usRegWriteAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1] );

        usRegWriteCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF] << 8U );
        usRegWriteCount |= ( uint16_t )( pPdu[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1] );

        ucRegWriteByteCount = pPdu[MB_PDU_FUNC_READWRITE_BYTECNT_OFF];

        if( ( usRegReadCount >= MB_READWRITE_READREG_CNT_MIN ) \
            && ( usRegReadCount <= MB_READWRITE_READREG_CNT_MAX ) \
            && ( usRegWriteCount >= MB_READWRITE_WRITEREG_CNT_MIN ) \
            && ( usRegWriteCount <= MB_READWRITE_WRITEREG_CNT_MAX ) \
            && ( ( 2 * usRegWriteCount ) == ucRegWriteByteCount ) ){
            
            /* Make callback to update the register values. */
            eRegStatus = __MbsRegHoldingCB(regs, &pPdu[MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF], usRegWriteAddress, usRegWriteCount, MB_REG_WRITE );

            if( eRegStatus == MB_ENOERR ){
                
                /* Set the current PDU data pointer to the beginning. */
                pucFrameCur = &pPdu[MB_PDU_FUNCODE_OFF];
                *usLen = MB_PDU_FUNCODE_OFF;

                /* First byte contains the function code. */
                *pucFrameCur++ = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
                *usLen += 1;

                /* Second byte in the response contain the number of bytes. */
                *pucFrameCur++ = ( uint8_t ) ( usRegReadCount * 2 );
                *usLen += 1;

                /* Make the read callback. */
                eRegStatus = __MbsRegHoldingCB(regs, pucFrameCur, usRegReadAddress, usRegReadCount, MB_REG_READ );
                if( eRegStatus == MB_ENOERR ){
                    *usLen += 2 * usRegReadCount;
                }
            }
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    
    return eStatus;
}
/**
 * @brief   function handlers:  read input register 
 * @param   regs - real slave register pointer
 * @param   pPdu - pdu frame pointer 
 * @param   usLen - usLen pdu frame length pointer
 * @return  exception code , see mbproto.h
 */
MbException_t MbsFuncRdInputRegister(MbReg_t *regs, uint8_t * pPdu, uint16_t * usLen )
{
    uint16_t usRegAddress;
    uint16_t usRegCount;
    uint8_t *pucFrameCur;

    MbException_t eStatus = MB_EX_NONE;
    MbErrorCode_t eRegStatus;

    if(*usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN )){
        
        usRegAddress = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_ADDR_OFF + 1] );

        usRegCount = ( uint16_t )( pPdu[MB_PDU_FUNC_READ_REGCNT_OFF] << 8 );
        usRegCount |= ( uint16_t )( pPdu[MB_PDU_FUNC_READ_REGCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usRegCount >= MB_READREG_CNT_MIN ) && ( usRegCount < MB_READREG_CNT_MAX ) ){
            
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pPdu[MB_PDU_FUNCODE_OFF];
            *usLen = MB_PDU_FUNCODE_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_INPUT_REGISTER;
            *usLen += 1;

            /* Second byte in the response contain the number of bytes. */
            *pucFrameCur++ = ( uint8_t )( usRegCount * 2 );
            *usLen += 1;

            eRegStatus = __MbsRegInputCB(regs, pucFrameCur, usRegAddress, usRegCount );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR ){
                eStatus = MbError2Exception( eRegStatus );
            }
            else{
                *usLen += usRegCount * 2;
            }
        }
        else{
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else{
        /* Can't be a valid read input register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    
    return eStatus;
}

#endif


