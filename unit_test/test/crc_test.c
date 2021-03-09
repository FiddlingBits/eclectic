/****************************************************************************************************
 * FILE:    crc_test.c
 * BRIEF:   Cyclic Redundancy Check (CRC) Test Source File
 ****************************************************************************************************/

/****************************************************************************************************
 * Definitions
 ****************************************************************************************************/

/*** CRC-8 ***/
#define CRC_TEST_CRC8_CHECK          (0xF4)
#define CRC_TEST_CRC8_CDMA2000_CHECK (0xDA)
#define CRC_TEST_CRC8_DARC_CHECK     (0x15)
#define CRC_TEST_CRC8_DVB_S2_CHECK   (0xBC)
#define CRC_TEST_CRC8_EBU_CHECK      (0x97)
#define CRC_TEST_CRC8_I_CODE_CHECK   (0x7E)
#define CRC_TEST_CRC8_ITU_CHECK      (0xA1)
#define CRC_TEST_CRC8_MAXIM_CHECK    (0xA1)
#define CRC_TEST_CRC8_ROHC_CHECK     (0xD0)

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#include "crc.h"
#include <stdbool.h>
#include <stddef.h>
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

TEST(crc_test, crc8Calculate)
{
    uint8_t crc8;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8 = crc_crc8Calculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_INITIAL_CRC8, crc8);
    
    /* Zero Length Data */
    crc8 = crc_crc8Calculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_INITIAL_CRC8, crc8);
    
    /*** Calculate CRC-8 For Check Data; Verify Result As Expected ***/
    crc8 = crc_crc8Calculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

TEST(crc_test, crc8CalculatePartial)
{
    uint8_t crc8, i;
    
    /*** Set Up ***/
    crc8 = CRC_CRC8_INITIAL_CRC8;
    
    /*** Calculate CRC-8 For Check Data; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8 = crc_crc8CalculatePartial(crcTest_CheckData[i], crc8);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CHECK, crc8);
}

TEST(crc_test, crc8Cdma2000Calculate)
{
    uint8_t crc8Cdma2000;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Cdma2000 = crc_crc8Cdma2000Calculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_CDMA2000_INITIAL_CRC8_CDMA2000, crc8Cdma2000);
    
    /* Zero Length Data */
    crc8Cdma2000 = crc_crc8Cdma2000Calculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_CDMA2000_INITIAL_CRC8_CDMA2000, crc8Cdma2000);
    
    /*** Calculate CRC-8/CDMA2000 For Check Data; Verify Result As Expected ***/
    crc8Cdma2000 = crc_crc8Cdma2000Calculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CDMA2000_CHECK, crc8Cdma2000);
}

TEST(crc_test, crc8Cdma2000CalculatePartial)
{
    uint8_t crc8Cdma2000, i;
    
    /*** Set Up ***/
    crc8Cdma2000 = CRC_CRC8_CDMA2000_INITIAL_CRC8_CDMA2000;
    
    /*** Calculate CRC-8/CDMA2000 For Check Data; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8Cdma2000 = crc_crc8Cdma2000CalculatePartial(crcTest_CheckData[i], crc8Cdma2000);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_CDMA2000_CHECK, crc8Cdma2000);
}

TEST(crc_test, crc8DarcCalculate)
{
    uint8_t crc8Darc;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Darc = crc_crc8DarcCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_DARC_INITIAL_CRC8_DARC, crc8Darc);
    
    /* Zero Length Data */
    crc8Darc = crc_crc8DarcCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_DARC_INITIAL_CRC8_DARC, crc8Darc);
    
    /*** Calculate CRC-8/DARC For Check Data; Verify Result As Expected ***/
    crc8Darc = crc_crc8DarcCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_DARC_CHECK, crc8Darc);
}

TEST(crc_test, crc8DarcCalculatePartial)
{
    uint8_t crc8Darc, i;
    
    /*** Set Up ***/
    crc8Darc = CRC_CRC8_DARC_INITIAL_CRC8_DARC;
    
    /*** Calculate CRC-8/DARC For Check Data; Verify Result As Expected ***/
    for(i = 0; i < (sizeof(crcTest_CheckData) - 1); i++)
        crc8Darc = crc_crc8DarcCalculatePartial(crcTest_CheckData[i], crc8Darc, false);
    crc8Darc = crc_crc8DarcCalculatePartial(crcTest_CheckData[i], crc8Darc, true);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_DARC_CHECK, crc8Darc);
}

