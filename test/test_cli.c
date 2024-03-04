/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define TEST_CLI_PRINTF_CALLBACK_STRING_LENGTH (256)

/****************************************************************************************************
 * Type Definition
 ****************************************************************************************************/

#include "cli.h"
#include <stdarg.h>
#include "unity.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

/*** External ***/
extern cli_alertProcessInputCallback_t cli_alertProcessInputCallback;
extern const char *cli_commandPrompt;
extern char cli_inputBuffer[CLI_CONFIG_MAXIMUM_INPUT_BUFFER_LENGTH];
extern uint16_t cli_inputBufferIndex;
extern cli_printfCallback_t cli_printfCallback;
extern cli_record_t *cli_recordList;

/*** Local ***/
static bool testCli_alertProcessInput;
static uint8_t testCli_commandHandlerCallbackArgc;
static char *testCli_commandHandlerCallbackArgv[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT];
static bool testCli_commandHandlerCallbackHelpCommand;
static char testCli_printfCallbackString[TEST_CLI_PRINTF_CALLBACK_STRING_LENGTH];
static size_t testCli_printfCallbackStringIndex;

/****************************************************************************************************
 * Helper
 ****************************************************************************************************/

/*** Alert Process Input Callback ***/
static void testCli_alertProcessInputCallback(void)
{
    testCli_alertProcessInput = true;
}

/*** Command Handler Callback ***/
static void testCli_commandHandlerCallback(uint8_t argc, char *argv[])
{
    uint8_t i;
    
    /*** Process Arguments ***/
    if((argc == 1) && (strcmp(argv[0], "--help") == 0))
    {
        testCli_commandHandlerCallbackHelpCommand = true;
    }
    else
    {
        testCli_commandHandlerCallbackArgc = argc;
        for(i = 0; i < CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT; i++)
        {
            if(i < argc)
                testCli_commandHandlerCallbackArgv[i] = argv[i];
            else
                testCli_commandHandlerCallbackArgv[i] = NULL;
        }
        testCli_commandHandlerCallbackHelpCommand = false;
    }
}

/*** Print Formatted String Callback ***/
static void testCli_printfCallback(const bool Flush, const char * const Format, ...)
{
    va_list arguments;

    /*** Print Formatted String Callback ***/
    /* Decode Format String And Optional Arguments And Write To Print String */
    va_start(arguments, Format);
    testCli_printfCallbackStringIndex += (size_t)vsnprintf(&testCli_printfCallbackString[testCli_printfCallbackStringIndex], sizeof(testCli_printfCallbackString) - testCli_printfCallbackStringIndex, Format, arguments);
    va_end(arguments);
    
    /* Print String */
    if(Flush)
        testCli_printfCallbackStringIndex = 0;
}

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

/*** External ***/
extern void cli_addRecord(cli_record_t * const newRecord);
extern void cli_helpCommandHandlerCallback(uint8_t argc, char *argv[]);
extern void cli_listCommandHandlerCallback(uint8_t argc, char *argv[]);
extern eclectic_status_t cli_getCommandAndArgumentList(char * const input, const uint8_t MaxArgumentListCount, char **command, uint8_t * const argumentListCount, char *argumentList[]);
extern eclectic_status_t cli_verifyRecordNameAcceptable(const char * const Name);

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_init(testCli_alertProcessInputCallback, testCli_printfCallback, "$ "));
    testCli_alertProcessInput = false;
    testCli_printfCallbackStringIndex = 0;
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Add Input Character ***/
void test_addInputCharacter_error(void)
{
    /*** Test Data ***/
    /* Set Up */
    cli_alertProcessInputCallback = NULL;
    
    /*** Add Input Character ***/
    /* Not Initialized */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, cli_addInputCharacter('A'));
}

void test_addInputCharacter_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const char * const ExpectedOutput;
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        {"\n", ""},
        {"help\n", "help"},
        {"\b\n", ""},
        {"hh\bee\bll\bpp\b\n", "help"},
        {" \t\r\n", " \t\r"},
        {
            "00000000111111112222222233333333444444445555555566666666777777778888888899999999AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEEFFFFFFFF", // CLI_CONFIG_MAXIMUM_INPUT_BUFFER_LENGTH
            "00000000111111112222222233333333444444445555555566666666777777778888888899999999AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDDEEEEEEEEFFFFFFF" // Last 'F' Changed To '\0'
        }
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);

    /* Variable */
    uint8_t i, j;
    
    /*** Add Input Character ***/
    for(i = 0; i < TestDataCount; i++)
    {   
        /* Set Up */
        cli_inputBufferIndex = 0;
        testCli_alertProcessInput = false;
        
        /* Add Input Character (All But Last) */
        for(j = 0; j < (strlen(TestData[i].Input) - 1); j++)
        {
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_addInputCharacter(TestData[i].Input[j]));
            TEST_ASSERT_FALSE(testCli_alertProcessInput);
        }
        
        /* Add Input Character (Last) */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_addInputCharacter(TestData[i].Input[j]));
        
        /* Alert Process Input */
        TEST_ASSERT_TRUE(testCli_alertProcessInput);
        TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedOutput, cli_inputBuffer);
        TEST_ASSERT_EQUAL_INT(strlen(TestData[i].ExpectedOutput) + 1, cli_inputBufferIndex); // Newline Character Is Converted To NUL Character And Counted In cli_inputBufferIndex
    }
}

