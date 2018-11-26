
#include "drv12864.h"
#include "hal_spi.h"
#include "hal_simspi.h"
#include "delay.h"

#define HAL_drv12864_WIDE_TIME()  delay_hunns(1)

#define HAL_drv12864_RS_PORT    GPIOC
#define HAL_drv12864_RS_PIN     LL_GPIO_PIN_6

// 定义基本接口,根据需要修改
#define HAL_drv12864_RS_HIGH()          LL_GPIO_SetOutputPin(HAL_drv12864_RS_PORT, HAL_drv12864_RS_PIN) // 数据寄存器
#define HAL_drv12864_RS_LOW()           LL_GPIO_ResetOutputPin(HAL_drv12864_RS_PORT, HAL_drv12864_RS_PIN)// 指令寄存器
#define HAL_drv12864_CS_ASSERT()        HAL_SPI2_CS_ASSERT()
#define HAL_drv12864_CS_DEASSERT()      HAL_SPI2_CS_DEASSERT()
#define HAL_drv12864_RESET_HIGH()       LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7)
#define HAL_drv12864_RESET_LOW()        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7)
#define HAL_drv12864_SPI_transmit(dat)  halsimSpi_transmit_byte(dat) //halSPI2_ReadWrite(dat);

#define drv12864SendCmd(cmd)    drv12864Send(TRUE,cmd)
#define drv12864SendData(data)  drv12864Send(FALSE,data)

static void drv12864Send( uint8_t iscmd, uint8_t val);
void drv12864ClearScreen(void);

void drv12864Show(char *dp);

void drv12864Init(void)
{
    //halSPI2_Init();// see stm32CubeMX
    halsimspiInit();

    // 用引脚复位??   要使用硬件
    HAL_drv12864_RESET_HIGH();
    delay_ms(50);
    HAL_drv12864_RESET_LOW();
    delay_ms(1);    
    drv12864SendCmd(DRV12864_CMD_RESET);
    delay_ms(1);    
    drv12864SendCmd(DRV12864_CMD_BIAS_1_9); // 1/9 bias
    drv12864SendCmd(DRV12864_CMD_POWER_CTL | 0x07); // 0x20 - 0x2f
	drv12864SendCmd(DRV12864_CMD_INTERNAL_RESISTOR | 0x05); // 粗调对比度 0x20 - 0x27
    drv12864SendCmd(DRV12864_CMD_VOLTAGE_MODE);// 细调对比度
    drv12864SendCmd(0x10);                   // 细调对比度
    drv12864SendCmd(DRV12864_CMD_ROW_SCAN_NORMAL);// 行扫描: 从上到下
    drv12864SendCmd(DRV12864_CMD_COL_NORMAL);// 列扫描: 从左到右
    drv12864SendCmd(DRV12864_CMD_ON);   // 开显示

    drv12864ClearScreen();
}


static void drv12864Send( uint8_t iscmd, uint8_t val)
{
    HAL_drv12864_CS_ASSERT(); 
    (iscmd == TRUE) ? HAL_drv12864_RS_LOW() : HAL_drv12864_RS_HIGH();
    HAL_drv12864_WIDE_TIME();
    HAL_drv12864_SPI_transmit(val);
    HAL_drv12864_WIDE_TIME();
    HAL_drv12864_CS_DEASSERT();
}


//仅测试使用
void drv12864Show( char *dp)
{
    uint8_t i,j;

    for(i = 0; i < DRV12864_PAGE_ADDR_MAX; i++){
        drv12864SendCmd(DRV12864_CMD_PAGE_ADDR + i); // 页
        drv12864SendCmd(DRV12864_CMD_COL_HI_ADDR); // 列起始高四位 0x0000
        drv12864SendCmd(DRV12864_CMD_COL_LO_ADDR); // 列起始低四位
        
        HAL_drv12864_CS_ASSERT(); 
        HAL_drv12864_RS_HIGH();
        HAL_drv12864_WIDE_TIME();
        for(j = 0; j < DRV12864_COL_ADDR_MAX; j++){
            HAL_drv12864_SPI_transmit(*dp++);
            HAL_drv12864_WIDE_TIME();
        }
        HAL_drv12864_CS_DEASSERT();  
    }
}

// 开缓存 支持改点画图画字模
/* 排列方法,x像素128,一共128列 , y轴像素64,分为8页,每页8行, */
static uint8_t Drv12864Gram[DRV12864_COL_ADDR_MAX][DRV12864_PAGE_ADDR_MAX];


void drv12864ClearScreen(void)
{
    uint8_t i,j;

    memset(Drv12864Gram,0x00,sizeof(Drv12864Gram));
    for(i = 0; i < (DRV12864_PAGE_ADDR_MAX + 1); i++){
        drv12864SendCmd(DRV12864_CMD_PAGE_ADDR + i); // 页
        drv12864SendCmd(DRV12864_CMD_COL_HI_ADDR); // 列起始高四位 0x0000
        drv12864SendCmd(DRV12864_CMD_COL_LO_ADDR); // 列起始低四位
        
        HAL_drv12864_CS_ASSERT(); 
        HAL_drv12864_RS_HIGH();
        HAL_drv12864_WIDE_TIME();
        for(j = 0; j < (DRV12864_COL_ADDR_MAX + 4 ); j++){
            HAL_drv12864_SPI_transmit(0xaa);
            HAL_drv12864_WIDE_TIME();
        }        
        HAL_drv12864_CS_DEASSERT(); 
    }    
}

// 画点, PointColor 对于单色屏只有0和1, 1: 点亮
void drv12864SetPoint(UG_S16 x, UG_S16 y, UG_COLOR PointColor)
{
    uint8_t page, row;
    
    page = y >> 3;// y/8 处于第几页
    row = y & 0x7; // y%8 处于列中第几行

    drv12864SendCmd(DRV12864_CMD_PAGE_ADDR + page); // 页
    drv12864SendCmd(DRV12864_CMD_COL_HI_ADDR | HI_UINT8(x)); // 列起始高四位
    drv12864SendCmd(DRV12864_CMD_COL_LO_ADDR | LO_UINT8(x)); // 列起始低四位

    if(PointColor){
        Drv12864Gram[x][page] |= 1 << row;
    }
    else{
        Drv12864Gram[x][page] &= ~(  1 << row);
    }
    drv12864SendData(Drv12864Gram[x][page]);
}

// 设置点的位置
void drv12864SetPos(UG_S16 x, UG_S16 y)
{
    drv12864SendCmd(DRV12864_CMD_ROW_ADDR | x); // 行地址
    drv12864SendCmd(DRV12864_CMD_COL_HI_ADDR | HI_UINT8(x)); // 列起始高四位
    drv12864SendCmd(DRV12864_CMD_COL_LO_ADDR | LO_UINT8(x)); // 列起始低四位
}

//读点的值
UG_COLOR drv12864GetPoint(UG_S16 x, UG_S16 y)
{
    //drv12864SetPos(x,y);
    return (Drv12864Gram[x][y >> 3] & (1 << (y & 0x07)));
}

