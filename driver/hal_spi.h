
//spi1
#define HAL_SPI1_MODE        (3)
/*定义硬件IO*/
#define HAL_SPI1_SCK_PORT        GPIOA
#define HAL_SPI1_SCK_PIN         GPIO_Pin_5
#define HAL_SPI1_SCK_PINGSOURCE  GPIO_PinSource5
#define HAL_SPI1_SCK_AF_SEL      GPIO_AF_0
#define HAL_SPI1_MISO_PORT       GPIOA
#define HAL_SPI1_MISO_PIN        GPIO_Pin_6
#define HAL_SPI1_MISO_PINGSOURCE  GPIO_PinSource5
#define HAL_SPI1_MISO_AF_SEL      GPIO_AF_0
#define HAL_SPI1_MOSI_PORT        GPIOA
#define HAL_SPI1_MOSI_PIN         GPIO_Pin_7
#define HAL_SPI1_MOSI_PINGSOURCE  GPIO_PinSource5
#define HAL_SPI1_MOSI_AF_SEL      GPIO_AF_0

#define HAL_SPI1_CS_PORT         GPIOA
#define HAL_SPI1_CS_PIN          GPIO_Pin_4

#define HAL_SPI1_GPIO_CLOCK_EN()	do{RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);}while(0)
#define HAL_SPI1_ACTIVE_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 ,ENABLE );}while(0)

#define HAL_SPI1_CS_HIGH()  GPIO_SetBits(HAL_SPI1_CS_PORT,HAL_SPI1_CS_PIN)
#define HAL_SPI1_CS_LOW()   GPIO_ResetBits(HAL_SPI1_CS_PORT,HAL_SPI1_CS_PIN)

void halSPI1_Init(uint16_t baud);									// SPI1初始化
uint8_t halSPI1_ReadWrite(uint8_t writedat);			// SPI1读写一字节数据

