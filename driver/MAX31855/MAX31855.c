


#include "MAX31855.h"

/*SPI��*/
#define MAX31855_SPI_TIME_CLOCK_WIDE()           //delay_us(1) //��ʱ���200ns����

#define MAX31855_SPI_SCK_OUTPUT_MODE()           /* ����Ϊoutputģʽ */ 
#define MAX31855_SPI_SCK_LOW()                   GPIO_ResetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)              
#define MAX31855_SPI_SCK_HIGH()                  GPIO_SetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)               

#define MAX31855_SPI_MISO_INPUT_MODE()            /* ����Ϊinputģʽ */ 
#define MAX31855_SPI_MISO_READ                   GPIO_ReadInputDataBit(HAL_SIMSPI_MISO_PORT,HAL_SIMSPI_MISO_PIN)

#define MAX31855_SPI_CS_OUTPUT_MODE()            /* ����Ϊoutputģʽ */ 
#define MAX31855_SPI_CS_DEASSERT()               GPIO_SetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)
#define MAX31855_SPI_CS_ASSERT()                 GPIO_ResetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)

#define MAX31855_SPI_BEGIN()        do{ HAL_SIMSPI_CS_ASSERT();}while(0)
#define MAX31855_SPI_END()          do{ HAL_SIMSPI_CS_DEASSERT();}while(0)



static uint32_t Max3288RawValue = 0;

/* ��ʼ������ */
void Max31855Init(void)
{


// for spi init
    MAX31855_SPI_CS_ASSERT();    //ѡ��
    MAX31855_SPI_SCK_LOW();      //   
    MAX31855_SPI_CS_DEASSERT();  //��ѡ��
}


/* SPIģʽ1: sck��̬Ϊ��,�½��ز��� */
static uint32_t Max31855ReadValue(void)
{
    uint32_t ret = 0;
    uint8_t i;

    MAX31855_SPI_BEGIN();    
    MAX31855_SPI_TIME_CLOCK_WIDE();   /*100ns*/ 
    for(i = 0; i < 32; i++) {
        MAX31855_SPI_SCK_HIGH();        /* ��ʼ�� */
        MAX31855_SPI_TIME_CLOCK_WIDE();   /*�ȴ��ʵ���ʱ�䣬�Եȴ������ȶ�*/ 

        ret <<= 1;
        if (MAX31855_SPI_MISO_READ){
            ret |= 0x0001;
        }
        MAX31855_SPI_SCK_LOW();       //�½��ز�����MOSI���ͣ�MISO����  
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


/*�Ŵ�4��*/
int16_t Max31855ThermocoupleTemp(void)
{
    uint16_t temp = (uint16_t)(Max3288RawValue >> 18);
    
    if(temp&(0x0001 << 14)){
        temp |= 0x0003 << 14; //��ֵ��λ��0
    }
    
    return (int32_t)temp;
}

/*�Ŵ�16��*/
int16_t Max31855InternalTemp(void)
{
    uint16_t temp = (uint16_t)Max3288RawValue;

    temp = temp >> 4;

    if(temp&(0x0001 << 12)){
        temp |= 0x000f << 12; /*��ֵ��λ��0*/
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