/*** Initialize ***/
void test_init_error(void)
{
    /*** Initialize ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_init(NULL, testCli_printfCallback, "$ "));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_init(testCli_alertProcessInputCallback, NULL, "$ "));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_init(testCli_alertProcessInputCallback, testCli_printfCallback, NULL));
}

void test_init_success(void)
{
    /*** Test Data ***/
    /* Variable */
    cli_record_t *record;
    
    /*** Initialize ***/
    /* Variable */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_init(testCli_alertProcessInputCallback, testCli_printfCallback, "$ "));
    TEST_ASSERT_EQUAL_PTR(testCli_alertProcessInputCallback, cli_alertProcessInputCallback);
    TEST_ASSERT_EQUAL_STRING("$ ", cli_commandPrompt);
    TEST_ASSERT_EQUAL_UINT16(0, cli_inputBufferIndex);
    TEST_ASSERT_EQUAL_PTR(testCli_printfCallback, cli_printfCallback);
    
    /* Built-In Command */
    record = cli_recordList;
    TEST_ASSERT_EQUAL_STRING(CLI_CONFIG_BUILT_IN_COMMAND_HELP_NAME, record->name);
    record = record->next;
    TEST_ASSERT_EQUAL_STRING(CLI_CONFIG_BUILT_IN_COMMAND_LIST_NAME, record->name);
    
    /* Serial Output */
    TEST_ASSERT_EQUAL_STRING("$ ", testCli_printfCallbackString);
    TEST_ASSERT_EQUAL_INT(0, testCli_printfCallbackStringIndex); // Flushed
}

/*** Process Input ***/
void test_processInput_error(void)
{
    /*** Test Data ***/
    /* Set Up */
    cli_printfCallback = NULL;
    
    /*** Process Input ***/
    /* Not Initialized */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_INITIALIZED, cli_processInput());
}

void test_processInput_success(void)
{
    /*** Test Data ***/
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const uint8_t ExpectedArgc;
        const char * const ExpectedArgv[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData[] =
    {
        /* Test Command Handler NOT Called */
        {"\n", 0, {NULL, NULL, NULL, NULL, NULL}}, // No Command
        {"bogus\n", 0, {NULL, NULL, NULL, NULL, NULL}}, // Command Not Registered
        
        /* Test Command Handler Called */
        {"test\n", 0, {NULL, NULL, NULL, NULL, NULL}},
        {"test --info\n", 1, {"--info", NULL, NULL, NULL, NULL}},
        {"test --info --value=5\n", 2, {"--info", "--value=5", NULL, NULL, NULL}},
        {"test --info --value=5 --list=1,2,3,4,5\n", 3, {"--info", "--value=5", "--list=1,2,3,4,5", NULL, NULL}},
        {"test --info --value=5 --list=1,2,3,4,5 this\n", 4, {"--info", "--value=5", "--list=1,2,3,4,5", "this", NULL}},
        {"test --info --value=5 --list=1,2,3,4,5 this and\n", 5, {"--info", "--value=5", "--list=1,2,3,4,5", "this", "and"}},
        {"test --info --value=5 --list=1,2,3,4,5 this and that\n", 5, {"--info", "--value=5", "--list=1,2,3,4,5", "this", "and"}}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    uint8_t i, j;
    cli_record_t record;
    
    /* Set Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_registerCommand(&record, "test", &testCli_commandHandlerCallback));
    
    /*** Process Input ***/
    for(i = 0; i < TestDataCount; i++)
    {   
        /* Set Up */
        testCli_alertProcessInput = false;
        testCli_commandHandlerCallbackArgc = 0;
        for(j = 0; j < CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT; j++)
            testCli_commandHandlerCallbackArgv[j] = NULL;
        
        /* Add Input Character */
        for(j = 0; j < strlen(TestData[i].Input); j++)
            TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_addInputCharacter(TestData[i].Input[j]));
        TEST_ASSERT_TRUE(testCli_alertProcessInput);
        
        /* Process Input */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_processInput());
        TEST_ASSERT_EQUAL_UINT8(TestData[i].ExpectedArgc, testCli_commandHandlerCallbackArgc);
        for(j = 0; j < CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT; j++)
            TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedArgv[j], testCli_commandHandlerCallbackArgv[j]);
    }
}

