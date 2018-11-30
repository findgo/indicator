
#include "mbmem.h"
#include "heap_mange.h"

/*********************************************************************
 * @brief   malloc memory
 *
 * @param   size - size of need
 *
 * @return  pointer the memory
 */
void *mo_malloc(size_t size)
{
    return pvPortMalloc(size);
    //return malloc(size);
}

/*********************************************************************
 * @brief   free memory
 *
 * @param   ptr - pointer the memory you want to free
 *
 * @return  
 */
void mo_free(void *ptr)
{
    vPortFree(ptr);
    //free(ptr);
}
/*********************************************************************
 * @brief   在内存的动态存储区中分配nmemb个长度为size的连续空间，函数返回一个指向分配起始地址的指针；如果分配不成功，返回NULL。
 *          并初始化该内存空间为0
 * @param   nmemb - 
 * @param   size -
 *
 * @return  
 */
void *mo_calloc(size_t nmemb, size_t size)
{
    void * addr=NULL;

    addr = (void *)pvPortMalloc(size * nmemb); 
    memset(addr,0,size * nmemb);

    return addr;
    //return calloc(nmemb, size);
}
/*********************************************************************
 * @brief  重新分配内存空间  
 *
 * @param   ptr - pointer the memory header you want to change
 * @param   size - new size of memory
 *
 * @return  pointer the memory
 */
void *mo_realloc(void *ptr, size_t size)
{
    void * addr=NULL;
        
    addr = (void*)pvPortMalloc(size);
    if( addr ){
        memset(addr, 0, size);
        memcpy(addr, ptr, size);
        vPortFree(ptr);
    }

    return addr;
    //return realloc(ptr, size);
}

