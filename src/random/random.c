/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "random.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

ECLECTIC_STATIC(random_getRandomUnsigned32BitIntegerCallback_t random_getRandomUnsigned32BitIntegerCallback);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Get Random Signed 32-Bit Integer ***/
int32_t random_getRandomSigned32BitInteger(void)
{
    /*** Error Check ***/
    /* Not Initialized */
    if(random_getRandomUnsigned32BitIntegerCallback == NULL)
        return RANDOM_CONFIG_ERROR_VALUE;
        
    /*** Get Random Signed 32-Bit Integer ***/
    return (int32_t)random_getRandomUnsigned32BitIntegerCallback();
}

/*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
int32_t random_getRandomSigned32BitIntegerWithinRange(const int32_t MinimumInteger, const int32_t MaximumInteger)
{    
    /*** Error Check ***/
    /* Not Initialized Or Invalid */
    if((random_getRandomUnsigned32BitIntegerCallback == NULL) || (MinimumInteger > MaximumInteger))
        return RANDOM_CONFIG_ERROR_VALUE;
    
    /*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
    return ((int32_t)random_getRandomUnsigned32BitIntegerWithinRange(0, (uint32_t)(MaximumInteger - MinimumInteger)) + MinimumInteger);
}

/*** Get Random Unsigned 32-Bit Integer ***/
uint32_t random_getRandomUnsigned32BitInteger(void)
{
    /*** Error Check ***/
    /* Not Initialized */
    if(random_getRandomUnsigned32BitIntegerCallback == NULL)
        return RANDOM_CONFIG_ERROR_VALUE;

    /*** Get Random Unsigned 32-Bit Integer ***/
    return random_getRandomUnsigned32BitIntegerCallback();
}

/*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
uint32_t random_getRandomUnsigned32BitIntegerWithinRange(const uint32_t MinimumInteger, const uint32_t MaximumInteger)
{
    /*** Error Check ***/
    /* Not Initialized Or Invalid */
    if((random_getRandomUnsigned32BitIntegerCallback == NULL) || (MinimumInteger > MaximumInteger))
        return RANDOM_CONFIG_ERROR_VALUE;
    
    /*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
    return (random_getRandomUnsigned32BitIntegerCallback() % (((uint64_t)MaximumInteger - MinimumInteger + 1))) + MinimumInteger;
}

/*** Initialize ***/
eclectic_status_t random_init(random_getRandomUnsigned32BitIntegerCallback_t getRandomUnsigned32BitIntegerCallback)
{
    /*** Error Check ***/
    /* NULL Pointer */
    if(getRandomUnsigned32BitIntegerCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /*** Initialize ***/
    random_getRandomUnsigned32BitIntegerCallback = getRandomUnsigned32BitIntegerCallback;

    return ECLECTIC_STATUS_SUCCESS;
}

/*** Set Buffer ***/
eclectic_status_t random_setBuffer(uint8_t * const buffer, const uint16_t BufferLength)
{
    size_t i;
    
    /*** Error Check ***/    
    /* Not Initialized */
    if(random_getRandomUnsigned32BitIntegerCallback == NULL)
        return ECLECTIC_STATUS_ERROR_NOT_INITIALIZED;
    
    /* NULL Pointer */
    if(buffer == NULL)
        return ECLECTIC_STATUS_ERROR_NULL_POINTER;
    
    /* Length */
    if(BufferLength == 0)
        return ECLECTIC_STATUS_ERROR_LENGTH;

    /*** Set Buffer to Random Integers ***/
    for(i = 0; i < BufferLength; i++)
        buffer[i] = (uint8_t)random_getRandomUnsigned32BitIntegerCallback();
    
    return ECLECTIC_STATUS_SUCCESS;
}
