

#include "mbtcp.h"

#if MB_TCP_ENABLED > 0

/* ----------------------- Start implementation -----------------------------*/
#if MB_SLAVE_ENABLED > 0

MbErrorCode_t MbsTCPInit(uint16_t ucTCPPort)
{
    if( MbTCPPortInit( ucTCPPort ) == FALSE )
        return MB_EPORTERR;
    
    return MB_ENOERR;
}

void MbsTCPStart(  void *dev)
{

}

void MbsTCPStop(  void *dev)
{
    /* Make sure that no more clients are connected. */
    MbTCPPortDisable( );
}

MbErrorCode_t MbsTCPReceive(void *dev, uint8_t *pucRcvAddress, uint8_t **pPdu, uint16_t *pusLength )
{
    MbErrorCode_t    eStatus = MB_EIO;
    uint8_t          *pucMBTCPFrame;
    uint16_t          usLength;
    uint16_t          usPID;

    if( MbTCPPortGetRequest( &pucMBTCPFrame, &usLength ) != FALSE ) {
        
        usPID = pucMBTCPFrame[MB_TCP_ADU_PID_OFFSET] << 8U;
        usPID |= pucMBTCPFrame[MB_TCP_ADU_PID_OFFSET + 1];

        if( usPID == MB_TCP_PROTOCOL_ID ){
            
            *pPdu = &pucMBTCPFrame[MB_TCP_ADU_PDU_OFFSET];
            *pusLength = usLength - MB_TCP_ADU_PDU_OFFSET;
            eStatus = MB_ENOERR;

            /* Modbus TCP does not use any addresses. Fake the source address such
             * that the processing part deals with this frame.
             */
            *pucRcvAddress = MB_TCP_PSEUDO_ADDRESS;
        }
    }
    else{
        eStatus = MB_EIO;
    }
    
    return eStatus;
}

MbErrorCode_t MbsTCPSend(void *dev, uint8_t _unused, const uint8_t *pPdu, uint16_t usLength )
{
    uint8_t *pAdu = ( uint8_t * ) pPdu - MB_TCP_ADU_PDU_OFFSET;
    uint16_t usTCPAduLength = usLength + MB_TCP_ADU_PDU_OFFSET;

    /* The MBAP header is already initialized because the caller calls this
     * function with the buffer returned by the previous call. Therefore we 
     * only have to update the length in the header. Note that the length 
     * header includes the size of the Modbus PDU and the UID Byte. Therefore 
     * the length is usLength plus one.
     */
    pAdu[MB_TCP_ADU_LEN_OFFSET] = ( usLength + 1 ) >> 8U;
    pAdu[MB_TCP_ADU_LEN_OFFSET + 1] = ( usLength + 1 ) & 0xFF;
    if( MbTCPPortSendResponse( pAdu, usTCPAduLength ) == FALSE )
        return MB_EIO;
    
    return MB_ENOERR;
}

#endif

#if MB_MASTER_ENABLED > 0
MbErrorCode_t MbmTCPInit(uint16_t ucTCPPort)
{
}
void MbmTCPStart(void *dev)
{
}
void MbmTCPStop(void *dev)
{
}
void MbmTCPClose(void *dev)
{
}
MbErrorCode_t MbmTCPReceive(void *dev, uint8_t *pucRcvAddress, uint8_t **pPdu,uint16_t *pusLength)
{
}
MbErrorCode_t MbmTCPSend(void *pdev,const uint8_t *pAdu, uint16_t usLength)
{
}


#endif

#endif
