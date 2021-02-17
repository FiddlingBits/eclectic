/****************************************************************************************************
 * FILE:    random.c
 * BRIEF:   Random Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  #define RANDOM_RANDOMIZATION_SEED_INITIAL_VALUE (0)
#endif

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "random.h"
#include <stdbool.h>
#include <stdint.h>
#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  #include <stdio.h>
  #include <stdlib.h>
#endif

/****************************************************************************************************
 * Type Definitions
 ****************************************************************************************************/

#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  /*** Callbacks ***/
  typedef uint32_t (*random_getIntegerUnsignedHelperCallback_t)(void);
#endif

/****************************************************************************************************
 * Constants And Variables
 ****************************************************************************************************/

static random_getIntegerUnsignedHelperCallback_t random_getIntegerUnsignedHelperCallback;

/****************************************************************************************************
 * Function Prototypes
 ****************************************************************************************************/

#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
  static FILE *random_createRandomizationSeedFile(void);
  static uint32_t random_getIntegerUnsignedHelper(void);
  static uint32_t random_getRandomizationSeed(void);
#endif

/****************************************************************************************************
 * Function Definitions (Public)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   random_getBit
 * BRIEF:   Get Random Bit
 * RETURN:  uint32_t: Random Bit
 * ARG:     void: No Arguments
 ****************************************************************************************************/
