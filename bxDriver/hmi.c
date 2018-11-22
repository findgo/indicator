


#include "hmi.h"
#include "drv12864.h"

#define MENU_WIN_OBJECTS   5

static UG_GUI mygui;
static UG_WINDOW menuWin;
static UG_OBJECT menuObjTab[MENU_WIN_OBJECTS];
static UG_TEXTBOX txb1;

void hmiInit(void)
{
    drv12864Init();
    UG_Init(&mygui, UG_LowSetPixel, UGUI_DISPLAY_PIXEL_X, UGUI_DISPLAY_PIXEL_Y);
    UG_SelectGUI(&mygui);
    UG_FontSelect(&FONT_8X12); // 最优显示
    UG_FontSetHSpace(0);
    UG_FontSetVSpace(0);
    
    

    UG_WindowCreate(&menuWin, menuObjTab, UBOUND(menuObjTab), NULL);
    //UG_WindowSetTitleText(&menuWin, "ugui text");
    //UG_WindowSetTitleTextFont(&menuWin, &FONT_8X12);
    
    UG_TextboxCreate(&menuWin, &txb1, TXB_ID_0, 10, 10, 100, 30);
    UG_TextboxSetText(&menuWin, TXB_ID_0, "text:");
    UG_WindowShow(&menuWin);
    //UG_TextboxShow(&menuWin, TXB_ID_0);
}





