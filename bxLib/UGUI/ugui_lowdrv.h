

#ifndef __UGUI_LOWDRV_H_
#define __UGUI_LOWDRV_H_

#include "ugui.h"


void UG_LowSetPixel(UG_S16 x, UG_S16 y, UG_COLOR PointColor);
UG_COLOR UG_LowGetPixel(UG_S16 x, UG_S16 y);
UG_RESULT UG_LowDrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c);
UG_RESULT UG_LowFillFrame( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT UG_LowFillArea( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );


#endif