/*** Register Command ***/
void test_registerCommand_error(void)
{
    /*** Test Data ***/
    /* Variable */
    cli_record_t record[2];
    
    /*** Register Command ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_registerCommand(NULL, "name", &testCli_commandHandlerCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_registerCommand(&record[0], NULL, &testCli_commandHandlerCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_registerCommand(&record[0], "name", NULL));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cli_registerCommand(&record[0], "", &testCli_commandHandlerCallback));
    
    /* String */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_STRING, cli_registerCommand(&record[0], "nam3", &testCli_commandHandlerCallback));
    
    /* Not Unique */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_registerCommand(&record[0], "name", &testCli_commandHandlerCallback));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NOT_UNIQUE, cli_registerCommand(&record[1], "name", &testCli_commandHandlerCallback));
}

void test_registerCommand_success(void)
{
    /*** Test Data ***/
    /* Define */
    #undef REGISTER_COMMAND_COUNT
    #define REGISTER_COMMAND_COUNT (6)
    
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const RegisterOrder[REGISTER_COMMAND_COUNT];
        const char * const ExpectedOrder[REGISTER_COMMAND_COUNT];
    } testData_t;

    /* Test Data */
    const testData_t TestData =
    {
        {"core", "access", "clock", "info", "halt", "random"}, // Not In Alphabetical Order
        {"access", "clock", "core", "halt", "info", "random"} // In Alphabetical Order
    };
    
    /* Variable */
    uint8_t i;
    cli_record_t *record, recordList[REGISTER_COMMAND_COUNT];
    
    /* Set Up */
    cli_recordList = NULL; // Set To NULL So Built-In Commands Are Removed
    
    /*** Register Command ***/
    /* Register Command */
    for(i = 0; i < REGISTER_COMMAND_COUNT; i++)
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_registerCommand(&recordList[i], TestData.RegisterOrder[i], &testCli_commandHandlerCallback));
    
    /* Record List Order */
    record = cli_recordList;
    for(i = 0; i < REGISTER_COMMAND_COUNT; i++)
    {
        TEST_ASSERT_EQUAL_STRING(TestData.ExpectedOrder[i], record->name);
        record = record->next;
    }
}

/****************************************************************************************************
 * Test (Private)
 ****************************************************************************************************/

/*** Add Record ***/
void test_addRecord_success(void)
{
    /*** Test Data ***/
    /* Variable */
    cli_record_t record;
    
    /* Set Up */
    record.name = "test";
    record.prev = NULL;
    record.next = NULL;
    record.commandHandlerCallback = testCli_commandHandlerCallback;
    
    /*** Add Record ***/
    cli_addRecord(&record); // Not Necessary To Test As It Doesn't Return Anything; Added For Completeness
}

/*** Help Command Handler Callback ***/
void test_helpCommandHandlerCallback_success(void)
{
    /*** Test Data ***/
    /* Variable */
    cli_record_t record;
    
    /* Set Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_registerCommand(&record, "test", &testCli_commandHandlerCallback));
    
    /*** Help Command Handler Callback ***/
    /* Help Command Handler Callback */
    cli_helpCommandHandlerCallback(0, NULL); // Arguments Ignored
    
    /* Test Command Arguments */
}

/*** List Command Handler Callback ***/
void test_listCommandHandlerCallback_success(void)
{
    /*** Test Data ***/
    /* Variable */
    cli_record_t record;
    
    /* Set Up */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_registerCommand(&record, "test", &testCli_commandHandlerCallback));
    
    /*** List Command Handler Callback ***/
    /* List Command Handler Callback */
    cli_listCommandHandlerCallback(0, NULL); // Arguments Ignored
    
    /* Serial Output */
    TEST_ASSERT_EQUAL_STRING("test\n", testCli_printfCallbackString);
    TEST_ASSERT_EQUAL_INT(0, testCli_printfCallbackStringIndex); // Flushed
}

/*** Get Command And Argument List ***/
void test_getCommandAndArgumentList_error(void)
{
    /*** Test Data ***/
    /* Variable */
    char *argumentList[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT];
    uint8_t argumentListCount;
    char *command, input[CLI_CONFIG_MAXIMUM_INPUT_BUFFER_LENGTH];
    
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_getCommandAndArgumentList(NULL, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, &argumentListCount, argumentList));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_getCommandAndArgumentList(input, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, NULL, &argumentListCount, argumentList));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_getCommandAndArgumentList(input, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, NULL, argumentList));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_getCommandAndArgumentList(input, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, &argumentListCount, NULL));
    
    /* Invalid */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cli_getCommandAndArgumentList(" \t\r", CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, &argumentListCount, argumentList));
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_INVALID, cli_getCommandAndArgumentList("123$%^", CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &command, &argumentListCount, argumentList));
}

