


#ifndef __MB_MEM_H_
#define __MB_MEM_H_

#include <stdlib.h>
#include <string.h>

#include "mbconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

void *mo_malloc(size_t size);
void mo_free(void *ptr);
void *mo_calloc(size_t nmemb, size_t size);
void *mo_realloc(void *ptr, size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif

