/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

#include "random.h"
#include "random_config.h"
#include <stdlib.h>
#include "unity.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

/*** External ***/
extern random_getRandomUnsigned32BitIntegerCallback_t random_getRandomUnsigned32BitIntegerCallback;

/****************************************************************************************************
 * Helper
 ****************************************************************************************************/

/*** Get Random Seed ***/
unsigned int randomHelper_getRandomSeed(void)
{
    static unsigned int seed = 0;
    
    /*** Get Random Seed ***/
    return seed++;
}

/*** Get Random Unsigned 32-Bit Integer Callback ***/
uint32_t randomHelper_getRandomUnsigned32BitIntegerCallback(void)
{
    /*** Get Random Unsigned 32-Bit Integer Callback ***/
    return (uint32_t)(((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF));
}

/*** Set Random Seed ***/
void randomHelper_setRandomSeed(const unsigned int Seed)
{
    /*** Set Random Seed ***/
    srand(Seed);
}

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomHelper_getRandomUnsigned32BitIntegerCallback));
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Get Random Signed 32-Bit Integer ***/
void test_getRandomSigned32BitInteger_error(void)
{    
    /*** Get Random Signed 32-Bit Integer ***/
    /* Set Up */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    
    /* Not Initialized */
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitInteger());
}

void test_getRandomSigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define INTEGER_LIST_COUNT (10)
    
    /* Variable */
    int32_t actualIntegerList[INTEGER_LIST_COUNT], expectedIntegerList[INTEGER_LIST_COUNT];
    size_t i;
    const unsigned int Seed = randomHelper_getRandomSeed();
    
    /* Set Up */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        expectedIntegerList[i] = (int32_t)randomHelper_getRandomUnsigned32BitIntegerCallback();
    
    /*** Get Random Signed 32-Bit Integer ***/
    /* Get Random Signed 32-Bit Integer */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        actualIntegerList[i] = random_getRandomSigned32BitInteger();
    
    /* Verify */
    TEST_ASSERT_EQUAL_INT32_ARRAY(expectedIntegerList, actualIntegerList, INTEGER_LIST_COUNT);
    
    /*** Clean Up ***/
    #undef INTEGER_LIST_COUNT
}

/*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
void test_getRandomSigned32BitIntegerWithinRange_error(void)
{ 
    /*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
    /* Set Up */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    
    /* Not Initialized */
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitIntegerWithinRange(-3, -1));
    
    /* Clean Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomHelper_getRandomUnsigned32BitIntegerCallback));
    
    /* Invalid */
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitIntegerWithinRange(-1, -3));
}

void test_getRandomSigned32BitIntegerWithinRange_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define INTEGER_LIST_COUNT (10)
    
    /* Variable */
    uint32_t actualIntegerList[INTEGER_LIST_COUNT], expectedIntegerList[INTEGER_LIST_COUNT];
    size_t i;
    const int32_t MinimumInteger = -1073741824, MaximumInteger = 1073741824; // -(2^32 * (1 / 4)) = 1,073,741,824;  2^32 * (1 / 4) = 1,073,741,824
    const unsigned int Seed = randomHelper_getRandomSeed();
    
    /* Set Up */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        expectedIntegerList[i] = (int32_t)((randomHelper_getRandomUnsigned32BitIntegerCallback() % (MaximumInteger - MinimumInteger + 1)) + MinimumInteger);
    
    /*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
    /* Get Random Signed 32-Bit Integer */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        actualIntegerList[i] = random_getRandomSigned32BitIntegerWithinRange(MinimumInteger, MaximumInteger);
    
    /* Verify */
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expectedIntegerList, actualIntegerList, INTEGER_LIST_COUNT);
    
    /*** Clean Up ***/
    #undef INTEGER_LIST_COUNT
}

/*** Get Random Unsigned 32-Bit Integer ***/
void test_getRandomUnsigned32BitInteger_error(void)
{
    /*** Get Random Unsigned 32-Bit Integer ***/
    /* Set Up */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    
    /* Not Initialized */
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitInteger());
}

