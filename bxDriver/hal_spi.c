
#include "hal_spi.h"

void halSPI1_Init(void)
{
    // see stm32CubeMX
    LL_SPI_Enable(SPI1);
}

uint8_t halSPI1_ReadWrite(uint8_t writedat)
{
	/* Loop while DR register in not emplty */
	while(!LL_SPI_IsActiveFlag_TXE(SPI1));

	/* Send byte through the SPI1 peripheral */
    LL_SPI_TransmitData8(SPI1, writedat);
    
#if !defined(HAL_SPI1_THREE_LINE)	
	/* Wait to receive a byte */
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
	
	/* Return the byte read from the SPI bus */
	return LL_SPI_ReceiveData8(SPI1);
#else
    return 0;
#endif
}

void halSPI2_Init(void)
{
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    /**SPI2 GPIO Configuration  
    PB13   ------> SPI2_SCK
    PB15   ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = LCD_SCK_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(LCD_SCK_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LCD_MOSI_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(LCD_MOSI_GPIO_Port, &GPIO_InitStruct);
    
    SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI2, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_DisableNSSPulseMgt(SPI2);

    // see stm32CubeMX
    LL_SPI_SetRxFIFOThreshold(SPI2, LL_SPI_RX_FIFO_TH_QUARTER);
    LL_SPI_Enable(SPI2);
}
uint8_t halSPI2_ReadWrite(uint8_t writedat)
{
	/* Loop while DR register in not emplty */
	while(!LL_SPI_IsActiveFlag_TXE(SPI2));

	/* Send byte through the SPI1 peripheral */
    LL_SPI_TransmitData8((SPI2), writedat);
    
#if !defined(HAL_SPI2_THREE_LINE)	
	/* Wait to receive a byte */
	while(!LL_SPI_IsActiveFlag_RXNE((SPI2)));
	
	/* Return the byte read from the SPI bus */
	return LL_SPI_ReceiveData8((SPI2));
#else
    return 0;
#endif
}	

/************************************************** end of file ***********************************************************/

