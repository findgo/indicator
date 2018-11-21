#include "ug_hz.h"
#include "ugui.h"

static UG_GUI* gui;
static UG_FONT hzfont;
static uint32_t hzfont_maskofs;
static int (*g_readf)(unsigned long, int, void*);

#ifdef USE_HZFONT_11X11
const UG_FONT HZFONT_11X11 = {NULL, FONT_TYPE_1BPP, 11, 11, 0, 255, NULL};
const uint32_t HZFONT_11X11_maskofs = 768432; // 存储汉字 flash 的地址
#endif

#ifdef USE_HZFONT_12X12
const UG_FONT HZFONT_12X12 = {NULL, FONT_TYPE_1BPP, 12, 12, 0, 255, NULL};
const uint32_t HZFONT_12X12_maskofs = 524288; // 存储汉字 flash 的地址
#endif

#ifdef USE_HZFONT_16X16
const UG_FONT HZFONT_16X16 = {NULL, FONT_TYPE_1BPP, 16, 16, 0, 255, NULL};
const uint32_t HZFONT_16X16_maskofs = 0; // 存储汉字 flash 的地址
#endif

void UG_HZ_Init(UG_GUI* g, int (*readf)(unsigned long, int, void*))
{
    gui = g;
    g_readf = readf;
}

void UG_HZ_FontSelect(const UG_FONT* font, uint32_t maskofs)
{
    hzfont = *font;
    hzfont_maskofs = maskofs;
}

// TODO: 改变汉字编译在此,目前使用GB2312,最好修改成Unicode
void _UG_HZ_PutChar(unsigned long icode, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc, const UG_FONT* font)
{
    UG_U16 i, j, k, xo, yo, c, bn, actual_char_width;
    UG_U32 b, index, pos;
    unsigned char buf[32];

    yo = y;
    bn = font->char_width;
    if(!bn)
        return;
    bn >>= 3;
    if(font->char_width % 8)
        bn++;
    actual_char_width = font->char_width;
    pos = ((icode >> 8) - 0xa1UL) * 94UL + ((icode & 0xff) - 0xa1UL);

    if(font->char_height == 11) {
        if(pos > 831)
            pos += 240;
        pos *= (font->char_height * bn);
        pos += 4;
    }
    else {
        pos *= (font->char_height * bn);
    }
    //TODO: 这里是从flash读出来,目前需要改成从程序flash中读. 可以改动api功能
    g_readf(hzfont_maskofs + pos, font->char_height * bn, buf);

    index = 0;

    for(j = 0; j < font->char_height; j++) {
        xo = x;
        c = actual_char_width;
        for(i = 0; i < bn; i++) {
            b = buf[index++];
            for(k = 0; (k < 8) && c; k++) {
                if(b & 0x80) {
                    gui->pset(xo, yo, fc);
                }
                else {
                    gui->pset(xo, yo, bc);
                }
                b <<= 1;
                xo++;
                c--;
            }
        }
        yo++;
    }
}

void UG_HZ_PutChar(unsigned long icode, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc)
{
    _UG_HZ_PutChar(icode, x, y, fc, bc, &hzfont);
}

void UG_HZ_PutString(int x, int y, unsigned char *str)
{
    unsigned char chr;
    int xp, yp, cw;
    xp = x;
    yp = y;

    while(*str) {
        chr = *str;
        if(chr <= 127) {
            str++;
            if(chr < gui->font.start_char || chr > gui->font.end_char)
                continue;
            if(chr == '\n')
                continue;       // do not wrap
            cw = gui->font.char_width;      // 暂不考虑不等宽字体
            UG_PutChar(chr, xp, yp, gui->fore_color, gui->back_color);
            xp += cw + gui->char_h_space;
        }
        else {
            unsigned long icode;
            icode = (*str) * 256UL + *(str + 1);
            str += 2;
            cw = hzfont.char_width;
            UG_HZ_PutChar(icode, xp, yp, gui->fore_color, gui->back_color);
            xp += cw + gui->char_h_space;
        }
        if(xp > gui->x_dim - 1)
            break;              // 超长则截断
    }
}
