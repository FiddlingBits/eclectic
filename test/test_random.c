/****************************************************************************************************
 * Include
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
unsigned int randomTest_getRandomSeed(void)
{
    static unsigned int seed = 0;

    /*** Get Random Seed ***/
    return seed++;
}

/*** Get Random Unsigned 32-Bit Integer Callback ***/
uint32_t randomTest_getRandomUnsigned32BitIntegerCallback(void)
{
    /*** Get Random Unsigned 32-Bit Integer Callback ***/
    return (uint32_t)(((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF));
}

/*** Set Random Seed ***/
void randomTest_setRandomSeed(const unsigned int Seed)
{
    /*** Set Random Seed ***/
    srand(Seed);
}

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomTest_getRandomUnsigned32BitIntegerCallback));
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
    /* Get Random Signed 32-Bit Integer (Not Initialized Error) */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitInteger());
}

void test_getRandomSigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t ListLength = 10;
    const unsigned int Seed = randomTest_getRandomSeed();
    
    /* Variable */
    int32_t actualInteger, expectedIntegerList[ListLength];
    size_t i;

    /* Set Up */
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
        expectedIntegerList[i] = (int32_t)randomTest_getRandomUnsigned32BitIntegerCallback();

    /*** Get Random Signed 32-Bit Integer ***/
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
    {
        actualInteger = random_getRandomSigned32BitInteger();
        TEST_ASSERT_EQUAL_INT32(expectedIntegerList[i], actualInteger);
    }
}

/*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
void test_getRandomSigned32BitIntegerWithinRange_error(void)
{
    /*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
    /* Get Random Signed 32-Bit Integer Within Range (Inclusive) (Not Initialized Error) */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitIntegerWithinRange(-3, -1));

    /* Get Random Signed 32-Bit Integer Within Range (Inclusive) (Invalid Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomTest_getRandomUnsigned32BitIntegerCallback));
    TEST_ASSERT_EQUAL_INT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomSigned32BitIntegerWithinRange(-1, -3));
}

void test_getRandomSigned32BitIntegerWithinRange_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t ListLength = 10;
    const int32_t MinimumInteger = -1073741824, MaximumInteger = 1073741824; // -(2^32 * (1 / 4)) = -1,073,741,824;  2^32 * (1 / 4) = 1,073,741,824
    const unsigned int Seed = randomTest_getRandomSeed();
    
    /* Variable */
    uint32_t actualInteger, expectedIntegerList[ListLength];
    size_t i;

    /* Set Up */
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
        expectedIntegerList[i] = (int32_t)((randomTest_getRandomUnsigned32BitIntegerCallback() % (MaximumInteger - MinimumInteger + 1)) + MinimumInteger);

    /*** Get Random Signed 32-Bit Integer Within Range (Inclusive) ***/
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
    {
        actualInteger = random_getRandomSigned32BitIntegerWithinRange(MinimumInteger, MaximumInteger);
        TEST_ASSERT_EQUAL_UINT32(expectedIntegerList[i], actualInteger);
    }
}

/*** Get Random Unsigned 32-Bit Integer ***/
void test_getRandomUnsigned32BitInteger_error(void)
{
    /*** Get Random Unsigned 32-Bit Integer ***/
    /* Get Random Unsigned 32-Bit Integer (Not Initialized Error) */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitInteger());
}

void test_getRandomUnsigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t ListLength = 10;
    const unsigned int Seed = randomTest_getRandomSeed();
    
    /* Variable */
    uint32_t actualInteger, expectedIntegerList[ListLength];
    size_t i;

    /* Set Up */
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
        expectedIntegerList[i] = randomTest_getRandomUnsigned32BitIntegerCallback();

    /*** Get Random Unsigned 32-Bit Integer ***/
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
    {
        actualInteger = random_getRandomUnsigned32BitInteger();
        TEST_ASSERT_EQUAL_UINT32(expectedIntegerList[i], actualInteger);
    }
}

/*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
void test_getRandomUnsigned32BitIntegerWithinRange_error(void)
{
    /*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
    /* Get Random Unsigned 32-Bit Integer Within Range (Inclusive) (Not Initialized Error) */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitIntegerWithinRange(1, 3));

    /* Get Random Unsigned 32-Bit Integer Within Range (Inclusive) (Invalid Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomTest_getRandomUnsigned32BitIntegerCallback));
    TEST_ASSERT_EQUAL_UINT32(RANDOM_CONFIG_ERROR_VALUE, random_getRandomUnsigned32BitIntegerWithinRange(3, 1));
}

void test_getRandomUnsigned32BitIntegerWithinRange_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t ListLength = 10;
    const uint32_t MinimumInteger = 1073741824U, MaximumInteger = 3221225472U; // 2^32 * (1 / 4) = 1,073,741,824;  2^32 * (3 / 4) = 3,221,225,472
    const unsigned int Seed = randomTest_getRandomSeed();
    
    /* Variable */
    uint32_t actualInteger, expectedIntegerList[ListLength];
    size_t i;

    /* Set Up */
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
        expectedIntegerList[i] = (randomTest_getRandomUnsigned32BitIntegerCallback() % (MaximumInteger - MinimumInteger + 1)) + MinimumInteger;

    /*** Get Random Unsigned 32-Bit Integer Within Range (Inclusive) ***/
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < ListLength; i++)
    {
        actualInteger = random_getRandomUnsigned32BitIntegerWithinRange(MinimumInteger, MaximumInteger);
        TEST_ASSERT_EQUAL_UINT32(expectedIntegerList[i], actualInteger);
    }
}

/*** Initialize ***/
void test_init_error(void)
{
    /*** Initialize ***/
    /* Initialize (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, random_init(NULL));
}

void test_init_success(void)
{
    /*** Initialize ***/
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomTest_getRandomUnsigned32BitIntegerCallback));
}

/*** Set Buffer ***/
void test_setBuffer_error(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t BufferLength = 10;
    
    /* Variable */
    uint8_t buffer[BufferLength];

    /*** Set Buffer ***/
    /* Set Buffer (Not Initialized Error) */
    random_getRandomUnsigned32BitIntegerCallback = NULL;
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, random_setBuffer(buffer, sizeof(buffer)));

    /* Set Buffer (NULL Pointer Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_init(randomTest_getRandomUnsigned32BitIntegerCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, random_setBuffer(NULL, sizeof(buffer)));

    /* Set Buffer (Length Error) */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, random_setBuffer(buffer, 0));
}

void test_setBuffer_success(void)
{
    /*** Test Data ***/
    /* Test Data */
    const size_t BufferLength = 10;
    const unsigned int Seed = randomTest_getRandomSeed();
    
    /* Variable */
    uint8_t actualBuffer[BufferLength], expectedBuffer[BufferLength];
    size_t i;

    /* Set Up */
    randomTest_setRandomSeed(Seed);
    for(i = 0; i < BufferLength; i++)
        expectedBuffer[i] = (uint8_t)randomTest_getRandomUnsigned32BitIntegerCallback();

    /*** Set Buffer ***/
    randomTest_setRandomSeed(Seed);
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, random_setBuffer(actualBuffer, sizeof(actualBuffer)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedBuffer, actualBuffer, BufferLength);
}
