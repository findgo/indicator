#include "ugui_lowdrv.h"
#include "drv12864.h"

#if   (!UGUI_MIRROR_X && !UGUI_MIRROR_Y && !UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (x)
#define LOG2PHYS_Y(x, y)    (y)
#elif (!UGUI_MIRROR_X && !UGUI_MIRROR_Y &&  UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (y)
#define LOG2PHYS_Y(x, y)    (x)
#elif (!UGUI_MIRROR_X &&  UGUI_MIRROR_Y && !UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (x)
#define LOG2PHYS_Y(x, y)    (UGUI_PHY_PIXEL_Y - 1 - (y))
#elif (!UGUI_MIRROR_X &&  UGUI_MIRROR_Y &&  UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (y)
#define LOG2PHYS_Y(x, y)    (UGUI_PHY_PIXEL_X - 1 - (x))
#elif ( UGUI_MIRROR_X && !UGUI_MIRROR_Y && !UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (UGUI_PHY_PIXEL_X - 1 - (x))
#define LOG2PHYS_Y(x, y)    (y)
#elif ( UGUI_MIRROR_X && !UGUI_MIRROR_Y &&  UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (UGUI_PHY_PIXEL_Y - 1 - (y))
#define LOG2PHYS_Y(x, y)    (x)
#elif ( UGUI_MIRROR_X &&  UGUI_MIRROR_Y && !UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (UGUI_PHY_PIXEL_X - 1 - (x))
#define LOG2PHYS_Y(x, y)    (UGUI_PHY_PIXEL_Y - 1 - (y))
#elif ( UGUI_MIRROR_X &&  UGUI_MIRROR_Y &&  UGUI_SWAP_XY) 
#define LOG2PHYS_X(x, y)    (UGUI_PHY_PIXEL_Y - 1 - (y))
#define LOG2PHYS_Y(x, y)    (UGUI_PHY_PIXEL_X - 1 - (x))
#endif


void UG_LowSetPixel(UG_S16 x, UG_S16 y, UG_COLOR PointColor)
{
#if UGUI_SWAP_XY | UGUI_MIRROR_X| UGUI_MIRROR_Y
    UG_S16 xPhys = LOG2PHYS_X(x, y);
    UG_S16 yPhys = LOG2PHYS_Y(x, y);
#else
    #define xPhys x
    #define yPhys y
#endif
    /* Write into hardware ... Adapt to your system */
    {
        drv12864SetPoint(xPhys, yPhys, PointColor);
    }
}


UG_COLOR UG_LowGetPixel(UG_S16 x, UG_S16 y)
{
    UG_COLOR PointColor = 0;

    /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
#if UGUI_SWAP_XY | UGUI_MIRROR_X| UGUI_MIRROR_Y
    UG_S16 xPhys = LOG2PHYS_X(x, y);
    UG_S16 yPhys = LOG2PHYS_Y(x, y);
#else
    #define xPhys x
    #define yPhys y
#endif
    /* Read from hardware ... Adapt to your system */
    {
        PointColor = drv12864GetPoint(xPhys, yPhys);
    }
    return PointColor;
}

UG_RESULT UG_LowDrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c ) 
{
    return UG_RESULT_OK;
}

UG_RESULT UG_LowFillFrame( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c ) 
{
    return UG_RESULT_OK;
}

UG_RESULT UG_LowFillArea( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c ) 
{
    return UG_RESULT_OK;
}

