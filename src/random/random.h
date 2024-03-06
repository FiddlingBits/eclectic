/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Define
 ****************************************************************************************************/

/*** Signed Integer */
#define RANDOM_GET_RANDOM_SIGNED_8_BIT_INTEGER()                       ((int8_t)random_getRandomSigned32BitInteger())
#define RANDOM_GET_RANDOM_SIGNED_8_BIT_INTEGER_WITHIN_RANGE(MIN, MAX)  ((int8_t)random_getRandomSigned32BitIntegerWithinRange((MIN), (MAX)))
#define RANDOM_GET_RANDOM_SIGNED_16_BIT_INTEGER()                      ((int16_t)random_getRandomSigned32BitInteger())
#define RANDOM_GET_RANDOM_SIGNED_16_BIT_INTEGER_WITHIN_RANGE(MIN, MAX) ((int16_t)random_getRandomSigned32BitIntegerWithinRange((MIN), (MAX)))
#define RANDOM_GET_RANDOM_SIGNED_32_BIT_INTEGER()                      (random_getRandomSigned32BitInteger())
#define RANDOM_GET_RANDOM_SIGNED_32_BIT_INTEGER_WITHIN_RANGE(MIN, MAX) (random_getRandomSigned32BitIntegerWithinRange((MIN), (MAX)))

/*** Unsigned Integer ***/
#define RANDOM_GET_RANDOM_UNSIGNED_8_BIT_INTEGER()                       ((uint8_t)random_getRandomUnsigned32BitInteger())
#define RANDOM_GET_RANDOM_UNSIGNED_8_BIT_INTEGER_WITHIN_RANGE(MIN, MAX)  ((uint8_t)random_getRandomUnsigned32BitIntegerWithinRange((MIN), (MAX)))
#define RANDOM_GET_RANDOM_UNSIGNED_16_BIT_INTEGER()                      ((uint16_t)random_getRandomUnsigned32BitInteger())
#define RANDOM_GET_RANDOM_UNSIGNED_16_BIT_INTEGER_WITHIN_RANGE(MIN, MAX) ((uint16_t)random_getRandomUnsigned32BitIntegerWithinRange((MIN), (MAX)))
#define RANDOM_GET_RANDOM_UNSIGNED_32_BIT_INTEGER()                      (random_getRandomUnsigned32BitInteger())
#define RANDOM_GET_RANDOM_UNSIGNED_32_BIT_INTEGER_WITHIN_RANGE(MIN, MAX) (random_getRandomUnsigned32BitIntegerWithinRange((MIN), (MAX)))

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "eclectic.h"
#include "random_config.h"
#include <stddef.h>
#include <stdint.h>

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

/*** Function Pointer ***/
typedef uint32_t (*random_getRandomUnsigned32BitIntegerCallback_t)(void);

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern int32_t random_getRandomSigned32BitInteger(void);
extern int32_t random_getRandomSigned32BitIntegerWithinRange(const int32_t MinimumInteger, const int32_t MaximumInteger);
extern uint32_t random_getRandomUnsigned32BitInteger(void);
extern uint32_t random_getRandomUnsigned32BitIntegerWithinRange(const uint32_t MinimumInteger, const uint32_t MaximumInteger);
extern eclectic_status_t random_init(random_getRandomUnsigned32BitIntegerCallback_t getRandomUnsigned32BitIntegerCallback);
extern eclectic_status_t random_setBuffer(uint8_t * const buffer, const uint16_t BufferLength);
