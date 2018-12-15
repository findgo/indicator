#include "mkey.h"

typedef struct {
    mKeyDownFunc_t IsmKeyDownFunc;  // 按键按下的判断函数,1表示按下 
    void *next;
    
    uint8_t filterCount;             // 滤波器计时器
    uint8_t filterTime;              // 滤波时间(如果设为0，则默认为20ms)
    uint16_t longrepCount;           // 长按连击共用计数器 
    uint16_t longTime;               // 按键按下持续时间, 0表示不检测长按 
    uint8_t repeatSpeed;             // 连击时间

    uint8_t KeyCodeUp;               // 起键键值
    uint8_t KeyCodeDown;             // 短按键值
    uint8_t KeyCodeLong;             // 长按键值
    uint8_t state;                   // 状态机
}keycfg_t;

typedef struct{                                                                     
    uint8_t     tHead;
    uint8_t     tTail;
    uint8_t     tCounter;
    uint8_t     tSize;
    uint8_t    tBuffer[KEY_QUEUE_MAX_SIZE];
}keyfifo_t;

//定义一个对象实列
static keyfifo_t tKeyDetectorQueue = {
    // 部份初始化需要c99支持或GNU
    .tHead = 0,
    .tTail = 0,
    .tCounter = 0,
    .tSize = KEY_QUEUE_MAX_SIZE
};
static keycfg_t *KeyHead = NULL;

static uint8_t key_put(uint8_t Obj)  
{
    keyfifo_t *ptQ = ( keyfifo_t * )&tKeyDetectorQueue;

    if (ptQ->tCounter != ptQ->tSize) {
        ptQ->tBuffer[ptQ->tTail] = Obj;
        if (++ptQ->tTail >= ptQ->tSize){
            ptQ->tTail = 0;
        }       
        ptQ->tCounter++;
        return TRUE;
    }
    
    return FALSE;
}
static uint8_t key_pop(uint8_t *ptObj)
{ 
    keyfifo_t *ptQ = (keyfifo_t *)&tKeyDetectorQueue;

    if(ptQ->tCounter != 0) {
        *ptObj = ptQ->tBuffer[ptQ->tHead];
        if(++ptQ->tHead >= ptQ->tSize){ 
            ptQ->tHead = 0; 
        }       
        ptQ->tCounter--;
        return TRUE;
    } 
    
    return FALSE;
}
#define KEY_DECETOR_CHECK_START         0
#define KEY_DECETOR_CHECK_DOWN          1
#define KEY_DECETOR_LONG_PRESS          2   
#define KEY_DECETOR_REPEAT_PRESS        3 
#define KEY_DECETOR_IS_UP               4

void mkeyAssign(mkeycfg_t *cfg, mKeyDownFunc_t IsmKeyDownFunc,
                    uint8_t KeyCodeDown,uint8_t KeyCodeLong,uint8_t KeyCodeUp,
                    uint8_t filterTime,uint16_t longTime,uint8_t repeatSpeed)
{
    keycfg_t *mcfg = ( keycfg_t * )cfg;

    mcfg->IsmKeyDownFunc = IsmKeyDownFunc;
    mcfg->filterTime = filterTime > 0 ? filterTime : KEY_DEFAULT_FILTER_TIME;
    mcfg->longTime = longTime;
    mcfg->repeatSpeed = repeatSpeed;
    mcfg->KeyCodeDown = KeyCodeDown;
    mcfg->KeyCodeLong = KeyCodeLong;
    mcfg->KeyCodeUp = KeyCodeUp;
    mcfg->filterCount = 0;
    mcfg->longrepCount = 0;
    mcfg->next = NULL;
    mcfg->state = KEY_DECETOR_CHECK_START;
    if( KeyHead ){
        mcfg->next = KeyHead;
    }
    KeyHead = mcfg;
}

/**
  * @brief   按键分析程序，短击，长击，连击,抬键
  * @param  none
  * @note   
  * @retval  none
  * @note 
  */
