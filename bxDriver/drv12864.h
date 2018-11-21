#ifndef __DRV12864_H__
#define __DRV12864_H__

#include "common_type.h"
#include "ugui.h"


#define DRV12864_CMD_ON             0xaf
#define DRV12864_CMD_OFF            0xae
#define DRV12864_CMD_ROW_ADDR       0x40 // 低5位 行地址
#define DRV12864_CMD_PAGE_ADDR      0xb0 // 低4位 页地址,每8行为一页,64行分为8个页
#define DRV12864_CMD_COL_HI_ADDR    0x10 // 低4位 列地址高字节
#define DRV12864_CMD_COL_LO_ADDR    0x00 // 低4位 列地址低字节

// 行扫描顺序选择
#define DRV12864_CMD_ROW_SCAN_NORMAL    0xc0 // 普通顺序 , 从上到下
#define DRV12864_CMD_ROW_SCAN_REVERSE   0xc8 // 反向扫描顺序, 从下到上
// 显示列地址增减
#define DRV12864_CMD_COL_NORMAL         0xa1    // 列地址:从左到右
#define DRV12864_CMD_COL_REVERSE        0xa0   // 列地址: 从右到左
// 显示正显/反显
#define DRV12864_CMD_DISPLAY_NORMAL     0xa7    // 正显
#define DRV12864_CMD_DISPLAY_REVERSE    0xa6  // 反显
// 显示全部点阵
#define DRV12864_CMD_DISPLAY_POINT_NORMAL   0xa4    // 常规
#define DRV12864_CMD_DISPLAY_POINT_ALL      0xa5      // 显示全部点阵
// 显示偏压比
#define DRV12864_CMD_BIAS_1_9           0xa2    // 1/9 bias
#define DRV12864_CMD_BIAS_1_7           0xa3      // 1/7 bias
// Read-modify-write  (Column address increment)
#define DRV12864_CMD_RMW                0xe0      // at write: +1, at read: 0
// 退出Read-modify-write  (Column address increment exit)
#define DRV12864_CMD_EXIT_RMW           0xee
// reset
#define DRV12864_CMD_RESET              0xe2
// 电源控制 低3位电压操作模式
#define DRV12864_CMD_POWER_CTL          0x28
// (对比度) 选择内部电阻比例 0 - 7
#define DRV12864_CMD_INTERNAL_RESISTOR  0x20
// (对比度)微调内部设置液晶电压, 命令两令紧接使用,低6位值,供64级
#define DRV12864_CMD_VOLTAGE_MODE       0x81  
#define DRV12864_CMD_VOLTAGE_VALUE      0x00
// 空指令
#define DRV12864_CMD_NOP                0xe3

#define DRV12864_PAGE_ADDR_MAX   8  // 每8行一页.64行分为8页
#define DRV12864_ROW_ADDR_MAX    64  // 行数
#define DRV12864_COL_ADDR_MAX    128//  列数

#define DRV12864_POWER_CTL_MAX     8
#define DRV12864_VOLTAGE_VALUE_MAX    64 

void drv12864Init(void);

void drv12864SetPoint(UG_S16 x, UG_S16 y, UG_COLOR PointColor);
void drv12864SetPos(UG_S16 x, UG_S16 y);
UG_COLOR drv12864GetPoint(UG_S16 x, UG_S16 y);

#endif


