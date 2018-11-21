/**
  ******************************************************************************
  * @file   common_type.h
  * @author  
  * @version 
  * @date     定义通用类型,以便平台移植
  * @brief  
  ******************************************************************************
  * @attention 	20180906     v1.2   	jgb     脱离平台限制
  ******************************************************************************
  */
#ifndef __COMMON_TYPE_H_
#define __COMMON_TYPE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define ACTIVE_LOW        !
#define ACTIVE_HIGH       !!    /* double negation forces result to be '1' */


#ifndef FALSE
#define FALSE 0
#endif


#ifndef TRUE
#define TRUE 1
#endif

#ifndef Success
#define Success 0
#endif


#ifndef Failed
#define Failed 1
#endif



#endif