void mkeydecetor_task(void)
{
    keycfg_t *curcfg = KeyHead;    
    
    while(curcfg)
    {
        switch(curcfg->state){
        case KEY_DECETOR_CHECK_START:
            if(curcfg->IsmKeyDownFunc()){
                curcfg->state = KEY_DECETOR_CHECK_DOWN;
            }
            break;
            
        case KEY_DECETOR_CHECK_DOWN:    
            if(++curcfg->filterCount >= curcfg->filterTime){ // 滤波
                curcfg->filterCount = 0;
                if(curcfg->IsmKeyDownFunc()){
                    if((curcfg ->longTime == 0) && (curcfg->repeatSpeed == 0)){ // 不支持长击和连击，直接到抬键状态
                        key_put(curcfg->KeyCodeDown);
                        curcfg->state = KEY_DECETOR_IS_UP;
                    }
                    else{
                        curcfg->state = KEY_DECETOR_LONG_PRESS;
                    }
                }
                else{
                    curcfg->state = KEY_DECETOR_CHECK_START;
                }
            }
            break;
            
        case KEY_DECETOR_LONG_PRESS:
            if(curcfg->longTime > 0){ // 支持长按
                if(curcfg->IsmKeyDownFunc()){
                    if(++curcfg->longrepCount >= curcfg->longTime){// 长按
                        key_put(curcfg->KeyCodeLong);
                        if(curcfg->repeatSpeed == 0)// 不支持连击，直接到抬键
                            curcfg->state = KEY_DECETOR_IS_UP;
                        else
                            curcfg->state = KEY_DECETOR_REPEAT_PRESS;
                        curcfg->longrepCount = 0;  
                    }
                }
                else{  //短按
                    key_put(curcfg->KeyCodeDown);
                    curcfg->state = KEY_DECETOR_IS_UP;
                    curcfg->longrepCount = 0;
                }
            }
            else{  // 不支持长按
                if(curcfg->repeatSpeed > 0){ //支持连击
                   if(curcfg->IsmKeyDownFunc()){
                        if(++curcfg->longrepCount >= KEY_DEFAULT_LONGTOREPEAT_TIME){
                            curcfg->state = KEY_DECETOR_REPEAT_PRESS;                        
                            curcfg->longrepCount = 0;
                        }
                   }
                   else{
                        key_put(curcfg->KeyCodeDown);
                        curcfg->longrepCount = 0;
                        curcfg->state = KEY_DECETOR_IS_UP; 
                   }
                }
                else{ // 不支持连击
                   key_put(curcfg->KeyCodeDown);
                   curcfg->state = KEY_DECETOR_IS_UP; 
                }
            }
            break;
            
        case KEY_DECETOR_REPEAT_PRESS:
            if(curcfg->repeatSpeed > 0){ // 支持连击
                if(curcfg->IsmKeyDownFunc()){
                    if(++curcfg->longrepCount >= curcfg->repeatSpeed){
                        key_put(curcfg->KeyCodeDown);
                        curcfg->longrepCount = 0;
                    }
                }
                else{
                     curcfg->longrepCount = 0;
                     curcfg->state = KEY_DECETOR_IS_UP;
                }
            }
            else{ // 不支持连击            
                curcfg->longrepCount = 0;
                curcfg->state = KEY_DECETOR_IS_UP;
            }
            break;
            
        case KEY_DECETOR_IS_UP:         //抬键处理
            if(++curcfg->filterCount >= curcfg->filterTime){
                if(!curcfg->IsmKeyDownFunc()){
                    if(curcfg->KeyCodeUp != MKEY_NULL)
                        key_put(curcfg->KeyCodeUp);
                    curcfg->state = KEY_DECETOR_CHECK_START;
                }
                curcfg->filterCount = 0;
            }
            break;
            
            default:
                break;
        }
        curcfg = (keycfg_t *)curcfg->next;
    }
}
/**
  * @brief  返回键值
  * @param  
  * @note   
  * @retval  none
  */
uint8_t mkeygetvalue(void)
{
    uint8_t keyvalue = MKEY_NULL;
    
    (void)key_pop(&keyvalue);

    return keyvalue;
}
