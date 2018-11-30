
#include "port.h"
#include "mbutils.h"
#include "mbascii.h"


#if MB_ASCII_ENABLED > 0 && MB_SLAVE_ENABLED > 0

MbErrorCode_t MbsASCIIInit(Mbshandle_t dev, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    (void)dev;
    
    ENTER_CRITICAL_SECTION();

    if( (MbPortSerialInit( ucPort, ulBaudRate, 7, eParity) != TRUE) \
            || (MbPortTimersInit(ucPort, MBS_ASCII_TIMEOUT_SEC * 20000UL) != TRUE) ){
        eStatus = MB_EPORTERR;
    }

    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void MbsASCIIStart(Mbshandle_t dev)
{
    ENTER_CRITICAL_SECTION();

    ((MbsDev_t *)dev)->sndrcvState = STATE_ASCII_RX_IDLE;
    MbPortSerialEnable(((MbsDev_t *)dev)->port, TRUE, FALSE);
    
    EXIT_CRITICAL_SECTION();
}

void MbsASCIIStop(Mbshandle_t dev)
{
    ENTER_CRITICAL_SECTION();
    
    MbPortSerialEnable(((MbsDev_t *)dev)->port, FALSE, FALSE);
    MbPortTimersDisable(((MbsDev_t *)dev)->port);
    
    EXIT_CRITICAL_SECTION();
}

void MbsASCIIClose(Mbshandle_t dev)
{

}

MbErrorCode_t MbsASCIIReceiveParse(Mbshandle_t dev,MbsAduFrame_t *aduFrame)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    MbsDev_t *pdev = (MbsDev_t *)dev;

    ENTER_CRITICAL_SECTION();

    /* Length and LRC check */
    if((pdev->rcvAduBufPos >= MB_ADU_ASCII_SIZE_MIN)
        && (MbLRC((uint8_t *)pdev->AduBuf, pdev->rcvAduBufPos) == 0)){
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        aduFrame->hdr.introute.slaveid = pdev->AduBuf[MB_SER_ADU_ADDR_OFFSET];
        
        aduFrame->FunctionCode = pdev->AduBuf[MB_SER_ADU_PDU_OFFSET + MB_PDU_FUNCODE_OFF];
        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        aduFrame->pduFrameLength = (uint16_t)(pdev->rcvAduBufPos - MB_SER_ADU_SIZE_ADDR - MB_SER_ADU_SIZE_LRC);

        /* Return the start of the Modbus PDU to the caller. */
        aduFrame->pPduFrame = (uint8_t *)&pdev->AduBuf[MB_SER_ADU_PDU_OFFSET];
    }
    else{
        eStatus = MB_EIO;
    }
    
    EXIT_CRITICAL_SECTION();
    
    return eStatus;
}

MbErrorCode_t MbsASCIISend(Mbshandle_t dev, uint8_t ucSlaveAddress, const uint8_t *pPdu, uint16_t usLength)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    uint8_t usLRC;
    uint8_t *pAdu;
    uint8_t ucByte;
    MbsDev_t *pdev = (MbsDev_t *)dev;
    
    ENTER_CRITICAL_SECTION(  );
    /* Check if the receiver is still in idle state. If not we where too
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if(pdev->sndrcvState == STATE_ASCII_RX_IDLE){
        
        /* First byte before the Modbus-PDU is the slave address. */
        pAdu = (uint8_t *)pPdu - 1;
        pdev->sndAduBufCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pAdu[MB_SER_ADU_ADDR_OFFSET] = ucSlaveAddress;
        pdev->sndAduBufCount += usLength;

        /* Calculate LRC checksum for Modbus-Serial-Line-PDU. */
        usLRC = MbLRC( (uint8_t *)pAdu, pdev->sndAduBufCount );
        pdev->AduBuf[pdev->sndAduBufCount++] = usLRC;

        /* Activate the transmitter. */
        pdev->sndrcvState = STATE_ASCII_TX_START;

        /* start the first transmitter then into serial tc interrupt */
        ucByte = ':';
        MbPortSerialPutByte(pdev->port,(char)ucByte);
        pdev->sndAduBufPos = 0;
        pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
        pdev->sndrcvState = STATE_ASCII_TX_DATA;
      
        MbPortSerialEnable(pdev->port, FALSE, TRUE );
    }
    else{
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION();
    
    return eStatus;
}

