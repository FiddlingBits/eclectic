/****************************************************************************************************
 * FILE:    random.h
 * BRIEF:   Random Header File
 ****************************************************************************************************/

#ifndef RANDOM_H
#define RANDOM_H

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

/*** Signed Integers ***/
#define RANDOM_GET_INTEGER_INT8()                       ((int8_t)random_getIntegerSigned())
#define RANDOM_GET_INTEGER_INT8_WITHIN_RANGE(MIN, MAX)  ((int8_t)random_getIntegerSignedWithinRange((MIN), (MAX)))
#define RANDOM_GET_INTEGER_INT16()                      ((int16_t)random_getIntegerSigned())
#define RANDOM_GET_INTEGER_INT16_WITHIN_RANGE(MIN, MAX) ((int16_t)random_getIntegerSignedWithinRange((MIN), (MAX)))
#define RANDOM_GET_INTEGER_INT32()                      (random_getIntegerSigned())
#define RANDOM_GET_INTEGER_INT32_WITHIN_RANGE(MIN, MAX) (random_getIntegerSignedWithinRange((MIN), (MAX)))

/*** Unsigned Integers ***/
#define RANDOM_GET_INTEGER_UINT8()                       ((uint8_t)random_getIntegerUnsigned())
#define RANDOM_GET_INTEGER_UINT8_WITHIN_RANGE(MIN, MAX)  ((uint8_t)random_getIntegerUnsignedWithinRange((MIN), (MAX)))
#define RANDOM_GET_INTEGER_UINT16()                      ((uint16_t)random_getIntegerUnsigned())
#define RANDOM_GET_INTEGER_UINT16_WITHIN_RANGE(MIN, MAX) ((uint16_t)random_getIntegerUnsignedWithinRange((MIN), (MAX)))
#define RANDOM_GET_INTEGER_UINT32()                      (random_getIntegerUnsigned())
#define RANDOM_GET_INTEGER_UINT32_WITHIN_RANGE(MIN, MAX) (random_getIntegerUnsignedWithinRange((MIN), (MAX)))

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include <stdint.h>

/****************************************************************************************************
 * Type Definitions
 ****************************************************************************************************/

#ifndef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  /*** Callbacks ***/
  typedef uint32_t (*random_getIntegerUnsignedHelperCallback_t)(void);
#endif

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

extern uint32_t random_getBit(void);
extern int32_t random_getIntegerSigned(void);
extern int32_t random_getIntegerSignedWithinRange(const int32_t MinimumInteger, const int32_t MaximumInteger);
extern uint32_t random_getIntegerUnsigned(void);
extern uint32_t random_getIntegerUnsignedUnique(const uint32_t * const Array, const uint32_t ArrayLength);
extern uint32_t random_getIntegerUnsignedWithinRange(const uint32_t MinimumInteger, const uint32_t MaximumInteger);
extern void random_setBuffer(uint8_t * const buffer, uint16_t BufferLength);
#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  extern void random_init(void);
#else
  extern void random_init(const random_getIntegerUnsignedHelperCallback_t GetIntegerUnsignedHelperCallback);
#endif

#endif
