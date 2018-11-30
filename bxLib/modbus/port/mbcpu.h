
#ifndef __MB_CPU_H
#define __MB_CPU_H

#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "mbconfig.h"

#include "stm32f0xx.h"


//#ifndef FALSE
//#define FALSE   0
//#endif

//#ifndef TRUE
//#define TRUE   1
//#endif

#define assert(expr) 

#define ENTER_CRITICAL_SECTION()     __set_PRIMASK(1)
#define EXIT_CRITICAL_SECTION()      __set_PRIMASK(0)

#endif
