#ifndef _HZ_H
#define _HZ_H

#include "ugui.h"

#define USE_HZFONT_11X11
#define USE_HZFONT_12X12
#define USE_HZFONT_16X16

#ifdef USE_HZFONT_11X11
   extern const UG_FONT HZFONT_11X11;
   extern const uint32_t HZFONT_11X11_maskofs; // 存储汉字 flash 的地址

#endif

#ifdef USE_HZFONT_12X12
   extern const UG_FONT HZFONT_12X12;
   extern const uint32_t HZFONT_12X12_maskofs; // 存储汉字 flash 的地址

#endif

#ifdef USE_HZFONT_16X16
   extern const UG_FONT HZFONT_16X16;
   extern const uint32_t HZFONT_12X12_maskofs; // 存储汉字 flash 的地址
#endif

void UG_HZ_Init(UG_GUI* g, int (*read_f)(unsigned long, int, void*));
void UG_HZ_FontSelect(const UG_HZ_FONT* font);
void UG_HZ_PutString(int x, int y, unsigned char *str);

#endif
