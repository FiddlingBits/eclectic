/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

#include "cli_helper.h"
#include "unity.h"

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Get Comma Separated Value List ***/
void test_getCommaSeparatedValueList_error(void)
{
    /*** Test Data ***/
    /* Define */
    #define MAX_LIST_COUNT (3)

    /* Variable */
    char *list[MAX_LIST_COUNT];
    uint8_t listCount;

    /*** Get Comma Separated Value List ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getCommaSeparatedValueList(NULL, MAX_LIST_COUNT, list, &listCount));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getCommaSeparatedValueList("0,1,2", MAX_LIST_COUNT, NULL, &listCount));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getCommaSeparatedValueList("0,1,2", MAX_LIST_COUNT, list, NULL));

    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getCommaSeparatedValueList("", MAX_LIST_COUNT, list, &listCount));
    
    /*** Clean Up ***/
    #undef MAX_LIST_COUNT
}

void test_getCommaSeparatedValueList_success(void)
{
    /*** Test Data ***/
    /* Define */
    #define MAX_LIST_COUNT (3)

    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const char *ExpectedList[MAX_LIST_COUNT];
        const uint8_t ExpectedListCount;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"0", {"0", NULL, NULL}, 1},
        {"0a,1b", {"0a", "1b", NULL}, 2},
        {"0a!,1b@,2c#", {"0a!", "1b@", "2c#"}, 3},
        {"0a!,1b@,2c#,3d$", {"0a!", "1b@", "2c#"}, 3}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    char *actualList[MAX_LIST_COUNT], input[64];
    uint8_t i, j, actualListCount;

    /*** Get Comma Separated Value List ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        strcpy(input, TestData[i].Input);

        /* Get Comma Separated Value List */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getCommaSeparatedValueList(input, MAX_LIST_COUNT, actualList, &actualListCount));
        TEST_ASSERT_EQUAL_UINT8(TestData[i].ExpectedListCount, actualListCount);
        for(j = 0; j < MAX_LIST_COUNT; j++)
            TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedList[j], actualList[j]);
    }
    
    /*** Clean Up ***/
    #undef MAX_LIST_COUNT
}

/*** Get Double ***/
void test_getDouble_error_preconversion(void)
{
    /*** Test Data ***/
    /* Variable */
    double d;

    /*** Get Double ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getDouble(NULL, &d));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getDouble("3.14", NULL));

    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getDouble("", &d));
}

void test_getDouble_error_postconversion(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const double ExpectedDouble;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"ABC", 0.0},
        {"-123.0ABC", -123.0},
        {"123.0ABC", 123.0},
        {"+123.0ABC", 123.0}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    double actualDouble;
    uint8_t i;

    /*** Get Double ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Double */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cliHelper_getDouble(TestData[i].Input, &actualDouble));
        TEST_ASSERT_EQUAL_DOUBLE(TestData[i].ExpectedDouble, actualDouble);
    }
}

void test_getDouble_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const double ExpectedDouble;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"-3.141592653589", -3.141592653589}, // Pi (Negative)
        {"-2.718281828459", -2.718281828459}, // Euler's Number (Negative)
        {"-0.0", 0.0},
        {"0.0", 0.0},
        {"+0.0", 0.0},
        {"2.718281828459", 2.718281828459}, // Euler's Number
        {"3.141592653589", 3.141592653589}, // Pi
        {" \t\n3.141592653589\n\t ", 3.141592653589} // Pi, With Whitespace
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    double actualDouble;
    uint8_t i;

    /*** Get Double ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Double */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getDouble(TestData[i].Input, &actualDouble));
        TEST_ASSERT_EQUAL_DOUBLE(TestData[i].ExpectedDouble, actualDouble);
    }
}

/*** Get Float ***/
void test_getFloat_error_preconversion(void)
{
    /*** Test Data ***/
    /* Variable */
    float f;

    /*** Get Float ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getFloat(NULL, &f));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getFloat("3.14", NULL));

    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getFloat("", &f));
}

void test_getFloat_error_postconversion(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const float ExpectedFloat;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"ABC", 0.0},
        {"-123.0ABC", -123.0},
        {"123.0ABC", 123.0},
        {"+123.0ABC", 123.0},
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    float actualFloat;
    uint8_t i;

    /*** Get Float ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Float */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cliHelper_getFloat(TestData[i].Input, &actualFloat));
        TEST_ASSERT_EQUAL_FLOAT(TestData[i].ExpectedFloat, actualFloat);
    }
}

