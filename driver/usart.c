/**
  ******************************************************************************
  * @file    usart.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "usart.h"


/**
  * @brief	usart����
  * @param	None
  * @note	
  * @retval  None
  */
void Usart_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	USART_USING1_GPIO_PeriphClock_EN();
	USART_USING1_PeriphClock_EN();

	GPIO_PinAFConfig(USART_USING1_TX_PORT, USART_USING1_TX_AF_PINSOURCE, USART_USING1_TX_AF_SEL);
	GPIO_PinAFConfig(USART_USING1_RX_PORT, USART_USING1_RX_AF_PINSOURCE, USART_USING1_RX_AF_SEL);
	//PB6 ����ΪUSAR1 TX, PB7 ����ΪUSART1 RX
	GPIO_InitStruct.GPIO_Pin   = USART_USING1_TX_PIN;//TX AF mode
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;  //��һ��ȷ���ĵ�ƽ?
	GPIO_Init(USART_USING1_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = USART_USING1_RX_PIN;//RX AF mode
	GPIO_Init(USART_USING1_RX_PORT, &GPIO_InitStruct);


	NVIC_InitStruct.NVIC_IRQChannel = USART_USING1_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART_USING1, &USART_ClockInitStruct);

	USART_InitStruct.USART_BaudRate = 115200;//������
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//ʹ�ܷ��ͺͽ���
	USART_InitStruct.USART_HardwareFlowControl = DISABLE;
	USART_InitStruct.USART_Parity = USART_Parity_No;//����У��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// 8λ����λ
	USART_Init(USART_USING1, &USART_InitStruct);

	USART_ClearFlag(USART_USING1, USART_FLAG_TC);
	USART_ITConfig(USART_USING1, USART_IT_TXE,  DISABLE);
	USART_ITConfig(USART_USING1, USART_IT_TC, DISABLE);
	USART_ITConfig(USART_USING1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_USING1, ENABLE);//ʹ��USART1
}


/* �ض���fputc */ 
#ifdef DEBUG_MSG
int fputc(int ch, FILE *f)
{
//	(void)Serial_WriteByte(ch);

    /* e.g. write a character to the USART */
	//USART_SendData(USART_USING1,ch);
    /* Loop until the end of transmission */
	//while(USART_GetFlagStatus(USART_USING1, USART_FLAG_TC) == RESET);
	
    return ch;
}
#endif
