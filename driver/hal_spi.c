
void halSPI1_Init(uint16_t baud)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

    HAL_SPI1_GPIO_CLOCK_EN();	
    HAL_SPI1_ACTIVE_CLOCK_EN();

	//配置对应复用脚
	GPIO_PinAFConfig(HAL_SPI1_SCK_PORT,HAL_SPI1_SCK_PINGSOURCE, HAL_SPI1_SCK_AF_SEL);
	GPIO_PinAFConfig(HAL_SPI1_MOSI_PORT,HAL_SPI1_MOSI_PINGSOURCE, HAL_SPI1_MOSI_AF_SEL);
	GPIO_PinAFConfig(HAL_SPI1_MISO_PORT,HAL_SPI1_MISO_PINGSOURCE, HAL_SPI1_MISO_AF_SEL);
    
	//--- 配置SPI1的SCK,MISO MOSI 
    GPIO_InitStructure.GPIO_Pin = HAL_SPI1_SCK_PIN;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HAL_SPI1_SCK_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HAL_SPI1_MOSI_PIN;
    GPIO_Init(HAL_SPI1_MOSI_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HAL_SPI1_MISO_PIN;
    GPIO_Init(HAL_SPI1_MISO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HAL_SPI1_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(HAL_SPI1_CS_PORT,&GPIO_InitStructure);

    HAL_SPI1_CS_HIGH();
    
	//--- SPI1
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
#if HAL_SPI1_MODE == 0
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //稳态      :时钟悬空为低
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //位捕获    :位于第一个上升沿后
#elif HAL_SPI1_MODE == 1
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //稳态      :时钟悬空为低
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;     //位捕获    :位于第二个上升沿后
#elif HAL_SPI1_MODE == 2
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       //稳态      :时钟悬空为高
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //位捕获    :位于第一个下降沿
#elif HAL_SPI1_MODE == 3
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;         //稳态      :时钟悬空为高
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;        //位捕获    :位于第一个下降沿
#else
    #error "set SPI1 mode error,it must be (0 - 3)"
#endif

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = baud;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);	

    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	//--- 使能SPI1 
	SPI_Cmd(SPI1, ENABLE); 
}
uint8_t halSPI1_ReadWrite(uint8_t writedat)
{
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_SendData8(SPI1, writedat);
	
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_ReceiveData8(SPI1);
}

