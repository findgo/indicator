


#include "MAX31855.h"

/*SPI宏*/
#define MAX31855_SPI_TIME_CLOCK_WIDE()           //delay_us(1) //延时间隔200ns以上

#define MAX31855_SPI_SCK_OUTPUT_MODE()           /* 设置为output模式 */ 
#define MAX31855_SPI_SCK_LOW()                   GPIO_ResetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)              
#define MAX31855_SPI_SCK_HIGH()                  GPIO_SetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)               

#define MAX31855_SPI_MISO_INPUT_MODE()            /* 设置为input模式 */ 
#define MAX31855_SPI_MISO_READ                   GPIO_ReadInputDataBit(HAL_SIMSPI_MISO_PORT,HAL_SIMSPI_MISO_PIN)

#define MAX31855_SPI_CS_OUTPUT_MODE()            /* 设置为output模式 */ 
#define MAX31855_SPI_CS_DEASSERT()               GPIO_SetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)
#define MAX31855_SPI_CS_ASSERT()                 GPIO_ResetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)

#define MAX31855_SPI_BEGIN()        do{ HAL_SIMSPI_CS_ASSERT();}while(0)
#define MAX31855_SPI_END()          do{ HAL_SIMSPI_CS_DEASSERT();}while(0)



static uint32_t Max3288RawValue = 0;

/* 初始化函数 */
void Max31855Init(void)
{


// for spi init
    MAX31855_SPI_CS_ASSERT();    //选中
    MAX31855_SPI_SCK_LOW();      //   
    MAX31855_SPI_CS_DEASSERT();  //不选中
}


/* SPI模式1: sck常态为低,下降沿采样 */
static uint32_t Max31855ReadValue(void)
{
    uint32_t ret = 0;
    uint8_t i;

    MAX31855_SPI_BEGIN();    
    MAX31855_SPI_TIME_CLOCK_WIDE();   /*100ns*/ 
    for(i = 0; i < 32; i++) {
        MAX31855_SPI_SCK_HIGH();        /* 起始沿 */
        MAX31855_SPI_TIME_CLOCK_WIDE();   /*等待适当的时间，以等待数据稳定*/ 

        ret <<= 1;
        if (MAX31855_SPI_MISO_READ){
            ret |= 0x0001;
        }
        MAX31855_SPI_SCK_LOW();       //下降沿采样，MOSI发送，MISO接收  
        MAX31855_SPI_TIME_CLOCK_WIDE();
    }
    MAX31855_SPI_END();
    
    return ret;
}

void Max31855Update(void)
{
   Max3288RawValue =  Max31855ReadValue();
}


uint32_t Max31855RawValue(void)
{
    return Max3288RawValue;
}


/*放大4倍*/
int16_t Max31855ThermocoupleTemp(void)
{
    uint16_t temp = (uint16_t)(Max3288RawValue >> 18);
    
    if(temp&(0x0001 << 14)){
        temp |= 0x0003 << 14; //负值高位补0
    }
    
    return (int32_t)temp;
}

/*放大16倍*/
int16_t Max31855InternalTemp(void)
{
    uint16_t temp = (uint16_t)Max3288RawValue;

    temp = temp >> 4;

    if(temp&(0x0001 << 12)){
        temp |= 0x000f << 12; /*负值高位补0*/
    }
        
    return (int16_t)temp;
}

uint8_t Max31855Fault(void)
{
    uint32_t fault = Max3288RawValue;
    uint8_t ret;

    if(fault&(0x00000001 << 16)){
        ret = (uint8_t)(fault&0x07)| (1 << 3);
    }else{
        ret = (uint8_t)fault&0x07;
    }

    return ret;
}

