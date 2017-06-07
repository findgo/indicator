/**
  ******************************************************************************
  * @file    bsp_ir.h
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef _USER_NEC_IR_DRIVER_
#define _USER_NEC_IR_DRIVER_

#include"app_cfg.h"

typedef struct
{
	uint8_t  counter;	//����ң����λ��
	uint8_t  timeout;       //�ɼ�ֵ
    uint8_t  flag;			//��ɱ�־
    uint8_t  value;			//��ɱ�־
	union  //С��ģʽ
	{
 	   uint32_t cache_value;
	   struct 
	   	{
	   		uint8_t user_value:  8;
			uint8_t user_ivalue: 8;
			uint8_t cmd_value:   8;
			uint8_t cmd_ivalue:  8;
	   	}bytes;
	}decode;
}remote_t;


void Remote_Configuration(void);
void RemoteDecode(void);
void RemoteTimeoutProcess(uint32_t elapsetime);
ExecuteStatus RemoteGetCmd(uint8_t *remotecmd);
void RemoteDeal(void);


#endif