TEST(crc_test, crc8DvbS2Calculate)
{
    uint8_t crc8DvbS2;

    /*** Error Check ***/
    /* NULL Pointer */
    crc8DvbS2 = crc_crc8DvbS2Calculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_DVB_S2_INITIAL_CRC8_DVB_S2, crc8DvbS2);
    
    /* Zero Length Data */
    crc8DvbS2 = crc_crc8DvbS2Calculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_DVB_S2_INITIAL_CRC8_DVB_S2, crc8DvbS2);
    
    /*** Calculate CRC-8/DVB-S2 For Check Data; Verify Result As Expected ***/
    crc8DvbS2 = crc_crc8DvbS2Calculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_DVB_S2_CHECK, crc8DvbS2);
}

TEST(crc_test, crc8DvbS2CalculatePartial)
{
    uint8_t crc8DvbS2, i;
    
    /*** Set Up ***/
    crc8DvbS2 = CRC_CRC8_DVB_S2_INITIAL_CRC8_DVB_S2;
    
    /*** Calculate CRC-8/DVB-S2 For Check Data; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8DvbS2 = crc_crc8DvbS2CalculatePartial(crcTest_CheckData[i], crc8DvbS2);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_DVB_S2_CHECK, crc8DvbS2);
}

TEST(crc_test, crc8EbuCalculate)
{
    uint8_t crc8Ebu;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Ebu = crc_crc8EbuCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_EBU_INITIAL_CRC8_EBU, crc8Ebu);
    
    /* Zero Length Data */
    crc8Ebu = crc_crc8EbuCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_EBU_INITIAL_CRC8_EBU, crc8Ebu);
    
    /*** Calculate CRC-8/EBU For Check Data; Verify Result As Expected ***/
    crc8Ebu = crc_crc8EbuCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_EBU_CHECK, crc8Ebu);
}

TEST(crc_test, crc8EbuCalculatePartial)
{
    uint8_t crc8Ebu, i;
    
    /*** Set Up ***/
    crc8Ebu = CRC_CRC8_EBU_INITIAL_CRC8_EBU;
    
    /*** Calculate CRC-8/EBU For Check Data; Verify Result As Expected ***/
    for(i = 0; i < (sizeof(crcTest_CheckData) - 1); i++)
        crc8Ebu = crc_crc8EbuCalculatePartial(crcTest_CheckData[i], crc8Ebu, false);
    crc8Ebu = crc_crc8EbuCalculatePartial(crcTest_CheckData[i], crc8Ebu, true);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_EBU_CHECK, crc8Ebu);
}

TEST(crc_test, crc8ICodeCalculate)
{
    uint8_t crc8ICode;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8ICode = crc_crc8ICodeCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_I_CODE_INITIAL_CRC8_I_CODE, crc8ICode);
    
    /* Zero Length Data */
    crc8ICode = crc_crc8ICodeCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_I_CODE_INITIAL_CRC8_I_CODE, crc8ICode);
    
    /*** Calculate CRC-8/I-CODE For Check Data; Verify Result As Expected ***/
    crc8ICode = crc_crc8ICodeCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_I_CODE_CHECK, crc8ICode);
}

TEST(crc_test, crc8ICodeCalculatePartial)
{
    uint8_t crc8ICode, i;
    
    /*** Set Up ***/
    crc8ICode = CRC_CRC8_I_CODE_INITIAL_CRC8_I_CODE;
    
    /*** Calculate CRC-8/I-CODE For Check Data; Verify Result As Expected ***/
    for(i = 0; i < sizeof(crcTest_CheckData); i++)
        crc8ICode = crc_crc8ICodeCalculatePartial(crcTest_CheckData[i], crc8ICode);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_I_CODE_CHECK, crc8ICode);
}

TEST(crc_test, crc8ItuCalculate)
{
    uint8_t crc8Itu;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Itu = crc_crc8ItuCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_ITU_INITIAL_CRC8_ITU, crc8Itu);
    
    /* Zero Length Data */
    crc8Itu = crc_crc8ItuCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_ITU_INITIAL_CRC8_ITU, crc8Itu);
    
    /*** Calculate CRC-8/ITU For Check Data; Verify Result As Expected ***/
    crc8Itu = crc_crc8ItuCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_ITU_CHECK, crc8Itu);
}

TEST(crc_test, crc8ItuCalculatePartial)
{
    uint8_t crc8Itu, i;
    
    /*** Set Up ***/
    crc8Itu = CRC_CRC8_ITU_INITIAL_CRC8_ITU;
    
    /*** Calculate CRC-8/ITU For Check Data; Verify Result As Expected ***/
    for(i = 0; i < (sizeof(crcTest_CheckData) - 1); i++)
        crc8Itu = crc_crc8ItuCalculatePartial(crcTest_CheckData[i], crc8Itu, false);
    crc8Itu = crc_crc8ItuCalculatePartial(crcTest_CheckData[i], crc8Itu, true);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_ITU_CHECK, crc8Itu);
}

