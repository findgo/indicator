
/* 主机模式 */
//模拟SPI
#include "hal_spi.h"


/* 初始化函数 */
void halsimspiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    HAL_SIMSPI_SCK_OUTPUT_MODE();
    HAL_SIMSPI_MISO_INPUT_MODE();
    HAL_SIMSPI_MOSI_OUTPUT_MODE();
    HAL_SIMSPI_CS_OUTPUT_MODE();

    HAL_SIMSPI_GPIO_CLOCK_EN();

    GPIO_InitStructure.GPIO_Pin = HAL_SIMSPI_SCK_PIN;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HAL_SIMSPI_SCK_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HAL_SIMSPI_MOSI_PIN;
    GPIO_Init(HAL_SIMSPI_MOSI_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = HAL_SIMSPI_CS_PIN;
    GPIO_Init(HAL_SIMSPI_CS_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HAL_SIMSPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(HAL_SIMSPI_MISO_PORT,&GPIO_InitStructure);

// for spi init
    HAL_SIMSPI_CS_ASSERT();    //选中
#if HAL_SIMSPI_MODE == 0 || HAL_SIMSPI_MODE == 1
    HAL_SIMSPI_SCK_LOW();        
#elif HAL_SIMSPI_MODE == 2 || HAL_SIMSPI_MODE == 3
    HAL_SIMSPI_SCK_HIGH();  
#endif
    HAL_SIMSPI_CS_DEASSERT();  //不选中
}


#if HAL_SIMSPI_MODE == 0
/* 模式0: sck常态为低,高位先发送 */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    // HAL_SIMSPI_SCK_LOW();
    for(i = 0; i < 8; i++) {
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();    //设置
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();  
        HAL_SIMSPI_SCK_HIGH();       //上升沿采样
        HAL_SIMSPI_TIME_CLOCK_WIDE();//MOSI发送，MISO接收

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_SCK_LOW(); //下降沿后可设置
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 1
/* 模式1: sck常态为低,高位先发送 */
uint32_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;

    for(i = 0; i < 8; i++) {
        HAL_SIMSPI_SCK_HIGH();        /* 起始沿 */
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();   /*等待适当的时间，以等待数据稳定*/   
        HAL_SIMSPI_SCK_LOW();       //下降沿采样，MOSI发送，MISO接收

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 2
/* 模式0: sck常态为高,高位先发送 */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    for(i = 0; i < 8; i++) {
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW(); //设置
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();  
        HAL_SIMSPI_SCK_LOW();    //下降沿采样，MOSI发送，MISO采亲
        HAL_SIMSPI_TIME_CLOCK_WIDE();

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_SCK_HIGH(); //上升沿可设置
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 3
/* 模式3: SCK常态为高.高位先发送 */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    for(i = 0; i < 8; i++) {
        HAL_SIMSPI_SCK_LOW();        /* 起始沿 */
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();//设置
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();   /*等待适当的时间，以等待数据稳定*/   
        HAL_SIMSPI_SCK_HIGH();        //上升沿采样，MOSI发送，MOSI接收

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}
#endif