void test_getFloat_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const float ExpectedFloat;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"-3.141592653589", -3.141592F}, // Pi (Negative)
        {"-2.718281828459", -2.718281F}, // Euler's Number (Negative)
        {"-0.0", 0.0},
        {"0.0", 0.0},
        {"+0.0", 0.0},
        {"2.718281828459", 2.718281F}, // Euler's Number
        {"3.141592653589", 3.141592F}, // Pi
        {" \t\n3.141592653589\n\t ", 3.141592F} // Pi, With Whitespace
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    float actualFloat;
    uint8_t i;

    /*** Get Float ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Float */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getFloat(TestData[i].Input, &actualFloat));
        TEST_ASSERT_EQUAL_FLOAT(TestData[i].ExpectedFloat, actualFloat);
    }
}

/*** Get Option/Argument Pair ***/
void test_getOptionArgumentPair_error_preconversion(void)
{
    /*** Test Data ***/
    /* Variable */
    cliHelper_optionArgumentPair_t optionArgumentPair;
    
    /*** Get Option/Argument Pair ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getOptionArgumentPair(NULL, &optionArgumentPair));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getOptionArgumentPair("--help", NULL));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getOptionArgumentPair("", &optionArgumentPair));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getOptionArgumentPair("-", &optionArgumentPair));
}

void test_getOptionArgumentPair_error_postconversion(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const cliHelper_optionArgumentPair_t ExpectedOptionArgumentPair;
        const eclectic_status_t ExpectedStatus;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {
        {"help", {NULL, NULL}, ECLECTIC_STATUS_ERROR_NO_ACTION},
        {" -h", {NULL, NULL}, ECLECTIC_STATUS_ERROR_NO_ACTION},
        {"--", {NULL, NULL}, ECLECTIC_STATUS_ERROR_INVALID},
        {"--=", {NULL, NULL}, ECLECTIC_STATUS_ERROR_INVALID},
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /*** Get Option/Argument Pair ***/
    /* Variable */
    cliHelper_optionArgumentPair_t actualOptionArgumentPair;
    uint8_t i;
    char input[64];
    
    /*** Get Option/Argument Pair ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        strcpy(input, TestData[i].Input);
        
        /* Get Option/Argument Pair */
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedStatus, cliHelper_getOptionArgumentPair(input, &actualOptionArgumentPair));
        TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedOptionArgumentPair.option, actualOptionArgumentPair.option);
    }
}

void test_getOptionArgumentPair_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const cliHelper_optionArgumentPair_t ExpectedOptionArgumentPair;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {
        /* Short Option */
        {"-a", {"a", NULL}},
        {"-bc", {"b", "c"}},
        {"-defg", {"d", "efg"}},
        {"- ", {" ", NULL}},
        {"-  ", {" ", " "}},
        
        /* Long Option */
        {"--help", {"help", NULL}},
        {"--help=", {"help", NULL}},
        {"--i", {"i", NULL}},
        {"--j=k", {"j", "k"}},
        {"--lmn=opq", {"lmn", "opq"}},
        {"-- ", {" ", NULL}},
        {"-- = ", {" ", " "}},
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /*** Get Option/Argument Pair ***/
    /* Variable */
    cliHelper_optionArgumentPair_t actualOptionArgumentPair;
    uint8_t i;
    char input[64];
    
    /*** Get Option/Argument Pair ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        strcpy(input, TestData[i].Input);
        
        /* Get Option/Argument Pair */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getOptionArgumentPair(input, &actualOptionArgumentPair));
        TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedOptionArgumentPair.option, actualOptionArgumentPair.option);
    }
}