TEST(crc_test, crc8MaximCalculate)
{
    uint8_t crc8Maxim;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Maxim = crc_crc8MaximCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_MAXIM_INITIAL_CRC8_MAXIM, crc8Maxim);
    
    /* Zero Length Data */
    crc8Maxim = crc_crc8MaximCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_MAXIM_INITIAL_CRC8_MAXIM, crc8Maxim);
    
    /*** Calculate CRC-8/MAXIM For Check Data; Verify Result As Expected ***/
    crc8Maxim = crc_crc8MaximCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_MAXIM_CHECK, crc8Maxim);
}

TEST(crc_test, crc8MaximCalculatePartial)
{
    uint8_t crc8Maxim, i;
    
    /*** Set Up ***/
    crc8Maxim = CRC_CRC8_MAXIM_INITIAL_CRC8_MAXIM;
    
    /*** Calculate CRC-8/MAXIM For Check Data; Verify Result As Expected ***/
    for(i = 0; i < (sizeof(crcTest_CheckData) - 1); i++)
        crc8Maxim = crc_crc8MaximCalculatePartial(crcTest_CheckData[i], crc8Maxim, false);
    crc8Maxim = crc_crc8MaximCalculatePartial(crcTest_CheckData[i], crc8Maxim, true);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_MAXIM_CHECK, crc8Maxim);
}

TEST(crc_test, crc8RohcCalculate)
{
    uint8_t crc8Rohc;
    
    /*** Error Check ***/
    /* NULL Pointer */
    crc8Rohc = crc_crc8RohcCalculate(NULL, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_ROHC_INITIAL_CRC8_ROHC, crc8Rohc);
    
    /* Zero Length Data */
    crc8Rohc = crc_crc8RohcCalculate(crcTest_CheckData, 0);
    TEST_ASSERT_EQUAL_HEX8(CRC_CRC8_ROHC_INITIAL_CRC8_ROHC, crc8Rohc);
    
    /*** Calculate CRC-8/ROHC For Check Data; Verify Result As Expected ***/
    crc8Rohc = crc_crc8RohcCalculate(crcTest_CheckData, sizeof(crcTest_CheckData));
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_ROHC_CHECK, crc8Rohc);
}

TEST(crc_test, crc8RohcCalculatePartial)
{
    uint8_t crc8Rohc, i;
    
    /*** Set Up ***/
    crc8Rohc = CRC_CRC8_ROHC_INITIAL_CRC8_ROHC;
    
    /*** Calculate CRC-8/ROHC For Check Data; Verify Result As Expected ***/
    for(i = 0; i < (sizeof(crcTest_CheckData) - 1); i++)
        crc8Rohc = crc_crc8RohcCalculatePartial(crcTest_CheckData[i], crc8Rohc, false);
    crc8Rohc = crc_crc8RohcCalculatePartial(crcTest_CheckData[i], crc8Rohc, true);
    TEST_ASSERT_EQUAL_HEX8(CRC_TEST_CRC8_ROHC_CHECK, crc8Rohc);
}

/****************************************************************************************************
 * Test Group Runner
 ****************************************************************************************************/

TEST_GROUP_RUNNER(crc_test)
{
    /*** CRC-8 ***/
    /* CRC-8 */
    RUN_TEST_CASE(crc_test, crc8Calculate);
    RUN_TEST_CASE(crc_test, crc8CalculatePartial);
    
    /* CRC-8/CDMA2000 */
    RUN_TEST_CASE(crc_test, crc8Cdma2000Calculate);
    RUN_TEST_CASE(crc_test, crc8Cdma2000CalculatePartial);
    
    /* CRC-8/DARC */
    RUN_TEST_CASE(crc_test, crc8DarcCalculate);
    RUN_TEST_CASE(crc_test, crc8DarcCalculatePartial);
    
    /* CRC-8/DVB-S2 */
    RUN_TEST_CASE(crc_test, crc8DvbS2Calculate);
    RUN_TEST_CASE(crc_test, crc8DvbS2CalculatePartial);
    
    /* CRC-8/EBU */
    RUN_TEST_CASE(crc_test, crc8EbuCalculate);
    RUN_TEST_CASE(crc_test, crc8EbuCalculatePartial);
    
    /* CRC-8/I-CODE */
    RUN_TEST_CASE(crc_test, crc8ICodeCalculate);
    RUN_TEST_CASE(crc_test, crc8ICodeCalculatePartial);
    
    /* CRC-8/ITU */
    RUN_TEST_CASE(crc_test, crc8ItuCalculate);
    RUN_TEST_CASE(crc_test, crc8ItuCalculatePartial);
    
    /* CRC-8/MAXIM */
    RUN_TEST_CASE(crc_test, crc8MaximCalculate);
    RUN_TEST_CASE(crc_test, crc8MaximCalculatePartial);
    
    /* CRC-8/ROHC */
    RUN_TEST_CASE(crc_test, crc8RohcCalculate);
    RUN_TEST_CASE(crc_test, crc8RohcCalculatePartial);
}

