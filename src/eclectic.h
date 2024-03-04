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
 * Type Definition
 ****************************************************************************************************/

typedef enum eclectic_status_e
{
    ECLECTIC_STATUS_SUCCESS = 0,
    ECLECTIC_STATUS_ERROR_INVALID,
    ECLECTIC_STATUS_ERROR_LENGTH,
    ECLECTIC_STATUS_ERROR_NO_ACTION,
    ECLECTIC_STATUS_ERROR_NOT_INITIALIZED,
    ECLECTIC_STATUS_ERROR_NOT_UNIQUE,
    ECLECTIC_STATUS_ERROR_NULL_POINTER,
    ECLECTIC_STATUS_ERROR_STRING
} eclectic_status_t;