/*** Get Signed 32-Bit Integer ***/
void test_getSigned32BitInteger_error_preconversion(void)
{
    /*** Test Data ***/
    /* Variable */
    int32_t s32;
    
    /*** Get Signed 32-Bit Integer ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getSigned32BitInteger(NULL, &s32));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getSigned32BitInteger("0", NULL));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getSigned32BitInteger("", &s32));
}

void test_getSigned32BitInteger_error_postconversion(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const int32_t ExpectedSigned32BitInteger;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {        
        {"ABC", 0},
        {"-123ABC", -123},
        {"123ABC", 123},
        {"+123ABC", 123}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    int32_t actualSigned32BitInteger;
    uint8_t i;
    
    /*** Get Signed 32-Bit Integer ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Signed 32-Bit Integer */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cliHelper_getSigned32BitInteger(TestData[i].Input, &actualSigned32BitInteger));
        TEST_ASSERT_EQUAL_INT32(TestData[i].ExpectedSigned32BitInteger, actualSigned32BitInteger);
    }
}

void test_getSigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const int32_t ExpectedSigned32BitInteger;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {        
        {"-2147483649", 2147483647}, // Undefined Behavior
        {"-2147483648", -2147483648},
        {"-1", -1},
        {"-0", 0},
        {"0", 0},
        {"0 ", 0},
        {" \t\n0\n\t ", 0}, // With Whitespace
        {"+0", 0},
        {"1", 1},
        {"+1", 1},
        {"2147483647", 2147483647},
        {"2147483648", -2147483648} // Undefined Behavior
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    int32_t actualSigned32BitInteger;
    uint8_t i;
    
    /*** Get Signed 32-Bit Integer ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Signed 32-Bit Integer */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getSigned32BitInteger(TestData[i].Input, &actualSigned32BitInteger));
        TEST_ASSERT_EQUAL_INT32(TestData[i].ExpectedSigned32BitInteger, actualSigned32BitInteger);
    }
}

/*** Get Unsigned 32-Bit Integer ***/
void test_getUnsigned32BitInteger_error_preconversion(void)
{
    /*** Test Data ***/
    /* Variable */
    uint32_t u32;
    
    /*** Get Unsigned 32-Bit Integer ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getUnsigned32BitInteger(NULL, &u32));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cliHelper_getUnsigned32BitInteger("0", NULL));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cliHelper_getUnsigned32BitInteger("", &u32));
}

void test_getUnsigned32BitInteger_error_postconversion(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const int32_t ExpectedUnsigned32BitInteger;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {        
        {"ABC", 0},
        {"123ABC", 123}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    int32_t actualUnsigned32BitInteger;
    uint8_t i;
    
    /*** Get Unsigned 32-Bit Integer ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Unsigned 32-Bit Integer */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cliHelper_getUnsigned32BitInteger(TestData[i].Input, &actualUnsigned32BitInteger));
        TEST_ASSERT_EQUAL_INT32(TestData[i].ExpectedUnsigned32BitInteger, actualUnsigned32BitInteger);
    }
}

void test_getUnsigned32BitInteger_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const int32_t ExpectedUnsigned32BitInteger;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {        
        /* Decimal */
        {"-1", 4294967295}, // -1 Is ~0 In 2s Complement
        {"0", 0},
        {" \t\n0\n\t ", 0}, // With Whitespace
        {"1", 1},
        {"4294967295", 4294967295},
        {"4294967296", 0}, // Truncated
        {"4294967297", 1}, // Truncated
        
        /* Hexadecimal */
        {"0x0", 0x00000000},
        {"0x00000000", 0x00000000},
        {"0x1", 0x00000001},
        {"0x00000001", 0x00000001},
        {"0xABC", 0x00000ABC},
        {"0xdeadbeef", 0xDEADBEEF},
        {"0xDEADBEEF", 0xDEADBEEF},
        {"0xFFFFFFFF", 0xFFFFFFFF}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    int32_t actualUnsigned32BitInteger;
    uint8_t i;
    
    /*** Get Unsigned 32-Bit Integer ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Get Unsigned 32-Bit Integer */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cliHelper_getUnsigned32BitInteger(TestData[i].Input, &actualUnsigned32BitInteger));
        TEST_ASSERT_EQUAL_INT32(TestData[i].ExpectedUnsigned32BitInteger, actualUnsigned32BitInteger);
    }
}
