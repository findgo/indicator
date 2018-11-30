
#ifndef __MB_PROTO_H
#define __MB_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
/*brief Use the default Modbus TCP port (502)*/
#define MB_TCP_PORT_USE_DEFAULT	(502)   

#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
/* 
 * Modbus TCP does not use any addresses. Fake the source address such 
 * that the processing part deals with this frame. 
 */
#define MB_TCP_PSEUDO_ADDRESS   ( 255 )
/* modbus tcp MBAP TID */
#define MB_TCP_PROTOCOL_ID  	 ( 0 )   /* 0 = Modbus Protocol */

/* modbus function code */
#define MB_FUNC_MIN                           ( 1 )
#define MB_FUNC_MAX                           ( 127 )

#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )

// proto coils disc holding input limit
#define MB_READBITS_CNT_MIN             ( 0x0001 )
#define MB_READBITS_CNT_MAX             ( 0x07D0 )
#define MB_WRITEBITS_CNT_MIN            ( 0x0001 )
#define MB_WRITEBITS_CNT_MAX            ( 0x07B0 )
#define MB_READREG_CNT_MIN              ( 0x0001 )
#define MB_READREG_CNT_MAX              ( 0x007D )
#define MB_WRITEREG_CNT_MIN             ( 0x0001 )
#define MB_WRITEREG_CNT_MAX             ( 0x007B )
#define MB_READWRITE_READREG_CNT_MIN    ( 0x0001 )
#define MB_READWRITE_READREG_CNT_MAX    ( 0x007D )
#define MB_READWRITE_WRITEREG_CNT_MIN   ( 0x0001 )
#define MB_READWRITE_WRITEREG_CNT_MAX   ( 0x0079 )

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    MB_EX_NONE 					= 0x00,
    MB_EX_ILLEGAL_FUNCTION 		= 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS 	= 0x02,
    MB_EX_ILLEGAL_DATA_VALUE 	= 0x03,
    MB_EX_SLAVE_DEVICE_FAILURE  = 0x04,
    MB_EX_ACKNOWLEDGE 			= 0x05,
    MB_EX_SLAVE_BUSY 			= 0x06,
    MB_EX_MEMORY_PARITY_ERROR 	= 0x08,
    MB_EX_GATEWAY_PATH_FAILED 	= 0x0A,
    MB_EX_GATEWAY_TGT_FAILED 	= 0x0B
} MbException_t;

/*************************modbus fram defined************************************/

/*!
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 * <------------------------ MODBUS SERIAL LINE ADU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_ADU_SIZE_MAX = 256
 * (2)  ... MB_SER_ADU_ADDR_OFFSET = 0
 * (3)  ... MB_SER_ADU_PDU_OFFSET  = 1
 * (4)  ... MB_SER_ADU_SIZE_CRC = 2
 *      ... MB_SER_ADU_SIZE_LRC = 1
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFFSET     = 0
 * (3') ... MB_PDU_DATA_OFFSET     = 1
 * </code>
 */
 
 /*!
 * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *                              <----------- MODBUS PDU (1') -------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  | Function Code  | Data                    |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |                                           
 * (2)   (3)   (4)      (5)    (6)                                          
 *
 * (2)  ... MB_TCP_ADU_TID_OFFSET          = 0 (Transaction Identifier - 2 Byte) 
 * (3)  ... MB_TCP_ADU_PID_OFFSET          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_ADU_LEN_OFFSET          = 4 (Number of bytes - 2 Byte)  ( UID + PDU length )
 * (5)  ... MB_TCP_ADU_UID_OFFSET          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_ADU_PDU_OFFSET          = 7 (Modbus PDU )
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit
 */

/* RS232 / RS485 ADU -- TCP MODBUS ADU */
#define MB_ADU_SIZE_MAX           256
#define MB_ADU_ASCII_SIZE_MIN     3       /*!< Minimum size of a Modbus ASCII frame. */
#define MB_ADU_RTU_SIZE_MIN       4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_ADU_TCP_SIZE_MIN       8       /*!< Minimum size of a Modbus TCP frame. */

/* MODBUS SERIAL RTU/ASCII Defines*/
#define MB_SER_ADU_SIZE_ADDR   1  /*!< Size of ADDRESS field in ADU. */
#define MB_SER_ADU_SIZE_CRC    2  /*!< Size of CRC field in ADU. */
#define MB_SER_ADU_SIZE_LRC    1  /*!< Size of CRC field in ADU. */
#define MB_SER_ADU_ADDR_OFFSET 0  /*!< Offset of slave address in Ser-ADU. */
#define MB_SER_ADU_PDU_OFFSET  1  /*!< Offset of Modbus-PDU in Ser-ADU. */

/* MODBUS TCP  ADU Defines*/
#define MB_TCP_ADU_SIZE_MBAP           7   /*!< Size of MBAP header field in ADU. */
#define MB_TCP_ADU_TID_OFFSET          0
#define MB_TCP_ADU_PID_OFFSET          2
#define MB_TCP_ADU_LEN_OFFSET          4   
#define MB_TCP_ADU_UID_OFFSET          6
#define MB_TCP_ADU_PDU_OFFSET          7

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_SIZE_MAX         253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN         1   /*!< Function Code */
#define MB_PDU_SIZE_FUNCODE     1   /*!< Size of Function Code in PDU */
#define MB_PDU_FUNCODE_OFF      0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF         1   /*!< Offset for response data in PDU. */

#ifdef __cplusplus
}
#endif

#endif