void MbsASCIIReceiveFSM( Mbshandle_t dev)
{
    uint8_t ucByte;
    uint8_t ucResult;
    MbsDev_t *pdev = (MbsDev_t *)dev;

    (void)MbPortSerialGetByte(pdev->port, (char *)&ucByte );
    switch (pdev->sndrcvState){
        /* A new character is received. If the character is a ':' the input
         * buffer is cleared. A CR-character signals the end of the data
         * block. Other characters are part of the data block and their
         * ASCII value is converted back to a binary representation.
         */
    case STATE_ASCII_RX_RCV:
        /* Enable timer for character timeout. */
        MbPortTimersEnable(pdev->port);
        if( ucByte == ':' ){
            /* Empty receive buffer. */
            pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            pdev->rcvAduBufPos = 0;
        }
        else if( ucByte == MB_ASCII_DEFAULT_CR ){
            pdev->sndrcvState = STATE_ASCII_RX_WAIT_EOF;
        }
        else{
            ucResult = MbChar2Bin( ucByte );
            switch (pdev->AsciiBytePos){
                /* High nibble of the byte comes first. We check for
                 * a buffer overflow here. */
            case BYTE_HIGH_NIBBLE:
                if( pdev->rcvAduBufPos < MB_ADU_SIZE_MAX ){
                    pdev->AduBuf[pdev->rcvAduBufPos] = (uint8_t)(ucResult << 4);
                    pdev->AsciiBytePos = BYTE_LOW_NIBBLE;
                    break;
                }
                else{
                    /* not handled in Modbus specification but seems
                     * a resonable implementation. */
                    pdev->sndrcvState = STATE_ASCII_RX_IDLE;
                    /* Disable previously activated timer because of error state. */
                    MbPortTimersDisable(pdev->port);
                }
                break;

            case BYTE_LOW_NIBBLE:
                pdev->AduBuf[pdev->rcvAduBufPos] |= ucResult;
                pdev->rcvAduBufPos++;
                pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        break;

    case STATE_ASCII_RX_WAIT_EOF:
        if( ucByte == MB_ASCII_DEFAULT_LF ){
            /* Disable character timeout timer because all characters are
             * received. */
            MbPortTimersDisable(pdev->port);
            /* Receiver is again in idle state. */
            pdev->sndrcvState = STATE_ASCII_RX_IDLE;

            /* Notify the caller of MbsASCIIReceive that a new frame was received. */
            MbsSemGive(pdev);
        }
        else if( ucByte == ':' ){
            /* Empty receive buffer and back to receive state. */
            pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            pdev->rcvAduBufPos = 0;
            pdev->sndrcvState = STATE_ASCII_RX_RCV;

            /* Enable timer for character timeout. */
            MbPortTimersEnable(pdev->port);
        }
        else{
            /* Frame is not okay. Delete entire frame. */
            pdev->sndrcvState = STATE_ASCII_RX_IDLE;
        }
        break;

    case STATE_ASCII_RX_IDLE:
        if( ucByte == ':' ){
            /* Enable timer for character timeout. */
            MbPortTimersEnable(pdev->port);
            /* Reset the input buffers to store the frame. */
            pdev->rcvAduBufPos = 0;
            pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
            pdev->sndrcvState = STATE_ASCII_RX_RCV;
        }
        break;
    }
}

void MbsASCIITransmitFSM(  Mbshandle_t dev)
{
    uint8_t ucByte;
    MbsDev_t *pdev = (MbsDev_t *)dev;
    
    switch(pdev->sndrcvState){
        /* Start of transmission. The start of a frame is defined by sending
         * the character ':'. */
    case STATE_ASCII_TX_START:
        ucByte = ':';
        MbPortSerialPutByte(pdev->port, (char)ucByte );
        pdev->sndAduBufPos = 0;
        pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
        pdev->sndrcvState = STATE_ASCII_TX_DATA;
        break;

        /* Send the data block. Each data byte is encoded as a character hex
         * stream with the high nibble sent first and the low nibble sent
         * last. If all data bytes are exhausted we send a '\r' character
         * to end the transmission. */
    case STATE_ASCII_TX_DATA:
        if( pdev->sndAduBufCount > 0 ){
            switch(pdev->AsciiBytePos){
            case BYTE_HIGH_NIBBLE:
                ucByte = MbBin2Char((uint8_t)(pdev->AduBuf[pdev->sndAduBufPos] >> 4));
                MbPortSerialPutByte(pdev->port, (char)ucByte);
                pdev->AsciiBytePos = BYTE_LOW_NIBBLE;
                break;

            case BYTE_LOW_NIBBLE:
                ucByte = MbBin2Char((uint8_t)(pdev->AduBuf[pdev->sndAduBufPos] & 0x0F));
                MbPortSerialPutByte(pdev->port, (char)ucByte);
                pdev->sndAduBufPos++;
                pdev->sndAduBufCount--;                
                pdev->AsciiBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        else{
            MbPortSerialPutByte(pdev->port, (char)MB_ASCII_DEFAULT_CR);
            pdev->sndrcvState = STATE_ASCII_TX_END;
        }
        break;

        /* Finish the frame by sending a LF character. */
    case STATE_ASCII_TX_END:
        MbPortSerialPutByte(pdev->port, (char)MB_ASCII_DEFAULT_LF);
        /* We need another state to make sure that the CR character has
         * been sent. */
        pdev->sndrcvState = STATE_ASCII_TX_NOTIFY;
        break;

        /* Notify the task which called MbsASCIISend that the frame has
         * been sent. */
    case STATE_ASCII_TX_NOTIFY:
        /* Disable transmitter. This prevents another transmit buffer
         * empty interrupt. */
        MbPortSerialEnable(pdev->port, TRUE, FALSE);
        pdev->sndrcvState = STATE_ASCII_RX_IDLE;
        break;
    }
}

void MbsASCIITimerT1SExpired(  Mbshandle_t dev)
{
    MbsDev_t *pdev = (MbsDev_t *)dev;

    /* If we have a timeout we go back to the idle state and wait for
     * the next frame.
     */
    if((pdev->sndrcvState == STATE_ASCII_RX_RCV) || (pdev->sndrcvState == STATE_ASCII_RX_WAIT_EOF)){
        pdev->sndrcvState = STATE_ASCII_RX_IDLE;
    }
    MbPortTimersDisable(pdev->port);
}
#endif
