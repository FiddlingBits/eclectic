/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#ifdef CEEDLING
  #define ECLECTIC_STATIC(DECLARATION) extern DECLARATION; DECLARATION
#else
  #define ECLECTIC_STATIC(DECLARATION) static DECLARATION
#endif
#define ECLECTIC_UNUSED(ARG) (void)ARG

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <stddef.h>
#include <stdlib.h>

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

/*** Function Pointer ***/
typedef void (*eclectic_freeCallback_t)(void *data);
typedef void *(*eclectic_mallocCallback_t)(size_t size);

/*** Enumeration ***/
typedef enum eclectic_status_e
{
    ECLECTIC_STATUS_SUCCESS = 0,
    ECLECTIC_STATUS_ERROR_DUPLICATE,
    ECLECTIC_STATUS_ERROR_INVALID,
    ECLECTIC_STATUS_ERROR_LENGTH,
    ECLECTIC_STATUS_ERROR_MEMORY_ALLOCATION,
    ECLECTIC_STATUS_ERROR_NO_ACTION,
    ECLECTIC_STATUS_ERROR_NOT_INITIALIZED,
    ECLECTIC_STATUS_ERROR_NOT_UNIQUE,
    ECLECTIC_STATUS_ERROR_NULL_POINTER,
    ECLECTIC_STATUS_ERROR_STRING
} eclectic_status_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void eclectic_free(void **data);
extern void eclectic_init(const eclectic_freeCallback_t FreeCallback, const eclectic_mallocCallback_t MallocCallback);
extern void *eclectic_malloc(const size_t size);