uint32_t random_getBit(void)
{
    return (0x00000001 << (random_getIntegerUnsignedHelperCallback() % 32));
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerSigned
 * BRIEF:   Get Random Signed Integer
 * RETURN:  int32_t: Random Signed Integer
 * ARG:     void: No Arguments
 * NOTE:    Integer Range −2,147,483,648 To 2,147,483,647 (0x80000000 To 0x7FFFFFFF)
 ****************************************************************************************************/
int32_t random_getIntegerSigned(void)
{
    return (int32_t)random_getIntegerUnsignedHelperCallback();
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerSignedWithinRange
 * BRIEF:   Get Random Signed Integer Within Range (Inclusive)
 * RETURN:  uint32_t: Random Signed Integer
 * ARG:     MinimumInteger: Minimum Integer In Range
 * ARG:     MaximumInteger: Maximum Integer In Range
 * NOTE:    Integer Range −2,147,483,648 To 2,147,483,647 (0x80000000 To 0x7FFFFFFF)
 ****************************************************************************************************/
int32_t random_getIntegerSignedWithinRange(const int32_t MinimumInteger, const int32_t MaximumInteger)
{
    int64_t difference, randomInteger;

    /*** Get Random Integer Within Range ***/
    difference = (int64_t)(MaximumInteger - MinimumInteger);
    randomInteger = (int64_t)random_getIntegerUnsignedWithinRange(0, (uint32_t)difference) + MinimumInteger;

    return (int32_t)randomInteger;
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerUnsigned
 * BRIEF:   Get Random Unsigned Integer
 * RETURN:  uint32_t: Random Unsigned Integer
 * ARG:     void: No Arguments
 * NOTE:    Integer Range 0 To 4,294,967,295 (0x00000000 To 0xFFFFFFFF)
 ****************************************************************************************************/
uint32_t random_getIntegerUnsigned(void)
{
    /*** Get Random Unsigned Integer ***/
    return random_getIntegerUnsignedHelperCallback();
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerUnsignedUnique
 * BRIEF:   Get Unique Random Unsigned Integer Not In Array
 * RETURN:  uint32_t: Unique Random Unsigned Integer
 * ARG:     Array: Array Holding Unsigned Integer Values New Integer Shouldn't Have
 * ARG:     ArrayLength: Array Length
 * NOTE:    Integer Range 0 To 4,294,967,295 (0x00000000 To 0xFFFFFFFF)
 ****************************************************************************************************/
uint32_t random_getIntegerUnsignedUnique(const uint32_t * const Array, const uint32_t ArrayLength)
{
    bool unique;
    uint32_t i, randomInteger;
    
    /*** Get Unique Random Unsigned Integer Not In Array ***/
    do
    {
        /* Generate Random Number */
        randomInteger = RANDOM_GET_INTEGER_UINT32();
        
        /* Verify Random Number Unique */
        unique = true; // Assume True Until Proven Otherwise
        for(i = 0; i < ArrayLength; i++)
        {
            if(randomInteger == Array[i])
            {
                unique = false;
                break;
            }
        }
    }
    while(!unique);
    
    return randomInteger;
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerUnsignedWithinRange
 * BRIEF:   Get Random Unsigned Integer Within Range (Inclusive)
 * RETURN:  uint32_t: Random Unsigned Integer
 * ARG:     MinimumInteger: Minimum Integer In Range
 * ARG:     MaximumInteger: Maximum Integer In Range
 * NOTE:    Integer Range 0 To 4,294,967,295 (0x00000000 To 0xFFFFFFFF)
 ****************************************************************************************************/
uint32_t random_getIntegerUnsignedWithinRange(const uint32_t MinimumInteger, const uint32_t MaximumInteger)
{
    uint32_t bucketSize, randomInteger;
    uint64_t range;

    /*** Get Random Integer Within Range ***/
    range = (uint64_t)(MaximumInteger - MinimumInteger + 1);
    if(range == 0x100000000)
    {
        randomInteger = random_getIntegerUnsignedHelperCallback(); // Full Range
    }
    else
    {
        bucketSize = 0xFFFFFFFF / range;
        do
        {
            randomInteger = random_getIntegerUnsigned() / bucketSize;
        }
        while(randomInteger >= range);
        randomInteger += MinimumInteger;
    }

    return randomInteger;
}

/****************************************************************************************************
 * FUNCT:   random_setBuffer
 * BRIEF:   Set 8-Bit Unsigned Buffer To Random Integers
 * RETURN:  void: Returns Nothing
 * ARG:     buffer: Buffer To Set To Random Integers
 * ARG:     BufferLength: Length Of Buffer
 * NOTE:    Integer Range 0 To 255 (0x00 To 0xFF)
 ****************************************************************************************************/
void random_setBuffer(uint8_t * const buffer, uint16_t BufferLength)
{
    uint32_t i;

    /*** Set Buffer To Random Integers ***/
    for(i = 0; i < BufferLength; i++)
        buffer[i] = (uint8_t)random_getIntegerUnsigned();
}

#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
/****************************************************************************************************
 * FUNCT:   random_init
 * BRIEF:   Initialize Random
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
void random_init(void)
{
    static bool firstCall = true;
    uint32_t randomizationSeed;

    /*** Only Initialize Randomization If First Call ***/
    if(firstCall)
    {
        /*** Initialize Randomization ***/
        random_getIntegerUnsignedHelperCallback = random_getIntegerUnsignedHelper;
        randomizationSeed = random_getRandomizationSeed();
        srand(randomizationSeed);
        firstCall = false;
    }
}
#else
/****************************************************************************************************
 * FUNCT:   random_init
 * BRIEF:   Initialize Random
 * RETURN:  void: Returns Nothing
 * ARG:     GetIntegerUnsignedHelperCallback: Get Integer Unsigned Helper Callback
 ****************************************************************************************************/
void random_init(const random_getIntegerUnsignedHelperCallback_t GetIntegerUnsignedHelperCallback)
{
    /*** Initialize Randomization ***/
    random_getIntegerUnsignedHelperCallback = GetIntegerUnsignedHelperCallback;
}
#endif

#ifdef RANDOM_RANDOMIZATION_SEED_FILE_NAME
/****************************************************************************************************
 * Function Definitions (Private)
 ****************************************************************************************************/

/****************************************************************************************************
 * FUNCT:   random_createRandomizationSeedFile
 * BRIEF:   Create Randomization Seed File
 * RETURN:  void: Returns Nothing
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static FILE *random_createRandomizationSeedFile(void)
{
    FILE *file;

    /*** Create File ***/
    file = fopen(RANDOM_RANDOMIZATION_SEED_FILE_NAME, "w+");

    /*** Write Initial Randomization Seed ***/
    (void)fprintf(file, "%u\n", RANDOM_RANDOMIZATION_SEED_INITIAL_VALUE);

    return file;
}

/****************************************************************************************************
 * FUNCT:   random_getIntegerUnsignedHelper
 * BRIEF:   Helper To Get Random Unsigned Integer
 * RETURN:  uint32_t: Random Unsigned Integer
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static uint32_t random_getIntegerUnsignedHelper(void)
{
    return (uint32_t)(((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF));
}

/****************************************************************************************************
 * FUNCT:   random_getRandomizationSeed
 * BRIEF:   Get Randomization Seed
 * RETURN:  uint32_t: Randomization Seed
 * ARG:     void: No Arguments
 ****************************************************************************************************/
static uint32_t random_getRandomizationSeed(void)
{
    char *fileContents;
    long int fileLength;
    uint32_t randomizationSeed;
    FILE *file;

    /*** Get Randomization Seed From File ***/
    /* Open Randomization Seed File */
    file = fopen(RANDOM_RANDOMIZATION_SEED_FILE_NAME, "r+");
    if(file == NULL)
        file = random_createRandomizationSeedFile();

    /* Get Length Of File */
    (void)fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    rewind(file);

    /* Allocate Memory For File */
    fileContents = malloc((size_t)fileLength);
    if(fileContents == NULL)
    {
        (void)fprintf(stderr, "Random: Error Allocating Memory For Randomization Seed File!\n");
        exit(EXIT_FAILURE);
    }

    /* Get Randomization Seed From File */
    if(fread(fileContents, sizeof(*fileContents), sizeof(fileContents), file) <= 0)
    {
        (void)fprintf(stderr, "Random: Error Reading Randomization Seed From File!\n");
        exit(EXIT_FAILURE);
    }
    randomizationSeed = (unsigned int)strtoul(fileContents, NULL, 10);

    /* Write New Randomization Seed To File */
    rewind(file);
    (void)fprintf(file, "%u\n", randomizationSeed + 1);

    /* Clean Up */
    free(fileContents);
    (void)fclose(file);

    return randomizationSeed;
}
#endif
