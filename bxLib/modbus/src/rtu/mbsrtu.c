
#include "port.h"
#include "mbrtu.h"
#include "mbutils.h"

#if MB_RTU_ENABLED > 0 &&  MB_SLAVE_ENABLED > 0
MbErrorCode_t MbsRTUInit(Mbshandle_t dev, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    uint32_t usTimerT35_50us;
    
    (void)dev;
    
    ENTER_CRITICAL_SECTION();
    /* Modbus RTU uses 8 Databits. */
    if(MbPortSerialInit(ucPort, ulBaudRate, 8, eParity) != TRUE){
        eStatus = MB_EPORTERR;
    }
    else{
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if(ulBaudRate > 19200){
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else{
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = (7UL * 220000UL) / (2UL * ulBaudRate);
        }
        if(MbPortTimersInit(ucPort, ( uint16_t )usTimerT35_50us) != TRUE){
            eStatus = MB_EPORTERR;
        }
    }
    
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void MbsRTUStart(Mbshandle_t dev)
{
    ENTER_CRITICAL_SECTION();
    
    ((MbsDev_t *)dev)->sndrcvState = STATE_RTU_RX_IDLE;
    MbPortSerialEnable(((MbsDev_t *)dev)->port, TRUE, FALSE);
    MbPortTimersDisable(((MbsDev_t *)dev)->port);

    EXIT_CRITICAL_SECTION();
}

void MbsRTUStop(Mbshandle_t dev)
{
    ENTER_CRITICAL_SECTION();
    MbPortSerialEnable(((MbsDev_t *)dev)->port, FALSE, FALSE );
    MbPortTimersDisable(((MbsDev_t *)dev)->port);
    EXIT_CRITICAL_SECTION();
}

void MbsRTUClose(Mbshandle_t dev)
{

}

MbErrorCode_t MbsRTUReceiveParse(Mbshandle_t dev, MbsAduFrame_t *aduFrame)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    MbsDev_t *pdev = (MbsDev_t *)dev;

    ENTER_CRITICAL_SECTION();
    /* Length and CRC check */
    if((pdev->rcvAduBufPos >= MB_ADU_RTU_SIZE_MIN)
        && (MbCRC16( (uint8_t *)pdev->AduBuf, pdev->rcvAduBufPos) == 0)){
        
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        aduFrame->hdr.introute.slaveid = pdev->AduBuf[MB_SER_ADU_ADDR_OFFSET];
        aduFrame->FunctionCode = pdev->AduBuf[MB_SER_ADU_PDU_OFFSET + MB_PDU_FUNCODE_OFF];
        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        aduFrame->pduFrameLength = (uint16_t)(pdev->rcvAduBufPos - MB_SER_ADU_SIZE_ADDR - MB_SER_ADU_SIZE_CRC);

        /* Return the start of the Modbus PDU to the caller. */
        aduFrame->pPduFrame = (uint8_t *) & pdev->AduBuf[MB_SER_ADU_PDU_OFFSET];
    }
    else{
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION();
    
    return eStatus;
}

MbErrorCode_t MbsRTUSend(Mbshandle_t dev,uint8_t ucSlaveAddress, const uint8_t *pPdu, uint16_t usLength)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    uint16_t usCRC16;
    uint8_t *pAdu;
    MbsDev_t *pdev = (MbsDev_t *)dev;
    
    ENTER_CRITICAL_SECTION();
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if(pdev->sndrcvState == STATE_RTU_RX_IDLE){
        /* First byte before the Modbus-PDU is the slave address. */
        pAdu = (uint8_t *) pPdu - 1;
        pdev->sndAduBufCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pAdu[MB_SER_ADU_ADDR_OFFSET] = ucSlaveAddress;
        pdev->sndAduBufCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = MbCRC16((uint8_t *)pAdu, pdev->sndAduBufCount);
        pdev->AduBuf[pdev->sndAduBufCount++] = ( uint8_t )(usCRC16 & 0xFF);
        pdev->AduBuf[pdev->sndAduBufCount++] = ( uint8_t )(usCRC16 >> 8);

        /* Activate the transmitter. */
		pdev->sndrcvState = STATE_RTU_TX_XMIT;
        
		/* start the first transmitter then into serial tc interrupt */
        MbPortSerialPutByte(pdev->port, pAdu[0]);
        pdev->sndAduBufPos = 1;  /* next byte in sendbuffer. */
        pdev->sndAduBufCount--;
     	
        MbPortSerialEnable(pdev->port, FALSE, TRUE);
    }
    else{
        eStatus = MB_EIO;
    }  
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void MbsRTUReceiveFSM(  Mbshandle_t dev)
{
    uint8_t ucByte;
    MbsDev_t *pdev = (MbsDev_t *)dev;

    /* Always read the character. */
    ( void )MbPortSerialGetByte(pdev->port, (char *)&ucByte);

    /* In the idle state we wait for a new character. If a character
     * is received the t1.5 and t3.5 timers are started and the
     * receiver is in the state STATE_RX_RECEIVCE.
     */
    if(pdev->sndrcvState == STATE_RTU_RX_IDLE){
        pdev->rcvAduBufPos = 0;
        pdev->AduBuf[pdev->rcvAduBufPos++] = ucByte;
        pdev->sndrcvState = STATE_RTU_RX_RCV;

    }
    else if(pdev->sndrcvState == STATE_RTU_RX_RCV){
        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
        if(pdev->rcvAduBufPos < MB_ADU_SIZE_MAX){
            pdev->AduBuf[pdev->rcvAduBufPos++] = ucByte;
        }
        else{
            /* In the error state we wait until all characters in the
             * damaged frame are transmitted.
             */
        }
    }
    
    /* Enable t3.5 timers. */
    MbPortTimersEnable(pdev->port);
}

void MbsRTUTransmitFSM(Mbshandle_t dev)
{
    MbsDev_t *pdev = (MbsDev_t *)dev;
    
    /* We should get a transmitter event in transmitter state.  */
    if(pdev->sndrcvState == STATE_RTU_TX_XMIT){
        /* check if we are finished. */
        if( pdev->sndAduBufCount != 0 ){
            MbPortSerialPutByte(pdev->port, (char)pdev->AduBuf[pdev->sndAduBufPos]);
            pdev->sndAduBufPos++;  /* next byte in sendbuffer. */
            pdev->sndAduBufCount--;
        }
        else{
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            MbPortSerialEnable(pdev->port, TRUE, FALSE);
            pdev->sndrcvState = STATE_RTU_RX_IDLE;
        }
    }
    else {
        /* enable receiver/disable transmitter. */
        MbPortSerialEnable(pdev->port, TRUE, FALSE);
    }
}

void MbsRTUTimerT35Expired  (Mbshandle_t         dev)
{
    MbsDev_t *pdev = (MbsDev_t *)dev;

    /* A frame was received and t35 expired. Notify the listener that
     * a new frame was received. */
    if(pdev->sndrcvState == STATE_RTU_RX_RCV)
        MbsSemGive(pdev);

    MbPortTimersDisable(pdev->port);
    pdev->sndrcvState = STATE_RTU_RX_IDLE;
}

#endif

