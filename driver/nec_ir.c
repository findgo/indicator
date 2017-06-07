/**
  ******************************************************************************
  * @file    nec_ir.c
  * @author  
  * @version 
  * @date    
  * @brief   用于标准NEC协议 用于解码。平台STM32F030
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "nec_ir.h"
#include "module_serial.h"

/**
  * @brief  Remote配置
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval  None
  */
void Remote_Configuration(void)               
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //GPIO CLOCK enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //TIM4 ENABLE

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           //PB3 复用TIM3_CH1 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用模方
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_4); //3?ê??ˉGPIOB.9
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //开启TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;  //中断优先级第3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ENABLE
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;  //48分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //加记数模式
    TIM_TimeBaseStructure.TIM_Period = 0xffff;  //65535周期 重载值为65535
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频1 
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化TIM3时钟源
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // 选择输入端IC1映射到TIM3_CH1
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;    //下降沿输入捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    //输入信号不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0011 开启3个周期滤波 
    
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化

    TIM_ClearFlag(TIM3,TIM_FLAG_CC1);
    TIM_ITConfig( TIM3,TIM_IT_CC1,ENABLE);//开启比较通道1中断                        
    TIM_Cmd(TIM3,ENABLE);   //TIM3 启动
}



#define ir_time_clock_div      ( 1 )    //!< (xxMH/1MH)以1MHZ为基准 
#define ir_time_13500US     (uint16_t)(13500 * ir_time_clock_div)  //!< 0x34bc  
#define ir_time_11250US     (uint16_t)(11250 * ir_time_clock_div)  //!< 0x2bf2
#define ir_time_2250US      (uint16_t)(2250 * ir_time_clock_div)   //!< 0x08ca
#define ir_time_1125US      (uint16_t)(1120 * ir_time_clock_div)   //!< 0x0460
#define ir_time_limit_US    (uint16_t)(250 * ir_time_clock_div)    //!< 0xc8

#define remote_state_start      0x00
#define remote_state_leadcode   0x01
#define remote_state_code       0x02
#define remote_state_reset()    do{Remote_state = remote_state_start;}while(0)

#define ir_continuous_count 5 // 按110ms一次连发码  可定义多少秒产生一次连发有效值  这里设置成500重发一次
#define ir_remote_timeoutcount 200   //!< 解码超时时间， 以ms为基准
#define ir_remoteok_flagmask     0x80 //!< code ok mask
#define ir_remotecoding_flagmask 0x40 //!< coding mask
static remote_t remote = {0,0,0,0,0};
static uint8_t Remote_state = remote_state_start;       //解码状态机

/**
  * @brief  Remote解码，中断调用
   *@note    定时器时钟1MHZ  下降沿触发捕获中断*
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval  None
  */
/**********************************standard NEC protocol*************************
 *    _      ____     _     ____       _     ________        _      ___
 *    |     |    |    |    |    |      |    |        |       |     |   |
 *    |_____|    |_   |____|    |      |____|        |_      |_____|   |
 *    ^     ^    ^    ^    ^    ^      ^    ^        ^       ^     ^   ^
 * (ms)  9   4.5(13.5) 0.56  0.56(1.12) 0.56   1.69(2.25)       9   2.25(11.25)
 *         引导码          数据0            数据1                 重复码      
 *******************************************************************************/
