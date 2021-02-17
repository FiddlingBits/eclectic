/****************************************************************************************************
 * FILE:    get_random.c
 * BRIEF:   Get Random Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "random.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

#ifndef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  static uint32_t getRandom_getIntegerUnsignedHelperCallback(void);
#endif

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   main
 * BRIEF:   Get Random Main Entry
 * RETURN:  int: Success Or Failure Status
 * ARG:     argc: Number Of Arguments
 * ARG:     argv: Argument String List
 ****************************************************************************************************/
int main(int argc, const char *argv[])
{
    uint32_t count, i;
    
    /*** Check Arguments ***/
    if(argc != 2)
    {
        printf("Usage: %s [count]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    /*** Get Count ***/
    count = strtoul(argv[1], NULL, 0);
    
    /*** Print Random Numbers ***/
#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
    random_init();
#else
    random_init(getRandom_getIntegerUnsignedHelperCallback);
#endif
    for(i = 0; i < (count - 1); i++)
    {
        if(((i + 1) % 10) == 0)
            printf("0x%08X,\n", RANDOM_GET_INTEGER_UINT32());
        else
            printf("0x%08X, ", RANDOM_GET_INTEGER_UINT32());
    }
    printf("0x%08X\n", RANDOM_GET_INTEGER_UINT32());
    
    exit(EXIT_SUCCESS);
}

#ifndef RANDOM_RANDOMIZATION_SEED_FILE_NAME
/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   getRandom_getIntegerUnsignedHelperCallback
 * BRIEF:   Helper To Get Random Unsigned Integer
 * RETURN:  uint32_t: Random Unsigned Integer
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static uint32_t getRandom_getIntegerUnsignedHelperCallback(void)
{
    static uint32_t i = 0;
    return i++; // Not Random; Used For Demonstration
}
#endif
