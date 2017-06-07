
/* ����ģʽ */
//ģ��SPI
#include "hal_spi.h"


/* ��ʼ������ */
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
    HAL_SIMSPI_CS_ASSERT();    //ѡ��
#if HAL_SIMSPI_MODE == 0 || HAL_SIMSPI_MODE == 1
    HAL_SIMSPI_SCK_LOW();        
#elif HAL_SIMSPI_MODE == 2 || HAL_SIMSPI_MODE == 3
    HAL_SIMSPI_SCK_HIGH();  
#endif
    HAL_SIMSPI_CS_DEASSERT();  //��ѡ��
}


#if HAL_SIMSPI_MODE == 0
/* ģʽ0: sck��̬Ϊ��,��λ�ȷ��� */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    // HAL_SIMSPI_SCK_LOW();
    for(i = 0; i < 8; i++) {
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();    //����
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();  
        HAL_SIMSPI_SCK_HIGH();       //�����ز���
        HAL_SIMSPI_TIME_CLOCK_WIDE();//MOSI���ͣ�MISO����

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_SCK_LOW(); //�½��غ������
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 1
/* ģʽ1: sck��̬Ϊ��,��λ�ȷ��� */
uint32_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;

    for(i = 0; i < 8; i++) {
        HAL_SIMSPI_SCK_HIGH();        /* ��ʼ�� */
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();   /*�ȴ��ʵ���ʱ�䣬�Եȴ������ȶ�*/   
        HAL_SIMSPI_SCK_LOW();       //�½��ز�����MOSI���ͣ�MISO����

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 2
/* ģʽ0: sck��̬Ϊ��,��λ�ȷ��� */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    for(i = 0; i < 8; i++) {
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW(); //����
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();  
        HAL_SIMSPI_SCK_LOW();    //�½��ز�����MOSI���ͣ�MISO����
        HAL_SIMSPI_TIME_CLOCK_WIDE();

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_SCK_HIGH(); //�����ؿ�����
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}


#elif HAL_SIMSPI_MODE == 3
/* ģʽ3: SCK��̬Ϊ��.��λ�ȷ��� */
uint8_t halsimSpi_transmit_byte(uint8_t dat)
{
    uint8_t ret = 0;
    uint8_t i;
    
    for(i = 0; i < 8; i++) {
        HAL_SIMSPI_SCK_LOW();        /* ��ʼ�� */
        if (dat & 0x80){
            HAL_SIMSPI_MOSI_HIGH();
        }else{
            HAL_SIMSPI_MOSI_LOW();//����
        }
        dat <<= 1;
        HAL_SIMSPI_TIME_CLOCK_WIDE();   /*�ȴ��ʵ���ʱ�䣬�Եȴ������ȶ�*/   
        HAL_SIMSPI_SCK_HIGH();        //�����ز�����MOSI���ͣ�MOSI����

        ret <<= 1;
        if (HAL_SIMSPI_MISO_READ){
            ret |= 0x01;
        }
        HAL_SIMSPI_TIME_CLOCK_WIDE();
    }

    return (ret);
}
#endif