void RemoteDecode(void)
{   
    static uint16_t CapHisValue = 0;//捕获上一次的值，即历史值
    uint16_t CapWide;//宽度
    remote_t *pMote = &remote;
    uint16_t CapCurValue = (uint16_t)TIM_GetCapture1(TIM3);//获得捕获值

    pMote->flag |= ir_remotecoding_flagmask;//coding on 
    pMote->timeout = 0;//coding timeout clear;
    //获得下降沿间时间宽度
    if(CapCurValue >= CapHisValue){
        CapWide = CapCurValue - CapHisValue;
    }
    else{
        CapWide = 0xffff - CapHisValue + CapCurValue;
    }
    CapHisValue = CapCurValue;//存现值
    
    switch(Remote_state){
    case remote_state_start: //第一次下降沿，后开始引导码 或 重复码
        Remote_state = remote_state_leadcode;   
        break;
        
    case remote_state_leadcode:   //确认引导码或重复码
        if((CapWide > (ir_time_13500US - ir_time_limit_US)) \
            && (CapWide < (ir_time_13500US + ir_time_limit_US))){//引导码4.5ms + 9ms = 13.5ms
          
            pMote->counter = 0;  //接收遥控码位数
            Remote_state = remote_state_code;//进入32位编码状态
        }
        else if((CapWide > (ir_time_11250US - ir_time_limit_US)) \
            && (CapWide < (ir_time_11250US + ir_time_limit_US))){//重复码
            if(++pMote->counter >= ir_continuous_count){
                pMote->counter = 0;
                pMote->flag |= ir_remoteok_flagmask;
            }
            remote_state_reset();
            pMote->flag &= ~ir_remotecoding_flagmask;////coding finish
        }
        else {
            pMote->counter = 0;
            remote_state_reset();   //不是引导码，也不是重复码退出回初始
        }
        break;
        
    case remote_state_code: //从最低位开始接收
        pMote->decode.cache_value >>= 1;
        if((CapWide > ir_time_1125US - ir_time_limit_US) \
            && (CapWide < ir_time_1125US + ir_time_limit_US)){  // 0位  //565US + 560US 为0   
            pMote->decode.cache_value |= 0x00000000;    
        }
        else if((CapWide > ir_time_2250US - ir_time_limit_US) \
            && (CapWide < ir_time_2250US + ir_time_limit_US)){   // 1位//1.685ms + 0.56ms为1
            pMote->decode.cache_value |= 0x80000000;
        }
        else{   //都不是，回初始
            pMote->counter = 0;
            remote_state_reset();
            break;
        }
        
        if(++pMote->counter >= 32){  //32位码已收到
            pMote->counter = 0;
            if((pMote->decode.bytes.user_value == (uint8_t)(~pMote->decode.bytes.user_ivalue))\
                &&(pMote->decode.bytes.cmd_value == (uint8_t)(~pMote->decode.bytes.cmd_ivalue))){//校验用户码和数据码
                pMote->flag |= ir_remoteok_flagmask;
                pMote->value = pMote->decode.bytes.cmd_value;
            }
            remote_state_reset();
            pMote->flag &= ~ir_remotecoding_flagmask;////coding finish
        }
        break;
        
    default:
        break;
    }
}
/**
  * @brief  remote 超时机制
  * @note    
  * @param  elapsetime: 流逝时间
  * @note   
  * @note    
  * @note   
  * @retval  None
  */
void RemoteTimeoutProcess(uint32_t elapsetime)
{
    if((remote.flag & ir_remotecoding_flagmask))//coding on then time add,the check it timeout?
    {
        remote.timeout += (uint8_t)elapsetime;
        if(remote.timeout >= ir_remote_timeoutcount)//coding time out
        {
            remote_state_reset();
            remote.timeout = 0;
            remote.flag &= ~ir_remotecoding_flagmask;// coding finish because it is err
        }
    }
}
/**
  * @brief  获得解码后的命令
  * @note    
  * @param  remotecmd: 解码后命令地址
  * @note   
  * @note    
  * @note   
  * @retval  bSUCCES or bFAIL
  */
ExecuteStatus RemoteGetCmd(uint8_t *remotecmd)
{
    if((remote.flag & ir_remoteok_flagmask))
    {
        remote.flag &= ~ir_remoteok_flagmask;
        *remotecmd = remote.value;
        return bSUCCESS;
    }
    return bFAIL;
}
/**
  * @brief  Remote服务程序，大循环调用
  * @note    
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval  None
  */
void RemoteDeal(void)
{
    static uint8_t cs = 1;
    uint8_t Cmdtemp;    
    if(RemoteGetCmd(&Cmdtemp))
    {
        Serial_WriteByte(Cmdtemp);
        if(cs)
        {
    GPIO_ResetBits(GPIOC,GPIO_Pin_8 | GPIO_Pin_9);
        cs = 0;
        }   
        else
        {
            GPIO_SetBits(GPIOC,GPIO_Pin_8 | GPIO_Pin_9);
            cs = 1;
        }
        switch(Cmdtemp)
        {
            case 0x00:
                break;
            default:break;
        }
    }
}
