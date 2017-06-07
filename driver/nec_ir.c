/**
  ******************************************************************************
  * @file    nec_ir.c
  * @author  
  * @version 
  * @date    
  * @brief   ���ڱ�׼NECЭ�� ���ڽ��롣ƽ̨STM32F030
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "nec_ir.h"
#include "module_serial.h"

/**
  * @brief  Remote����
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
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           //PB3 ����TIM3_CH1 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //����ģ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_4); //3?��??��GPIOB.9
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //����TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;  //�ж����ȼ���3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ENABLE
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1;  //48��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //�Ӽ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = 0xffff;  //65535���� ����ֵΪ65535
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ1 
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ��TIM3ʱ��Դ
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // ѡ�������IC1ӳ�䵽TIM3_CH1
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;    //�½������벶��
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    //�����źŲ���Ƶ 
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0011 ����3�������˲� 
    
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//��ʼ��

    TIM_ClearFlag(TIM3,TIM_FLAG_CC1);
    TIM_ITConfig( TIM3,TIM_IT_CC1,ENABLE);//�����Ƚ�ͨ��1�ж�                        
    TIM_Cmd(TIM3,ENABLE);   //TIM3 ����
}



#define ir_time_clock_div      ( 1 )    //!< (xxMH/1MH)��1MHZΪ��׼ 
#define ir_time_13500US     (uint16_t)(13500 * ir_time_clock_div)  //!< 0x34bc  
#define ir_time_11250US     (uint16_t)(11250 * ir_time_clock_div)  //!< 0x2bf2
#define ir_time_2250US      (uint16_t)(2250 * ir_time_clock_div)   //!< 0x08ca
#define ir_time_1125US      (uint16_t)(1120 * ir_time_clock_div)   //!< 0x0460
#define ir_time_limit_US    (uint16_t)(250 * ir_time_clock_div)    //!< 0xc8

#define remote_state_start      0x00
#define remote_state_leadcode   0x01
#define remote_state_code       0x02
#define remote_state_reset()    do{Remote_state = remote_state_start;}while(0)

#define ir_continuous_count 5 // ��110msһ��������  �ɶ�����������һ��������Чֵ  �������ó�500�ط�һ��
#define ir_remote_timeoutcount 200   //!< ���볬ʱʱ�䣬 ��msΪ��׼
#define ir_remoteok_flagmask     0x80 //!< code ok mask
#define ir_remotecoding_flagmask 0x40 //!< coding mask
static remote_t remote = {0,0,0,0,0};
static uint8_t Remote_state = remote_state_start;       //����״̬��

/**
  * @brief  Remote���룬�жϵ���
   *@note    ��ʱ��ʱ��1MHZ  �½��ش��������ж�*
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
 *         ������          ����0            ����1                 �ظ���      
 *******************************************************************************/
void RemoteDecode(void)
{   
    static uint16_t CapHisValue = 0;//������һ�ε�ֵ������ʷֵ
    uint16_t CapWide;//���
    remote_t *pMote = &remote;
    uint16_t CapCurValue = (uint16_t)TIM_GetCapture1(TIM3);//��ò���ֵ

    pMote->flag |= ir_remotecoding_flagmask;//coding on 
    pMote->timeout = 0;//coding timeout clear;
    //����½��ؼ�ʱ����
    if(CapCurValue >= CapHisValue){
        CapWide = CapCurValue - CapHisValue;
    }
    else{
        CapWide = 0xffff - CapHisValue + CapCurValue;
    }
    CapHisValue = CapCurValue;//����ֵ
    
    switch(Remote_state){
    case remote_state_start: //��һ���½��أ���ʼ������ �� �ظ���
        Remote_state = remote_state_leadcode;   
        break;
        
    case remote_state_leadcode:   //ȷ����������ظ���
        if((CapWide > (ir_time_13500US - ir_time_limit_US)) \
            && (CapWide < (ir_time_13500US + ir_time_limit_US))){//������4.5ms + 9ms = 13.5ms
          
            pMote->counter = 0;  //����ң����λ��
            Remote_state = remote_state_code;//����32λ����״̬
        }
        else if((CapWide > (ir_time_11250US - ir_time_limit_US)) \
            && (CapWide < (ir_time_11250US + ir_time_limit_US))){//�ظ���
            if(++pMote->counter >= ir_continuous_count){
                pMote->counter = 0;
                pMote->flag |= ir_remoteok_flagmask;
            }
            remote_state_reset();
            pMote->flag &= ~ir_remotecoding_flagmask;////coding finish
        }
        else {
            pMote->counter = 0;
            remote_state_reset();   //���������룬Ҳ�����ظ����˳��س�ʼ
        }
        break;
        
    case remote_state_code: //�����λ��ʼ����
        pMote->decode.cache_value >>= 1;
        if((CapWide > ir_time_1125US - ir_time_limit_US) \
            && (CapWide < ir_time_1125US + ir_time_limit_US)){  // 0λ  //565US + 560US Ϊ0   
            pMote->decode.cache_value |= 0x00000000;    
        }
        else if((CapWide > ir_time_2250US - ir_time_limit_US) \
            && (CapWide < ir_time_2250US + ir_time_limit_US)){   // 1λ//1.685ms + 0.56msΪ1
            pMote->decode.cache_value |= 0x80000000;
        }
        else{   //�����ǣ��س�ʼ
            pMote->counter = 0;
            remote_state_reset();
            break;
        }
        
        if(++pMote->counter >= 32){  //32λ�����յ�
            pMote->counter = 0;
            if((pMote->decode.bytes.user_value == (uint8_t)(~pMote->decode.bytes.user_ivalue))\
                &&(pMote->decode.bytes.cmd_value == (uint8_t)(~pMote->decode.bytes.cmd_ivalue))){//У���û����������
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
  * @brief  remote ��ʱ����
  * @note    
  * @param  elapsetime: ����ʱ��
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
  * @brief  ��ý���������
  * @note    
  * @param  remotecmd: ����������ַ
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
  * @brief  Remote������򣬴�ѭ������
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
