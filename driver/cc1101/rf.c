
#include "cc2500.h"
#include "myrf_setting.h"
#include "rf.h"



#define RF_FIFO_MAX_LEN		64
#define RF_APPEND_LEN		2

typedef enum
{
	RF_RX_STATE_IDLE,
	RF_RX_STATE_RX,
	RF_RX_STATE_PENDING,
	RF_RX_STATE_OK,
	RF_TX_STATE_TXING,
} rf_state_t;

typedef struct
{
  rf_state_t rf_state;
  uint8_t index;
  /* TXFIFO,RXFIFO: length+rf_buf[0~62] */
  uint8_t length;    /* 不包括长度本身 */
  uint8_t rf_buf[255];   /* addr+playload */
  uint8_t status[RF_APPEND_LEN];
}rf_data_t;




//*****************************************************************************************
void Rf_Init(void)
{
	halsimspiInit();
	halRfResetChip();
	halRfConfig(&rfSettings, PaTabel, 8);;
}

void rf_check(void)
{
	DBG_LOG("chip ver: 0x%02x\r\n",halRfGetChipVer());
	DBG_LOG("chip id: 0x%02x\r\n",halRfGetChipId());
}

//*****************************************************************************************
//函数名：void halRfSendPacket(uint8_t *txBuffer, uint8_t size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1100发送一组数据
//*****************************************************************************************
void RfSendPacket(uint8_t *txBuffer, uint8_t size) 
{
	halIntState_t bintstate;

	size = MIN(size, RF_FIFO_MAX_LEN);

	if(size == 0)
		return;
	
	ENTER_SAFE_ATOM_CODE(bintstate);
	halRfWriteReg(CC2500_TXFIFO, size);
    halRfWriteFifo(txBuffer, size);	//写入要发送的数据
    halSpiStrobe(CC2500_STX);		//进入发送模式发送数据
    //while (!HAL_CC1101_GDO2_READ);
    // Wait for GDO0 to be cleared -> end of packet
    //while (HAL_CC1101_GDO2_READ);
	//halSpiStrobe(CC2500_SFTX);
	
	EXIT_SAFE_ATOM_CODE(bintstate);
}

void RfSendComplete(void)
{
	halSpiStrobe(CC2500_SFTX);
}


void RfReceivePacket(uint8_t *rxBuffer,uint8_t length) 
{
    uint8_t status[2];
    uint8_t packetLength;

	if ((halRfReadStatusReg(CC2500_RXBYTES) & 0x7f)) //如果接的字节数不为0
	{
		packetLength = halRfReadReg(CC2500_RXFIFO);//读出第一个字节，此字节为该帧数据长度
		if(packetLength == 0 | packetLength > RF_FIFO_MAX_LEN){
			halRfStrobe(CC2500_SIDLE);
			halRfStrobe(CC2500_SFRX);
		}else{//如果所要的有效数据长度小于等于接收到的数据包的长度
			halRfReadFifo(rxBuffer, packetLength); //读出所有接收到的数据
			length = packetLength;				//把接收数据长度的修改为当前数据的长度

			// Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
			halRfReadFifo(status, RF_APPEND_LEN); 	//读出CRC校验位
			halSpiStrobe(CC2500_SFRX);		//清洗接收缓冲区
			//return (status[1] & 0x80);			//CRC_OK = 0x80如果校验成功返回接收成功
        }
    } else {
		length = packetLength;
		halSpiStrobe(CC2500_SFRX);		//清洗接收缓冲区
	}
}