void test_getCommandAndArgumentList_success(void)
{
    /*** Test Data ***/    
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Input;
        const char * const ExpectedCommand;
        const uint8_t ExpectedArgumentListCount;
        const char * const ExpectedArgumentList[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT];
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[] =
    {
        {"help", "help", 0, {NULL, NULL, NULL, NULL, NULL}},
        {"help 1", "help", 1, {"1", NULL, NULL, NULL, NULL}},
        {"help \t\r1", "help", 1, {"1", NULL, NULL, NULL, NULL}},
        {"help 123 456", "help", 2, {"123", "456", NULL, NULL, NULL}},
        {"help 123 456 789", "help", 3, {"123", "456", "789", NULL, NULL}},
        {"help abc def ghi jkl", "help", 4, {"abc", "def", "ghi", "jkl", NULL}},
        {"help abc def ghi jkl mno", "help", 5, {"abc", "def", "ghi", "jkl", "mno"}},
        {"help abc def ghi jkl mno", "help", 5, {"abc", "def", "ghi", "jkl", "mno"}},
        {"help abc def ghi jkl mno pqr", "help", 5, {"abc", "def", "ghi", "jkl", "mno"}}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    char *actualCommand, input[CLI_CONFIG_MAXIMUM_INPUT_BUFFER_LENGTH];
    char *actualArgumentList[CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT];
    uint8_t actualArgumentListCount, i, j;
    
    /*** Get Command And Argument List ***/
    for(i = 0; i < TestDataCount; i++)
    {
        /* Set Up */
        strcpy(input, TestData[i].Input);
        
        /* Get Command And Argument List */
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_SUCCESS, cli_getCommandAndArgumentList(input, CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT, &actualCommand, &actualArgumentListCount, actualArgumentList));
        TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedCommand, actualCommand);
        TEST_ASSERT_EQUAL_UINT8(TestData[i].ExpectedArgumentListCount, actualArgumentListCount);
        for(j = 0; j < CLI_CONFIG_MAXIMUM_ARGUMENT_LIST_COUNT; j++)
            TEST_ASSERT_EQUAL_STRING(TestData[i].ExpectedArgumentList[j], actualArgumentList[j]);
    }
}

/*** Verify Record Name Acceptable ***/
void test_verifyRecordNameAcceptable_error_preprocess(void)
{
    /*** Verify Record Name Acceptable ***/
    /* NULL Pointer */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_NULL_POINTER, cli_verifyRecordNameAcceptable(NULL));
    
    /* Length */
    TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_LENGTH, cli_verifyRecordNameAcceptable(""));
}

void test_verifyRecordNameAcceptable_error_postprocess(void)
{
    /*** Test Data ***/
    /* Test Data */
    const char *TestData[] = {"1st", "info!", "num6er", "command name"};
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    uint8_t i;
    cli_record_t record;
    
    /*** Verify Record Name Acceptable ***/
    for(i = 0; i < TestDataCount; i++)
        TEST_ASSERT_EQUAL_INT(ECLECTIC_STATUS_ERROR_STRING, cli_registerCommand(&record, TestData[i], testCli_commandHandlerCallback));
}

void test_verifyRecordNameAcceptable_success(void)
{
    /*** Test Data ***/
    /* Define */
    #undef REGISTER_COMMAND_COUNT
    #define REGISTER_COMMAND_COUNT (3)
    
    /* Type Definition */
    typedef struct testData_s
    {
        const char * const Name;
        const eclectic_status_t ExpectedStatus;
    } testData_t;
    
    /* Test Data */
    const testData_t TestData[REGISTER_COMMAND_COUNT] =
    {
        {"info", ECLECTIC_STATUS_SUCCESS},
        {"Info", ECLECTIC_STATUS_ERROR_NOT_UNIQUE},
        {"INFO", ECLECTIC_STATUS_ERROR_NOT_UNIQUE}
    };
    const size_t TestDataCount = sizeof(TestData) / sizeof(TestData[0]);
    
    /* Variable */
    uint8_t i;
    cli_record_t record[REGISTER_COMMAND_COUNT];
    
    /*** Verify Record Name Acceptable ***/
    for(i = 0; i < TestDataCount; i++)
        TEST_ASSERT_EQUAL_INT(TestData[i].ExpectedStatus, cli_registerCommand(&record[i], TestData[i].Name, testCli_commandHandlerCallback));
}
