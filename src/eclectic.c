/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "eclectic.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

ECLECTIC_STATIC(eclectic_freeCallback_t eclectic_freeCallback);
ECLECTIC_STATIC(eclectic_mallocCallback_t eclectic_mallocCallback);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Free ***/
void eclectic_free(void **data)
{
    /*** Free ***/
    if((data != NULL) && (eclectic_freeCallback != NULL))
    {
        eclectic_freeCallback(*data);
        *data = NULL;
    }
}

/*** Initialize ***/
void eclectic_init(const eclectic_freeCallback_t FreeCallback, const eclectic_mallocCallback_t MallocCallback)
{
    /*** Initialize ***/
    eclectic_freeCallback = FreeCallback;
    eclectic_mallocCallback = MallocCallback;
}

/*** Malloc ***/
void *eclectic_malloc(const size_t size)
{
    void *memory = NULL;
    
    /*** Malloc ***/
    if((eclectic_mallocCallback != NULL) && (size > 0))
        return eclectic_mallocCallback(size);

    return memory;
}
