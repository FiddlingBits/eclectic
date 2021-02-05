/****************************************************************************************************
 * FILE:    crc_test.c
 * BRIEF:   Cyclic Redundancy Check (CRC) Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

/*** CRC-8 ***/
#define CRC_TEST_CRC8_CHECK (0xF4)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "crc.h"
#include <stdint.h>
#include "unity_fixture.h"

/****************************************************************************************************
 * Constants and Variables
 ****************************************************************************************************/

static const uint8_t crcTest_CheckData[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
 
/****************************************************************************************************
 * Test Group
 ****************************************************************************************************/

TEST_GROUP(crc_test);

/****************************************************************************************************
 * Test Setup And Teardown
 ****************************************************************************************************/

TEST_SETUP(crc_test)
{
}

TEST_TEAR_DOWN(crc_test)
{
}

/****************************************************************************************************
 * Tests
 ****************************************************************************************************/

TEST(crc_test, crc8CalculateLookupTable)
{
    uint8_t crc8;
    
    /*** Calculate CRC-8; Verify Result As Expected ***/
    crc8 = crc_crc8CalculateLookupTable(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

TEST(crc_test, crc8CalculateLoop)
{
    uint8_t crc8;
    
    /*** Calculate CRC-8; Verify Result As Expected ***/
    crc8 = crc_crc8CalculateLoop(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

TEST(crc_test, crc8CalculatePartialLookupTable)
{
    uint8_t crc8, i;
    
    /*** Set Up ***/
    crc8 = CRC_CRC8_INITIAL_CRC8;
    
    /*** Calculate CRC-8; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8 = crc_crc8CalculatePartialLookupTable(crcTest_CheckData[i], crc8);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

TEST(crc_test, crc8CalculatePartialLoop)
{
    uint8_t crc8, i;
    
    /*** Set Up ***/
    crc8 = CRC_CRC8_INITIAL_CRC8;
    
    /*** Calculate CRC-8; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8 = crc_crc8CalculatePartialLoop(crcTest_CheckData[i], crc8);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(crc_test)
{
    RUN_TEST_CASE(crc_test, crc8CalculateLookupTable);
    RUN_TEST_CASE(crc_test, crc8CalculateLoop);
    RUN_TEST_CASE(crc_test, crc8CalculatePartialLookupTable);
    RUN_TEST_CASE(crc_test, crc8CalculatePartialLoop);
}

