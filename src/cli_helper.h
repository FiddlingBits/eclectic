/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <ctype.h>
#include "eclectic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

typedef struct cliHelper_optionArgumentPair_s
{
    const char *option;
    const char *argument;
} cliHelper_optionArgumentPair_t;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern eclectic_status_t cliHelper_getCommaSeparatedValueList(char * const input, const uint8_t MaxListCount, char *list[], uint8_t * const actualListCount);
extern eclectic_status_t cliHelper_getDouble(const char * const Input, double * const d);
extern eclectic_status_t cliHelper_getFloat(const char * const Input, float * const f);
extern eclectic_status_t cliHelper_getOptionArgumentPair(char * const input, cliHelper_optionArgumentPair_t * const optionArgumentPair);
extern eclectic_status_t cliHelper_getSigned32BitInteger(const char * const Input, int32_t * const s32);
extern eclectic_status_t cliHelper_getUnsigned32BitInteger(const char * const Input, uint32_t * const u32);
