/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

typedef enum eclectic_status_e
{
    ECLECTIC_STATUS_SUCCESS = 0,
    ECLECTIC_STATUS_ERROR_INVALID,
    ECLECTIC_STATUS_ERROR_LENGTH,
    ECLECTIC_STATUS_ERROR_NO_ACTION,
    ECLECTIC_STATUS_ERROR_NULL_POINTER
} eclectic_status_t;