void test_getRandomUnsigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define INTEGER_LIST_COUNT (10)
    
    /* Variable */
    uint32_t actualIntegerList[INTEGER_LIST_COUNT], expectedIntegerList[INTEGER_LIST_COUNT];
    size_t i;
    const unsigned int Seed = randomHelper_getRandomSeed();
    
    /* Set Up */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        expectedIntegerList[i] = randomHelper_getRandomUnsigned32BitIntegerCallback();
    
    /*** Get Random Unsigned 32-Bit Integer ***/
    /* Get Random Unsigned 32-Bit Integer */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        actualIntegerList[i] = random_getRandomUnsigned32BitInteger();
    
    /* Verify */
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expectedIntegerList, actualIntegerList, INTEGER_LIST_COUNT);
    
    /*** Clean Up ***/
    #undef INTEGER_LIST_COUNT
}

/*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
void test_getRandomUnsigned32BitIntegerWithinRange_error(void)
{    
    /*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
    /* Set Up */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    
    /* Not Initialized */
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitIntegerWithinRange(1, 3));
    
    /* Clean Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomHelper_getRandomUnsigned32BitIntegerCallback));
    
    /* Invalid */
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitIntegerWithinRange(3, 1));
}

void test_getRandomUnsigned32BitIntegerWithinRange_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define INTEGER_LIST_COUNT (10)
    
    /* Variable */
    uint32_t actualIntegerList[INTEGER_LIST_COUNT], expectedIntegerList[INTEGER_LIST_COUNT];
    size_t i;
    const uint32_t MinimumInteger = 1073741824U, MaximumInteger = 3221225472U; // 2^32 * (1 / 4) = 1,073,741,824;  2^32 * (3 / 4) = 3,221,225,472
    const unsigned int Seed = randomHelper_getRandomSeed();
    
    /* Set Up */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        expectedIntegerList[i] = (randomHelper_getRandomUnsigned32BitIntegerCallback() % (MaximumInteger - MinimumInteger + 1)) + MinimumInteger;
    
    /*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
    /* Get Random Unsigned 32-Bit Integer */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < INTEGER_LIST_COUNT; i++)
        actualIntegerList[i] = random_getRandomUnsigned32BitIntegerWithinRange(MinimumInteger, MaximumInteger);
    
    /* Verify */
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expectedIntegerList, actualIntegerList, INTEGER_LIST_COUNT);
    
    /*** Clean Up ***/
    #undef INTEGER_LIST_COUNT
}

/*** Initialize ***/
void test_init_error(void)
{
    /*** Initialize ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, random_init(NULL));
}

void test_init_success(void)
{
    /*** Initialize ***/
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomHelper_getRandomUnsigned32BitIntegerCallback));
}

/*** Set Buffer ***/
void test_setBuffer_error(void)
{
    uint8_t buffer[1];
    
    /*** Set Buffer ***/
    /* Set Up */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    
    /* Not Initialized */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, random_setBuffer(buffer, sizeof(buffer)));
    
    /* Clean Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomHelper_getRandomUnsigned32BitIntegerCallback));
    
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, random_setBuffer(NULL, sizeof(buffer)));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, random_setBuffer(buffer, 0));
}

void test_setBuffer_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define BUFFER_COUNT (10)
    
    /* Variable */
    uint8_t actualBuffer[BUFFER_COUNT], expectedBuffer[BUFFER_COUNT];
    size_t i;
    const unsigned int Seed = randomHelper_getRandomSeed();
    
    /* Set Up */
    randomHelper_setRandomSeed(Seed);
    for(i = 0; i < BUFFER_COUNT; i++)
        expectedBuffer[i] = (uint8_t)randomHelper_getRandomUnsigned32BitIntegerCallback();
    
    /*** Set Buffer ***/
    /* Set Buffer */
    randomHelper_setRandomSeed(Seed);
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_setBuffer(actualBuffer, sizeof(actualBuffer)));
    
    /* Verify */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedBuffer, actualBuffer, BUFFER_COUNT);
    
    /*** Clean Up ***/
    #undef BUFFER_COUNT
}
