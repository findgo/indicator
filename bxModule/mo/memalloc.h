
/*
 *
 * */

#ifndef __MO_MEMALLOC_H__
#define __MO_MEMALLOC_H__

#include <stddef.h>
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

#endif /* __MO_MEMALLOC_H__ */

