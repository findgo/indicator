
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
  uint8_t length;    /* ���������ȱ��� */
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
//��������void halRfSendPacket(uint8_t *txBuffer, uint8_t size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************
void RfSendPacket(uint8_t *txBuffer, uint8_t size) 
{
	halIntState_t bintstate;

	size = MIN(size, RF_FIFO_MAX_LEN);

	if(size == 0)
		return;
	
	ENTER_SAFE_ATOM_CODE(bintstate);
	halRfWriteReg(CC2500_TXFIFO, size);
    halRfWriteFifo(txBuffer, size);	//д��Ҫ���͵�����
    halSpiStrobe(CC2500_STX);		//���뷢��ģʽ��������
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

	if ((halRfReadStatusReg(CC2500_RXBYTES) & 0x7f)) //����ӵ��ֽ�����Ϊ0
	{
		packetLength = halRfReadReg(CC2500_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
		if(packetLength == 0 | packetLength > RF_FIFO_MAX_LEN){
			halRfStrobe(CC2500_SIDLE);
			halRfStrobe(CC2500_SFRX);
		}else{//�����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
			halRfReadFifo(rxBuffer, packetLength); //�������н��յ�������
			length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���

			// Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
			halRfReadFifo(status, RF_APPEND_LEN); 	//����CRCУ��λ
			halSpiStrobe(CC2500_SFRX);		//��ϴ���ջ�����
			//return (status[1] & 0x80);			//CRC_OK = 0x80���У��ɹ����ؽ��ճɹ�
        }
    } else {
		length = packetLength;
		halSpiStrobe(CC2500_SFRX);		//��ϴ���ջ�����
	}
}






